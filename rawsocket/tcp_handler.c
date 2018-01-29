#include "tcp_handler.h"

#define STARTING_SEQUENCE 1
#define TCP_WORD_LENGTH_WITH_NO_OPTIONS 5
#define HAS_TCP_OPTIONS(ptr) (ptr->doff > TCP_WORD_LENGTH_WITH_NO_OPTIONS)
#define TCP_OPTION_OFFSET(ptr) ((char *)ptr + (TCP_WORD_LENGTH_WITH_NO_OPTIONS * WORD_LENGTH))
#define TCP_OPTION_LENGTH(ptr) ((ptr->doff - TCP_WORD_LENGTH_WITH_NO_OPTIONS) * WORD_LENGTH)
#define END_OF_TCP_OPTION_CHECK(ptr) ((*ptr) == 0)
#define TCP_OPTIONS_LEN(ptr) ((ptr->doff - TCP_WORD_LENGTH_WITH_NO_OPTIONS) * WORD_LENGTH)
#define IS_NO_OPERATION(ptr) ((*ptr) == 1)
#define IS_MSS(ptr) ((*ptr) == 2)
#define OPTION_LENGTH(ptr) (*(ptr + 1))
#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#define TCP_OPTION_DATA_OFFSET 2

#define IS_DUPLICATE_TCP_SEGMENT(tcph) (ntohl(tcph->seq) < tcp_state.server_next_seq_num)
#define IS_DUPLICATE_ACK(tcph) (tcph->ack && (tcph->ack_seq == tcp_state.last_acked_seq_num))
#define WRAP_ROUND_BUFFER_SIZE(index) \
  ({ __typeof__ (index) _index = (index); \
   ( _index + 1 ) > MAX_BUFFER_SIZE ? 0 : (_index + 1); })

tcp_state_t tcp_state;

static unsigned short csum(uint16_t *ptr, unsigned int nbytes)
{
  uint32_t sum;
  uint16_t answer;

  sum = 0;

  while (nbytes > 1)
  {
    sum += *ptr++; // ptr type is uint16_t
    nbytes -= 2;
  }

  if (nbytes == 1)
  {
    sum += *(unsigned char *)ptr;
  }

  sum = (sum >> 16) + (sum & 0xffff);
  sum = sum + (sum >> 16);
  answer = (short) ~sum;

  return (answer);
}

static void calculate_tcp_checksum(struct tcphdr *tcph,
    uint16_t tcp_payload_len, uint32_t src_addr, uint32_t dst_addr)
{
  pseudo_header psh;
  char *pseudogram;
  uint16_t tcphdr_len = (tcph->doff * WORD_LENGTH);

  bzero(&psh, sizeof(pseudo_header));
  psh.source_address = src_addr;
  psh.dest_address = dst_addr;
  psh.protocol = IPPROTO_TCP;
  psh.tcp_length = htons(tcphdr_len + tcp_payload_len);

  int psize = sizeof(pseudo_header) + tcphdr_len + tcp_payload_len;
  pseudogram = (char *)malloc(psize);

  bzero(pseudogram, psize);
  memcpy(pseudogram, &psh, sizeof(pseudo_header));
  memcpy(pseudogram + sizeof(pseudo_header), tcph, tcphdr_len + tcp_payload_len);
  
  tcph->check = csum((uint16_t *)pseudogram, (unsigned int)psize);
  free(pseudogram);
}

// TODO: may fix more easy way
static int validate_ip_checksum(struct iphdr *iph)
{
  int ret = -1;
  uint16_t received_checksum = iph->check;
  iph->check = 0;

  if (received_checksum == csum((uint16_t *)iph, (unsigned int) (iph->ihl * WORD_LENGTH)))
    ret = 1;

  return ret;
}

// TODO: may fix more easy way
static int validate_tcp_checksum(struct tcphdr *tcph, uint16_t tcp_payload_length)
{
  int ret = -1;
  uint16_t received_checksum = tcph->check;
  tcph->check = 0;
  calculate_tcp_checksum(tcph, tcp_payload_length,
      *(uint32_t *) &tcp_state.session_info.dst_addr.sin_addr.s_addr,
      *(uint32_t *) &tcp_state.session_info.src_addr.sin_addr.s_addr);

  if (received_checksum == tcph->check)
    ret = 1;

  return ret;
}

