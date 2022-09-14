#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define port 2000
#define user "user"
#define password "password"

void main()
{
    int sockfd, logged_in = 0, opt = 1;
    FILE *fin, *fout;
    struct sockaddr_in servaddr;
    struct stat info;
    char buff[8 * 1024] = {'\0'};
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Socket bind error");
        exit(EXIT_FAILURE);
    }
    listen(sockfd, 5);
    printf("Listening\n");

    int clifd;

    if ((clifd = accept(sockfd, 0, 0)) > 0)
    {
        printf("Connection accepted\n");
    }
    do
    {
        bzero(buff, sizeof(buff));
        if (read(clifd, buff, sizeof(buff)) > 0)
        {
            printf("Client:%s\n", buff);
            if (!logged_in)
            {
                if (strstr(buff, "USER") != NULL)
                {
                    if (strstr(buff, user) != NULL)
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "331 User name okay,need a password");
                    }
                    else
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "430 Invalid username or password");
                    }
                    write(clifd, buff, strlen(buff));
                }

                if (strstr(buff, "PASS") != NULL)
                {
                    if (strstr(buff, password) != NULL)
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "200 Command okay");
                        logged_in = 1;
                    }
                    else
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "430 Invalid username or password");
                    }
                    write(clifd, buff, strlen(buff));
                }
            }
            // else
            //     {
            //         bzero(buff, sizeof(buff));
            //         strcpy(buff, "530 Not logged in");
            //         write(clifd, buff, strlen(buff));
            //         fflush(stdin);
            //     }
            else
            {
                if (strstr(buff, "RETR") != NULL)
                {

                    char filename[100] = {'\0'};
                    int k = 0;
                    for (int i = 5; buff[i] != '\0'; i++)
                    {
                        filename[k] = buff[i];
                        k++;
                    }
                    if ((fin = fopen(filename, "r")) == NULL)
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "550 File not found");
                        write(clifd, buff, strlen(buff));
                        perror("Couldn't read file");
                        continue;
                    }
                    else
                    {
                        stat(filename, &info);
                        int size = info.st_size;
                        if (size >= 8 * 1024)
                        {
                            strcpy(buff, "550 File too large");
                            write(clifd, buff, strlen(buff));
                            perror("File Size too large");
                            continue;
                        }
                        int i = 0;
                        char ch;
                        strcpy(buff, "200 Command Okay.");
                        write(clifd, buff, strlen(buff));
                        bzero(buff, sizeof(buff));
                        while (i <= size)
                        {
                            ch = fgetc(fin);
                            if (i == size)
                            {
                                printf("File:%s\n", buff);
                                write(clifd, buff, strlen(buff));
                            }
                            strncat(buff, &ch, 1);
                            i++;
                        }
                    }
                    fclose(fin);
                }
                if (strstr(buff, "STOR") != NULL)
                {

                    char filename[100] = {'\0'};
                    int k = 0;
                    for (int i = 5; buff[i] != '\0'; i++)
                    {
                        filename[k] = buff[i];
                        k++;
                    }
                    if ((fout = fopen(filename, "w+")) == NULL)
                    {
                        bzero(buff, sizeof(buff));
                        strcpy(buff, "550 File error");
                        write(clifd, buff, strlen(buff));
                        perror("Couldn't read file");
                        continue;
                    }
                    else
                    {
                        int i = 0;
                        char ch;
                        strcpy(buff, "200 Command Okay.");
                        write(clifd, buff, strlen(buff));
                        bzero(buff, sizeof(buff));
                        if (read(clifd, buff, sizeof(buff)) > 0)
                        {
                            fprintf(fout, "%s", buff);
                            fclose(fout);
                        }
                        printf("File retrieval completed successfully\n");
                    }
                }
            }
        }

    } while (strstr(buff, "QUIT") == NULL);
    close(clifd);
    printf("Connection closed\n");
}
