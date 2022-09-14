#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<time.h>

void main(){

	int server_fd, client_fd;
	char msg[100];
	
	struct sockaddr_in server_addr, client_addr;
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	int c = sizeof(server_addr);
	
	recvfrom(server_fd, msg, 100, MSG_WAITALL, (struct sockaddr*)&server_addr, &c);
	printf("recieved message : %s\n", msg);
	time_t t = time(NULL);
	printf("%s", ctime(&t));
	
	
	sendto(server_fd, ctime(&t),100, MSG_CONFIRM, (struct sockaddr*)&server_addr, c);
	

}
