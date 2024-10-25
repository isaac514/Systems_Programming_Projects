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

//Function for printing connection information
void printaddr(s_in , s1 , s2 )
struct sockaddr_in *s_in; char *s1, *s2;
{
    char buf2[1000];
    write(1 , "Accepted Connection From: " , 27);
    sprintf(buf2 , "(%d,%d)\n" , s_in->sin_addr.s_addr , s_in->sin_port);
    write(1 , &buf2 , strlen(buf2));

}

//Function to the max file descriptor 
int findMax(int arr[] , int n)
{
    int maxVal = arr[0];

    for(int i = 1 ; i < n ; i++)
    {
        if(arr[i] > maxVal)
        {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

int main( int argc , char** argv)
{
    int socketfd;
    int conn;
    int length; 
    char buf1[1000];
    char buf3[128] = {0};
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

        //iterate over the descriptor array and add the the connections to the readset
        for(int i = 0 ; i < 32 ; i++)
        {
            if(statusVector[i] == 1 || statusVector[i] == 2)
            {
                FD_SET(clientDesVector[i] , &readfd);
            }
        }

        //Get largest file descriptor
        int max = findMax(clientDesVector , 32);

        //add the file descriptors for stdin and the socket to the read set
        FD_SET(0,&readfd);
        FD_SET(socketfd , &readfd);

        selectVal = select(max+1 , &readfd , NULL , NULL , NULL);
        

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

                    //Search for free spot in status Vector array
                    for ( int a = 0 ; a < 32 ; a++)
                    {
                        //Found a free spot in the status vector array
                        if (statusVector[a] == 0)
                        {
                            //Set status vector,client descriptor vector and file position vector
                            statusVector[a] = 1;
                            clientDesVector[a] = conn;
                            filePosVector[a] = 0;
                            break;
                        }
                    }
                }  
            } 

            //Case for client when there is input on client connections
            //iterate through status vector to get each of the connected clients
            for (int j = 0 ; j < 32 ; j++)
            {
                //Case for connected client
                if((statusVector[j] == 1) || (statusVector[j] == 2))
                {
                    //Case for input on client
                    if(FD_ISSET(clientDesVector[j] , &readfd))
                    {
                        bzero(buf3 , 128);
                        //Read from the client
                        int retVal = read(clientDesVector[j] , &buf3 , 128);

                        //Case for client disconnect
                        if(retVal == 0)
                        {
                            statusVector[j] = 0;

                            //Write to stdout that the client has disconnected
                            write(1 , "Client closed\n" , strlen("Client closed\n"));
                        }
                        //Case for read returning a postive integer, check for commands
                        else
                        {
                            //Case for log command
                            if(strncmp(buf3, "log " ,4) == 0)
                            {
                                statusVector[j] = 1;
                                write(clientDesVector[j] , "  logging" , 10);
                            }

                            //Case for list command
                            else if(strncmp(buf3 , "list " , 5) == 0)
                            {
                                write(clientDesVector[j] , "  listing" , 10);

                                //Modify the status of the listing vector
                                statusVector[j] = 2;
                            }
                            //Case for \n after listing command
                            else if((strncmp(buf3 , "\r\n" , 2 ) == 0) && (statusVector[j] == 2) )
                            {
                                write(clientDesVector[j] , "  more" , 7);
                            }

                            //Case for unrecognized command
                            else
                            {
                                write(clientDesVector[j] , "  Command not recognized" , 25);
                            }
                        }  
                    }
                }
            }
        }
    }
    //To complie
    //gcc -Wall -pedantic -g -o step8 step8.c 
}