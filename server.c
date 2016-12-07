#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc,char *argv[])
{
	int maxi,maxfd,socket_fd,client_fd,data_fd;
	int port_no,i,j,client[FD_SETSIZE];
	fd_set active_fd,read_fd;
	int client_len;
	char buffer[256];
	struct sockaddr_in server_addr,client_addr;
	if(argc<2){
		error("Port number not provided \n");
	}

	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd<0)
		error("Error in creating socket \n");
	bzero((char*)&server_addr,sizeof(server_addr));
	port_no                     =atoi(argv[1]);
	server_addr.sin_family      =AF_INET;
	server_addr.sin_addr.s_addr =INADDR_ANY;
	server_addr.sin_port        =htons(port_no);

	if(bind(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
		error("Error on binding the socket \n");
	listen(socket_fd,5);
	maxfd=socket_fd;
	maxi=-1;
	for(i=0;i<FD_SETSIZE;i++)
		client[i]=-1;
	FD_ZERO (&active_fd);
	FD_SET (socket_fd, &active_fd);
	for(;;){
		read_fd=active_fd;
		select(maxfd+1,&read_fd,NULL,NULL,NULL);
		if(FD_ISSET(socket_fd,&read_fd)){
			client_len=sizeof(client_addr);
			client_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&client_len);
			for(i=0;i<FD_SETSIZE;i++){
				if(client[i]<0){
					client[i]=client_fd;
					break;
				}
			}
			if(i==FD_SETSIZE)
				error("tooo many clients");
			
			FD_SET(client_fd,&active_fd);
			if(client_fd>maxfd)
				maxfd=client_fd;
			if(i>maxi)
				maxi=i;
		}
		
		for(i=0;i<=maxi;i++){
			if((data_fd=client[i])<0)
				continue;
			if(FD_ISSET(data_fd,&read_fd)){
				if(read(data_fd,buffer,256)==0)
					close(data_fd);
				else{
					printf("message is:%s",buffer);
					for(j=0;j<=maxi;j++){
						write(client[j],buffer,256);
					}
			
				}
			}
		}
	}

	return 0;

}
