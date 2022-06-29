#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 9000
#define MAXCLI 5
#define BUFFER_SIZE 1024

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

    char buf[100] = "GET /search/howsearchworks/?fg=1", retbuf[BUFFER_SIZE];

    int sent = send(sockfd, buf, sizeof(buf), 0);
    if (sent > 0)
    {
        printf("Message sent to server %s\n", buf);
    }
    for (;;)
    {
        int recvd = read(sockfd, retbuf, sizeof(retbuf));
        if (recvd > 0)
        {
            printf("\nMessage received from server %s\n", retbuf);
        }
    }
}