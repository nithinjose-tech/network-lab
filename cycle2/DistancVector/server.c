#include<netinet/in.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define port 8080
struct node
{
	unsigned dist[20];
	unsigned from[20];
};


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
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=INADDR_ANY;	
	
	if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		perror("Binding failed");
		exit(0);
	}
	
	
	listen(server_fd,5);
	
	int c=sizeof(struct sockaddr_in);
	
	client_fd=accept(server_fd,(struct sockaddr*)&client_addr,(socklen_t *)&c);
	
	if(client_fd<0)
	{
		perror("Accept failed");
		exit(0);
	}
	
	int n,i,j,k,count=0;
	recv(client_fd,&n,sizeof(int),0);
	struct node rt[n];
	int dmat[n][n];
	recv(client_fd,dmat,sizeof(int)*n*n,0);
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{	
			dmat[i][i]=0;
			rt[i].dist[j]=dmat[i][j];
			rt[i].from[j]=j;
		}
	}
	do
	{	
		count=0;
		for(i=0;i<n;i++)
		for(j=0;j<n;j++)
		for(k=0;k<n;k++)
			if(rt[i].dist[j]>dmat[i][k]+rt[k].dist[j])
			{
				rt[i].dist[j]=rt[i].dist[k]+rt[k].dist[j];
				rt[i].from[j]=k;
				count++;
			}
	}while(count!=0);


	send(client_fd,rt,sizeof(struct node)*n,0);
	for(i=0;i<n;i++)
	{
		printf("\n\nState value for router %d is \n",i+1);
		for(j=0;j<n;j++)
		{
			printf("\t\nnode %d via %d Distance %d ",j+1,rt[i].from[j]+1,rt[i].dist[j]);
		}
	}	
	printf("\n\n");
}
