#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <memory>
#include <sockLib.h>
#include <in.h>
#include "CTcpClient.h"

//pthread_cond_t CTcpClient::m_thread_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t CTcpClient::m_mutex_lock = PTHREAD_MUTEX_INITIALIZER;

CTcpClient::CTcpClient( )
{
	m_eClientStatus = E_CLIENT_NOACTION;
	m_fpExternRecv = NULL;
	m_fpDisconnect = NULL;
	m_fpConnect = NULL;
	m_fpDoRecvCall = DoRecvCall;
	m_bIsCreated = false;
	m_nSock = 0;
	m_nPort = 0;
	m_pConnectionParam = NULL;
	memset( m_strServerIp, 0, sizeof( m_strServerIp ) );
	setClose( );
	if( m_pRecvBuffer == NULL )
	{
		m_pRecvBuffer = new CBuffer( DEFAUL_CLIENT_RECV_BUFFER );
	}

	CreateThread( );
}

CTcpClient::~CTcpClient( )
{
	Destroy( );
	if( m_pRecvBuffer != NULL )
	{
		m_pRecvBuffer->Destroy( );
		delete m_pRecvBuffer;
		m_pRecvBuffer = NULL;
	}
}

int CTcpClient::CreateSocket( void )
{
	if( isOpen( ) )
	{
		return 0;
	}

	shutdown( m_nSock, SHUT_RDWR );
//	close( m_nSock );

	m_nSock = (int)socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );

	if( m_nSock == ERROR )
	{
		fprintf( stderr, "Socket Open Error\n" );
		return -1;
	}
	setOpen( );

	/* sock opt 설정 */
	int nSocOptError = 0;
	//소켓 송신 버퍼 설정 
	if( m_nSocketSendBufferSize > 0 )
		nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_SNDBUF, (const char*)&m_nSocketSendBufferSize, sizeof( m_nSocketSendBufferSize ) );

	//소켓 수신 버퍼 설정
	if( m_nSocketRecvBufferSize > 0 )
		nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_RCVBUF, (const char*)&m_nSocketRecvBufferSize, sizeof( m_nSocketRecvBufferSize ) );

	//보류중인 데이터를 버리고 연결 중단
	nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_LINGER, (const char*)&m_lingerop, sizeof( m_lingerop ) );

	int option = 1; //option 설정
	nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof( option ) );
	//	nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&option, sizeof( option ) );
	//	nSocOptError = setsockopt( m_nSock, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof( option ) );
	if( ERROR == nSocOptError )
		fprintf( stderr, "Socket Option Error\n" );

	//	socklen_t nSize = sizeof(int);
	//
	//	int nSocketRecvBufferSize = 0;
	//	int nSocketSendBufferSize = 0;
	//	getsockopt( m_nSock, SOL_SOCKET, SO_RCVBUF, (char*)&nSocketRecvBufferSize, &nSize );
	//	getsockopt( m_nSock, SOL_SOCKET, SO_SNDBUF, (char*)&nSocketSendBufferSize, &nSize );

	return m_nSock;
}

void CTcpClient::InitServer( const char *pServerIp, int nPort )
{
	memset( &m_addrServerSock, 0, sizeof( m_addrServerSock ) );
	m_addrServerSock.sin_family = PF_INET;
	m_addrServerSock.sin_addr.s_addr = inet_addr( pServerIp );
	m_addrServerSock.sin_port = htons( nPort );
}

//int CTcpClient::Reconnect( void* pRecvParam )
//{
//	if( isOpen( ) )
//		return -1;
//
//	if( pRecvParam == NULL )
//		pRecvParam = m_pRecvParam;
//
//	char strServerIp[16];
//	memset( strServerIp, 0, sizeof( strServerIp ) );
//	memcpy( strServerIp, m_strServerIp, sizeof( m_strServerIp ) );
//
//	return Connect( strServerIp, m_nPort, m_fpExternRecv, pRecvParam );
//}

int CTcpClient::Connect( const char * pLocalIp, int nLocalPort, const char * pServerIp, int nServerPort, RECV_CALL fpExternRecv, void* pParam )
{
	int nRet = 0;
	if( isOpen( ) )
		return nRet;

	m_pRecvParam = NULL;
	size_t nIpLen = strlen( pServerIp );

	m_fpExternRecv = fpExternRecv;
	m_nPort = nServerPort;

	memset( m_strServerIp, 0, sizeof( m_strServerIp ) );
	memcpy( m_strServerIp, pServerIp, nIpLen );

	// 1. 소켓 생성.
	fprintf( stderr, "2-1\n" );
	nRet = CreateSocket( );
	if( nRet < 0 )
	{
		m_eClientStatus = E_CLIENT_CREATE_ERROR;
		fprintf( stderr, "2-2\n" );
		return nRet;
	}
	fprintf( stderr, "CreateSocket( ) \n" );

	// 2. 서버 정보 설정.
	InitServer( m_strServerIp, m_nPort );

	//2.5 -------------- ADD에서 Port제한 요청. bind() 추가.
	//	memset( &m_addrLocal, 0, sizeof( m_addrLocal ) );
	//	m_addrLocal.sin_family = PF_INET;
	//	m_addrLocal.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	//	m_addrLocal.sin_port = htons( 10000 );

	//	nRet = bind( m_nSock, (sockaddr*)&m_addrLocal, sizeof( m_addrLocal ) );
	//	if( nRet < 0 )
	//	{
	//		perror( "Socket Bind Error " );
	//		return nRet;
	//	}

	// 3. 서버 연결.
	struct timeval stTime;
	stTime.tv_sec = 10;
	stTime.tv_usec = 0;

	int nLen = sizeof( m_addrServerSock );
//	nRet = connect( m_nSock, (sockaddr*)&m_addrServerSock, nLen );
	nRet = connectWithTimeout( m_nSock, (sockaddr*)&m_addrServerSock, nLen, &stTime );
	if( ERROR == nRet )
	{
		fprintf( stderr, "connect Fail\n" );
		perror( "Connection fail " );
		m_eClientStatus = E_CLIENT_CONNECTION_ERROR;
		return nRet;
	}
	fprintf( stderr, "Connect( ) : %d \n", nRet );

	m_pRecvParam = pParam;
	//	if( m_fpConnect != NULL )
	//		m_fpConnect( m_pConnectionParam );

	m_eClientStatus = E_CLIENT_CONNECTION_SUCCESS;
	fprintf( stderr, "return : %d \n", nRet );
	return nRet;
}

