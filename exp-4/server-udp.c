#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <limits.h>

#define PORT 8000
#define MAXCLI 5

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cliaddr;
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

    memset(&cliaddr, 0, sizeof(cliaddr));
    len = sizeof(cliaddr);

    int buf[10], res[3];

    for (;;)
    {
        memset(&buf, 0, sizeof(buf));
        int val = recvfrom(sockfd, buf, 10 * sizeof(int), 0, (struct sockaddr *)&cliaddr, &len);
        res[0] = 0;
        res[1] = INT_MAX;
        res[2] = INT_MIN;
        if (val > 0)
        {
            printf("Message received from client \n");
            for (int i = 0; i < 10; i++)
            {
                if (buf[i] < res[1])
                {
                    res[1] = buf[i];
                }
                if (buf[i] > res[2])
                {
                    res[2] = buf[i];
                }
                res[0] += buf[i];
            }
            res[0] /= 10;
            sendto(sockfd, res, 3 * sizeof(int), MSG_CONFIRM, (struct sockaddr *)&cliaddr, len);
        }
    }
}