#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <wiringPi.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define D 0
#define C 1
#define B 2
#define A 3
#define G 4
#define DI 5
#define CLK 6
#define LAT 7
#define GPIO_PORT_1 12
#define GPIO_PORT_2 20
#define GPIO_PORT_3 21
#define GPIO_PORT_4 16

unsigned char buffer[2];
unsigned char chooseS[32] =
    {
        255,
        0b11111111,
        0b11000011,
        0b11011011,
        0b11011011,
        0b11000011,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11100111,
        0b11011011,
        0b11101111,
        0b11110111,
        0b11011011,
        0b11100111,
        0b11111111,
        0b11111111,
        0b11011101,
        0b11001001,
        0b11010101,
        0b11011101,
        0b11011101,
        0b11111111,
        0b11111111};
unsigned char chooseM[32] =
    {
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11000011,
        0b11011011,
        0b11011011,
        0b11000011,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11100111,
        0b11011011,
        0b11101111,
        0b11110111,
        0b11011011,
        0b11100111,
        0b11111111,
        0b11111111,
        0b11011101,
        0b11001001,
        0b11010101,
        0b11011101,
        0b11011101,
        0b11111111,
        0b11111111};
unsigned char empty[32] =
    {
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111,
        0b11111111};
unsigned char number[10][16] =
    {
        {
            0b11111111,
            0b11100011,
            0b11011101,
            0b11011101,
            0b11011101,
            0b11011101,
            0b11011101,
            0b11100011,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100111,
            0b11010111,
            0b11110111,
            0b11110111,
            0b11110111,
            0b11110111,
            0b11000001,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100111,
            0b11011011,
            0b11111011,
            0b11110111,
            0b11101111,
            0b11011111,
            0b11000011,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100111,
            0b11011011,
            0b11111011,
            0b11100111,
            0b11111011,
            0b11011011,
            0b11100111,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11110111,
            0b11100111,
            0b11010111,
            0b10110111,
            0b10000001,
            0b11110111,
            0b11110111,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11000001,
            0b11011111,
            0b11011111,
            0b11000011,
            0b11111101,
            0b11011101,
            0b11100011,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100011,
            0b11011111,
            0b11011111,
            0b11000011,
            0b11011101,
            0b11011101,
            0b11100011,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11000001,
            0b11011101,
            0b11111011,
            0b11110111,
            0b11110111,
            0b11110111,
            0b11110111,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100111,
            0b11011011,
            0b11011011,
            0b11100111,
            0b11011011,
            0b11011011,
            0b11100111,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }, {
            0b11111111,
            0b11100011,
            0b11011101,
            0b11011101,
            0b11100001,
            0b11111101,
            0b11011101,
            0b11100011,

            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
            0b11111111,
        }

};

int headX = 7, headY = 7, fruitX, fruitY, width = 17, height = 17;
int gameOver = 0, direction = 2, size = 0, score = 0, speed = 200000;
int bodyX[256], bodyY[256];

void render(void);
void buildMap(void);
void moveSnake(void);
void gameOverCheck(void);
void placeFruit(void);
void fruitCheck(void);
void fruitCheck_m(void);
void showScore(void);
void *rcvpos(void *);
void *inputButton(void *threadid);
void *outputLed(void *threadid);


pthread_t threads[5];
int rc;
void *status;
FILE *ptr;
char ch[4];
int mode = 1;
int sockfd = 0;
char ip[30];
char port[10];

// char map1616[16][16];
char map0832[8][32];
unsigned char dotMatrix[32];
unsigned char dotMatrix_iverse[32];
unsigned char dotMatrix_score[32];

