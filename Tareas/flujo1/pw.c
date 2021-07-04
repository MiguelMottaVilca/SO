#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
#define MAX_BUF 256
//p


int main(){
    int fdfifo;
    char *myfifo="/tmp/myfifo";
    char buf[100];
    fdfifo=open(myfifo,O_RDONLY);
    int n=read(fdfifo,buf,MAX_BUF);
    int valor=atoi(buf);
    valor=valor*7;
    printf("\nresu1 : %d\n",valor);


    int fd[2],nbytes;
    pid_t childpid;
    char readbuffer[80];
    int c;
    pipe(fd);
    if((childpid=fork())==-1){
        perror("fork");
        exit(1);
    }
    if(childpid==0){
        
        close(fd[1]);

        char readbuffer[100];
        char process3[100];
  

        nbytes=read(fd[0],readbuffer,sizeof(readbuffer));
        int valor3=atoi(readbuffer);
        valor3=valor3/2;
        printf("\nresu : %d\n",valor3);
        sprintf(process3,"%d\n",valor3);
        printf("\nprocess3 : %s\n",process3);
        int fd2;
        char *myfifo2="/tmp/myfifo1";
        //mkfifo(myfifo2,0666);  
        fd2=open(myfifo2,O_WRONLY);
        write(fd2,process3,sizeof(process3));        
        close(fd2);
    }
    else{
    char *myfifo3="/tmp/myfifo";
    char buf[100];
    char process2[100];
    int fifo=open(myfifo3,O_RDONLY);
    int n=read(fifo,buf,sizeof(buf));
    int valor2=atoi(buf);
    
    close(fifo);
    
    valor2=valor*valor;
    
    close(fd[0]);
    
    sprintf(process2,"%d\n",valor2);

    write(fd[1],process2,(sizeof(process2)));
    }
}