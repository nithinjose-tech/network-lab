#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
int main()
{
char message[]="You have received a message!";
int server_socket=socket(AF_INET,SOCK_STREAM,0);
//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=INADDR_ANY; //server is binded with LocalHost(same machine)

//binding the socket with specified IP
bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));
//listening for connection
listen(server_socket,5);
//accepting connection
int client_socket= accept(server_socket ,NULL,NULL);
//sending the message

int n;

printf("Enter the value of n:\n");
scanf("%d",&n);

int mat[n][n];

printf("Enter the mtrx:\n");

for(int i=0;i<n;i++)
 {
   for(int j=0;j<n;j++)
   {
     scanf("%d",&mat[i][j]);
   }
 }

send(client_socket,&n,sizeof(n),0);
send(client_socket,mat,sizeof(mat),0);
//close the socket
close(server_socket);
return 0;
}
