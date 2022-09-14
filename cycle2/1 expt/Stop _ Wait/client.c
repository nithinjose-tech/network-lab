#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8000
  
int main(int argc, char const* argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    client_fd=connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    if(client_fd<0)
    {
    	printf("error");
    	return -1;
    }
    int n,received;
    while(1)
    {
    	printf("enter packet number : \n");
    	scanf("%d",&n);		
		send(sock,&n,sizeof(n),0);	
		if(n==-1)
    		break;		
		read(sock,&received,sizeof(received));
		if(received==1)
			printf("ACK received for packet %d\n",n);
		else
			printf("ACK not received for packet %d\n",n);
    } 
    return 0;
}
