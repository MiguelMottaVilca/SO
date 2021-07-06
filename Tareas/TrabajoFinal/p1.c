#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>   /* time */
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/sem.h>
#include <stdbool.h>
#include "queue.h"
#define MAXSIZE     128
#define SHMSZ     27
#define SIGINT  2   /* Interrupt the process */ 
//#define SIZE 150
//px-pw
#define P1_2 0x1111
#define P2_1 0x1112

//pm-px
#define P4_1 0x1119
#define P1_4 0x1120

int id;//p1_2
int id2;//p2_1
 
  
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

void encolar_signal(){
    int n = rand() % 10 + 1;
    enqueue(n);
    printf("Caught signal %d, coming out...\n", n);
}

void sighandler(int signum){  // 2
    pthread_t encolar;
    pthread_create(&encolar , NULL, encolar_signal,NULL);
}

void controlar_shemaphore(void *vargp){
    int valor;
    while (1){
    if (Rear-Front>=1){//myqueue.size() > 0
            //int input;
            int fd;
            char *myfifo="/tmp/myfifo";
            char entrada[100];
            
            //scanf("%d",&input);
            //sprintf(entrada,"%d\n",input);
            mkfifo(myfifo,0666);
            printf("Espera en semaforo  : ");
            ////////////////////////////////////////////////////////////
            if (semop(id, &p, 1) < 0){
                perror("semop p");                              ///
                exit(15);
            }  
            ///////////////////////////////////////////////////////////////  
            printf("Entra al semaforo  : ");                                               ////
            fd=open(myfifo,O_WRONLY);
            valor = inp_arr[Front];
            dequeue();
            valor = valor * 7;
            printf("Enviando valor:  %d \n", valor);
            sprintf(entrada,"%d\n",valor);
            write(fd,entrada,sizeof(entrada));
            //std::cout << "Valor que se envia al FIFO :" << valor << std::endl;
            
            if (semop(id2, &v, 1) < 0){
                perror("semop p");
                exit(16);
            } 
            //////////////////////////////////////////////////////////////
            close(fd);
            unlink(myfifo);

        }
    }
}


int main(){

    // int id = semget(P1_2, 1, 0666 | IPC_CREAT);    
    // union semun u;
    // u.val = 1;
    // if (semctl(id, 0, SETVAL, u) < 0)
    // {
    //     perror("semctl");
    //     exit(12);
    // }

    signal(SIGINT, sighandler); // callback - eventos
    //signal(SIGUSR1, printq);    // callback - eventos
    srand(time(NULL));


    id = semget(P1_2, 1, 0666 | IPC_CREAT);
    if (id < 0){
        perror("semget");
        exit(11);
    }
    id2 = semget(P2_1, 1, 0666 | IPC_CREAT);
    if (id2 < 0){
        perror("semget");
        exit(11);
    }
    if (semop(id, &v, 1) < 0){
        perror("semop p");                              ///
        exit(15);
    }  

    pthread_t controlar;
    pthread_create(&controlar , NULL, controlar_shemaphore,NULL);

    int ejemplo;
    while (1){
        sleep(2);
        printf("COLA SIZE:  %d \n",Rear-Front);
    }
    return (0);

    
    //////////////////////////
    // int opcion;

    // printf("Esperando resultado ...\n");
    // printf("Ingrese 1 para obtener respuesta \n");
    // scanf("%d",&opcion);

    // if(opcion==1){
    //     int shmid;
    //     key_t key;
    //     char *shm, *s;

    //     key = 5678;
    //     if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
    //         perror("shmget");
    //         exit(1);
    //     }

    //     if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    //         perror("shmat");
    //         exit(1);
    //     }

    //     for (s = shm; *s != NULL; s++)
    //         putchar(*s);
    //     putchar('\n');

    //     //printf("\nprocess3 : %s\n",s);
    //     *shm = '*';
        

    //     exit(0);
    // }
    //////////////////////////////
}
