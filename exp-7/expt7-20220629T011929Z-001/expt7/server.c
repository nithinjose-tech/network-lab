#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT 8888
#define MAXCLI 30
#define BUFFER_SIZE 1024

void main()
{
    int master_sockfd, activity, read_len, addrlen, max_sd = 0, on = 1;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[BUFFER_SIZE];
    int client_sockfd[MAXCLI], msg_freq[MAXCLI], conn_started[MAXCLI];
    char *start_msg = "Welcome";
    char *stop_msg = "Goodbye";
    char *max_freq_msg = "Message limit reached.Disconnecting...";

    fd_set read_sockfds; // Socket descriptors set

    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    for (int i = 0; i < MAXCLI; i++)
	{
		client_sockfd[i]=0;
        msg_freq[i]=0;
        conn_started[i]=0;
	}

    master_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (master_sockfd < 0)
    {
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket created\n");
    }
    if (setsockopt(master_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
    {
        perror("Socket config failed\n");
        exit(EXIT_FAILURE);
    }

    if (bind(master_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Socket binding failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Socket binded\n");
    }

    if (listen(master_sockfd, MAXCLI) < 0)
    {
        perror("Socket listen failed\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Listening on port %d\n", ntohs(servaddr.sin_port));
    }
    while (1)
    {
        FD_ZERO(&read_sockfds);               // Reset socket set
        FD_SET(master_sockfd, &read_sockfds); // Add master socket to set
        max_sd = master_sockfd;
        for (int i = 0; i < MAXCLI; i++)
        { // Add active client sockets to set
            if (client_sockfd[i] > 0)
            {
                FD_SET(client_sockfd[i], &read_sockfds);
            }
            if (client_sockfd[i] > max_sd)
            {
                max_sd = client_sockfd[i];
            }
        }
        activity = select(max_sd + 1, &read_sockfds, NULL, NULL, NULL); // Returns number of ready descriptors - last parameter is timeout which is NULL, so indefinite wait

        if (activity < 0 && errno != EINTR)
        {
            perror("Error with select system call\n");
        }
        if (FD_ISSET(master_sockfd, &read_sockfds))
        {
            int new_sockfd;
            if ((new_sockfd = accept(master_sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&addrlen)) < 0)
            {
                perror("Accept error\n");
                exit(EXIT_FAILURE);
            }
            printf("Incoming connection, sockfd is %d, address is %s:%d\n", new_sockfd, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
            // add new socket to array of sockets
            for (int i = 0; i < MAXCLI; i++)
            {
                // if position is empty
                if (client_sockfd[i] == 0)
                {
                    client_sockfd[i] = new_sockfd;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }
        for (int i = 0; i < MAXCLI; i++)
        {
            int sd = client_sockfd[i];

            if (FD_ISSET(sd, &read_sockfds))
            {
                // Check if it was for closing , and also read the incoming message
                if ((read_len = read(sd, buffer, 1024)) == 0) // Closing connection
                {
                    getpeername(sd, (struct sockaddr *)&cliaddr,
                                (socklen_t *)&addrlen);
                    printf("Host disconnected , address %s:%d \n",
                           inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                    // Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_sockfd[i] = 0;
                    msg_freq[i] = 0;
                    conn_started[i] = 0;
                }
                else
                {
                    buffer[read_len] = '\0';
                    if (conn_started[i] == 0)
                    {
                        if (strcmp(buffer, "start") == 0)
                        {
                            send(sd, start_msg, strlen(start_msg), 0);
                            conn_started[i] = 1;
                        }
                    }
                    else
                    {
                        if (strcmp(buffer, "stop") == 0)
                        {
                            send(sd, stop_msg, strlen(stop_msg), 0);
                            printf("Host disconnected , address %s:%d \n",
                                   inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                            close(sd);
                            client_sockfd[i] = 0;
                            msg_freq[i] = 0;
                            conn_started[i] = 0;
                        }
                        else
                        {
                            send(sd, buffer, strlen(buffer), 0);
                            msg_freq[i]++;
                        }
                        if (msg_freq[i] > 25) // Max message frequency reached
                        {
                            send(sd, max_freq_msg, strlen(max_freq_msg), 0);
                            printf("Max message frequency, client disconnected , address %s:%d \n",
                                   inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                            close(sd);
                            client_sockfd[i] = 0;
                            msg_freq[i] = 0;
                            conn_started[i]=0;
                        }
                    }
                }
            }
        }
    }
}
