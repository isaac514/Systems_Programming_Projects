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
        //-Need to write in size of 256 bytes

    //Determines parent/child        
    pid_t pid;

    //Size of array for execvp
    int argn;

    //File descriptor for outfile
    int outFd;

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

    //open file
    outFd = open(outfile , O_RDWR | O_CREAT | O_TRUNC , 0644);

    //Will need to open file for stderr

    
    //Write remain arguments to the outfile
    for(int j = 1 ; j < argc-1 ; j++)
    {
        //write arguments to outfile
        write(outFd , (char*)subArray[j] ,strlen(subArray[j]));
        write(outFd , "\n" , strlen("\n"));
    }

    //Step 2
    pid = fork();
    //Child process
    if(pid == 0)
    {
            execve((char*)argv[2] , subArray , NULL);
    }
    //Parent process 
    else
    {
        wait(NULL);
    }
    
    //To complie
    //gcc -Wall -pedantic -g -o step2 step2.c
}