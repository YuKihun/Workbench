#include "common.h"

void printMem(void *pTarget, int32 iSize)
{
	char *p = (char *)pTarget;
	
	for ( int32 i = 0; i < iSize; i ++ )
	{
		printf("%02x ", (char)p[i]);
		if ( i % 8 == 7 )
		{
			printf("\n");
		}
	}
	printf("\n");
}


