

#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<arpa/inet.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>



void establish_connection()
{

int nsocket;
nsocket=socket(AF_INET,SOCK_STREAM,0);

struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=inet_addr("0.0.0.0");


int connection_status=connect(nsocket,(struct sockaddr *) &server_address,sizeof(server_address));

if(connection_status==-1)
{
printf("Error in creating connection");
}

int message[2];
message[0]=getpid();
message[1]=-1;

printf("\n\n%d\n\n",message[0]);
send(nsocket,message,sizeof(message),0);

//recv(nsocket,&message,sizeof(message),0);
//printf("Recieved data is %d ",message);



}





void getchopstick()
{

int nsocket;
nsocket=socket(AF_INET,SOCK_STREAM,0);

struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=inet_addr("0.0.0.0");


int connection_status=connect(nsocket,(struct sockaddr *) &server_address,sizeof(server_address));

if(connection_status==-1)
{
printf("Error in creating connection");
}

int message[2];
message[0]=getpid();
message[1]=1;
//printf("\n\n%d\n\n",message[0]);
send(nsocket,message,sizeof(message),0);

//recv(nsocket,&message,sizeof(message),0);
//printf("Recieved data is %d ",message);
close(nsocket);

}




void putchopstick()
{

int nsocket;
nsocket=socket(AF_INET,SOCK_STREAM,0);

struct sockaddr_in server_address;
server_address.sin_family=AF_INET;
server_address.sin_port=htons(9200);
server_address.sin_addr.s_addr=inet_addr("0.0.0.0");


int connection_status=connect(nsocket,(struct sockaddr *) &server_address,sizeof(server_address));

if(connection_status==-1)
{
printf("Error in creating connection");
}

int message[2];
message[0]=getpid();
message[1]=0;
//printf("\n\n%d\n\n",message[0]);
send(nsocket,message,sizeof(message),0);

//recv(nsocket,&message,sizeof(message),0);
//printf("Recieved data is %d ",message);
close(nsocket);

}



int main()
{

fork();
fork();

establish_connection();
//sleep((rand()%5)+1 );

getchopstick();
sleep((rand()%5)+1 );
putchopstick();
sleep((rand()%5)+1 );
getchopstick();
sleep((rand()%5)+1 );

}
