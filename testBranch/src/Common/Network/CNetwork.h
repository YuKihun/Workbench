#ifndef CNETWORK_H_
#define CNETWORK_H_

#include <netinet/in.h>
#include <signal.h>
#include "Global.h"
#include "../Buffer/CBuffer.h"

typedef void (*CONNECT_CALL) ( void* );
typedef void (*RECV_CALL) ( CBuffer&, void* );
typedef void* (*DO_RECV) ( void* lParam );
typedef bool (*RECV_PROC) ( CBuffer&, void* );

const unsigned int DEFAULT_SEND_BUFFER_SIZE = 8192; //8k
const unsigned int DEFAULT_RECV_BUFFER_SIZE	= 8192;

const unsigned int DEFAULT_RECV_SOCKET_BUFFER_SIZE = 8192;
const unsigned int DEFAULT_SEND_SOCKET_BUFFER_SIZE = 8192;

class CNetwork
{
	public:
		CNetwork( );
		virtual ~CNetwork( );

	protected:
		E_HTNetworkType m_eNetworkType; //NetworkType

		CBuffer* m_pRecvBuffer;
		CBuffer* m_pProcBuffer;

		unsigned int m_nSocketRecvBufferSize; //socket buffer size
		unsigned int m_nSocketSendBufferSize; //socket buffer size
		
		void* m_pRecvParam;
		int m_nSock;
		linger m_lingerop; 
		bool m_bIsProcSuccessed;

	protected:
		void ReallocRecvBuffer( int nRecvBufferSize );
#ifdef SwapBuffer
		void ChangeBuffer( void ); //recv Buffer¿Í Proc Buffer ±³Ã¼
#endif

	public:
		void SetSocketSendBufferSize( unsigned int nSendBufferSize );
		void SetSocketRecvBufferSize( unsigned int nRecvBufferSize );
		int GetSocketSendBufferSize( );
		int GetSocketRecvBufferSize( );

#ifdef SwapBuffer
		void SetProcSuccessed( bool bIsProcSuccessed );
		bool GetProcSuccessed( void );
#endif
		
};

#endif /* CNETWORK_H_ */