void main(int argc, char **argv)
{
  /*input button and output led*/
  wiringPiSetup();
  wiringPiSetupGpio();
  rc = pthread_create(&threads[0], NULL, inputButton, NULL);
  rc = pthread_create(&threads[1], NULL, outputLed, NULL);

  /*setting connection ip and port*/
  memset(ip, '\0', sizeof(ip));
  memset(port, '\0', sizeof(port));
  strcpy(ip, argv[1]);
  strcpy(port, argv[2]);

  /*choose mode*/
  printf("1.Single\n2.Multiple\n");
  sleep(1);
  while (ch[1] == 1)
  {
    if (ch[0] == 0)
      mode = 1;
    else if (ch[2] == 0)
      mode = 2;
  }
  /*start single game*/
  if (mode == 1)
  {
    mode = 3;
    placeFruit();
    while (!gameOver)
    {
      // render();
      buildMap();
      moveSnake();
      fruitCheck();
      gameOverCheck();
      usleep(speed);
    }
  }
  /*start multiple game*/
  else if (mode == 2)
  {
      mode = 3;
    pthread_create(&threads[2], NULL, rcvpos, NULL);
    // placeFruit_m();
    fruitX = 5;
    fruitY = 5;
    while (!gameOver)
    {

      // render();
      buildMap();
      moveSnake();
      fruitCheck_m();
      gameOverCheck();
      usleep(speed);
    }
  }
  mode = 5;
  // system("clear");
  printf("Your final Score : %d\nThank You for playing!! (press any key to exit)\n", score);
  sleep(2);
  mode = 6;
  showScore();
  sleep(5);

  while (1)
  {
    
  }
  // system("clear");
  // pthread_exit(NULL);
  
}
void *rcvpos(void *dummy)
{
  char buf[50]={};

  int read_yes = 0;
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1)
  {
  printf("Fail to create a socket");
  }
    
  struct sockaddr_in info;
  bzero(&info, sizeof(info));
  info.sin_family = PF_INET;

  info.sin_addr.s_addr = inet_addr(ip);
  info.sin_port = htons((unsigned short)atoi(port));
  
  int err = connect(sockfd, (struct sockaddr *)&info, sizeof(info));
  if (err==-1)
  {
    printf("Connection error");
  }  

  while (1)
  {
    int tok_count = 0;
    memset(buf, '\0', sizeof(buf));

    read(sockfd, buf, sizeof(buf));
    
    char *ptr = strtok(buf, " ");

    while (ptr!=NULL)
    {
      if (tok_count == 0)
      {
        fruitX = atoi(ptr);
      }
      if (tok_count == 1)
      {
        fruitY = atoi(ptr);
      }

      ptr = strtok(NULL," ");
      tok_count++;
    }
    

    // read(sockfd, buf, sizeof(buf));
    
  }
}

void *inputButton(void *threadid)
{
  pinMode(GPIO_PORT_1, INPUT);
  pinMode(GPIO_PORT_2, INPUT);
  pinMode(GPIO_PORT_3, INPUT);
  pinMode(GPIO_PORT_4, INPUT);

  while (!gameOver)
  {
    ch[0] = digitalRead(GPIO_PORT_1);
    ch[1] = digitalRead(GPIO_PORT_2);
    ch[2] = digitalRead(GPIO_PORT_3);
    ch[3] = digitalRead(GPIO_PORT_4);

    if (direction != 3 && ch[0] == 0)
      direction = 1; // up
    else if (direction != 4 && ch[1] == 0)
      direction = 2; // right
    else if (direction != 1 && ch[2] == 0)
      direction = 3; // down
    else if (direction != 2 && ch[3] == 0)
      direction = 4;
  }
  // pthread_exit(NULL);
}

