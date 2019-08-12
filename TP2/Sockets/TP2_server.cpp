#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int main(){
	int server_fd,new_socket;
	int opt = 1;
	sockaddr_in address;
	int addrlen = sizeof(address);
	char server_ans[1024] = "hola capo, tu conexion fue exitosa";

	// Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
	
	// Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 

    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

	while(true)
	{
		char buffer[1024] = {0};
		if (listen(server_fd, 3) < 0) 
    	{ 
       		perror("listen"); 
        	exit(EXIT_FAILURE); 
   		}
   		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  (socklen_t*)&addrlen))<0) 
	    { 
	        perror("accept"); 
	        exit(EXIT_FAILURE); 
	    }
	    int valread = read( new_socket , buffer, 1024); 
    	printf("server recieve: %s\n",buffer);
    	send(new_socket , server_ans , strlen(server_ans) , 0 ); 
    	printf("Server answer sent\n"); 
	}
}
