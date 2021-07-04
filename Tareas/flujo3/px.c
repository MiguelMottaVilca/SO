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
#define MAXSIZE     128
#define SHMSZ     27

void die(char *s){
    perror(s);
    exit(1);
}

struct msgbuf{
    long   mtype;
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

    if(opcion==1){
        int shmid;
        key_t key;
        char *shm, *s;

        key = 5678;
        if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
            perror("shmget");
            exit(1);
        }

        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
        }

        for (s = shm; *s != NULL; s++)
            putchar(*s);
        putchar('\n');

        //printf("\nprocess3 : %s\n",s);
        *shm = '*';
        

        exit(0);
    }
    
}
