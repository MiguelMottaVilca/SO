#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

using namespace std ;
using namespace std::chrono;

#define KEY 0x1111

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

void sighandler(int); //2
void printq(int);     // 10

int id;
int maquina=10;


void Consumidor(){
    
    while(1){
    if (semop(id, &p, 1) < 0){
        perror("semop p");
        exit(13);
    }
    cout << "No Se puede comprar, consumidor en maquina" << '\n';
    std::this_thread::sleep_for(1200ms);
    if(maquina==0){
        cout << "No habia gaseosa, asu, tela" << endl;
    }
    else {
        maquina--;
        cout << "SE PUEDE COMPRAR" << '\n';
        cout << "Disponible : " << maquina << '\n';
    }

    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }
    }
}

void Encargado(){
    while(1){
      if(maquina==0){
      if (semop(id, &p, 1) < 0){
        perror("semop p");
        exit(13);
    }
    cout << "No se puede comprar Encargado llenando " << '\n';
    cout << " gaseosas +10, gosu" << '\n';
    std::this_thread::sleep_for(4000ms);
    maquina+=10;
    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }
      }
    }
}


int main(){


    id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0){
        perror("semget");
        exit(11);
    }

    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }

    std::thread first (Consumidor);

    std::thread second (Encargado);

    while (1){
        std::this_thread::sleep_for(1000ms);
    }

  return 0 ;
}

