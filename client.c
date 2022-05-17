#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
int main()
{
int network_socket=socket(AF_INET,SOCK_STREAM,0);
//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=INADDR_ANY;

int con_status=connect(network_socket,(struct sockaddr*)
&server_address,sizeof(server_address));
//gives the status of he connection
if(con_status==-1)
{
printf("Connection couldn't be established");
}
char server_response[50];
recv(network_socket,& server_response,sizeof(server_response),0);
printf(" The received message is %s ",server_response);
close(network_socket);
}
