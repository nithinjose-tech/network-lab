#include <unistd.h> 
#include<stdlib.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8000 
int main()
{
	int n;
	int sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_address;
	int new_socket,new_sock;
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(PORT);
	server_address.sin_addr.s_addr=INADDR_ANY;
	
	int bindres=bind(sock,(struct sockaddr*)&server_address,sizeof(server_address));
	if(bindres<0)
	{
		printf("bind failed");
		return -1;
	}
	listen(sock,3); 	 
	int addrlen=sizeof(server_address);
	new_socket=accept(sock,(struct sockaddr*)&server_address,&addrlen);
	while(1)
	{
		read(new_socket,&n,sizeof(int));
		if(n==-1)
			break;
		int data=rand()%2;
		printf("Received packet %d\n",n);
		send(new_socket,&data,sizeof(int),0);
	}
 	return 0;
}
