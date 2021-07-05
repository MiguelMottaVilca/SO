#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define MAXSIZE     128
#define SHMSZ     27
#define KEY 0x111


union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

void die(char *s){
    perror(s);
    exit(1);
}

struct msgbuf{
    long   mtype;
    char mtext[MAXSIZE];
};

int main(){
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0){
        perror("semget");
        exit(11);
    }
    union semun u;
    u.val = 1; // val semaforo
    if (semctl(id, 0, SETVAL, u) < 0){
        perror("semctl");
        exit(12);
    }

    int input;
    int fd;
    char *myfifo="/tmp/myfifo";
    char entrada[100];
    printf("Ingrese numero : ");
    scanf("%d",&input);
    sprintf(entrada,"%d\n",input);
    mkfifo(myfifo,0666);
    fd=open(myfifo,O_WRONLY);
    //
    if (semop(id, &p, 1) < 0){
        perror("semop p");
        exit(13);
    }
    write(fd,entrada,sizeof(entrada));
    //
    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }
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