int CTcpClient::SendData( char* pBuffer, int nBufferLength )
{
	if( isClose( ) )
	{
		fprintf( stderr, "Send_Socket Closed\n" );
		m_eClientStatus = E_CLIENT_SEND_FAIL;
		return -1;
	}

	//소켓 버퍼 사이즈를 넘겼을 경우, 어떻게 보낼지 결정하자
	if( nBufferLength > m_nSocketSendBufferSize )
		return send( m_nSock, pBuffer, m_nSocketSendBufferSize, 0 );
	//		return send( m_nSock, pBuffer, m_nSocketSendBufferSize, MSG_NOSIGNAL ); //상대편 연결이 닫힌경우에도 SIGPIPE 시그널을 발생시키지 않음
	else
		return send( m_nSock, pBuffer, nBufferLength, 0 );
}

void CTcpClient::CloseSocket( void )
{
	if( isClose( ) )
	{
		return;
	}

	shutdown( m_nSock, SHUT_RDWR );
//	close( m_nSock );
	m_nSock = 0;
	setClose( );

	if( m_fpDisconnect != NULL )
		m_fpDisconnect( this );
}

void CTcpClient::DoRecv( void )
{
	fprintf( stderr, "2.DoRecv() \n " );

	int nRecvSize = 0;
	while( isOpen( ) )
	{
		if( m_pRecvBuffer != NULL )
		{
			// m_pRecvBuffer->Reset();
			// recv()를 호출했을 때 0이 리턴되거나, send를 호출했을 때 -1로 에러 리턴하면서 errno가 EPIPE이면 상대편(peer)연결이 종료된 것
			nRecvSize = recv( m_nSock, m_pRecvBuffer->GetWritePositon( ), m_pRecvBuffer->RemainBuffer( ), 0 );

			if( m_pRecvBuffer == NULL )
				continue;

			if( nRecvSize > 0 )
			{
				m_pRecvBuffer->MoveWritePosition( nRecvSize );
			}
			else if( nRecvSize == 0 ) //X버튼 끊어지면 0 No Such File or Directory
			{
				m_pRecvBuffer->Reset( );
				perror( "CloseSocket()_0_perror!! " );
				CloseSocket( );
			}
			else //몇초후에 여기 //Process 종료
			{
				m_pRecvBuffer->Reset( );
				perror( "CloseSocket()_1_perror!! " );
				CloseSocket( );
			}

			if( m_pRecvBuffer->GetBufferingSize( ) > 0 )
			{
				m_fpExternRecv( *m_pRecvBuffer, m_pRecvParam );
			}
		}
	}
}

void* CTcpClient::DoRecvCall( void *lParam )
{
	fprintf( stderr, "Thread_test\n" );

	CTcpClient *pThis = (CTcpClient*)lParam;

	while( 1 )
	{
		fprintf( stderr, "1.DoRecvCall()\n " );
		if( pThis == NULL )
		{
			//  GP_LOG_INFO((char*)"HTClient - DoRecvCall Error -> pThis is NULL",E_LOG_STATUS_LEVEL_2);
		}
		else
		{
			pThis->DoRecv( );
		}
		sleep( 1 ); //110610
	}

	return 0;
}

void CTcpClient::BindConnectionEvent( CONNECT_CALL fpConnection )
{
	m_fpConnect = fpConnection;
}

void CTcpClient::BindDisconnectionEvent( CONNECT_CALL fpDisconnection )
{
	m_fpDisconnect = fpDisconnection;
}

void CTcpClient::CreateThread( )
{
	if( m_bIsCreated )
		return;

	int nRet = 0;
//Thread 우선순위가 설정이 안되
	pthread_attr_t attr;
	sched_param param;
	pthread_attr_init( &attr );
	nRet = pthread_attr_getschedparam( &attr, &param );
	attr.threadAttrSchedpolicy = PTHREAD_INHERIT_SCHED;
	param.sched_priority = 111;
	nRet = pthread_attr_setschedparam( &attr, &param );
	
	if( ( nRet = pthread_create( &m_recvThread, &attr , m_fpDoRecvCall, this ) ) != 0 )
	{
		fprintf( stderr, "RecvThread Create Fail ret : %d, err : %d \n", nRet, errno );
	}

	fprintf( stderr, "RecvThread Create \n " );
	m_bIsCreated = true;
}

void CTcpClient::Destroy( )
{
	CloseSocket( );
	if( m_bIsCreated )
		pthread_cancel( m_recvThread );
	m_bIsCreated = false;
}

void CTcpClient::SetConnectionParam( void* pParam )
{
	m_pConnectionParam = pParam;
}
