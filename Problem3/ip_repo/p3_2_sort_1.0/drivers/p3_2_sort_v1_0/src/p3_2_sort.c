

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
