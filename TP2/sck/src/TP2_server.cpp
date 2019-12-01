#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

int cantOk;
int cantUsuarioIncorrecto;
int cantPassIncorrecta;
int cantTotal;

void autenticar_1_svc(char buffer[1024], int new_socket)
{
    static int result;
    int n;
    char nombreUsuario[128],pass[128];
    int usuarioOk=0,i=0;
    char new_buffer[1024];

    cantTotal++;
    
    if(freopen("usuarios.txt","r",stdin)==NULL)
    {
        send(new_socket , "2" , 1 , 0 ); 
        return;
    }
    scanf("%d",&n);

    char nombreUsuarioAuth[128];
    char passAuth[128];

    int posNombreAuth = 0;
    int posPassAuth = -1;

    for (int i = 2;buffer[i] != '\0'; ++i)
    {
        if (buffer[i] == '|')
        {
            posPassAuth = 0;
        }else{
            if (posPassAuth != -1)
            {
                passAuth[posPassAuth++] = buffer[i];
            }else{
                nombreUsuarioAuth[posNombreAuth++] = buffer[i];
            }
        }
        
    }

    if (posPassAuth == -1)
    {
        posPassAuth++;
    }

    passAuth[posPassAuth] = '\0';
    nombreUsuarioAuth[posNombreAuth] = '\0';

    while(i<n)
    {
        scanf("%s",nombreUsuario);
        scanf("%s",pass);

        if(strcmp(nombreUsuario,nombreUsuarioAuth)==0)
        {
            usuarioOk=1;
            if(strcmp(pass,passAuth)==0)
            {
                cantOk++;
                send(new_socket , "0" , 1 , 0 ); 
                return;
            }
        }
        i++;
    }
    if(usuarioOk) cantPassIncorrecta++;
    else cantUsuarioIncorrecto++;

    send(new_socket , "1" , 1 , 0 ); 
}

int toString(int x, char str[]){
    if(x<10){
	str[0]=x+'0';
	return 1;
    }
    int aux = toString(x/10, str);
    str[aux] = x%10+'0';
}

void getestadisticas_1_svc(int new_socket)
{
    char aux[128];
    char new_buffer[1024]="";

    toString(cantOk,aux);
    strcat(new_buffer,aux);
    strcat(new_buffer,"|");

    toString(cantUsuarioIncorrecto,aux);
    strcat(new_buffer,aux);
    strcat(new_buffer,"|");

    toString(cantPassIncorrecta,aux);
    strcat(new_buffer,aux);
    strcat(new_buffer,"|");

    toString(cantTotal,aux);
    strcat(new_buffer,aux);

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

	while(true)
	{
		
	    int valread = read( new_socket , buffer, 1024); 
    
        if (buffer[0] == '1')
        {
          autenticar_1_svc(buffer,new_socket);
        }else{
          getestadisticas_1_svc(new_socket);
        }

	}
}
