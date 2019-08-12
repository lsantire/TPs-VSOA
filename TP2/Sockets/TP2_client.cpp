#include <stdio.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#define PORT 8080 

using namespace std;

int main(int argc, char const *argv[]) {
    
    sockaddr_in address,serv_addr; 
    int sock = 0, valread; 
    char buffer[1024] = {0}, *message;// = string("Mensaje por defecto").c_str();
    printf("%d\n",argc );
    if(argc>1) strcpy(message, argv[1]);

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
    send(sock , message , strlen(message) , 0 ); 
    printf("Client message sent\n"); 
    valread = read( sock , buffer, 1024); 
    printf("Client receive: %s\n",buffer ); 
    return 0; 
}