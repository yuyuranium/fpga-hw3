/*
 *
 * sorting.c
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

#define SIZE 20 
#define SW_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID

XGpio SW_Gpio;

void Sorting(int* data,int mode)
{
	/*
	 *mode 1 for DESCENDING
	 *mode 0 for INCREMENT
	 */
	int key;
	int j;
	for(int i=1; i<SIZE; i++)
	{
		key = data[i];
		for(j = i-1; j>=0; j--)
		{
			if(!((key>data[j])^mode))
				data[j+1] = data[j];
			else
				break;
		}
		data[j+1] = key;
	}
}

int main()
{
	int SW_Status;
	int data[SIZE];
	char str[100];
	u32 sw_data;
	/* Initialize the GPIO driver */
	SW_Status = XGpio_Initialize(&SW_Gpio, SW_DEVICE_ID);
	if (SW_Status != XST_SUCCESS)
	{
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}
	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&SW_Gpio, 1, 0x0f);

	while(1)
	{
		/* Scan input data*/
		for(int i=0; i<SIZE; i++)
		{
			printf("Enter the No.%d Data:", i+1);
			scanf("%s", str);
			printf("%s\r\n", str);
			data[i] = atoi(str);
			
			/*non-numeric-input-error detection*/
			for(int j=0; j<strlen(str); j++)
			{
				if(!isdigit(str[j]))
				{
					printf("Please re-enter a numeric input.\r\n");
					data[i--] = 0;
					break;
				}
			}

		}
		/* Sorting Process*/
		sw_data = XGpio_DiscreteRead(&SW_Gpio, 1);
		xil_printf("switches data = %d\r\n", sw_data);
		Sorting(data, sw_data);
		/* Show the Result*/
		printf("RESULT:");
		for(int i=0; i<SIZE; i++)
		{
			printf(" %d", data[i]);
		}
		printf("\r\n\r\n");
	};
	xil_printf("Successfully ran Gpio Example\r\n");
	return XST_SUCCESS;
}

