#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

void main(){
	
	int server_fd, client_fd;
	char msg[100];
	
	struct sockaddr_in server_addr, client_addr;
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
		printf("connection error");
	}
	
	int c = sizeof(server_addr);
	
	sendto(server_fd, "time", sizeof("time"), MSG_CONFIRM, (struct sockaddr*)&server_addr, c);
	recvfrom(server_fd, msg, 100, MSG_WAITALL, (struct sockaddr*)&client_addr,&c);
	printf("sytem time %s", msg);
	
	
}
