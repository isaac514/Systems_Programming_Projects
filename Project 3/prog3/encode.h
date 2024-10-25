
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

//Read bit function
int readbit()
{
    static unsigned char c;
    static int bit_count = 0;
    int retcheck;

    if (bit_count == 0)
    {
        retcheck = read_byte();
        if(retcheck == 256)
        {
            return 2;
        }
        c = (unsigned char) retcheck;
        bit_count = 8;
    }
    int bit = (c >> (bit_count-1)) & 1;
    bit_count--;

    return bit;    
}

//Read byte function
int read_byte()
{
    unsigned char c;
    static char buf[256];
    static int buf_count = 0;
    static int buf_index = 0;

    
    if(buf_count == 0)
    {
        buf_count = read(0 , buf , sizeof(buf));
        buf_index = 0;
    }

    //End of file
    if(buf_count == 0)
    {
        return 256;
    }

    c = buf[buf_index];
    buf_index++;
    buf_count--;
    
    return c;
}

//Write bit function
int writebit(int bit)
{
    static unsigned char c = 0;
    static int bit_count = 0;
    //flush out existing byte
    if (bit == 2)
    {
        //flush out existing byte
        while(bit_count < 8)
        {
            c = (c << 1) | 0;
            bit_count++;
        }
        write_byte(c);
        bit_count = 0;
        return 0;
    }
    c = (c << 1) | (bit & 1);
    bit_count++;

    if (bit_count == 8)
    {
        write_byte(c);
        bit_count = 0;
    }
    return 0;
}

//Write byte function
int write_byte(int input)
{
    char c;
    static char buf[256];
    static int buf_count = 0;
    static int buf_index = 0; 

    if ( input == 256)
    {
        write(1 , buf , buf_count);
        buf_count = 0;
        buf_index = 0;
        return 0;
    }

    if (buf_count < 256)
    {
        buf[buf_index] = (char) input;
        buf_index++;
        buf_count++;
    }

    if (buf_count == 256)
    {
        //Buffer is full, flush to file
        write(1 , buf , 256);
        buf_count = 0;
        buf_index = 0;
    }

    return 0;
}

//Usage: n can be at most 15
int binaryConvertFreq(int n)
{
    // 4 bit code
    for ( int i = 3 ; i >= 0 ; i-- )
    {
        int k = n >> i;
        if ( k & 1)
        {
            writebit(1);
        }
        else
        { 
            writebit(0);
        }

    }
}


//Usage: n can be at most 255
int binaryConvertInFreq(int n)
{
    //8 bit code
    for ( int i = 7 ; i >= 0 ; i-- )
    {
        int k = n >> i;
        if ( k & 1)
        {
            //Replace with writebit
            writebit(1);
        }
        else
        {   
            //Replace with writebit
            writebit(0);
        }
    }
}

//Compare function
int  cmpfunc ( const void *a , const void *b)
{
    int num1 = *(int*) a;
    int num2 = *(int*) b;

    if (num1 > num2)
    {
        return -1;
    }
    else if (num1 < num2)
    {
        return 1;
    }
    return 0;
}

//subRoutine method for index finding
int *subRountine(int arr[] , int k , int ArrSize , int *retArr ) 
{
    int pos;
    int temp[ArrSize];

        for( int i = 0 ; i < ArrSize ; i++)
        {
            temp[i] = 0;
        }
    
        //First duplicate Array to temp array
        for ( int i = 0 ; i < ArrSize ; i++)
        {
            temp[i] = arr[i];
        }    

    //create output array for size k+1
    int outputArr[k+1];
    //Initialize output Array at zero to be zero
    outputArr[0] = 0;

    //Get output Array Size
    int outputArrSize = k + 1;

    for( int i = 0 ; i < outputArrSize ; i++)
        {
            outputArr[i] = 0;
        }

    //Sort the tempory array 
    qsort(temp , ArrSize , sizeof(int) , cmpfunc);

    //Search for the indexes of the k highest elements in the array
    pos = 0;
        while(pos < outputArrSize)
        {
            for (int j = 1 ; j <= k ; j++)
            {
                for ( int a = 1 ; a < ArrSize ; a++)
                {
                            //Case for duplicates
                            if (temp[pos] == temp[pos+1] )
                            {
                                //Add first occurance, check to make the temp[pos] isn't zero 
                                //which means this character does not exist in the text file
                                if (temp[pos] == arr[a] && (temp[pos] != 0))
                                {
                                    outputArr[pos+1] = a;
                                    pos++;
                                }
                                for (int i = 0 ; i < ArrSize ; i++)
                                {
                                    if (temp[pos] == arr[i] && (temp[pos] != 0))
                                    {
                                        outputArr[pos+1] = i;
                                        pos++;
                                        
                                    }
                                }
                            }
                        else
                        {
                            //Indexs of elements
                            if (temp[j-1] == arr[a])
                            {
                            outputArr[j] = a;
                            break;
                            }
                        }
                    }
                    pos++;
                }
            }

    //Return array
    //Caller is responsible for deallocation of memory
    retArr = (int*)malloc((outputArrSize) * sizeof(int));

    //Error checking for memory allocation
    if ( retArr == NULL )
    {
        return -1;
    }
    else
    {
        for ( int x = 1 ; x < outputArrSize ; x++)
        {
            retArr[x] = outputArr[x];
            
        }
        return retArr;
    }
}