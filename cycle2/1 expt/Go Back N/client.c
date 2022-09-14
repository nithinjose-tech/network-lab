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
    int n,r1,r2,r3,r4;
    printf("Let our window size be 4\n");
    int a=1,b=2,c=3,d=4;
    send(sock,&a,sizeof(a),0);	
	send(sock,&b,sizeof(b),0);	
	send(sock,&c,sizeof(c),0);	
	send(sock,&d,sizeof(d),0);	
    while(1)
    {	
		sleep(2);
		read(sock,&r1,sizeof(r1));
		if(r1==1)
		{
			printf("ACK received for packet %d\n",a);
			a+=1;
			d=d+1;
			send(sock,&d,sizeof(d),0);
			printf("Send packet %d \n",d);
		}
		else
		{
			sleep(2);
			printf("ACK not received for any packet\n");
    	}
    } 
    return 0;
}
