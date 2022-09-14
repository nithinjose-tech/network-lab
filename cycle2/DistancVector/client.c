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
	printf("\nEnter the cost matrix :\n");
	int dmat[n[0]][n[0]];
	for(i=0;i<n[0];i++)
	{
		int c;
		for(j=0;j<n[0];j++)
		{	
			scanf("%d",&dmat[i][j]);
			
		}
	}

	struct node result[n[0]];
	send(server_fd,n,sizeof(int),0);
	send(server_fd,dmat,sizeof(int)*n[0]*n[0],0);
	recv(server_fd,result,sizeof(struct node)*n[0],0);
	for(i=0;i<n[0];i++)
	{
		printf("\n\nState value for router %d is \n",i+1);
		for(j=0;j<n[0];j++)
		{
			printf("\t\nnode %d via %d Distance %d ",j+1,result[i].from[j]+1,result[i].dist[j]);
		}
	}	
	printf("\n\n");
	
}
	