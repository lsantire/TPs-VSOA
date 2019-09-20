#include <stdio.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#define PORT 8080 

using namespace std;

void autenticar_client(int sock)
{
char message[128]="1|";
char *nombreUsuario = malloc(128);
char *pass = malloc(128);	
int valread,result;
char buffer[1024] = {0};

	printf("\nNombre de usuario: ");
	scanf("%s",nombreUsuario);
	printf("Password: ");
	scanf("%s",pass);

	strcat(message,nombreUsuario);
	strcat(message,"|");
	strcat(message,pass);

	send(sock , message , strlen(message) , 0 );
	valread = read( sock , buffer, 1024); 
	result = atoi(buffer);

		printf("\n\n\tRESULTADO");
		printf("\n\t------------------------------\n");
		switch(result)
		{
		case 0:
			printf("\tUsuario y password correctos :-)\n");
			break;
		case 1:
			printf("\tUsuario y/o password incorrectos :-(\n");
			break;
		case 2:
			printf("\tOcurrio un error :-(\n");
			break;
		default:
			printf("\nERROR FATAL");
			exit(0);
		}
}

void getEstadisticas_client(int sock)
{
 char message[128]="2";
 int valread;
 char buffer[1024] = {0};


    send(sock , message , strlen(message) , 0 );
	valread = read(sock, buffer, 1024); 

		printf("\n\n\tRESULTADO");
		printf("\n\t------------------------------\n");
		printf("\tCantidad de autenticaciones correctas: %d\n",);
		printf("\tCantidad de autenticaciones incorrectas: %d\n",);
		printf("\t\tCantidad de fallos con usuario incorrecto: %d\n",);
		printf("\t\tCantidad de fallos con password incorrecta: %d\n",);
		printf("\t\tCantidad de fallos inesperados: %d\n",);
}

int main(int argc, char const *argv[]) {
    
    sockaddr_in address,serv_addr; 
    int sock = 0; 
   

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
//==================MENUUUUU=======================//
	do{
		printf("\n\tMENU");
		printf("\n\t------------------------------");
		printf("\n\n\t 1. AUTENTICAR");
		printf("\n\t 2. ESTADISTICAS");
		printf("\n\t 3. SALIR");
		printf("\n\n Enter Your Choice: ");
		scanf("%c",&choice);
		switch(choice)
			{
			case '1':
				autenticar_client(sock);
			break;
			case '2':
				getEstadisticas_client(sock);
		    break;
			case '3':
		    exit(0);
			default:
			printf("\nINVALID SELECTION...Please try again");
			}
		getchar();
		printf("\nPresione ENTER para continuar\n");
		getchar();
		//getc();
	}while(1);

    return 0; 
}

