/*
 * 
 * main.c
 *  
 */

#include <stdio.h>
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include "p3_1_arith.h"


int main()
{
	u32	opd1, opd2;
    u32 arith_mode;
	u32 result;
	
	printf("Program Start.\n\r");
	while(getchar() != EOF) {

		printf("Input opdata1:");
		scanf("%d", &opd1);
		printf(" %d\r\n", opd1);
		
		printf("Input opdata2:");
		scanf("%d", &opd2);
		printf(" %d\r\n", opd2);
		
		printf("Input arithmetic mode:");
		scanf("%d",&arith_mode);
		printf(" %d\r\n", arith_mode);

		result = arith(XPAR_P3_1_ARITH_0_S00_AXI_BASEADDR, opd1, opd2, arith_mode);


		switch(arith_mode)
		{
			case 0: printf("opdata1 + opdata2 = %d\n\r", result);
					break;
			case 1: printf("opdata1 - opdata2 = %d\n\r", result);
					break;
			case 2: printf("opdata1 x opdata2 = %d\n\r", result);
					break;
			default: printf("arithmetic mode is error\n\r");
					break;
		}

	}
	printf("Program End.\n\r");
    return 0;
}