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
int value;
recv(network_socket,&value,sizeof(value),0);

int mat[value][value];

recv(network_socket,mat,sizeof(mat),0);
printf(" The received message is %d",value);

printf("The matrix is:\n");

for(int i=0;i<value;i++)
 {
   for(int j=0;j<value;j++)
    {
      int temp = mat[i][j];
      mat[i][j] = mat[j][i];
      mat[j][i] = temp;
    }
 }
 
 for(int i=0;i<value;i++)
 {
   for(int j=i;j<value;j++)
    {
      printf("%d\t",mat[i][j]);
    }
    printf("\n");
 }
close(network_socket);
}
