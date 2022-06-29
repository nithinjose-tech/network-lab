#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8888
#define MAXCLI 5

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        exit(0);
    }
    else
    {
        printf("Socket created\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Connection establishment failed\n");
        exit(0);
    }
    else
    {
        printf("Connection established\n");
    }

    char sendbuf[100] = "hello", retbuf[100];
    send(sockfd, "start", strlen("start"), 0);
    int recvd = read(sockfd, retbuf, sizeof(retbuf));
    if (recvd > 0)
    {
        printf("Message received from server address %s\n", retbuf);
    }
    for (int i=0;i<20;i++)
    {
        int sent = send(sockfd, sendbuf, strlen(sendbuf), 0);
        memset(&retbuf, 0, sizeof(retbuf));
        int recvd = read(sockfd, retbuf, sizeof(retbuf));
        if (recvd > 0)
        {
            printf("Message received from server address %s\n", retbuf);
        }
        sleep(1);
    }
}