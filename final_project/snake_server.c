#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <pthread.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <arpa/inet.h>

int sockfd;
int forClientSockfd;
volatile int stop = 0;

int width = 17;
int height = 17;
int fruitX, fruitY;
int FLAG = 0;

void *fruit_handler(void *);
void *sender(void *);
void *gen_pos(void *);

pthread_t thread1[3];
int len[5];
int sem;

int main(int argc, char *argv[])
{

    // creating socket
    int yes = 1;
    char inputBuffer[1024] = {};
    char message[] = {"Hi, this is server.\n"};

    // int sockfd = 0, forClientSockfd=0;
    int sockfd = 0, forClientSockfd = 0;

    if (sockfd == -1)
    {
        printf("Fail to create a socket.");
    }

    struct sockaddr_in serverInfo, clientInfo;
    socklen_t addrlen = sizeof(clientInfo);

    // memset(&serverInfo, 0, sizeof(serverInfo));
    bzero(&serverInfo, sizeof(serverInfo));

    // serverInfo.sin_family = AF_INET;
    serverInfo.sin_family = PF_INET;
    // serverInfo.sin_addr.s_addr = INADDR_ANY;

    // sockfd = socket(PF_INET, SOCK_STREAM, 0);
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
    serverInfo.sin_port = htons((unsigned short)atoi(argv[2]));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    bind(sockfd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
    listen(sockfd, 3);

    // signal(SIGINT, CLShandler);

    while (!stop)
    {
        forClientSockfd = accept(sockfd, (struct sockaddr *)&clientInfo, &addrlen);
        // send(forClientSockfd, message, sizeof(message),0);
        // recv(forClientSockfd, inputBuffer, sizeof(inputBuffer),0);
        // printf("Get:%s\n",inputBuffer);

        pthread_create(&thread1[0], NULL, fruit_handler, (void *)&forClientSockfd);
        pthread_create(&thread1[1], NULL, gen_pos, NULL);
        pthread_create(&thread1[2], NULL, sender, (void *)&forClientSockfd);

        // signal(SIGINT, intHandler);
    }

    // signal(SIGCHLD, ZBhandler);

    close(sockfd);
    return 0;
}

void *sender(void *clientskfd)
{
    int sock = *(int *)clientskfd;
    while (1)
    {   
        // len[sock] = ;
        // sleep(1);
        if (FLAG == 1)
        {
            printf("Send pos \n");
            // srand((int)time(NULL));
            char fruitpos[10] = {};
            memset(fruitpos, '\0', sizeof(fruitpos));
            // fruitX = rand() % (width - 1) + 1;
            // fruitY = rand() % (height - 1) + 1;
            // break;
            sprintf(fruitpos, "%d %d\n", fruitX, fruitY);
            write(sock, fruitpos, strlen(fruitpos));
            printf("sent fruit position %s\n", fruitpos);
            FLAG = 0;
        }
    }
}

void *fruit_handler(void *clientskfd)
{
    int sock = *(int *)clientskfd;
    // int sock = forClientSockfd;
    int read_size;
    char flag_msg[10], fruitpos[100];
    memset(flag_msg, '\0', sizeof(flag_msg));
    memset(fruitpos, '\0', sizeof(fruitpos));

    while (1)
    {
        memset(flag_msg, '\0', sizeof(flag_msg));
        memset(fruitpos, '\0', sizeof(fruitpos));
        read(sock, flag_msg, sizeof(flag_msg));
        if (flag_msg[0] == '1')
        {

            if (sem == 0){
                sem = 1;
                FLAG = 1;
                sem = 0;
            }else{
                char temp[10];
                strcpy(temp, flag_msg);
                char *flgptr = strtok(temp, " ");
                flgptr = strtok(NULL, " ");
                len[sock] = atoi(flgptr);
                int min = 0;
                for (int k=0; k<5; k++)
                {
                    if (min>len[k])
                    {
                        min = len[k];
                    }
                }            
                if (len[sock]==min){
                    FLAG = 1;
                }
            }
            
            

            printf("A snake got a berry\n");
        }
        // if (FLAG == 1)
        // {
        //     printf("Send pos \n");
        //     // srand((int)time(NULL));
        //     // fruitX = rand() % (width - 1) + 1;
        //     // fruitY = rand() % (height - 1) + 1;
        //     // break;
        //     sprintf(fruitpos, "%d %d\n", fruitX, fruitY);
        //     write(sock, fruitpos, strlen(fruitpos));
        //     printf("sent fruit position (%s)\n", fruitpos);
        //     FLAG = 0;
        // }
    }

    // char mesg[] = "Hello this thread receive your message\n";

    // write(sock, mesg, strlen(mesg));
    // memset(mesg, '\0', sizeof(mesg));
    // read(sock, buffer, sizeof(buffer));
    // printf("%s",buffer);

    // while (read_size = read(sock, buffer, 100) > 0)
    // {
    // srand((int)time(NULL));
    // fruitX = rand() % (width - 1) + 1;
    // fruitY = rand() % (height - 1) + 1;

    // signal(SIGINT, intHandler);
    // }
    // read(sock, buffer, sizeof(buffer));
    // printf("receive rpi's message: %s\n", buffer);
    // pthread_exit(NULL);
}

void *gen_pos(void *arg)
{
    while (1)
    {

        srand((int)time(NULL));
        fruitX = rand() % (width - 1) + 1;
        fruitY = rand() % (height - 1) + 1;
        usleep(200000);
    }
}
