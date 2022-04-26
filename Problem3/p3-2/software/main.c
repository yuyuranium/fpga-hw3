/*
 * 
 * main.c
 *  
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "p3_2_sort.h"


int main()
{
	printf("Program Start.\n\n\r");
	while(getchar() != EOF) {
        
        int A[16] = {0};

        // generate number for input
        printf("input sequence is: ");
        for(int i = 0; i < 16; i++)
        {
            A[i] = rand()%256;
            printf("%d ",A[i]);
        }
        printf("\n");

        // doing data mask
		u32 data[4] = {0};
        for(int i = 0; i<4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
            	u32 data_mask = A[4*i+j] & 0xff;
                data[i] = data[i] + (data_mask << 8*j);
            }
        }
        sort(XPAR_P3_2_SORT_0_S00_AXI_BASEADDR, data[0], data[1], data[2], data[3]);
	}
	printf("Program End.\n\r");
    return 0;
}
