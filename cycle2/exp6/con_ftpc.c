#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<string.h>

void main(){
  int sockfd,choice,filehandle,size;
  struct sockaddr_in servaddr;
  char filename[30],buff[100],*f;
  
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd==-1){
     perror("Socket creation faild\n");
     exit(0);
  }
  
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  servaddr.sin_port=htons(3030);
  
  if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1){
      perror("Connectionnfailed\n");
      exit(0);
  } 
  
  int i=1;
  char *command="quit";
  recv(sockfd,&size,sizeof(int),0);
  printf("\nID of the thread: %d\n\n",size);
   
  while(1){
      printf("Enter your choice:\n1-get\n2-quit\n\n");
      scanf("%d",&choice);
      switch(choice){
          case 1:
          
              printf("Enter filename: ");
              scanf("%s",filename);
              
              if(send(sockfd,filename,sizeof(filename),0)<0){
                   perror("Send faild");
                   exit(0);
              }     
              
              if(recv(sockfd,&size,sizeof(int),0)<0){
                   perror("Receive failed");
                   exit(0);
              }     
              
              if(size){
              
                  f=malloc(size);
                  if(recv(sockfd,f,size,0)<0){
                        perror("Receive failed");
                        exit(0);
                  }
                        
                  while(1){
                      filehandle=open(filename,O_CREAT | O_EXCL | O_WRONLY,0666);
                      if(filehandle==-1)
                          sprintf(filename+strlen(filename),"%d",i);
                      else break;
                  }
                  
                  write(filehandle,f,size);
                  close(filehandle);
                  
                  strcpy(buff,"cat ");
                  strcat(buff,filename);
                  system(buff);         
              }
              else{
                  printf("No such file\n");
              }
              break;
              
          case 2:
          
              send(sockfd,command,sizeof(command),0);
              recv(sockfd,&i,sizeof(int),0);
              if(i){
                  printf("Client quitting\n");
                  close(sockfd);
                  exit(0);
              }
              printf("Failed to close connection\n");
      }        
  }   
}
