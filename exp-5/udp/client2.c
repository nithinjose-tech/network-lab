#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8000
#define CLIENTPORT 9000
#define MAXCLI 5

int main()
{
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
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
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_port = htons(CLIENTPORT);
    if(bind(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0){
        printf("Socket bind failed\n");
    }
    else{
        printf("Socket binded\n");
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    int res;

    memset(&res, 0, sizeof(res));
    for (;;)
    {
        int length = recvfrom(sockfd, &res, sizeof(res),0,0,0);
        if (length > 0)
        {
            printf("Message received from Server %d\n", res);
            break;
        }
    }
    close(sockfd);
}
