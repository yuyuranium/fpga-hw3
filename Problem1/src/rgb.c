/*
 *
 *rgb.c
 *
 */


#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

#define RGB_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID

#define PERIOD   10000

#define RED    0xff0000
#define ORANGE 0xff6100
#define YELLOW 0xffff00
#define GREEN  0x00ff00
#define BLUE   0x0000ff
#define PURPLE 0x7f1fff
#define WHITE  0xffffff

XGpio RGB_Gpio;

int main() {
	int RGB_Status;
	int RGB_State = PURPLE;
	int red, grn, blu, PeriodCnt=0;
	u32 rgb_data = 0x00;

	/* Initialize the GPIO driver */
	RGB_Status = XGpio_Initialize(&RGB_Gpio, RGB_DEVICE_ID);
	if (RGB_Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}
	/* Set the direction for RGB signals*/
	XGpio_SetDataDirection(&RGB_Gpio, 1, 0x00);

	while (1) {

		/*Control Color States*/
		switch(RGB_State){
			case RED:
				RGB_State = ORANGE;
				break;
			case ORANGE :
				RGB_State = YELLOW;
				break;
			case YELLOW :
				RGB_State = GREEN;
				break;
			case GREEN :
				RGB_State = BLUE;
				break;
			case BLUE :
				RGB_State = PURPLE;
				break;
			case PURPLE :
				RGB_State = RED;
				break;
			default:
				RGB_State = WHITE;
				break;
		}

		/*Decide the duty cycle for RGB*/
		red = (RGB_State & 0xff0000) >> 16;
		grn = (RGB_State & 0x00ff00) >> 8 ;
		blu = (RGB_State & 0x0000ff);

		/*outer loop is repeat so that RGB change is visible*/
		for(int j = 0; j < PERIOD; j++){
		/*inner loop mix the color for RGB led*/
			for (int i = 0; i < 0xff; i++){
			rgb_data = ((i<blu)<<2) + ((i<grn)<<1) + (i<red);
			XGpio_DiscreteWrite(&RGB_Gpio,1,rgb_data);
			}
		}
	}

	xil_printf("Successfully ran HW3 Problem1 RGB-LED\r\n");
	return XST_SUCCESS;
}

