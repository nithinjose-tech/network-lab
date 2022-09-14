#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

void main()
{
	struct sockaddr_in sock_addr,client_addr;
	int sockfd,clientfd;

	sockfd=socket(AF_INET,SOCK_STREAM,0);

	int opt;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))<0)
	{
		printf("Socket manipulation failed");
	}

	sock_addr.sin_family=AF_INET;
	sock_addr.sin_addr.s_addr=INADDR_ANY;
	sock_addr.sin_port=htons(8080);

	if(bind(sockfd,(struct sockaddr*)&sock_addr,sizeof(sock_addr))<0)
	{
		perror("Binding failed");
		exit(0);
	}

	listen(sockfd,5);

	int c=sizeof(struct sockaddr);
	clientfd=accept(sockfd,(struct sockaddr*)&client_addr,(socklen_t *)&c);

	if(clientfd<0)
	{
		perror("Accept failed");
		exit(0);
	}

	int n,incoming,outgoing,size;
	recv(clientfd,&n,sizeof(int),0);
	int a[n];
	recv(clientfd,&outgoing,sizeof(int),0);
	recv(clientfd,&size,sizeof(int),0);
	recv(clientfd,a,sizeof(int)*n,0);

	int current=0;

	for(int i=0;i<n;i++)
	{
		current+=a[i];
		if(current<=size)
		{
			printf("\n%d is added to the bucket size",current);

		}
		else
		{
			printf("\nPacket loss = %d",current-size);
		}
		if(current>0)
		{
			current-=outgoing;
			
			
		}
	}
}
