#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
void error(char* msg){
        perror(msg);
        exit(1);
}

int main(int argc,char *argv[])
{
	pid_t pid;
        int socket_fd;
        int port_no;
        struct sockaddr_in server_addr;
        struct hostent *server;
        char buffer[256];
        if(argc<3)
                error("Something is missing,either server IP or port number");
        port_no=atoi(argv[2]);
        socket_fd=socket(AF_INET,SOCK_STREAM,0);
        if(socket_fd<0)
                error("Error in creating socket");
        server=gethostbyname(argv[1]);
        bzero((char*)&server_addr,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port   = htons(port_no);
        bcopy((char *)server->h_addr,(char *)&server_addr.sin_addr.s_addr,server->h_length);
        connect(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	pid = fork();
	if(pid==0)
	{
        	while(1){
                	printf("please type your message:");
                	bzero(buffer,sizeof(buffer));
                	fgets(buffer,256,stdin);
                	if(strcmp(buffer,"bye\n")!=0){
                        	write(socket_fd,buffer,sizeof(buffer));
                        	bzero(buffer,sizeof(buffer));
                        	bzero(buffer,256);
                	}
                	else{
                        	write(socket_fd,buffer,sizeof(buffer));
                        	bzero(buffer,sizeof(buffer));
                        	exit(1);
                	}

        	}
	}
	else{
		while(1){
			read(socket_fd,buffer,sizeof(buffer));
			printf("message from server is :%s",buffer);
			bzero(buffer,256);
		}
	}
        return 0;


}

