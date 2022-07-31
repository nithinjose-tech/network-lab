#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
int main()
{






int server_socket=socket(AF_INET,SOCK_STREAM,0);
//define the server address
struct sockaddr_in server_address;
server_address.sin_family=AF_INET;


int chopstick[4];

for(int i=0;i<4;i++)chopstick[i]=0;


server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=inet_addr("0.0.0.0");  //communication bw the same machine

//binding the socket with specified IP
bind(server_socket,(struct sockaddr*) &server_address, sizeof(server_address));

//listening for connection
listen(server_socket,10); //2 -specifies the no of clients that can server can listen to

//accepting connection

int pids[4];

/*int client_socket1= accept(server_socket ,NULL,NULL);
int client_socket2= accept(server_socket ,NULL,NULL);
int client_socket3= accept(server_socket ,NULL,NULL);
int client_socket4= accept(server_socket ,NULL,NULL);*/
int message[2];
//send(client_socket,message,sizeof(message),0);
int i=0;
while(1)
{
if(i>=4)
{
break;
}
int client_socket1= accept(server_socket ,NULL,NULL);
recv(client_socket1,&message,sizeof(message),0);
printf("The message for 1 is %d\n ",message[0]);


pids[i]=message[0];

i++;
printf("%d\n",i);




}
/*pids[0]=message[0];

recv(client_socket2,&message,sizeof(message),0);

printf("The message for 2 is %d\n ",message[0]);

pids[1]=message[0];

recv(client_socket3,&message,sizeof(message),0);

printf("The message for 1 is %d\n ",message[0]);

pids[2]=message[0];

recv(client_socket4,&message,sizeof(message),0);

printf("The message for 2 is %d\n ",message[0]);

pids[3]=message[0];
*/


int small=pids[0];

for(int i=1;i<4;i++)
{	
	if(small>pids[i])small=pids[i];
	printf("%d \n\n",pids[i]);
}


printf("The samllest pid value %d\n",small);


while(1)
{
int client_socket1= accept(server_socket ,NULL,NULL);
recv(client_socket1,&message,sizeof(message),0);
//printf("The message for philosopher %d is %d\n ",message[0]-small,message[1]);
int philno=message[0]-small;

if(message[1]==1)
{

if(chopstick[philno]==0 && chopstick[(philno+1)%4]==0)
{
printf("philosopher %d has obtained the chopstick \n",philno);
chopstick[philno]=1;
chopstick[(philno+1)%4]=1;
}

else
{
printf("%d cant obtain the chopstick\n",philno);
}

}


else if(message[1]==0)
{

if(chopstick[philno]==1 && chopstick[(philno+1)%4]==1)
{
printf("philosopher %d has put down the chopstick \n",philno);
chopstick[philno]=0;
chopstick[(philno+1)%4]=0;
}
else
{
chopstick[philno]=0;
chopstick[(philno+1)%4]=0;

}

}

}






close(server_socket);


}
