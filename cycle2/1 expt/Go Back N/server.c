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
	int n1,n2,n3,n4;
	new_socket=accept(sock,(struct sockaddr*)&server_address,&addrlen);
	read(new_socket,&n1,sizeof(int));
	read(new_socket,&n2,sizeof(int));
	read(new_socket,&n3,sizeof(int));
	read(new_socket,&n4,sizeof(int));
	while(1)
	{
		int data=rand()%2;
		//printf("Received packet %d\n",n);
		send(new_socket,&data,sizeof(int),0);
		sleep(2);
	}
 	return 0;
}
