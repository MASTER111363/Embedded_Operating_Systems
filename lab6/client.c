#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  // open()
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> // socket(), connect()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

int ClientSockect(const char* host, int port);

int main(int argc, char **argv){
  int conn_fd, msg_cnt;
  char action;
  char msg_buf[50];

  if (argc != 6){
    printf("Usage: %s <host> <port> <deposit/withdraw> <amount> <times>\n", argv[0]);
    exit(-1);
  }

  if (strcmp(argv[3], "deposit") == 0){
    action = 'D';
  }else if (strcmp(argv[3], "withdraw") == 0){
    action = 'W';
  }else {
    sprintf(msg_buf, "Invalid action %s\n", argv[3]);
    perror(msg_buf);
    exit(-1);
  }

  msg_cnt = sprintf(msg_buf, "%c,%s", action, argv[4]);
  // printf("%s\n",msg_buf);

  for (int i = 0; i < atoi(argv[5]); i++)
  {
    conn_fd = ClientSockect(argv[1], atoi(argv[2]));
    write(conn_fd, msg_buf, msg_cnt);
    close(conn_fd);
  }

  return 0;
} 

int ClientSockect(const char *host, int port)
{
  int s;
  struct sockaddr_in sin;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr(host);
  sin.sin_port = htons((unsigned short)port);
  s = socket(AF_INET, SOCK_STREAM, 0);
  connect(s, (struct sockaddr *)&sin, sizeof(sin));

  return s;
}