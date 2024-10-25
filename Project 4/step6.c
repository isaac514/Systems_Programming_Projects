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
    char buf1[256];
    char buf2[256];
    fd_set readfd;
    int selectVal;

    //Out pipe
    int p1[2];

    //Error pipe
    int p2[2];

    //File descriptor for outfile and errfile
    int outFd;
    int errFd;

    //Intialize size array for execvp 
    char* subArray[argc-1];

    //Set last element of subArray to be NULL
    subArray[argc - 2] = NULL;

    //Populate array of execve
    for (int i = 2 ; i < argc ; i++)
    {
        subArray[i-2] = (char*)argv[i];
    }

    //Set up for outfile and errorfile
    char outfile[1000];
    char temp1[1000];

    char errfile[1000];
    char temp2[1000];

    strcpy(outfile , (char*)argv[1]);
    strcpy(temp1 , (char*)".stdout");

    strcpy(errfile , (char*)argv[1]);
    strcpy(temp2 , (char*)".stderr");

    //Get the file name for outfile and errorfile 
    strcat(outfile , temp1);
    strcat(errfile , temp2);

    //open files
    outFd = open(outfile , O_RDWR | O_CREAT | O_TRUNC , 0644);
    errFd = open(errfile , O_RDWR | O_CREAT | O_TRUNC , 0644); 

    pipe(p1);
    pipe(p2);

    //Child process 
    if (fork() == 0)
    {
        close(p1[0]);
        close(p2[0]);
        close(1);
        dup(p1[1]);
        close(p1[1]);
        close(2);
        dup(p2[1]);
        close(p2[1]);

        //Call to execve() with argument array set up
        execvp((char*)argv[2] , subArray);
    }
    //Parent process
    else
    {
        while(1)
        {   
            //Reset the readset
            //Add Unamed pipes to the read set
            FD_ZERO(&readfd);

            //Add the p1 file descriptor to the FD_SET
            FD_SET(p1[0] , &readfd);

            //Add the p2 file descriptor to the FD_SET
            FD_SET(p2[0] , &readfd);

            //Call Select
            selectVal = select(p2[0]+1 , &readfd , NULL  , NULL , NULL);
            if ( selectVal < 0)
            {
                printf("Select Failed");
            }

            if (selectVal > 0)
            {

                //Check to see if there are is data to be read from the pipe
                if(FD_ISSET(p1[0] , &readfd))
                {
                    //Read data from p1
                    size_t bytes_read1 = read(p1[0] , &buf1 , sizeof(buf1));
                    //Error reading from named pipe
                    if (bytes_read1 < 0)
                    {
                        return -1;
                    }
                    //Write file descriptor for pipe and write pipe contents to stdout
                    //write(1 , buf1 , bytes_read1);
                }
                if(FD_ISSET(p2[0] , &readfd))
                {
                    //Read data from p2 
                    size_t bytes_read2 = read(p2[0] , &buf2 , sizeof(buf2));
                    //Error reading from named pipe 
                    if(bytes_read2 < 0)
                    {
                        return -1;
                    }      
                }
            } 
            //gcc -Wall -pedantic -g -o step6 step6.c
        }
    }
}