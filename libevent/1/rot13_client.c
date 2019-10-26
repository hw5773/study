#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  const char query[] = "test message\n";
  struct sockaddr_in sin;
  const char *cp;
  int fd;
  ssize_t n_written, remaining;
  char buf[1024];

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
  {
    perror("socket");
    return 1;
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(40713);
  sin.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(fd, (struct sockaddr *) &sin, sizeof(sin)))
  {
    perror("connect");
    close(fd);
    return 1;
  }

  cp = query;
  remaining = strlen(query);
  while (remaining) 
  {
    n_written = send(fd, cp, remaining, 0);
    if (n_written <= 0)
    {
      perror("send");
      return 1;
    }
    remaining -= n_written;
    cp += n_written;
  }

  while (1)
  {
    ssize_t result = recv(fd, buf, sizeof(buf), 0);
    if (result == 0)
    {
      break;
    }
    else if (result < 0)
    {
      perror("recv");
      close(fd);
      return 1;
    }
    fwrite(buf, 1, result, stdout);
  }

  close(fd);
  return 0;
}
