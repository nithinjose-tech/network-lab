#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int socket_connect(char *host, in_port_t port);
int create_socket(struct sockaddr_in servaddr, int maxcli);

void main(int argc, char *argv[])
{
    int fd, sockfd;
    struct sockaddr_in servaddr, cliaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(9000);

    char buffer[BUFFER_SIZE], buf[BUFFER_SIZE];

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <hostname> <port>\n", argv[0]);
        exit(1);
    }
    sockfd = create_socket(servaddr, 5);
    fd = socket_connect(argv[1], atoi(argv[2]));
    if (fd < 0)
    {
        printf("Cannot connect to remote server\n");
        exit(0);
    }

    int len = sizeof(cliaddr);
    int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    if (connfd < 0)
    {
        printf("Connection establishment failed\n");
    }
    else
    {
        printf("Connection established\n");
    }
    while (1)
    {
        printf("Waiting for client message\n");
        memset(&buf, 0, sizeof(buf));
        int recvd = read(connfd, buf, sizeof(buf));
        if (recvd > 0)
        {
            printf("\nMessage received from client %s\n", buf);
            char *req = strcat(buf, "\r\n");
            printf("HTTP Request: %s:%s %s\n", argv[1], argv[2], req);

            write(fd, req, strlen(req)); // write(fd, char[]*, len);
            bzero(buffer, BUFFER_SIZE);
            while(read(fd, buffer, BUFFER_SIZE - 1) != 0)
            {
                printf("HTTP Response: %s\n", buffer);
                send(connfd, buffer, sizeof(buffer), 0);
                bzero(buffer, BUFFER_SIZE);
            }
        }
    }
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

int socket_connect(char *host, in_port_t port)
{
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;

    if ((hp = gethostbyname(host)) == NULL)
    {
        herror("gethostbyname");
        exit(1);
    }
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));

    if (sock == -1)
    {
        perror("setsockopt");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
    {
        perror("connect");
        exit(1);
    }
    return sock;
}

int create_socket(struct sockaddr_in servaddr, int maxcli)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        exit(1);
    }
    else
    {
        printf("Socket created\n");
    }

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("Socket binding failed\n");
        exit(1);
    }
    else
    {
        printf("Socket binded\n");
    }

    if (listen(sockfd, maxcli) < 0)
    {
        printf("Listen failed\n");
        exit(1);
    }
    else
    {
        printf("Listening on port %d\n", ntohs(servaddr.sin_port));
    }
    return sockfd;
}
