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

#include <string.h>
#include <stdbool.h>

#define MAXSIZE     128

void die(char *s){
  perror(s);
  exit(1);
}

struct msgbuf{
  long mtype;
  char mtext[MAXSIZE];
};

int main(){
  int input;
  int fd;
  char *myfifo="/tmp/myfifo";
  char entrada[100];
  printf("Ingrese numero : ");
  scanf("%d",&input);
  sprintf(entrada,"%d\n",input);
  mkfifo(myfifo,0666);
  fd=open(myfifo,O_WRONLY);
  write(fd,entrada,sizeof(entrada));
  close(fd);
  unlink(myfifo);

  int opcion;

  printf("Esperando resultado ...\n");
  printf("Ingrese 1 para obtener respuesta \n");
  scanf("%d",&opcion);

  if(opcion == 1){
    int msqid;
    key_t key;
    struct msgbuf rcvbuffer;
    key = 1234;
    
    if((msqid = msgget(key, 0666)) < 0){
      die("msgget()");
    }
    if(msgrcv(msqid, &rcvbuffer, MAXSIZE, 1, 0) < 0){
      die("msgrcv");
    }
    printf("Respuesta : %s\n",rcvbuffer.mtext);
    exit(0);
}

  return 0 ;
}
