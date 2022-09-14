#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define sender "user@user.com"
#define domain "smtp.client.com"
#define port 2500
#define buff_size 1024

void main()
{

    int sockfd;
    struct sockaddr_in servaddr;
    struct hostent *server;
    char mail[100];
    char buff[buff_size];
    char recv_mail[100];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // server = gethostbyname("localhost");
    if (sockfd < 0
        //  || server == NULL
    )
    {
        perror("Socket creation or hostname error\r");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    // bzero((char *)&servaddr, sizeof(servaddr));
    // bcopy((char *)server->h_addr, (char *)&servaddr.sin_addr.s_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Connect error");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Connected\n");
    }
    char cmd[10];
    do
    {
        printf("Enter command\n");
        char code[4];
        scanf("%s", cmd);
        if (strcmp(cmd, "HELO") == 0)
        {
            bzero(buff, buff_size);
            strcpy(buff, "HELO ");
            strcat(buff, domain);
            strcat(buff, "\r\n");
            if (write(sockfd, buff, strlen(buff)) < 0)
            {
                perror("Write error");
                continue;
            }
            printf("Message sent: %s\n", buff);
            bzero(buff, buff_size);
            if (read(sockfd, buff, buff_size - 1) > 0)
            {
                printf("Message received: %s\n", buff);

                code[0] = buff[0];
                code[1] = buff[1];
                code[2] = buff[2];
                code[3] = '\0';

                if (strcmp(code, "250") == 0)
                {
                    printf("\nGo to next command...\n\n");
                }
                else
                {
                    printf("\nError occured!\n\n");
                }
                fflush(stdin);
            }
        }
        else if (strcmp(cmd, "MAIL") == 0)
        {
            bzero(buff, buff_size);

            strcpy(buff, "MAIL FROM:<");
            strcat(buff, sender);
            strcat(buff, ">");
            strcat(buff, "\r\n");
            if (write(sockfd, buff, strlen(buff)) < 0)
            {
                printf("\nError occured while writing to socket!\n");
                continue;
            }
            printf("Message sent : %s", buff);
            bzero(buff, buff_size);
            if (read(sockfd, buff, buff_size) > 0)
            {
                printf("Message received : %s\n", buff); // 250 OK

                // checking error
                code[0] = buff[0];
                code[1] = buff[1];
                code[2] = buff[2];
                code[3] = '\0';

                if (strcmp(code, "250") == 0)
                {
                    printf("\nGo to next command...\n\n");
                }
                else
                {
                    printf("\nError occured!\n\n");
                }
                fflush(stdin);
            }
        }
        else if (strcmp(cmd, "RCPT") == 0)
        {
            bzero(buff, buff_size);
            printf("\nEnter Receiver mail : ");
            scanf("%s", recv_mail);
            strcpy(buff, "MAIL TO:<");
            strcat(buff, recv_mail);
            strcat(buff, ">");
            strcat(buff, "\r\n");
            if (write(sockfd, buff, strlen(buff)) < 0)
            {
                printf("\nError occured while writing to socket!\n");
                continue;
            }
            printf("Message sent : %s", buff);
            bzero(buff, buff_size);
            if (read(sockfd, buff, buff_size) > 0)
            {
                printf("Message received : %s\n", buff); // 250 OK

                // checking error
                code[0] = buff[0];
                code[1] = buff[1];
                code[2] = buff[2];
                code[3] = '\0';

                if (strcmp(code, "250") == 0)
                {
                    printf("\nGo to next command...\n\n");
                }
                else
                {
                    printf("\nError occured!\n\n");
                }
                fflush(stdin);
            }
        }
        else if (strcmp(cmd, "DATA") == 0)
        {
            char sub[100], content[500];
            printf("Enter subject:");
            scanf("\n%[^\n]s", sub);
            printf("Enter content:");
            scanf("\n%[^\n]s", content);
            time_t t;
            time(&t);
            char *header = NULL;
            char date[26];
            char DATE_h[8 + strlen(date)];
            char sendr[8 + strlen(sender)];    // FROM: sender's_email\r\n
            char recep[6 + strlen(recv_mail)]; // TO: recepient's_email\r\n
            char subject[11 + strlen(sub)];
            char content_a[1 + strlen(content) + 2 + 1 + 1];
            strftime(date, (33), "%a %d %b %Y %H:%M:%S", localtime(&t));
            sprintf(DATE_h, "DATE: %s\r\n", date);
            sprintf(sendr, "FROM: %s\r\n", sender);
            sprintf(subject, "Subject: %s\r\n", sub);
            sprintf(recep, "TO: %s\r\n", recv_mail);
            // extra \n is used to end the header part

            sprintf(content_a, "%s\r\n", content);

            int header_length = strlen(DATE_h) + strlen(sendr) + strlen(subject) + strlen(recep) + strlen(content_a);

            header = (char *)malloc(header_length * sizeof(char));

            memcpy(&header[0], &DATE_h, strlen(DATE_h));
            memcpy(&header[0 + strlen(DATE_h)], &sendr, strlen(sendr));
            memcpy(&header[0 + strlen(DATE_h) + strlen(sendr)], &subject, strlen(subject));
            memcpy(&header[0 + strlen(DATE_h) + strlen(sendr) + strlen(subject)], &recep, strlen(recep));
            memcpy(&header[0 + strlen(DATE_h) + strlen(sendr) + strlen(subject) + strlen(recep)], &content_a, strlen(content_a));
            bzero(buff, buff_size);
            strcpy(buff, "DATA ");
            strcat(buff, header);
            if (write(sockfd, buff, strlen(buff)) < 0)
            {
                printf("\nError occured while writing to socket!\n");
                continue;
            }
            printf("Message sent email header and content : %s\n", buff);
            bzero(buff, buff_size);
            strcpy(buff, ".\r\n");
            if (write(sockfd, buff, strlen(buff)) < 0)
            {
                perror("Error during write\n");
            };

            if (read(sockfd, buff, buff_size) > 0)
            {
                printf("Message received : %s\n", buff); // 250 OK

                // checking error
                code[0] = buff[0];
                code[1] = buff[1];
                code[2] = buff[2];
                code[3] = '\0';

                if (strcmp(code, "250") == 0)
                {
                    printf("\nGo to next command...\n\n");
                }
                else
                {
                    printf("\nError occured!\n\n");
                }
                fflush(stdin);
            }
        }
        else if (strcmp(cmd, "QUIT") == 0)
        {
            bzero(buff, buff_size);
            strcpy(buff, "QUIT");
            strcat(buff, "\r\n");
            int n = write(sockfd, buff, strlen(buff));
            if (n < 0)
            {
                printf("\nError occured while writing to socket!\n");
            }
            printf("Message sent : %s", buff); // QUIT
            bzero(buff, buff_size);
            n = read(sockfd, buff, buff_size);
            if (n < 0)
            {
                printf("\nError occured while reading from socket!\n");
            }
            printf("Message sent : %s\n", buff); // 221 Bye

            // checking error
            code[0] = buff[0];
            code[1] = buff[1];
            code[2] = buff[2];
            code[3] = '\0';

            if (strcmp(code, "221") == 0)
            {
                printf("\nConnection closed successfully with SMTP Server!\n\n");
            }
            else
            {
                printf("\nError occured!\n\n");
            }
            fflush(stdin);
        }
    } while (strcmp(cmd, "QUIT") != 0);
}