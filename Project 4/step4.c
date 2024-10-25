//Isaac Cooper
//CS_3411 Program 4
//04/04/24

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

int main(int argc , char** argv)
{

//Set up variables for select call 
int nfds;
char buf[128];
char *msg;
fd_set readfd;
struct timeval timeout; 
int selectVal;
timeout.tv_sec = 10;  
 
 

//Set number for max file descriptor
nfds = 1;

while(1)
    {   
        //Add file descriptor for stdin to fdset
        FD_ZERO(&readfd);
        FD_SET(0 , &readfd);  
        timeout.tv_sec = 10;   

        //Call Select
        selectVal = select(nfds , &readfd , NULL  , NULL , &timeout);
        if ( selectVal < 0)
        {
            printf("Select Failed");
        }

        //Print tick
        if (selectVal == 0)
        {
            msg = "Tick";
            write(1 , "Tick" , strlen(msg));
            write(1 , "\n" , strlen("\n"));
        }

        if (selectVal > 0)
        {
            if(FD_ISSET(0 ,&readfd))
            {
                ssize_t bytes_read = read(0 , &buf , sizeof(buf));
                //Error reading from stdin
                if (bytes_read < 0 )
                {
                    return -1;
                }    
                //Write input to stdout 
                write(1 , buf , bytes_read);
            }
        }
        //To complie
        //gcc -Wall -pedantic -g -o step4 step4.c
    }
}