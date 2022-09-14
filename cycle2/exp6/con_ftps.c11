#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>

int clientCount = 0;

struct client{
    int index;
    int sockID;
    struct sockaddr_in cliaddr;
    int len;
};

struct client Client[1024];
pthread_t thread[1024];

void* doNetworking(void* ClientDetail){

    struct client* clientDetail=(struct client*) ClientDetail;
    int index=clientDetail->index;
    int connfd=clientDetail->sockID;

    printf("\nClient %d connected.\n",index+1);
    
    int filehandle,size;
    char buf[100],filename[20];
    struct stat obj;
    
    int i=1;
    size=pthread_self();
    if(send(connfd,&size,sizeof(int),0)<0){
        perror("Send failed");
        return NULL;
    }    
    while(1){
         recv(connfd,buf,100,0);
         if(!strcmp(buf, "quit")){
	      printf("Client %d quitting..\n",index+1);
	      i=1;
	      if(send(connfd,&i,sizeof(int),0)<0){
	          perror("Send faild\n");
	          return NULL;
	      }
	      return NULL;
	      
	  }else{
	      strcpy(filename,buf);
	      stat(filename, &obj);
	      filehandle=open(filename,O_RDONLY);
	      if(filehandle==-1)
	           size=0;
	      else 
	           size=obj.st_size;
	      if(send(connfd,&size,sizeof(int),0)<0){
	          perror("Send faild\n");
	          return NULL;
	      }    
	      if(size){
	          if(sendfile(connfd,filehandle,NULL,size)<0){
	              perror("Send failed");
	              return NULL;
	          }
	      }              
	 }
    }
}
    
void main(){

     struct sockaddr_in servaddr;
     int sockfd;
        
     sockfd=socket(AF_INET, SOCK_STREAM, 0);
     if(sockfd==-1){
         printf("Socket creation failed");
         exit(1);
     }
     
     servaddr.sin_family=AF_INET;
     servaddr.sin_port=htons(3030);
     servaddr.sin_addr.s_addr=INADDR_ANY;
     
     if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1){
         perror("Binding error\n");
         exit(1);
     }
     
     if(listen(sockfd,1)==-1){
         perror("Listen failed\n");
         exit(1);
     }
     
     while(1){

	Client[clientCount].sockID=accept(sockfd,(struct sockaddr*)&Client[clientCount].cliaddr, &Client[clientCount].len);
	Client[clientCount].index=clientCount;

	pthread_create(&thread[clientCount],NULL,doNetworking,(void *)&Client[clientCount]);

	clientCount ++;
     }
}
