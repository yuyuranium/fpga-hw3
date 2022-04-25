/***************************** Include Files *******************************/
#include "p3_1_arith.h"

/************************** Function Definitions ***************************/
u32 arith(UINTPTR baseAddr, u32 opd1, u32 opd2, u32 arith_mode)
{
    u32 result;

	u32 opd1_mask = opd1 & 0xff; //mask to 8 bits
	u32 opd2_mask = opd2 & 0xff; //mask to 8 bits
	u32 arith_mode_mask = arith_mode & 0x11; //mask to 2 bits

	u32 data = opd1_mask + (opd2_mask << 8) + (arith_mode_mask << 16);
	P3_1_ARITH_mWriteReg(baseAddr, 0, data);
	result = P3_1_ARITH_mReadReg (baseAddr, 4);
	return result;
}
