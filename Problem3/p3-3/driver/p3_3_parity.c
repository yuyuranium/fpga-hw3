

/***************************** Include Files *******************************/
#include "p3_3_parity.h"

/************************** Function Definitions ***************************/
u32 parity(UINTPTR baseAddr, u32 data)
{
	u32 result;

	P3_3_PARITY_mWriteReg(baseAddr, 0, data);
	result = P3_3_PARITY_mReadReg (baseAddr, 4);
	return result;
}
