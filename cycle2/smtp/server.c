#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#define port 2500
#define buff_size 1024
#define domain "smtp.server.com"

void main()
{
    int sockfd, clifd;
    struct sockaddr_in servaddr;
    char buff[buff_size];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("Socket creation error\n");
        exit(EXIT_FAILURE);
    }
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);
    printf("Listening\n");
    clifd = accept(sockfd, 0, 0);
    if (clifd < 0)
    {
        perror("Accept error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Accepted\n");
    }
    do
    {
        if (read(clifd, buff, buff_size) > 0)
        {
            if (strstr(buff, "HELO") != NULL)
            {
                printf("Message received:%s\n", buff);
                bzero(buff, buff_size);
                strcpy(buff, "250 Hello ");
                strcat(buff, domain);
                write(clifd, buff, strlen(buff));
            }
            else if (strstr(buff, "MAIL") != NULL || strstr(buff, "RCPT") != NULL)
            {
                printf("Message received:%s\n", buff);
                bzero(buff, buff_size);
                strcpy(buff, "250 OK ");
                strcat(buff, domain);
                write(clifd, buff, strlen(buff));
            }
            else if (strstr(buff, "DATA") != NULL)
            {
                printf("Message received:%s\n", buff);
                bzero(buff, buff_size);
                printf("Email received\n");
                bzero(buff, buff_size);
                strcpy(buff, "250 OK ");
                strcat(buff, domain);
                write(clifd, buff, strlen(buff));
            }
            else if (strstr(buff, "QUIT") != NULL)
            {
                break;
            }
        }

    } while (1);
    printf("Message received : %s", buff);
    bzero(buff, buff_size);
    strcpy(buff, "221 Bye");
    printf("SENT : %s\n\n", buff);
    int n = write(clifd, buff, strlen(buff));
    if (n < 0)
    {
        perror("Error occured while writing to socket!");
    }
    printf("\nConnection closed successfully with the client!\n\n");
}