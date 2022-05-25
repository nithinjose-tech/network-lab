#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8000
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

    int buf[10] = {9, 4, 5, 2, 88, 231, -323, 233, 232, 0}, res[3];
    int min, max, avg;

    int length = send(sockfd, buf, sizeof(buf), 0);
    if (length > 0)
    {
        printf("Array send to server \n");
        for (int i = 0; i < 10; i++)
        {
            printf("%d ", buf[i]);
        }
        printf("\n");
    }
    int recv_length = read(sockfd, res, 3 * sizeof(int));
    if (recv_length > 0)
    {
        printf("Maximum is %d\n", res[2]);
        printf("Minimum is %d\n", res[1]);
        printf("Average is %d\n", res[0]);
    }
    close(sockfd);
}