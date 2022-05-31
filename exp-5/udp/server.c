#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8000
#define CLIENT2PORT 9000
#define MAXCLI 5

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cliaddr;
    pid_t childpid;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Socket binding failed\n");
        exit(0);
    }
    else
    {
        printf("Socket binded\n");
    }
    int num,sq;
    int val = recvfrom(sockfd, &num, sizeof(num),0,0,0);
    if (val > 0)
    {
        printf("Message received from client 1 %d\n", num);
        // break;
    }
    sq=num*num;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_port = htons(CLIENT2PORT);
    val = sendto(sockfd,&sq , sizeof(sq), 0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    if (val > 0)
    {
        printf("Message send to client 2 %d\n", sq);
    }
    close(sockfd);
}

// }
