/*
 * 
 * main.c
 *  
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "p3_3_parity.h"


int main()
{
	u32 data;
	u32 result;
	
	printf("Program Start.\n\r");
	while(getchar() != EOF) {

		printf("Input data:");
		scanf("%d", &data);
		printf(" %d\r\n", data);
		

		result = parity(XPAR_P3_3_PARITY_0_S00_AXI_BASEADDR, data);


		if(result == 1)
		{
			printf("parity bit is one\n\r");
		}
		else
		{
			printf("parity bit is zero\n\r");
		}

	}
	printf("Program End.\n\r");
    return 0;
}
