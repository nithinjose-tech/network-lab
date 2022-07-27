#include<netinet/in.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>

struct node
{
	unsigned dist[20];
	unsigned from[20];
};


#define port 8080

void main()
{
	int server_fd,client_fd;
	int opt=1;
	
	char msg[50];
	
	
	
	struct sockaddr_in server_addr,client_addr;
	
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	
	if(server_fd<0)
	{
		perror("Socket creation failed");
		exit(0);
	}
	
	if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))<0)
	{
		perror("Socket manipulation failed");
		exit(0);
	}
	
	
	//127.0.0.1
	//172.20.34.222
	//172.20.33.64
	
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	/*
	if(inet_pton(AF_INET,"172.20.33.64",&server_addr.sin_addr)<0)
	{
		perror("Connection failed");
		exit(0);
	}
	*/
	
	if(connect(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		perror("connect failed");
		exit(0);
	}
	int n[0],i,j,k;
	printf("\nEnter the number of nodes : ");
	scanf("%d",&n[0]);
	struct node rt[n[0]];
	printf("\nEnter the cost matrix (put 99 for infinity):\n");
	int dmat[n[0]][n[0]];
	for(i=0;i<n[0];i++)
	{
		int c;
		for(j=0;j<n[0];j++)
		{	
			scanf("%d",&dmat[i][j]);
			
		}
	}
	printf("\nEnter the root Node: \n");
	int s;
	scanf("%d",&s);
	struct node result[n[0]];
	send(server_fd,n,sizeof(int),0);
	send(server_fd,dmat,sizeof(int)*n[0]*n[0],0);
	send(server_fd,&s,sizeof(int),0);

	int dist[n[0]];

	recv(server_fd,dist,sizeof(int)*n[0],0);
	printf("Vertex \tDistance from Source %d\n",s);
	printf("---------------------------------\n");
    for (int i=0;i<n[0];i++)
        printf("%d \t %d\n", i, dist[i]);
	
	printf("\n\n");
	
}
	
/*output:
0 4 99 99 99 99 99 8 99
4 0 8 99 99 99 99 11 99
99 8 0 7 99 4 99 99 2
99 99 7 0 9 14 99 99 99
99 99 99 9 0 10 99 99 99
99 99 4 14 10 0 2 99 99
99 99 99 99 99 2 0 1 6
8 11 99 99 99 99 1 0 7
99 99 2 99 99 99 6 7 0
*/