#include<netinet/in.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

#define port 8080

int n;

int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = 99, min_index;
  
    for (int v = 0;v<n;v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
  
    return min_index;
}

void main()
{
	int server_fd,client_fd;
	int opt=1;
	
	char msg[50];
	
	
	struct sockaddr_in server_addr,client_addr;
	
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	
	if(server_fd<0)
	{
		perror("Socket creation failed");
		exit(0);
	}
	
	if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&opt,sizeof(opt))<0)
	{
		perror("Socket manipulation failed");
		exit(0);
	}
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=INADDR_ANY;	
	
	if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
	{
		perror("Binding failed");
		exit(0);
	}
	
	
	listen(server_fd,5);
	
	int c=sizeof(struct sockaddr_in);
	
	client_fd=accept(server_fd,(struct sockaddr*)&client_addr,(socklen_t *)&c);
	
	if(client_fd<0)
	{
		perror("Accept failed");
		exit(0);
	}
	
	int i,j,k,count=0;

	recv(client_fd,&n,sizeof(int),0);
	int dmat[n][n];
	recv(client_fd,dmat,sizeof(int)*n*n,0);
	int s;
	recv(client_fd,&s,sizeof(int),0);
 
 	int g[n];
 	int d[n];

 	int dist[n]; 
  
    bool sptSet[n];
  
    
    for (int i = 0;i<n;i++)
        dist[i] = 99;
        sptSet[i] = false;
  
    
    dist[s] = 0;
  
    
    for (count = 0;count<n-1;count++) {
        int u = minDistance(dist, sptSet);
  
        sptSet[u] = true;
  
        for (int v=0;v<n; v++)
  
            if (!sptSet[v] && dmat[u][v]!=99 && dist[u] != 99 && dist[u] + dmat[u][v] < dist[v])
                dist[v] = dist[u] + dmat[u][v];
    }

    send(client_fd,dist,sizeof(int)*n,0);
    printf("Vertex \tDistance from Source %d\n",s);
    printf("---------------------------------\n");
    for (int i=0;i<n;i++)
        printf("%d \t %d\n", i, dist[i]);
	


	printf("\n\n");
}
