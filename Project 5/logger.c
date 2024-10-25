//Isaac Cooper
//CS_3411 Program 5
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
#include <fcntl.h>

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
    int logfileDes;
    char logfileBuf[128];
    size_t logfileBytes;
    size_t logfileBytes2;
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

    //Open and create logfile if need be
    logfileDes = open("logfile.txt" , O_CREAT | O_TRUNC | O_RDWR , 0644);


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
                                lseek(logfileDes , 0 , SEEK_END);
                                statusVector[j] = 1;

                                //Set up for writting starting at user written data
                                int index=4;
                                const char *bounds = buf3 + index;
                                logfileBytes = strlen(bounds);

                                 write(logfileDes, bounds , logfileBytes);

                                //Write logging information
                                write(clientDesVector[j] , "#log:  logging\n" , 16);
                                write(clientDesVector[j] , "#log: " , 7);
                            }

                            //Case for list command
                            else if(strncmp(buf3 , "list " , 5) == 0)
                            {
                                //Reset bytes read and logfile buffer
                                //Seek to the beginning of the log file
                                lseek(logfileDes , 0 , SEEK_SET);
                                logfileBytes2 = 0;
                                bzero(logfileBuf , 128);

                                //Read from logfile in chunks for 128 
                                logfileBytes2 = read(logfileDes , &logfileBuf , 128);


                                //All of the contents in the logfile read
                                if(logfileBytes2 < 128)
                                {
                                    //No longer listing
                                    statusVector[j] = 1;

                                    write(clientDesVector[j] , "#log listing\n" , 14);

                                    //Write message
                                    write(clientDesVector[j] , &logfileBuf , strlen(logfileBuf));
                                }
                                else
                                {
                                    //Still listing as more data remains
                                    statusVector[j] = 2;

                                    write(clientDesVector[j] , "#log listing\n" , 14);

                                    //Write message, expecting \n command
                                    write(clientDesVector[j] , &logfileBuf , 128);

                                    //Need to update the file postion vector for lseek()
                                    filePosVector[j] += 128;
                                }
                            }
                            //Case for \n after listing command
                            else if((strncmp(buf3 , "\r\n" , 2 ) == 0) && (statusVector[j] == 2) )
                            {
                                //Reset bytes read and logfile buffer
                                logfileBytes2 = 0;
                                bzero(logfileBuf , 128);

                                //Need to lseek to the updated file position
                                lseek(logfileDes , filePosVector[j] , SEEK_SET);

                                //Read from logfile in chunks of 128
                                logfileBytes2 = read(logfileDes , &logfileBuf , 128);
                                
                                //All of the contents in the logfile read
                                if(logfileBytes2 < 128)
                                {

                                    write(clientDesVector[j] , "#log:  more\n" , 13);
                                    
                                    //Write message to client
                                    write(clientDesVector[j] , &logfileBuf , strlen(logfileBuf));

                                    //Update status vector to connected
                                    statusVector[j] = 1;
                                }

                                //More data remains
                                else
                                {
                                    write(clientDesVector[j] , "#log:  more\n" , 13);
                                    
                                    //Write message, expecting \n command
                                    write(clientDesVector[j] , &logfileBuf , 128);

                                    //Update file position vector
                                    filePosVector[j] += 128;
                                }
                            }

                            //Case for unrecognized command
                            else
                            {
                                write(clientDesVector[j] , "#log:  Command not recognized\n" , 30);

                            }
                        }  
                    }
                }
            }
        }
    }
    //To complie
    //gcc -Wall -pedantic -g -o logger logger.c 
}