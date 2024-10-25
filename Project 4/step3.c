//Isaac Cooper
//CS_3411 Program 4
//04/04/24

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main( int argc , char **argv)
{
    //argv 0 => ./prose ,1 => filename for out and error file  2 => binary for execvp , 2-n => arguements for execvp 
    //Notes: 
        //-Need to read in size of 256 bytes

    //Determines parent/child        
    pid_t pid;


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

    //Write remain arguments to the outfile
    for(int j = 1 ; j < argc-1 ; j++)
    {
        //write arguments to outfile
        write(outFd , (char*)subArray[j] ,strlen(subArray[j]));
        write(outFd , "\n" , strlen("\n"));
    }

    //Child process
    pid = fork();
    if(pid == 0)
    {
            close(1);
            close(2);
            dup(outFd);
            dup(errFd);
            close(outFd);
            close(errFd);
            execvp((char*)argv[2] , subArray );
    }
    //Parent process 
    else
    {
        //wait(NULL);
    }
    
    //To complie
    //gcc -Wall -pedantic -g -o step3 step3.c
}