static packet_t *create_packet()
{
  packet_t *packet = (packet_t *)malloc(sizeof(packet_t));
  bzero(packet, sizeof(packet_t));
  packet->offset[IP_OFFSET] = packet->payload; // the start point of the ip header
  packet->offset[TCP_OFFSET] = packet->paylaod + sizeof(struct iphdr); // the start point of the tcp header
  packet->offset[DATA_OFFSET] = packet->payload + sizeof(struct iphdr) + sizeof(struct tcphdr); // the start point of the data
  packet->retransmit_timer_id = NULL;
  return packet;
}

static void adjust_layer_offset(packet_t *packet)
{
  struct tcphdr *tcph;
  struct iphdr *iph;

  iph = (struct iphdr *)packet->payload;
  tcph = (struct tcphdr *)(packet->payload + (iph->ihl * WORD_LENGTH));
  packet->offset[TCP_OFFSET] = (char *)tcph;
  packet->offset[DATA_OFFSET] = (char *)(packet->offset[TCP_OFFSET] + (tcph->doff * WORD_LENGTH));
}

static void destroy_packet(packet_t *packet)
{
  if (packet->retransmit_timer_id != NULL)
    timer_delete(packet->retransmit_timer_id);

  free(packet);
}

static void remove_acked_entries(uint32_t next_expected_seq)
{
  pthread_mutex_lock(&tcp_state.sender_info.tcp_retx_lock);
  while ((tcp_state.sender_info.retx_buffer[tcp_state.sender_info.retx_buffer_head].packet_seq 
        < next_expected_seq)
        && !(tcp_state.sender_info.retx_buffer_head == tcp_state.sender_info.retx_buffer_tail))
  {
    destroy_packet(tcp_state.sender_info.retx_buffer[tcp_state.sender_info.retx_buffer_head].packet);
    tcp_state.sender_info.retx_buffer[tcp_state.sender_info.retx_buffer_head].packet = NULL;
    tcp_state.sender_info.retx_buffer_head = WRAP_ROUND_BUFFER_SIZE(tcp_state.sender_info.retx_buffer_head);
  }

  pthread_mutex_unlock(&tcp_state.sender_info.tcp_retx_lock);
}

static void reset_packet_retransmission_timer(timer_t *timer_id, uint16_t time_in_secs)
{
  struct itimerspec timer_value = { 0 };
  timer_value.it_interval_tv_sec = time_in_secs;
  timer_value.it_value.tv_sec = time_in_secs;

  if (timer_settime(*timer_id, 0, &timer_value, NULL) < 0)
  {
    printf("Failed to set time!\n");
    timer_delete(*timer_id);
    *timer_id = NULL;
  }
}

static void build_ip_header(struct iphdr *iph, uint16_t ip_payload_len)
{
  iph->daddr = *(uint32_t *)&tcp_state.session_info.dst_addr.sin_addr.s_addr;
  iph->saddr = *(uint32_t *)&tcp_state.session_info.src_addr.sin_addr.s_addr;
  iph->ihl = 5;
  iph->protocol = IPPROTO_TCP;
  iph->ttl = 255;
  iph->version = 4;
  iph->tot_len = sizeof(struct iphdr) + ip_payload_len;
  iph->check = csum((unsigned short *) iph, sizeof(struct iphdr));
}

static void build_tcp_header(struct tcphdr *tcph, tcp_flags_t *flags, uint16_t payload_len)
{
  tcph->dest = *(uint16_t *) &tcp_state.session_info.dst_addr.sin_port;
  tcph->source = *(uint16_t *) &tcp_state.session_info.src_addr.sin_port;
  tcph->window = htons(tcp_state.client_window_size);
  tcph->seq = htonl(tcp_state.client_next_seq_num);
  tcp_state.client_next_seq_num += (flags->syn || flags->fin) ? 1 : payload_len;
  tcph->doff = (flags->syn) ? 6 : 5;
  tcph->syn = flags->syn;
  tcph->ack = flags->ack;
  tcph->fin = flags->fin;
  tcph->psh = flags->psh;
  tcph->ack_seq = htonl(tcp_state.server_next_seq_num);

  if (flags->syn)
  {
    char *tcp_options = ((char *)tcph) + sizeof(struct tcphdr);
    tcp_options_t mss = { 0 };
    mss.option_type = 2;
    mss.option_len = 4;
    mss.option_value = htons(1460);
    memcpy(tcp_options++, &mss.option_type, sizeof(char));
    memcpy(tcp_options++, &mss.option_len, sizeof(char));
    memcpy(tcp_options, &mss.option_value, sizeof(uint16_t));
  }
}

