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
char buf[256];
fd_set readfd;
int selectVal;
char *pipe = "transit";
int pipefd;

//Create Named pipe
mkfifo(pipe , 0666);

while(1)
    {   
        //Open the pipe 
        pipefd = open(pipe , O_RDONLY | O_NONBLOCK );

        //Reset the readset
        FD_ZERO(&readfd);

        //Add file descriptor for stdin to fdset
        FD_SET(0 , &readfd);
        //Add the pipe file descriptor to the FD_SET
        FD_SET(pipefd , &readfd);
  
        //Call Select
        selectVal = select(pipefd+1 , &readfd , NULL  , NULL , NULL);
        if ( selectVal < 0)
        {
            printf("Select Failed");
        }

        if (selectVal > 0)
        {
            //Check to see if there is data to be read from stdin
            if(FD_ISSET(0 ,&readfd))
            {
                //Read data from stdin
                ssize_t bytes_read1 = read(0 , &buf , sizeof(buf));
                //Error reading from stdin
                if (bytes_read1 < 0 )
                {
                    return -1;
                }    
                //Write input to stdout 
                write(1 , "0 " , strlen("0 "));
                write(1 , buf , bytes_read1);
            }


            //Check to see if there are is data to be read from the pipe
            if(FD_ISSET(pipefd , &readfd))
            {
                //Read data from named pipe
                size_t bytes_read2 = read(pipefd , &buf , sizeof(buf));
                //Error reading from named pipe
                if (bytes_read2 < 0)
                {
                    return -1;
                }
                //Write file descriptor for pipe and write pipe contents to stdout
                write(1 , buf , bytes_read2);
            }
            //To complie
            //gcc -Wall -pedantic -g -o step5 step5.c
        }   
    }
}