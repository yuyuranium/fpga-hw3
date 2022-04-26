

/***************************** Include Files *******************************/
#include "p3_2_sort.h"

/* My thought on the driver code, feel free to remove these comments once done -- by yuyuranium */
/* void sort(unsigned char *arr)
 * {
 *   loop i = 1 ~ 4:
 *     assemble 4 elements in arr into a u32
 *     write the u32 to the memory-mapped register[i] via AXI 
 *   write 1 to reg0
 *
 *   while (reg0 == 1):
 *     read reg0
 *
 *   loop i = 1 ~ 4:
 *     retrieve u32 data via AXI and store to temp
 *     loop j = 1 ~ 4:
 *       arr[i * 4 + j] = temp & 0xff
 *       temp = temp >> 8
 * }
 *
 */

/************************** Function Definitions ***************************/
void sort(UINTPTR baseAddr, u32 data1, u32 data2, u32 data3, u32 data4)
{


	P3_2_SORT_mWriteReg(baseAddr, 4, data1);
    P3_2_SORT_mWriteReg(baseAddr, 8, data2);
    P3_2_SORT_mWriteReg(baseAddr, 12, data3);
    P3_2_SORT_mWriteReg(baseAddr, 16, data4);
    
    printf("data_in:%x,%x,%x,%x\n",P3_2_SORT_mReadReg(baseAddr,4),P3_2_SORT_mReadReg(baseAddr,8),P3_2_SORT_mReadReg(baseAddr,12),P3_2_SORT_mReadReg(baseAddr,16));


    // write busy signal into control reg
    P3_2_SORT_mWriteReg(baseAddr, 0, 1);
    
    u32 sorted_data[4] = {0};
    u32 result[16] = {0};
    // wait data after sorting
    while(P3_2_SORT_mReadReg(baseAddr,0) == 1);

    // store the data
    sorted_data[0] = P3_2_SORT_mReadReg(baseAddr, 20);
    sorted_data[1] = P3_2_SORT_mReadReg(baseAddr, 24);
    sorted_data[2] = P3_2_SORT_mReadReg(baseAddr, 28);
    sorted_data[3] = P3_2_SORT_mReadReg(baseAddr, 32);

    printf("data_out:%x,%x,%x,%x\n",P3_2_SORT_mReadReg(baseAddr,20),P3_2_SORT_mReadReg(baseAddr,24),P3_2_SORT_mReadReg(baseAddr,28),P3_2_SORT_mReadReg(baseAddr,32));

    // do data mask;
    for(int times = 0; times < 4; times++)
    {
        for(int i = 0; i<4; i++)
        {
        	u32 data_mask = sorted_data[times] & 0xff;
            result[4*times+i] = data_mask;
            sorted_data[times] = sorted_data[times] >> 8;
        }
    }

    // print result
    printf("the result is: ");
    for(int i = 0; i < 16; i++)
    {
        printf("%u ",result[i]);
    }
    printf("\n\n");
}
