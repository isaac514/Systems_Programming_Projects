//Isaac Cooper
//CS_3411 Program 3
//03/17/24

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "encode.h"

//HELPER FUNCTIONS
    //Bit Functions: readbit() writebit() writebyte() readbyte() binaryConvertFreq() binaryConvertInFreq()
    //Sorting Functions: cmpfunc() subRoutine() 

int main(int argc , int argv[])
{
    unsigned char c;
    unsigned char next;
    int nextRet;
    int freq[256] = {0};
    int *dictonary;
    int ArrSize;
    int freqCount;
    int repeatCount;
    int retVal;
    int freqFlag;
    size_t bytesRead = 0;
    //Read until read of the file bytes in frequency array while the end of 
    //the file has not been reached
    while ( (bytesRead = read(0 , &c , sizeof(c))) > 0 )
    {
        //Need to read bytes into the frequency array
        freq[c]++;
    }
    //After the frequency array has been populated and before the call to subRoutine 
    //Set the occurences of the zero byte value to be zero
    freq[0] = 0;

    ArrSize = sizeof(freq) / sizeof(freq[0]);
    //Call subRoutine
    //Output dictionary, change subRoutine to return output array which will be of size 16
    //Need to pass in return array, subRoutine should output the dictonary
    //After encoding is generated need to free dictionary
    dictonary = subRountine(freq , 15 , ArrSize , dictonary);

    //output dictionary 
    for (int i = 1 ; i < 16 ; i++)
    {
        char buf[1];
        buf[0] = (char)dictonary[i];
        write(1 , buf , sizeof(buf));
    }

    //Reset the currency indicator in the file to the beginning to compare characters 
    retVal = lseek(0 , SEEK_SET , 0); 
    //Reset bytes reads
    bytesRead = 0;
    if(retVal == -1)
    {
        printf("File is not seekable");
        return -1;
    }

    //Read through the dictonary
    while ( (bytesRead = read(0 , &c , sizeof(c))) > 0 ) 
    {
        //If current character is a frequent character
        //Iterate over the dictonary
        freqFlag = 0;

        //Case for zero byte
        if(c == 0)
        {
            writebit(1);
            writebit(0);
        }
        else
        {
            for ( int i = 1 ; i < 16 ; i++ )
            {
                //case for frequent characters
                if ( c == dictonary[i] )
                {
                    //First beginning bits to be 11
                    writebit(1);
                    writebit(1); 

                    //Update frequency flag
                    freqFlag++;
                    //set repeast count
                    repeatCount = 1;

                        nextRet = read(0 , &next , sizeof(next));
                        //If reading from next returns zero
                        if(nextRet == 0)
                        {
                            writebit(0);
                            writebit(0);
                            binaryConvertFreq(i);
                            break;
                        }

                        while (next == c && (repeatCount < 4))
                        {   
                            repeatCount++;
                            read(0 , &next , sizeof(next));
                        }
                            //encoded the next two bits corresponding to the repeatCount
                            if(repeatCount == 4)
                            {
                                writebit(1); 
                                writebit(1);
                                lseek(0 , -1 , SEEK_CUR );
                            }
                            else if(repeatCount == 3)
                            {
                                writebit(1); 
                                writebit(0);
                                lseek(0 , -1 , SEEK_CUR );
                            }
                            else if(repeatCount == 2)
                            {
                                writebit(0); 
                                writebit(1);
                                lseek(0 , -1 , SEEK_CUR );
                            }
                            else
                            {
                                writebit(0); 
                                writebit(0);
                                lseek(0 , -1 , SEEK_CUR );
                            }

                            //encoded the index for the last four bits
                            binaryConvertFreq(i);
                            break;
                        }
                    }
                    //Case for infrequent characters
                    //Dictonary loop has finished so this is a infrequent character
                    if (freqFlag == 0)
                    {
                        writebit(0);
                        binaryConvertInFreq(c);
                    }     
                }
            }         

    //If the end of the file has been reached output end of file symbol
    if(bytesRead == 0) 
    {
        //Need to output the end of file symbol
        //Write first two bits as ones and rest with zeros
        writebit(1);
        writebit(1);
        writebit(0);
        writebit(0);
        writebit(0);
        writebit(0);
        writebit(0);
        writebit(0);

        writebit(2);
        write_byte(256);
    //All cases should be accounted for and the encoding is outputed
    }
    //free allocated memory
    free(dictonary);
}
