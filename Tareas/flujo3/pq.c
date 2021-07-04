#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define MAXSIZE 128

int status = 1 ;
char buf[MAXSIZE];
long long numThread = 0;

void die(char *s){
  perror(s);
  exit(1);
}

struct msgbuf{
  long mtype;
  char mtext[MAXSIZE];
};

void *T1(void *vargp){
  while(status){}
  
  long long num = numThread;
  num /= 7;
  printf("num T1: %lld\n" , num);

  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key ;
  struct msgbuf sbuf;
  size_t buflen;

  key = 1234;
  if((msqid = msgget(key , msgflg)) < 0)
    die("msgget");

  sbuf.mtype = 1;
  sprintf(sbuf.mtext, "%lld\n", num);
  buflen = strlen(sbuf.mtext) + 1;
  
  if(msgsnd(msqid , &sbuf , buflen , IPC_NOWAIT) < 0){
    printf ("%d, %ld, %s, %d \n", msqid, sbuf.mtype, sbuf.mtext, (int)buflen);
    die("msgsnd"); 
  }
  else{
    printf("message send\n");
  }
  exit(0);
}

void *T2(void *vargp){

  int fd;
  char * myfifo = "/tmp/myfifoAns";
  char bufT2[1024];
  fd = open(myfifo , O_RDONLY);
  read(fd , bufT2 , 1024);
  long long num = atoll(bufT2);
  printf("num T2: %lld\n" ,num );
  num = num * num * num; //num ^3
  printf("num T2: %lld\n" ,num );
  numThread = num ;
  status = 0;
}

int main(){

  pthread_t thread1;
  pthread_t thread2;

  pthread_create(&thread1 , NULL, T1,NULL);
  pthread_create(&thread2 , NULL, T2,NULL);
  
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  return 0;
}
