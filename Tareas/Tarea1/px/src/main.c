#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x1111

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};


int main(){

    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0){
        perror("semget");
        exit(11);
    }
    union semun u;
    u.val = 1;
    if (semctl(id, 0, SETVAL, u) < 0){
        perror("semctl");
        exit(12);
    }


    if (semop(id, &p, 1) < 0){
        perror("semop p");
        exit(13);
    }

    int input;
    int fd;
    char *myfifo="/tmp/myfifo";
    char entrada[100];
    scanf("%d",&input);
    sprintf(entrada,"%d\n",input);
    mkfifo(myfifo,0666);
    fd=open(myfifo,O_WRONLY);
    write(fd,entrada,sizeof(entrada));
    close(fd);
    unlink(myfifo);
    
    char *myfifo1="/tmp/myfifo1";
    char buf[100];
    char process2[100];
    int fifo=open(myfifo1,O_RDONLY);
    int n=read(fifo,buf,sizeof(buf));
    printf("respuesta %s\n",buf);
    close(fifo);


    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }
}
