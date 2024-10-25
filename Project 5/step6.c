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
void printaddr(s_in , s1 , s2 )
struct sockaddr_in *s_in; char *s1, *s2;
{
    char buf2[1000];
    write(1 , "Accepted Connection From: " , 27);
    sprintf(buf2 , "(%d,%d)\n" , s_in->sin_addr.s_addr , s_in->sin_port);
    write(1 , &buf2 , strlen(buf2));

}

int main( int argc , char** argv)
{
    int socketfd;
    int conn;
    char ch;
    int length; 
    int counter = 0;
    char buf1[1000];
    char buf3[256];
    int selectVal;
    fd_set readfd;
    struct sockaddr_in s1 , s2;
    socketfd = socket(AF_INET , SOCK_STREAM , 0 );

    //Connection Status Vector
    // 0 => Not Connected 1 => Connected 2 = listing
    int statusVector[32] = {0}; 

    //Client descriptor Vector
    int clientDesVector[32];

    //File position Vector
    int filePosVector[32];

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

    //Get current address for socket fd is bound 
    getsockname(socketfd, (struct sockaddr *)&s1, (socklen_t*) &length);
    int port_num = ntohs(s1.sin_port);
    
    //Print the port number of the server
    write(1 , "Assigned port number: " , strlen("Assigned port number: "));
    sprintf(buf1 , "%d\n" , port_num);
    write(1 , &buf1 , strlen(buf1));

    listen(socketfd , 1);
    length = sizeof(s2);


    while (1)
    {
        //Reset the read set
        FD_ZERO(&readfd);

        //add the file descriptors for stdin and the socket to the read set
        FD_SET(0,&readfd);
        FD_SET(socketfd , &readfd);

        selectVal = select(socketfd+1 , &readfd , NULL , NULL , NULL);

        //Error checking for select
        if (selectVal < 0 )
        {
            return -1;
        }
        if (selectVal > 0)
        {   
            //Case for when there is input on socket
            if(FD_ISSET(socketfd , &readfd))
            {
                //Accept the client
                conn = accept(socketfd , (struct sockaddr *)&s2 , (socklen_t*) &length);

                //Print client information
                printaddr(&s2);

                if (conn > 0)
                {
                    write(conn , "log #:" , 7);

                    //Set the Connection status vector accordingly 
                    statusVector[counter] = 1;
                    clientDesVector[counter] = conn;

                    //Set the file postion vector to be zero
                    filePosVector[counter] = 0;

                  
                if (read(conn , &ch , 1) == 0)
                    {
                    write(1 , "TEST client closed" , 19);
                    //Set status vector accordingly
                    statusVector[counter] = 0;
                    }
                }
            } 
        }
    }
    //To complie
    //gcc -Wall -pedantic -g -o step6 step6.c 
}