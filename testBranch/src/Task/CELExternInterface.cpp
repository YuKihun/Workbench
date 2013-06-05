#include <string.h>
#include "CELExternInterface.h"
#include "unistd.h"
#include "../Common/common.h"
#include "../Common/MsgQueuePool/CMsgQueuePool.h"

CELExternInterface* CELExternInterface::GetInstance( )
{
	static CELExternInterface Instance;
	return &Instance;
}

CELExternInterface::CELExternInterface( )
{
}

CELExternInterface::~CELExternInterface( )
{

}

void CELExternInterface::startBehavior( )
{

}

void CELExternInterface::SetSocketSendBufferSize( int nSize )
{
	CNetwork::SetSocketSendBufferSize( nSize );
}

void CELExternInterface::SetSocketRecvBufferSize( int nSize )
{
	CNetwork::SetSocketRecvBufferSize( nSize );
}

bool CELExternInterface::ServerBegin( const char * pLocalIp, int nLocalPort )
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
//bool CELExternInterface::Begin( const char * pLocalIp, int nLocalPort , const char * pServerIp, int nServerPort ) //, RECV_PROC fpRecvProc )
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

void CELExternInterface::RecvMain( CBuffer& buffer, void* pRecvParam )
{
	CELExternInterface *pThis = (CELExternInterface*) (((SServerRecvParam*)pRecvParam)->pParam);
	int nCurrClient = ((SServerRecvParam*)pRecvParam)->nClientSock;

	if( pThis == NULL )
	{
		return;
	}

	if( buffer.GetBufferingSize( ) < 1 )
		return;
	
	//Test
	pThis->SendData( buffer.GetData( ), buffer.GetBufferingSize( ), nCurrClient );
	printMem( buffer.GetData( ), buffer.GetBufferingSize( ) );
	//Test

	int nSendTest = 0;
	memmove( &nSendTest, buffer.GetData( ), sizeof( nSendTest ) );

	switch( nSendTest )
	{
		case 0x3412 :
		{
			msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_SYSCON ), (char*)&nSendTest, sizeof( nSendTest ), NO_WAIT, MSG_PRI_NORMAL );
			fprintf( stderr, "Interface -> SysCtrl: Request Receiving Control \n" );
			break;
		}
		case 0x1234 :
		{
			msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_SYSCON ), (char*)&nSendTest, sizeof( nSendTest ), NO_WAIT, MSG_PRI_NORMAL );
			fprintf( stderr, "Interface -> SysCtrl : Request HighGain Receiving Control - Q Send \n" );
			break;
		}
		default :
			fprintf( stderr, "Interface : Default Recv \n" );
			break;
	}
	buffer.Reset( );

}

