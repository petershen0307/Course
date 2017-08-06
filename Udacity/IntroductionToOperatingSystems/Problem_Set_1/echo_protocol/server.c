#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void upper(char *);

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    if(argc != 2)
    {
        printf("\n Usage: %s <listen port on server> \n",argv[0]);
        return 1;
    }

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int port = atoi(argv[1]);
    serv_addr.sin_port = htons(port);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    //while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        printf("got connection\n");
        char recvBuff[1024];
        memset(recvBuff, 0, 1024);
        int res = read(connfd, recvBuff, sizeof(recvBuff)-1);
        printf("received:%s\n", recvBuff);
        upper(recvBuff);
        printf("tranform:%s\n", recvBuff);
        write(connfd, recvBuff, strlen(sendBuff));
        // ticks = time(NULL);
        // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        // write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
     }
}