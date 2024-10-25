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

int main( int argc , int *argv[])
{
    //argv 0 => ./prose , 1 => binary for execvp , 2-n => arguements for execvp 
    //Notes: 
        //-Need to write in size of 256 bytes

    //Determines parent/child        
    pid_t pid;

    //Size of array for execvp
    int argn;

    //File descriptor for outfile
    int outFd;

    //Program path for subProgram 
    char* progPath;
    //Intialize size array for execvp 
    argn = argc - 1;

    //Files for redirecting stderr and stdout
    char* outfile;
    char* errfile;
    char* subArray[argn];

    //Populate array of execlp
    for (int i = 1 ; i < argc ; i++)
    {
        subArray[i-1] = argv[i];
    }

    char temp1[1000];
    char temp2[1000];

    strcpy(temp1 , (char*)argv[1]);
    strcpy(temp2 , (char*)".stdout");
    //Get the file name 
    strcat(temp1 , temp2);

    //open file
    outFd = open(temp1 , O_RDWR | O_CREAT | O_TRUNC , 0644);

    //Will need to open file for stderr

    
    //Write remain arguments to the outfile
    for(int j = 1 ; j < argn ; j++)
    {
        //write arguments to outfile
        write(outFd , (char*)subArray[j] ,strlen(subArray[j]));
        write(outFd , "\n" , strlen("\n"));
        
    }
    
    //To complie
    //gcc -Wall -pedantic -g -o step1 step1.c
}