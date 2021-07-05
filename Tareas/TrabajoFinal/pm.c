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
#define MAXSIZE 128
#define SHMSZ 27
#define SIZE 256
double pi = 3.1416;

void die(char *s){
    perror(s);
    exit(1);
}

struct msgbuf{
    long   mtype;
    char mtext[MAXSIZE];
};

void shareMemory(){
    int msqid;
    key_t key2;
    struct msgbuf rcvbuffer;
    long long rpta;
    key2=1234;

    if((msqid = msgget(key2,0666)) < 0){
        die("msgrcv");
    }
    if(msgrcv(msqid,&rcvbuffer,MAXSIZE,1,0) < 0){
        die("msgrcv");
    }
    //printf("rpta : %s\n",rcvbuffer.mtext); // string 
    rpta = atoll(rcvbuffer.mtext);
    printf("rpta : %lld\n",rpta); // long long

    long long a = (long long)rpta;
    printf("res : %lld\n" , a) ;
    
    int shmid;
    key_t key;
    char *shm , *s;
    key = 5678;

    if((shmid=shmget(key,SHMSZ,IPC_CREAT|0666)) < 0){
        perror("shmget");
        exit(1);
    }

    if((shm = shmat(shmid,NULL,0)) == (char *) -1){
        perror("shmat");
        exit(1);
    }

    s = shm;
    //a = a + pi ;
    a = a + 3 ;  // problemas con .1416 
    printf("res += 3 : %lld\n" , a) ;

    char newC[SIZE];
    sprintf(newC,"%lld",a);   
    printf("res char* : %s\n" , newC);
   

    int i;
    for(i = 0 ; i < sizeof(newC) ; i++){
        *s = newC[i];
        s++;
    }
    *s = NULL ;
    while(*shm != '*'){
        sleep(1);
    }
    exit(0); 
}

int main(){

    shareMemory();

    return 0 ;
}
