#ifndef _INTERNMSG_H_
#define	_INTERNMSG_H_

const int anInternEthrOpCode[] =
{
		0xc1c20001, //초기화요구 
		
};

struct SInternLanMsgHeader
{
	int nOpCode;
	int nSize;
};

#endif

