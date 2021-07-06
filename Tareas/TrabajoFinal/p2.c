#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>
#include <sys/sem.h>
#include "queue.h"

#define MAX_BUF 256

int id;
int id2;
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

//px-pw
#define P1_2 0x1111
#define P2_1 0x1112

//pw-pw
#define P2_2_F 0x1113
#define P2_2_R 0x1114

//pw-pq
#define P2_3 0x1115
#define P3_2 0x1116


int main(){

    id = semget(P1_2, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    id2 = semget(P2_1, 1, 0666 | IPC_CREAT);
    if (id2 < 0){
        perror("semget");
        exit(11);
    }
 

  //int fdfifo;
  //char *myfifo = "/tmp/myfifo";
  //char buf[100];
    
    //////
    //fdfifo = open(myfifo,O_RDONLY);
    //printf("printf : \n");
   
    
  //int valor = atoi(buf);
  //valor = valor * 7;
  //printf("num : %d\n",valor);
 
  int fd[2],nbytes;
  pid_t childpid;
  char readbuffer[80];
  int c;
  pipe(fd);
  if((childpid = fork()) == -1){
    perror("fork");
    exit(1);
  }
  if(childpid == 0){
    
    close(fd[1]);

    char readbuffer[100];
    char process3[100];

    nbytes = read(fd[0],readbuffer,sizeof(readbuffer));
    int num = atoi(readbuffer);
    printf("num  : %d\n", num);
    num = num / 2;
    printf("num / 2 : %d\n", num);
    sprintf(process3,"%d\n", num);

    int fd2;
    char *myfifo2 = "/tmp/myfifoAns";
    mkfifo(myfifo2,0666);  
    fd2 = open(myfifo2,O_WRONLY);
    write(fd2,process3,sizeof(process3));        
    close(fd2);

  }
  else{

    while(1){
  
      char *myfifo3 = "/tmp/myfifo";
      char buf[100];
      char process2[100];
      printf("printf 1 : \n");
      int fifo = open(myfifo3,O_RDONLY);
      printf("printf 2: \n");

      //////////////////////////////////////////////////////////////
      if (semop(id2, &p, 1) < 0){
                    perror("semop p");
                    exit(15);
      }
      printf("printf 3 : \n");
      int n = read(fifo,buf,sizeof(buf));
      int num = atoi(buf);
      close(fifo);
      if (semop(id, &v, 1) < 0){
                    perror("semop p");
                    exit(15);
      }
        /////////////////////////////////////////////////////////////////

      printf("num  : %d\n", num);
      num = num * num ; // num ^ 2
      printf("num ^ 7 : %d\n", num);
      close(fd[0]);

      sprintf(process2,"%d\n",num);

      write(fd[1],process2,(sizeof(process2)));
    }
  } 

  return 0;
}
