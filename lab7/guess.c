#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // memset
#include <signal.h>   // sigaction
#include <time.h>    // timespec, nanosleep
#include <sys/time.h> // itimerval
#include <sys/shm.h>  // shmget, shmat, shmdt, shmctl

typedef struct{
  int guess;
  char result[8];
} data;

pid_t game_pid;
data *dataPtr;
int guess, upper, lower;
int stop = 0;

void timerHandler(int);

int main(int argc, char *argv[]){
  int shmid;
  key_t key;
  struct itimerval timer;
  struct sigaction sa;

  if (argc != 4){
    printf("Usage: %s <key> <upper_bound> <pid>\n", argv[0]);
    exit(1);
  }
  key = atoi(argv[1]);
  upper = atoi(argv[2]);
  game_pid = atoi(argv[3]);
  lower = 0;

  // Create shared memory
  if ((shmid = shmget(key, sizeof(data), 0666)) < 0){
    perror("Error: shmget\n");
    exit(1);
  } 
  if ((dataPtr = shmat(shmid, NULL, 0)) < 0){
    perror("Error: shmat\n");
    exit(1);
  }

  // Register signal
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = timerHandler;
  sigaction(SIGALRM, &sa, NULL);

  // Set timer
  memset(&timer, 0, sizeof(timer));
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL, &timer, NULL);

  while(!stop);

  // Detach shared memory
  shmdt(dataPtr);

  return 0;
}

void timerHandler(int signum){
  if(strcmp(dataPtr->result, "bingo") == 0){
    stop = 1;
    return;
  }else if(strcmp(dataPtr->result, "bigger") == 0){
    lower = dataPtr->guess;
  }else if(strcmp(dataPtr->result, "smaller") == 0){
    upper = dataPtr->guess;
  }else if(strcmp(dataPtr->result, "start") == 0){
  }else{
    printf("Invalid result: %s\n", dataPtr->result);
  }
  dataPtr->guess = (upper + lower)/2;
  printf("[game] Guess: %d\n", dataPtr->guess);

  // After guess
  kill(game_pid, SIGUSR1);
}