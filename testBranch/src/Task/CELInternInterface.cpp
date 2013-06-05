#include <string.h>
#include "CELInternInterface.h"
#include "unistd.h"
#include "../Common/common.h"
#include "../Common/MsgQueuePool/CMsgQueuePool.h"
#include "../Common/MsgFormat/InternMsg.h"

CELInternInterface* CELInternInterface::GetInstance( )
{
	static CELInternInterface Instance;
	return &Instance;
}


CELInternInterface::CELInternInterface( )
{
}

CELInternInterface::~CELInternInterface( )
{
	
}

void CELInternInterface::startBehavior( )
{
	
}

void CELInternInterface::SetSocketSendBufferSize( int nSize )
{
    CNetwork::SetSocketSendBufferSize( nSize );
}

void CELInternInterface::SetSocketRecvBufferSize( int nSize )
{
    CNetwork::SetSocketRecvBufferSize( nSize );
}

bool CELInternInterface::ServerBegin( const char * pLocalIp, int nLocalPort )
{
	if( true == m_bIsOpen )
	{
		return false;
	}
	
	if( CTcpServer::Ready( pLocalIp, nLocalPort, RecvMain, this ) == OK )
	{
		fprintf( stderr, "Server Begin Success \n" );
		m_bIsOpen = true;
		
		return true;
	}

	CloseSocket( );
	fprintf( stderr, "Server Begin Fail\n" );
	return false;
}
//bool CELInternInterface::Begin( const char * pLocalIp, int nLocalPort , const char * pServerIp, int nServerPort ) //, RECV_PROC fpRecvProc )
//{
//	fprintf( stderr, "Begin() \n" );
//	if( true == m_bIsOpen )
//	{
//		return false;
//		fprintf( stderr, "1\n" );
//	}
//	fprintf( stderr, "2\n" );
//	
////	m_fpRecvProc = fpRecvProc;
////	BindConnectionEvent( &CHTAdaptorCND::Connected );
////	BindDisconnectionEvent( &CHTAdaptorCND::Disconnected );
//
//	if( CTcpClient::Connect( pLocalIp, nLocalPort, pServerIp, nServerPort, RecvMain, this ) == OK )
//	{
//		fprintf( stderr, "3\n" );
//		fprintf( stderr, "Connect Success\n" );
//		m_bIsOpen = true;
//		
//		return true;
//	}
//
//	fprintf( stderr, "4\n" );
//	fprintf( stderr, "Connect Fail\n" );
//	return false;
//}

void CELInternInterface::RecvMain( CBuffer& buffer, void* pRecvParam )
{
	CELInternInterface *pThis = (CELInternInterface*)pRecvParam;
    if( pThis == NULL )
    {
        return;
    }

    if( buffer.GetBufferingSize() < 1 )
        return;
  
    // -- Test
    pThis->SendData( buffer.GetData(), buffer.GetBufferingSize(), 0);
    printMem( buffer.GetData(), buffer.GetBufferingSize() );
    fprintf( stderr, "1. Send Size : %d\n", buffer.GetBufferingSize() );

//    char * test = "\n";
//    pThis->SendData( test, sizeof( test ) + 1 , 0 );
//    fprintf( stderr, "2. Send Size : %d\n", buffer.GetBufferingSize() );
//    printMem( buffer.GetData(), buffer.GetBufferingSize() );
    // -- Test
    
    int nSendTest = 0;
    memmove( &nSendTest, buffer.GetData(), sizeof(nSendTest) );
    
    if( nSendTest || 0x01 )
    {
//    	msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_SYSCON ), (char*)&nSendTest, sizeof(nSendTest), NO_WAIT, MSG_PRI_NORMAL );
//        fprintf( stderr, "Q Send \n" );
    }
    
    buffer.Reset();
    
}


