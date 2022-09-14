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
	sock_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	sock_addr.sin_port=htons(8080);


	int c=sizeof(struct sockaddr);
	accept(sockfd,(struct sockaddr*)&client_addr,(socklen_t *)&c);

	if((connect(sockfd,(struct sockaddr*)&sock_addr,sizeof(sock_addr)))<0)
	{
		perror("Accept failed");
		exit(0);
	}

	int n,incoming,outgoing,size;
	printf("Enter n,outgoing and size of bucket\n");
	scanf("%d%d%d",&n,&outgoing,&size);

	int a[n];

	for(int i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}

	send(sockfd,&n,sizeof(int),0);
	send(sockfd,&outgoing,sizeof(int),0);
	send(sockfd,&size,sizeof(int),0);
	send(sockfd,a,sizeof(int)*n,0);

}
