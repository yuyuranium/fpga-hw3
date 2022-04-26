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
	printf("Program Start.\n");
	printf("press any buttom to start. \n\n\r");
	while(getchar() != EOF) {
        
        u32 A[16] = {0};

        // generate number for input
        printf("input sequence is: ");
        for(int i = 0; i < 16; i++)
        {
            A[i] = rand()%256;
            printf("%d ",A[i]);
        }
        printf("\n");

        sort(XPAR_P3_2_SORT_0_S00_AXI_BASEADDR, A, 16);
        // print result
        printf("the result is: ");
        for(int i = 0; i < 16; i++)
        {
            printf("%u ",A[i]);
        }
        printf("\n\n");
	}
	printf("Program End.\n\r");
    return 0;
}
