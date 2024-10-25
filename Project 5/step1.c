//Isaac Cooper
//CS_3411 Program 4
//04/09/24

#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
struct sockaddr_in *s_in; char *s1, *s2;


int main( int argc , char** argv)
{
    int socketfd;
    int conn; 
    int length; 
    
    char buf1[80];
    struct sockaddr_in s1 , s2;
    socketfd = socket(AF_INET , SOCK_STREAM , 0);

    //Zero out struct object
    bzero((char * ) &s1 , sizeof(s1));

    //Assign a domain
    s1.sin_family = (short) AF_INET;

    //Assign an address
    s1.sin_addr.s_addr =  htonl(INADDR_ANY);

    //Assign a random port when bind is called
    s1.sin_port = htons(0);

    bind(socketfd , (struct sockaddr *) &s1 , sizeof(s1));
    length = sizeof(s1);

    //Print port number


    //Get current address for socket fd is bound 
    getsockname(socketfd, (struct sockaddr *)&s1, (socklen_t*) &length);
    int port_num = ntohs(s1.sin_port);
    
    //Print the port number of the server
    write(1 , "Assigned port number: " , strlen("Assigned port number: "));
    sprintf(buf1 , "%d" , port_num);
    write(1 , &buf1 , strlen(buf1));
    

    //To complie
    //gcc -Wall -pedantic -g -o step1 step1.c 

}