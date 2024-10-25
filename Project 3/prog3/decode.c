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

int main(int argc , int argv[])
{
    int indexCounter;
    int bitVal;
    int repeatCount;
    unsigned char c;
    unsigned char dictArr[16];
    dictArr[0] = 0;
    

    //Read first 15 bytes into dictionary array
    for( int i = 1 ; i < 16 ; i++)
    {
        read(0 , &dictArr[i] , sizeof(dictArr[0]));
    }

    while(1)
    {
        //Case for infequent character
        //Reset indexCounter
        indexCounter = 0;
        bitVal = readbit();
        if(bitVal == 0)
        {
            //Get the encoded value and write it
            //Read bit 8 times
            for(int i = 0 ; i < 8 ; i++ )
            {
                c = (c << 1) | readbit();
            }
            //Write character
            write(1 , &c , sizeof(c));
            
        }
        //Case for frequent characters
        else
        {
            if(readbit() == 0)
            {
                //Case for the null symbolmv
                unsigned char n = 0;
                write(1 , &n , sizeof(n));
            }

            //Frequent character, read repeat count  and get index
            else
            {
                //Get the repeat count
                //bitval = readbit();
                //Repeat Count 1X
                if (readbit() == 1)
                {
                    //bitval = readbit();
                    if(readbit() == 1)
                    {
                        //Case for 11
                        repeatCount = 4;
                    }
                    else
                    {
                        //Case for 10
                        repeatCount = 3;
                    }
                }
                //Repeat Count 0X
                else
                {
                    if(readbit() == 1)
                    {
                        //Case for 01
                        repeatCount = 2;
                    }
                    else
                    {
                        //Case for 00 
                        repeatCount = 1;
                    }
                }


                //Get the index
                for (int j = 0 ; j < 4 ; j++)
                {
                    //First iteration
                    if(j == 0)
                    {
                        if(readbit() == 1)
                        {
                            //Add 8
                            indexCounter+=8;
                        }
                        else
                        {
                            //Dont add
                        }
                    }

                    //Second iteration
                    if(j == 1)
                    {
                        if(readbit() == 1)
                        {
                            //Add 4
                            indexCounter+=4;
                        }
                        else
                        {
                            //Dont add
                        }  
                    }
                    
                    //Thrid iteration
                    if (j == 2)
                    {
                        if(readbit() == 1)
                        {
                            //Add 2
                            indexCounter+=2;
                        }
                        else
                        {
                            //Dont add
                        }  

                    }

                    //Fourth iteration
                    if (j == 3)
                    {
                        if(readbit() == 1)
                        {
                            //Add 4
                            indexCounter+=1;
                        }
                        else
                        {
                            //Dont add
                        }  
                    }
                }

                //index and repeat have been generated
                if((repeatCount == 1) && (indexCounter == 0) )
                {
                    //End of file encoding have been found
                    //break out of while loop
                    return 0;
                }

                //Find character in the dictionary and write it repeatCount times
                else
                {
                    for (int i = 0 ; i < repeatCount ; i++)
                    {
                        c = (char)dictArr[indexCounter];
                        write(1 , &c , sizeof(c));
                    }
                }
            }
        }
    }
    return 0;
}