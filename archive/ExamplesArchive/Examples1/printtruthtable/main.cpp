#include "stdio.h"
#include "math.h"
#include <cstring>


void PrintTruthTable(int n)
{
    //setup the mapping from bool to char
    // 0 = true
    // 1 = false
    char lookup[2] = {'T','F'};
    unsigned int current_table_permutation = 0;

    //create the output buffer that will hold the n elements
    //add '1' to array size account for the null termination character
    //freestore allocation BEWARE: potential memory leak
    char* line_output_buffer = new char[n+1]; 
    
    //zeroize the buffer
    memset(line_output_buffer,0,n+1);

    //foreach permutation
    for (int index = 0; index < _Pow_int(2,n); index++)
    {
        //construct your bit array
        current_table_permutation = index;
        //for each bit in array
        for (int i = 0; i < n ; i++)
        {
            //extract the least significant bit
            size_t val = current_table_permutation & 0x1;
            //lookup the corresponding true/false character
            //Increment the line output buffer in reverse order
            line_output_buffer[n-i-1] =  lookup[val];
            //shift the bits over by one in preparation for the next pass
            current_table_permutation = current_table_permutation >> 1;
        }
        //Print the resulting character array as a null-terminated string
        printf("%s\n",line_output_buffer);
    }
    //deallocate freestore memory
    delete[] line_output_buffer;
}



int main()
{
    PrintTruthTable(18);
    return 0;
}
