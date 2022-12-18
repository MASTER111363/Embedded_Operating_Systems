#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // memset
#include <signal.h>  // sigaction
#include <time.h>    // timespec, nanosleep
#include <sys/shm.h> // shmget, shmat, shmdt, shmctl

typedef struct{
  int guess;
  char result[8];
} data;

int answer;
int stop = 0;
data *dataPtr;

void sigHandler(int);
void intHandler(int);

int main(int argc, char *argv[]){
  int shmid;
  int ret = 1; 
  key_t key;
  struct sigaction sa;
  struct timespec timer;

  if (argc != 3){
    printf("Usage: %s <shm key> <answer>\n", argv[0]);
    exit(1);
  }

  key = atoi(argv[1]);
  answer = atoi(argv[2]);

  // Create shared memory
  if ((shmid = shmget(key, sizeof(data), IPC_CREAT | 0666)) < 0){
    perror("Error: shmget\n");
    exit(1);
  }
  if ((dataPtr = shmat(shmid, NULL, 0)) < 0){
    perror("Error: shmat\n");
    exit(1);
  }
  sprintf(dataPtr->result, "start");

  // Set timer
  memset(&timer, 0, sizeof(timer));
  timer.tv_sec = 60;

  // Register signal
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = sigHandler;

  sigaction(SIGUSR1, &sa, NULL);
  signal(SIGINT, intHandler);

  printf("PID = %d\n", getpid());

  while (ret & !stop){
    ret = nanosleep(&timer, &timer);
  }

  // Detach and delete shared memory
  shmdt(dataPtr);
  if(shmctl(shmid, IPC_RMID, NULL) < 0){
    perror("Error: shmctl\n");
    exit(1);
  }

  return 0;
}

void sigHandler(int signum){
  if(answer > dataPtr->guess){
    sprintf(dataPtr->result, "bigger");
  }else if(answer < dataPtr->guess){
    sprintf(dataPtr->result, "smaller");
  }else{
    sprintf(dataPtr->result, "bingo");
  }
  printf("[game] Guess %d, ", dataPtr->guess);
  printf("%s\n", dataPtr->result);
}

void intHandler(int signum){
  stop = 1;
}