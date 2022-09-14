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
    int sockfd, user_corr = 0, pass_corr = 0;
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
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Socket connect error");
        exit(EXIT_FAILURE);
    }
    char cmd[4], code[4];
    do
    {
        printf("Enter command\n");
        scanf("%s", cmd);

        if (!pass_corr)
        {
            if (strcmp(cmd, "USER") == 0 && user_corr == 0)
            {
                bzero(buff, sizeof(buff));
                strcpy(buff, "USER ");
                strcat(buff, user);
                write(sockfd, buff, strlen(buff));
                if (read(sockfd, buff, sizeof(buff)) > 0)
                {
                    printf("Server: %s\n", buff);
                    code[0] = buff[0];
                    code[1] = buff[1];
                    code[2] = buff[2];
                    code[3] = '\0';
                    if (strcmp(code, "331") == 0)
                    {
                        user_corr = 1;
                    }
                }
                fflush(stdin);
            }

            if (strcmp(cmd, "PASS") == 0 && user_corr == 1)
            {
                bzero(buff, sizeof(buff));
                strcpy(buff, "PASS ");
                strcat(buff, password);
                write(sockfd, buff, strlen(buff));
                if (read(sockfd, buff, sizeof(buff)) > 0)
                {
                    printf("Server: %s\n", buff);
                    code[0] = buff[0];
                    code[1] = buff[1];
                    code[2] = buff[2];
                    if (strcmp(code, "200") == 0)
                    {
                        pass_corr = 1;
                    }
                }
                fflush(stdin);
            }
        }
        else
        {
            if (strcmp(cmd, "RETR") == 0)
            {
                char filename[20] = {'\0'};
                printf("Enter filename\n");
                scanf("%s", filename);
                bzero(buff, sizeof(buff));
                strcpy(buff, "RETR ");
                strcat(buff, filename);

                write(sockfd, buff, strlen(buff));
                if (read(sockfd, buff, sizeof(buff)) > 0)
                {
                    printf("Server: %s\n", buff);
                    code[0] = buff[0];
                    code[1] = buff[1];
                    code[2] = buff[2];
                    code[3] = '\0';
                    if (strcmp(code, "200") == 0)
                    {
                        fout = fopen(filename, "w+");
                        bzero(buff, sizeof(buff));
                        if (read(sockfd, buff, sizeof(buff)) > 0)
                        {
                            fprintf(fout, "%s", buff);
                            fclose(fout);
                        }
                        printf("File retrieval completed successfully\n");
                    }
                }
                fflush(stdin);
            }
            if (strcmp(cmd, "STOR") == 0)
            {
                char filename[20] = {'\0'};
                printf("Enter filename\n");
                scanf("%s", filename);
                bzero(buff, sizeof(buff));
                strcpy(buff, "STOR ");
                strcat(buff, filename);

                if ((fin = fopen(filename, "r")) == NULL)
                {
                    printf("File not found\n");
                    continue;
                }
                else
                {
                    write(sockfd, buff, strlen(buff));
                    if (read(sockfd, buff, sizeof(buff)) > 0)
                    {
                        printf("Server: %s\n", buff);
                        code[0] = buff[0];
                        code[1] = buff[1];
                        code[2] = buff[2];
                        code[3] = '\0';
                        if (strcmp(code, "200") == 0)
                        {
                            stat(filename, &info);
                            int size = info.st_size;
                            if (size >= 8 * 1024)
                            {
                                perror("File Size too large");
                                continue;
                            }
                            int i = 0;
                            char ch;
                            bzero(buff, sizeof(buff));
                            while (i <= size)
                            {
                                ch = fgetc(fin);
                                if (i == size)
                                {
                                    printf("File:%s\n", buff);
                                    write(sockfd, buff, strlen(buff));
                                }
                                strncat(buff, &ch, 1);
                                i++;
                            }
                        }
                    }
                }
                fflush(stdin);
                fclose(fin);
            }
        }
        if (strcmp(cmd, "QUIT") == 0)
        {
            write(sockfd, "QUIT", 4);
            // if (read(sockfd, buff, sizeof(buff)) > 0)
            // {
            //     printf("Server:%s\n", buff);
            //     code[0] = buff[0];
            //     code[1] = buff[1];
            //     code[2] = buff[2];
            //     if (strcmp(code, "200") == 0)
            //     {
            //         printf("Connection closed successfully");
            //     }
            // }
        }
    } while (strcmp(cmd, "QUIT") != 0);
}