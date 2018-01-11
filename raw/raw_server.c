#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>

#define DEST_MAC0   0x00
#define DEST_MAC1   0x00
#define DEST_MAC2   0x00
#define DEST_MAC3   0x00
#define DEST_MAC4   0x00
#define DEST_MAC5   0x00

#define ETHER_TYPE  0x0800

#define DEFAULT_IF  "eth0"
#define BUF_SIZE    1024

int main(int argc, char *argv[])
{
  char sender[INET6_ADDRSTRLEN];
  int sock, ret, i;
  int sockopt;
  ssize_t numbytes;
  struct ifreq ifopts;
  struct ifreq if_ip;
  struct sockaddr_storage their_addr;
  uint8_t buf[BUF_SIZE];
  char if_name[IFNAMSIZ];

  if (argc > 1)
    strcpy(if_name, argv[1]);
  else
    strcpy(if_name, DEFAULT_IF);

  struct ether_header *eh = (struct ether_header *) buf;
  struct iphdr *iph = (struct iphdr *) (buf + sizeof(struct ether_header));
  struct udphdr *udph = (struct udphdr *) (buf + sizeof(struct udphdr) + sizeof(struct ether_header));

  memset(&if_ip, 0, sizeof(struct ifreq));

  if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) < 0)
  {
    perror("listener: socket");
    return -1;
  }

  strncpy(ifopts.ifr_name, if_name, IFNAMSIZ-1);
  ioctl(sock, SIOCGIFFLAGS, &ifopts);
  ifopts.ifr_flags |= IFF_PROMISC;
  ioctl(sock, SIOCSIFFLAGS, &ifopts);

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) == -1)
  {
    perror("setsockopt");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, if_name, IFNAMSIZ-1) == -1) 
  {
    perror("SO_BINDTODEVICE");
    close(sock);
    exit(EXIT_FAILURE);
  }

repeat:
  printf("listener: Waiting to recvfrom...\n");
  numbytes = recvfrom(sock, buf, BUF_SIZE, 0, NULL, NULL);
  printf("listener: got packet %lu bytes\n", numbytes);

  if (eh->ether_dhost[0] == DEST_MAC0 &&
      eh->ether_dhost[1] == DEST_MAC1 &&
      eh->ether_dhost[2] == DEST_MAC2 &&
      eh->ether_dhost[3] == DEST_MAC3 &&
      eh->ether_dhost[4] == DEST_MAC4 &&
      eh->ether_dhost[5] == DEST_MAC5)
  {
    printf("Correct destination MAC address\n");
  }
  else
  {
    printf("Wrong destination MAC: %x:%x:%x:%x:%x:%x\n",
        eh->ether_dhost[0],
        eh->ether_dhost[1],
        eh->ether_dhost[2],
        eh->ether_dhost[3],
        eh->ether_dhost[4],
        eh->ether_dhost[5]);

    ret = -1;
    goto done;
  }

  ((struct sockaddr_in *)&their_addr)->sin_addr.s_addr = iph->saddr;
  inet_ntop(AF_INET, &((struct sockaddr_in *)&their_addr)->sin_addr, sender, sizeof(sender));

  strncpy(if_ip.ifr_name, if_name, IFNAMSIZ-1);
  if (ioctl(sock, SIOCGIFADDR, &if_ip) >= 0)
  {
    printf("Source IP: %s\n My IP: %s\n", sender,
        inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));

    if (strcmp(sender, inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr)) == 0)
    {
      printf("but I send it :(\n");
      ret = -1;
      goto done;
    }
  }

  ret = ntohs(udph->len) - sizeof(struct udphdr);

  printf("\tData:");

  for (i=0; i<numbytes; i++)
    printf("%02x:", buf[i]);
  printf("\n");

done:
  goto repeat;

  close(sock);
  return ret;
}
