#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <memory>
#include <pthread.h>
#include "CNetwork.h"

CNetwork::CNetwork( )
{
	m_pRecvBuffer = new CBuffer( DEFAULT_RECV_BUFFER_SIZE );
	m_pProcBuffer = new CBuffer( DEFAULT_RECV_BUFFER_SIZE );
	
	m_nSocketSendBufferSize = DEFAULT_SEND_SOCKET_BUFFER_SIZE;
	m_nSocketRecvBufferSize = DEFAULT_RECV_SOCKET_BUFFER_SIZE;
	
	m_pRecvParam = NULL;
    m_lingerop.l_onoff = 1; 
    m_lingerop.l_linger = 0;

	m_bIsProcSuccessed = true;
}

CNetwork::~CNetwork()
{
	if(m_pRecvBuffer != NULL)
	{
		m_pRecvBuffer->Destroy();
		delete m_pRecvBuffer;
		m_pRecvBuffer = NULL;
	}
	
	if(m_pProcBuffer != NULL)
	{
		m_pProcBuffer->Destroy();
		delete m_pProcBuffer;
		m_pProcBuffer = NULL;
	}
}

void CNetwork::ReallocRecvBuffer(int nRecvBufferSize)
{
	m_pRecvBuffer->ReAllocBuffer(nRecvBufferSize);
	m_pProcBuffer->ReAllocBuffer(nRecvBufferSize);
}

void CNetwork::SetSocketSendBufferSize( unsigned int nSendBufferSize )
{
	m_nSocketSendBufferSize = nSendBufferSize;
}

void CNetwork::SetSocketRecvBufferSize( unsigned int nRecvBufferSize )
{
	m_nSocketRecvBufferSize = nRecvBufferSize;
}

int CNetwork::GetSocketSendBufferSize(void)
{
	return m_nSocketSendBufferSize;
}

int CNetwork::GetSocketRecvBufferSize(void)
{
	return m_nSocketRecvBufferSize;
}

#ifdef SwapBuffer
void CNetwork::SetProcSuccessed(bool bIsProcSuccessed)
{
//	pthread_mutex_lock(&m_mutexMsgProc);
	m_bIsProcSuccessed = bIsProcSuccessed;
//	pthread_mutex_unlock(&m_mutexMsgProc);
}

bool CNetwork::GetProcSuccessed(void)
{
//	pthread_mutex_lock(&m_mutexMsgProc);
//	bool bIsProcSuccessed = m_bIsProcSuccessed;
//	pthread_mutex_unlock(&m_mutexMsgProc);

//	return bIsProcSuccessed;
    return m_bIsProcSuccessed;
}

void CNetwork::ChangeBuffer(void)
{
	CBuffer* pChangeBuffer = m_pRecvBuffer;
	m_pRecvBuffer = m_pProcBuffer;
	m_pProcBuffer = pChangeBuffer;
}
#endif