static void build_packet_headers(packet_t *packet, int payload_len, tcp_flags_t *flags)
{
  struct tcphdr *tcph = (struct tcphdr *) packet->offset[TCP_OFFSET];
  struct iphdr *iph = (struct iphdr *) packet->offset[IP_OFFSET];

  build_tcp_header(tcph, flags, payload_len);
  calculate_tcp_checksum(tcph, payload_len,
      *(uint32_t *) &tcp_state.session_info.src_addr.sin_addr.s_addr,
      *(uint32_t *) &tcp_state.session_info.dst_addr.sin_addr.s_addr);
  build_ip_header(iph, ((tcph->doff * WORD_LENGTH) + payload_len));
}

static int send_packet(void *buffer, int total_packet_len)
{
  int ret = -1;

  pthread_mutex_lock(&tcp_state.session_info.send_fd_lock);
  while (total_packet_len > 0)
  {
    if ((ret = sendto(tcp_state.session_info.send_fd, buffer,
            total_packet_len, 0,
            (struct sockaddr *) &tcp_state.session_info.dst_addr,
            sizeof(struct sockaddr_in))) < 0)
    {
      if (errno == EINTR)
      {
        printf("Sendto() Interrupted!\n");
        continue;
      }
      else
      {
        perror("sendto failed");
        goto EXIT;
      }
    }

    if (ret == total_packet_len)
      break;

    total_packet_len -= ret;
    buffer += ret;
  }

EXIT:
  pthread_mutex_unlock(&tcp_state.session_info.send_fd_lock);
  return ret;
}

static void handle_packet_retransmission()
{
  packet_t *packet = NULL;
  pthread_mutex_lock(&tcp_state.sender_info.tcp_retx_lock);
  int index = tcp_state.sender_info.retx_buffer_head;

  while (index != tcp_state.sender_info.retx_buffer_tail)
  {
    packet = tcp_state.sender_info.retx_buffer[index].packet;
    reset_packet_retransmission_timer(&packet->retransmit_timer_id, 0);

    if (send_packet(packet->payload, packet->payload_len) < 0)
      printf("Failed to retransmit packet!\n");

    reset_packet_retransmission_timer(&packet->retransmit_timer_id, 60);
    index++;
  }
  pthread_mutex_unlock(&tcp_state.sender_info.tcp_retx_lock);
}

static int send_ack_segment(uint8_t fin)
{
  int ret = -1;
  packet_t *packet = create_packet();
  tcp_flags_t flags = { 0 };

  flags.ack = 1;
  flags.fin = fin;

  build_packet_headers(packet, 0, &flags);

  if ((ret = send_packet(&packet->payload, 
          ((struct iphdr *)packet->offset[IP_OFFSET])->tot_len)) < 0)
  {
    printf("Send error! Exiting...\n");
  }

EXIT:
  destroy_packet(packet);
  return ret;
}

static int receive_packet(packet_t *packet)
{
  int ret = -1;
  while (1)
  {
    if ((ret = recvfrom(tcp_state.session_info.recv_fd, &packet->payload, 
            sizeof(packet->payload), 0,
            NULL, NULL)) < 0)
    {
      if (errno == EINTR)
        continue;
      else
      {
        perror("recv failed");
        return ret;
      }
    }

    struct iphdr *iph = (struct iphdr *) &packet->payload;

    if (validate_ip_checksum(iph) < 0)
    {
      printf("IP Checksum validation failed! Packet dropped!\n");
      continue;
    }

    uint16_t iphdr_len = iph->ihl * WORD_LENGTH;
    struct tcphdr *tcph = (struct tcphdr *) ((char *) iph + iphdr_len);
    uint16_t tcphdr_len = tcph->doff * WORD_LENGTH;

    // TODO: Need to check whether we need to drop the packet in the middlebox
    if (iph->saddr != tcp_state.session_info.dst_addr.sin_addr.s_addr
        && tcph->dest != tcp_state.session_info.src_port
        && tcph->source != tcp_state.session_info.dst_port)
      continue;

    if (validate_tcp_checksum(tcph,
          (ntohs(iph->tot_len) - iphdr_len - tcphdr_len)) < 0)
    {
      printf("TCP Checksum validation failed! Packet dropped!\n");
      continue;
    }

    if (IS_DUPLICATE_ACK(tcph))
    {
      handle_packet_retransmission();
      continue;
    }
    else if (IS_DUPLICATE_TCP_SEGMENT(tcph))
    {
      send_ack_segment(0);
      continue;
    }

    adjust_layer_offset(packet);
    packet->payload_len = (ntohs(iph->tot_len) - iphdr_len - tcphdr_len);
    break;
  }
  return ret;
}

static void process_ack(struct tcphdr *tcph, uint16_t payload_len)
{
}
