#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

void autenticar_1_svc(char buffer[1024], int new_socket)
{
    static int result;
    int n;
    char *nombreUsuario,*pass;
    int usuarioOk=0,i=0;
    char new_buffer[1024];

    nombreUsuario = malloc(128);
    pass = malloc(128);

    cantTotal++;
    
    if(freopen("usuarios.txt","r",stdin)==NULL)
    {
        result=2;
        return &result;
    }
    scanf("%d",&n);
    while(i<n)
    {
        scanf("%s",nombreUsuario);
        scanf("%s",pass);
        if(strcmp(nombreUsuario,argp->nombreUsuario)==0)
        {
            usuarioOk=1;
            if(strcmp(pass,argp->pass)==0)
            {
                cantOk++;
                result=0;
                return &result;
            }
        }
        i++;
    }
    if(usuarioOk) cantPassIncorrecta++;
    else cantUsuarioIncorrecto++;
    result=1;

send(new_socket , new_buffer , strlen(new_buffer) , 0 ); 

}

void getestadisticas_1_svc(char buffer[1024], int new_socket)
{
    char new_buffer[1024];

    strcat(new_buffer[1024],cantOk);
    strcat(new_buffer[1024],"|");
    strcat(new_buffer[1024],cantUsuarioIncorrecto);
    strcat(new_buffer[1024],"|");
    strcat(new_buffer[1024],cantPassIncorrecta);
    strcat(new_buffer[1024],"|");
    strcat(new_buffer[1024],cantTotal);

    send(new_socket , new_buffer , strlen(new_buffer) , 0 ); 
}


int main(){
	int server_fd,new_socket;
	int opt = 1;
	sockaddr_in address;
	int addrlen = sizeof(address);

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
    
        if (buffer[0] == '1')
        {
          autenticar_1_svc(buffer,new_socket);
        }else{
          getestadisticas_1_svc(buffer,new_socket);
        }

	}
}
