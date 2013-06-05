#ifndef	_COMMON_H_
#define	_COMMON_H_

#include "xdr_nfs.h"

template <typename Type>
inline Type Difference(const Type a, const Type b)
{
	Type Result = 0;

	if( a < b )
	{
		Result = b - a;
	}
	else
	{
		Result = a - b;
	}

	return Result;
}

template <typename Type>
inline Type Min(const Type a, const Type b)
{
	Type Result;

	if(a < b)
	{
		Result = a;
	}
	else
	{
		Result = b;
	}

	return Result;
		
}

template <typename Type>
inline Type Max(const Type a, const Type b)
{
	Type Result;

	if(a < b)
	{
		Result = b;
	}
	else
	{
		Result = a;
	}

	return Result;
}

void printMem(void *pTarget, int32 iSize);

#endif	// _COMMON_H_
