#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // fork()
#include <signal.h>  // signal()
#include <pthread.h> // pthread_create, pthread_join, pthread_exit
#include <sys/wait.h>
#include <sys/sem.h>
#include <fcntl.h>  // open()
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> // socket(), connect()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <errno.h>

#define SEM_KEY 123456789
#define SEM_MODE 0666

int total_amount = 0;
void counting(int conn_fd);
int ServerSockect(int port);

int sem;
int P(int s);
int V(int s);

int sock_fd;
int stop = 0;
void interrupt(int signum);

int main(int argc, char **argv)
{
  int conn_fd;
  struct sockaddr_in cln_addr;
  socklen_t sLen = sizeof(cln_addr);
  pthread_t thread;

  if (argc != 2){
    printf("Usage: %s <port>\n", argv[0]);
    exit(-1);
  }

  sock_fd = ServerSockect(atoi(argv[1]));
  if (sock_fd < 0){
    perror("Error create socket\n");
    exit(-1);
  }

  // Creating semaphore
  sem = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | SEM_MODE);
  if (sem < 0){
    perror("Error create semaphore\n");
    exit(-1);
  }else{
    printf("Semaphore %d created\n", SEM_KEY);
  }
  
  // Initializing semaphore sem[0] value to 1
  if (semctl(sem, 0, SETVAL, 1) < 0){
    perror("Error initialized semaphore\n");
    exit(-1);
  }else{
    printf("Semaphore %d has been initialized to 1\n", SEM_KEY);
  }

  signal(SIGINT, interrupt);

  while (!stop){
    conn_fd = accept(sock_fd, (struct sockaddr *)&cln_addr, &sLen);
    if (conn_fd == -1){
      perror("Error socket accept()");
      continue;
    }
  
    if (pthread_create(&thread, NULL, (void *(*)(void *))counting, (void *)conn_fd)){
      perror("Error pthread_create()\n");
    }
  }
  printf("closing sock_fd\n");
  close(sock_fd);

  return 0;
}

void counting(int conn_fd){
  char rcv[50];
  int n;
  
  while ((n = read(conn_fd, rcv, 50)) != 0){
    P(sem);
    if (rcv[0] == 'D'){
      total_amount += atoi(rcv + 2);
      printf("After deposit: %d\n", total_amount);
    }else if (rcv[0] == 'W'){
      total_amount -= atoi(rcv + 2);
      printf("After withdraw: %d\n", total_amount);
    }
    V(sem);
  }

  close(conn_fd);
  pthread_exit(NULL);
  return;
}

// wait 
int P(int s){
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_op = -1;
  sop.sem_flg = 0;

  if(semop(s, &sop, 1) < 0){
    perror("P(): semop failed\n");
    return -1;
  }else{
    return 0;
  }
}

// signal
int V(int s){
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_op = 1;
  sop.sem_flg = 0;

  if(semop(s, &sop, 1) < 0){
    perror("V(): semop failed\n");
    return -1;
  }else{
    return 0;
  }
}

void interrupt(int signum){
  stop = 1;
  close(sock_fd);
  printf("remove semaphore\n");
  if(semctl(sem, 0, IPC_RMID, 0) < 0){
    perror("Error removing semaphore\n");
    exit(-1);
  }
}

int ServerSockect(int port)
{
  int s, yes = 1;
  struct sockaddr_in sin;

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons((unsigned short)port);
  s = socket(PF_INET, SOCK_STREAM, 0);
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  bind(s, (struct sockaddr *)&sin, sizeof(sin));
  listen(s, 10);

  return s;
}