void *outputLed(void *threadid)
{
  pinMode(D, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LAT, OUTPUT);

  int count = 0;
  while (1)
  {
    if (mode == 1)
    {
      Display(chooseS);
    }
    else if (mode == 2)
    {
      Display(chooseM);
    }
    else if (mode == 3)
    {
      Display(dotMatrix);
    }
    else if (mode == 5)
    {
      count++;
      if (count < 3000)
      {
        Display(dotMatrix_iverse);
      }
      else if (count < 6000)
      {
        Display(dotMatrix);
      }
      else if (count < 9000)
      {
        Display(dotMatrix_iverse);
      }
      else if (count < 12000)
      {
        Display(dotMatrix);
      }
      else if (count < 15000)
      {
        Display(dotMatrix_iverse);
      }
      else if (count < 18000)
      {
        Display(dotMatrix);
      }
      else
      {
        Display(empty);
      }
    }
    else if (mode == 6)
    {
      Display(dotMatrix_score); 
    }
  }
  // pthread_exit(NULL);
}

void buildMap(void)
{
  // system("clear");
  int i, j, k, p;
  for (j = 1; j <= height - 1; j++)
  {
    for (i = 1; i <= width - 1; i++)
    {
      p = 1;
      if (i == 0 || j == 0 || j == height || i == width)
      {
        // printf("*");
      }
      else if (i == fruitX && j == fruitY)
      {
        // printf("0,");
        // map1616[i - 1][j - 1] = '0';
        if (i - 1 < 8)
          map0832[i - 1][j - 1] = '0';
        else
          map0832[i - 1 - 8][j - 1 + 16] = '0';
      }
      else if (i == headX && j == headY)
      {
        // printf("o,");
        // map1616[i - 1][j - 1] = '0';
        if (i - 1 < 8)
          map0832[i - 1][j - 1] = '0';
        else
          map0832[i - 1 - 8][j - 1 + 16] = '0';
      }
      else
      {
        for (k = 0; k < size; k++)
        {
          if (i == bodyX[k] && j == bodyY[k])
          {
            // printf("+,");
            // map1616[i - 1][j - 1] = '0';
            if (i - 1 < 8)
              map0832[i - 1][j - 1] = '0';
            else
              map0832[i - 1 - 8][j - 1 + 16] = '0';
            p = 0;
          }
        }
        if (p)
        {
          // printf("1,");
          // map1616[i - 1][j - 1] = '1';
          if (i - 1 < 8)
            map0832[i - 1][j - 1] = '1';
          else
            map0832[i - 1 - 8][j - 1 + 16] = '1';
        }
      }
    }
    // printf("\n");
  }

  // for (j = 0; j < 16; j++)
  // {
  //   for (i = 0; i < 16; i++)
  //   {
  //     if (i < 8)
  //       map0832[i][j] = map1616[i][j];
  //     else
  //       map0832[i - 8][j + 16] = map1616[i][j];
  //   }
  // }

  for (j = 0; j < 32; j++)
  {
    int c = 0;
    int c_inverse = 0;
    for (i = 0; i < 8; i++)
    {
      if (map0832[i][j] == '1')
      {
        int p = pow(2, (7 - i));
        c += p;
      }
      else
      {
        int p = pow(2, (7 - i));
        c_inverse += p;
      }
    }
    dotMatrix[j] = c;
    dotMatrix_iverse[j] = c_inverse;
  }
}

void showScore(void)
{ 
  int ten = score/10;
  int one = score%10;
  for (int j = 0; j < 32; j++)
  {
      if (j<16){
        dotMatrix_score[j] = number[ten][j];
      }else{
        dotMatrix_score[j] = number[one-1][j];
      }
  }

}

void placeFruit(void)
{
  srand((int)time(NULL));
  fruitX = rand() % (width - 1) + 1;
  fruitY = rand() % (height - 1) + 1;
}

void fruitCheck(void)
{
  if (headX == fruitX && headY == fruitY)
  {
    score += 1;
    size++;
    speed -= 4000;
    placeFruit();
  }
}

void fruitCheck_m(void)
{
  if (headX == fruitX && headY == fruitY)
  {
    score += 1;
    size++;
    speed -= 4000;
    // placeFruit_m();
    char flag[10];
    // flag[0] = '1';
    sprintf(flag, "1 %d", score);

    write(sockfd, flag, sizeof(flag));
  }
}



