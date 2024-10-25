#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  F_first        1   /* This is the first call to the function. */
#define  F_last         2   /* This is the last call to the function. Free the memory area. */
#define  F_data_int     3   /* Void * argument points to integer data. */
#define  F_data_char    4   /* Void * argument points to character string. */
#define  F_data_float   5   /* Void * argument points to a float data value. */
#define  F_print        6   /* Print the accumulated values. */

void * f (int code, void * mem, void * data)
{
    long long int z_data = (long long int)data;
    short* end;
    char* local;

    switch (code)
    {
    //F_first intially allocates memory to store values passed in by function argument 
    case F_first:
    //Allocate memory intially 
    //Constraint for mem being null it F_first is null
        if (mem != 0)
        {
            return -1;
        }
        else
        {
            //Intitally allocate Memory
            mem = (void *) malloc(z_data);
            //Allocate to two bytes for "end "
            end = (short*)mem;
            //Store offset to 2 
            *end = 2;
            break;
        }
        return mem;

    //F_data_int stores int values
    case F_data_int: 
        //Read the offset
        end = (short *)mem;

        //Increment local by offset bytes
        local = (char*)mem;
        local += *end;

        //local variable points to the beginning of free space,
        //Store data type and point to where data is to be stored
        *local = F_data_int;
        local++;

        //Write the data to the next four bytes
        *(int *)local = *((int *) data);

        //Update the offset
        *end += 5;
        break;
    
    //F_data_float stores float values
    case F_data_float: 
        //Read the offset
        end = (short *)mem;

        //Increment local by offset bytes
        local = (char*)mem;
        local += *end;

        //local variable points to the beginning of free space,
        //Store data type and point to where data is to be stored
        *local = F_data_float;
        local++;

        //Write the data to the next four bytes
        *(float *)local = *((float *) data);

        //Update the offset
        *end += 5;
        break;

    //F_data_char stores char values 
    case F_data_char: 
        //Read offset
        end = (short *)mem;
        
        //Increment local by offset bytes
        local = (char*)mem;
        local += *end;

        //local variable points to the beginning of free space,
        //Store data and point to where data is to be stored
        *local = F_data_char;
        local++;
        (*end)++;

        //Size of chars passed in
       int stringSize = strlen(data); 
       
        //Create an array of char holding the contents of data 
        char* str = (char*)data;
        //Size of chars passed in 
        
        //Add each char into memory 
        for (int i = 0 ; i < stringSize ; i++)
        {
            //increment offset
            (*end)++;
            *local = str[i];
            local++;
        }
        (*end)++;
        *local = 0;
        ++local;
        break;

    //F_print prints the values passed in by user
    case F_print: 
        //Read offset
        //At this point the offset (end) should have been continously updated by calls to the previous cases 
        end = (short*)mem;

        //Local pointer to index through the memory segment
        local =(char*)mem;
        local += 2;

        // do the following cases while local is not equal to offset
        //Case for F_data_int
        do
        {
            if (*local == F_data_int)
            {
                local++;
                int temp = (int)*local;
            
                printf("%d" , temp);
                local+=4;
            }
            //Case for F_data_float
            else if (*local == F_data_float) 
            {
                local++;
                float temp = *(float*)local;
                printf("%.2f ", temp);
                local+=4;
            }
            //Case for F_data_char
            else if (*local == F_data_char) 
            {
                //printf("test\n");
                //Get into the string
                local++;

            //while the pointer value is not the null terminating character
                while(*local != 0)
                {
                    printf("%c" , *local);
                    local++;
                }
                local++;
            } 
            else 
            {
                printf("Unknown datatype:%d\n", *local);
            }
            //if local is greater than or equal to the offset we have reached the end of the memory segment and can exit
        } while (local < (char*)mem + *end);
        break;

    //F_last frees memory region
    case F_last: 
    //Call to free to free memory pointer
        free(mem);
        break;
    }
    //printf("%d : %p %p %lld\n",code, mem, data, z_data);
    return mem;
}
 