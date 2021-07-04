#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>
//px
int main(){
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
}