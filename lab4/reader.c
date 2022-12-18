#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
if(argc != 4) {
	printf("Usage: reader <server_ip> <port> <device_path>");
	exit(-1);
	}
	/* Socket client setup*/
	// setup
	int port = (u_short)atoi(argv[2]);
	int connfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in client_sin;
	memset(&client_sin, 0, sizeof(client_sin));
	client_sin.sin_family = AF_INET;
	client_sin.sin_addr.s_addr = inet_addr(argv[1]);
	client_sin.sin_port = htons(port);
	// connect

	if(connect(connfd, (struct sockaddr *) &client_sin, sizeof(client_sin)) == -1) {
		printf("Error connect to server\n");
		exit(-1);
	}
	int fd;
	if((fd = open(argv[3], O_RDWR)) < 0) {
		printf("Error open %s\n",argv[3]);
		exit(-1);
	}
	char buf[17] = {0};
	while(1) {
		sleep(1);
		if( read(fd, buf, 16) < 0) {
			printf("Error read %s\n",argv[3]);
			exit(-1);
		}
		// sent msg
		int n;
		if((n = write(connfd, buf, 16)) == -1) {
			printf("Error write to server\n");
			exit(-1);
		}
	}
	close(fd);
	close(connfd);
	return 0;
	}