void moveSnake(void)
{
  int x1, x2, y1, y2, i;
  if (size == 1)
  {
    bodyX[0] = headX;
    bodyY[0] = headY;
  }
  else
  {
    x1 = headX;
    y1 = headY;
    for (i = 0; i < size; i++)
    {
      x2 = bodyX[i];
      y2 = bodyY[i];
      bodyX[i] = x1;
      bodyY[i] = y1;
      x1 = x2;
      y1 = y2;
    }
  }
  switch (direction)
  {
  case 1: // up
    headY--;
    break;
  case 2: // right
    headX++;
    break;
  case 3: // down
    headY++;
    break;
  case 4: // left
    headX--;
  }
}

void gameOverCheck(void)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (headX == bodyX[i] && headY == bodyY[i])
      gameOver = 1;
  }
  if (headX == width || headX == 0 || headY == height || headY == 0)
    gameOver = 1;
}

void Display(char dat[32])
{
  char i;
  for (i = 0; i < 16; i++)
  {
    digitalWrite(G, HIGH);
    buffer[0] = dat[i];
    buffer[1] = dat[i + 16];

    Send(buffer[1]);
    Send(buffer[0]);

    digitalWrite(LAT, HIGH);
    // nanosleep(1);
    digitalWrite(LAT, LOW);
    // nanosleep(1);
    scan(i);
    digitalWrite(G, LOW);
    // nanosleep(100);
  }
}

void scan(char m)
{
  switch (m)
  {
  case 0:
    digitalWrite(D, LOW);
    digitalWrite(C, LOW);
    digitalWrite(B, LOW);
    digitalWrite(A, LOW);
    break;
  case 1:
    digitalWrite(D, LOW);
    digitalWrite(C, LOW);
    digitalWrite(B, LOW);
    digitalWrite(A, HIGH);
    break;
  case 2:
    digitalWrite(D, LOW);
    digitalWrite(C, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(A, LOW);
    break;
  case 3:
    digitalWrite(D, LOW);
    digitalWrite(C, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(A, HIGH);
    break;
  case 4:
    digitalWrite(D, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(A, LOW);
    break;
  case 5:
    digitalWrite(D, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(A, HIGH);
    break;
  case 6:
    digitalWrite(D, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(A, LOW);
    break;
  case 7:
    digitalWrite(D, LOW);
    digitalWrite(C, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(A, HIGH);
    break;
  case 8:
    digitalWrite(D, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(B, LOW);
    digitalWrite(A, LOW);
    break;
  case 9:
    digitalWrite(D, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(B, LOW);
    digitalWrite(A, HIGH);
    break;
  case 10:
    digitalWrite(D, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(A, LOW);
    break;
  case 11:
    digitalWrite(D, HIGH);
    digitalWrite(C, LOW);
    digitalWrite(B, HIGH);
    digitalWrite(A, HIGH);
    break;
  case 12:
    digitalWrite(D, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(A, LOW);
    break;
  case 13:
    digitalWrite(D, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(B, LOW);
    digitalWrite(A, HIGH);
    break;
  case 14:
    digitalWrite(D, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(A, LOW);
    break;
  case 15:
    digitalWrite(D, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(A, HIGH);
    break;

  default:
    break;
  }
}

void Send(char dat)
{
  char i;
  digitalWrite(CLK, LOW);
  // nanosleep(10);
  digitalWrite(LAT, LOW);
  // nanosleep(10);
  for (i = 0; i < 8; i++)
  {
    if (dat & 0x01)
    {
      digitalWrite(DI, HIGH);
    }
    else
    {
      digitalWrite(DI, LOW);
    }
    // nanosleep(1);
    digitalWrite(CLK, HIGH);
    // nanosleep(1);
    digitalWrite(CLK, LOW);
    // nanosleep(1);
    dat >>= 1;
  }
}