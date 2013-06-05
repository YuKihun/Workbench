#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <memory>
#include <sockLib.h>
#include <in.h>
#include "CTcpServer.h"

//pthread_cond_t CTcpServer::m_thread_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t CTcpServer::m_mutex_lock = PTHREAD_MUTEX_INITIALIZER;

CTcpServer::CTcpServer( )
{
	m_eServerStatus = E_SERVER_NOACTION;
	m_fpExternRecv = NULL;
	m_fpDisconnect = NULL;
	m_fpConnect = NULL;
	m_fpDoRecvCall = DoRecvCall;
	m_nSock = 0;
	m_nServerPort = 0;
	m_nCurClientNum = 0;
	m_pConnectionParam = NULL;
	memset( m_strServerIp, 0, sizeof( m_strServerIp ) );
	memset( &m_addrServerSock, 0, sizeof( m_addrServerSock ) );
	memset( &m_addrClientSock, 0, sizeof( m_addrClientSock ) );
	memset( &m_nClientSock, 0, sizeof( m_nClientSock ) );
	memset( &m_clientParam, 0, sizeof( m_clientParam ) );
	memset( &m_clientTemp, 0, sizeof( m_clientTemp ) );
	memset( &m_bIsCreated, 0, sizeof( m_bIsCreated ) );

	setClose( );
	if( m_pRecvBuffer == NULL )
	{
		m_pRecvBuffer = new CBuffer( DEFAUL_SERVER_RECV_BUFFER );
	}

	//	CreateThread( );
}

CTcpServer::~CTcpServer( )
{
	Destroy( );
	if( m_pRecvBuffer != NULL )
	{
		m_pRecvBuffer->Destroy( );
		delete m_pRecvBuffer;
		m_pRecvBuffer = NULL;
	}
}

int CTcpServer::CreateSocket( void )
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

	/* sock opt ���� */
	int nSocOptError = 0;
	//���� �۽� ���� ���� 
	if( m_nSocketSendBufferSize > 0 )
		nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_SNDBUF, (const char*)&m_nSocketSendBufferSize, sizeof( m_nSocketSendBufferSize ) );

	//���� ���� ���� ����
	if( m_nSocketRecvBufferSize > 0 )
		nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_RCVBUF, (const char*)&m_nSocketRecvBufferSize, sizeof( m_nSocketRecvBufferSize ) );

	//�������� �����͸� ������ ���� �ߴ�
	nSocOptError = setsockopt( m_nSock, SOL_SOCKET, SO_LINGER, (const char*)&m_lingerop, sizeof( m_lingerop ) );

	int option = 1; //option ����
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

void CTcpServer::InitServer( const char *pLocalIp, int nLocalPort )
{
	memset( &m_addrServerSock, 0, sizeof( m_addrServerSock ) );
	m_addrServerSock.sin_family = PF_INET;
	m_addrServerSock.sin_addr.s_addr = inet_addr( pLocalIp ); //INADDR_ANY;
	m_addrServerSock.sin_port = htons( nLocalPort );
}

//int CTcpServer::Reconnect( void* pRecvParam )
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
//	return Connect( strServerIp, m_nServerPort, m_fpExternRecv, pRecvParam );
//}

//Bind, Listen, Accept 
int CTcpServer::Ready( const char * pLocalIp, int nLocalPort, RECV_CALL fpExternRecv, void* pParam )
{
	int nRet = 0;
	if( isOpen( ) )
		return nRet;

	m_pRecvParam = NULL;
	size_t nIpLen = strlen( pLocalIp );

	m_fpExternRecv = fpExternRecv;
	m_nServerPort = nLocalPort;

	memset( m_strServerIp, 0, sizeof( m_strServerIp ) );
	memcpy( m_strServerIp, pLocalIp, nIpLen );

	// 1. ���� ����.
	fprintf( stderr, "2-1\n" );
	nRet = CreateSocket( );
	if( nRet < 0 )
	{
		m_eServerStatus = E_SERVER_CREATE_ERROR;
		fprintf( stderr, "2-2\n" );
		return nRet;
	}
	fprintf( stderr, "CreateSocket( ) \n" );

	// 2. ���� ���� ����.
	InitServer( m_strServerIp, m_nServerPort );

	// 3. Bind()
	//	memset( &m_addrLocal, 0, sizeof( m_addrLocal ) );
	//	m_addrLocal.sin_family = PF_INET;
	//	m_addrLocal.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	//	m_addrLocal.sin_port = htons( 10000 );

	nRet = bind( m_nSock, (sockaddr*)&m_addrServerSock, sizeof( m_addrServerSock ) );
	if( nRet < 0 )
	{
		perror( "Socket Bind Error " );
		return nRet;
	}
	fprintf( stderr, "Bind( ) : %d \n", nRet );

	// 4. Listen
	nRet = listen( m_nSock, DEFAUL_CLIENT_NUM );
	if( nRet < 0 )
	{
		perror( "Socket Listen Error " );
		return nRet;
	}
	fprintf( stderr, "Listen( ) : %d \n", nRet );
	int nLenOfAddrClienSock = sizeof( m_addrClientSock );

	// 5. Accept
	while( m_nCurClientNum < DEFAUL_CLIENT_NUM )
	{
		fprintf( stderr, "Pre_Accept\n" );
		m_nClientSock[m_nCurClientNum] = accept( m_nSock, (sockaddr *)&m_addrClientSock, &nLenOfAddrClienSock );
		if( m_nClientSock[m_nCurClientNum] == -1 )
		{
			fprintf( stderr, "Accept Error( ) : %d \n", m_nClientSock[m_nCurClientNum] );
			perror( "Socket Accept Error " );
		}
		m_nCurClientNum++;
		fprintf( stderr, "Accept( ) Clinet : %d, Sock : %d \n", m_nCurClientNum, m_nClientSock[m_nCurClientNum - 1] );
		m_pRecvParam = pParam;
		CreateThread( );
	}

	//	m_eServerStatus = E_SERVER_CONNECTION_SUCCESS;
	//	fprintf( stderr, "return : %d \n", nRet );
	return nRet;
}

//�ӽ� Send msg
int CTcpServer::SendData( char* pBuffer, int nBufferLength, int nDestClientSock )
{
	if( isClose( ) )
	{
		fprintf( stderr, "Send_Socket Closed\n" );
		m_eServerStatus = E_SERVER_SEND_FAIL;
		return -1;
	}
	//���� ���� ����� �Ѱ��� ���, ��� ������ ��������
	if( nBufferLength > m_nSocketSendBufferSize )
		return send( nDestClientSock, pBuffer, m_nSocketSendBufferSize, 0 );
	//		return send( m_nSock, pBuffer, m_nSocketSendBufferSize, MSG_NOSIGNAL ); //����� ������ ������쿡�� SIGPIPE �ñ׳��� �߻���Ű�� ����
	else
		return send( nDestClientSock, pBuffer, nBufferLength, 0 );

	return 0;
}

void CTcpServer::CloseSocket( void )
{
	if( isClose( ) )
	{
		return;
	}

	shutdown( m_nSock, SHUT_RDWR );

	//��� �ϴ� �ӽþ�	
	for( int i = 0 ; i < m_nCurClientNum ; m_nCurClientNum++ )
	{
		shutdown( m_nClientSock[i], SHUT_RDWR );
	}
	//	close( m_nSock );
	m_nSock = 0;
	setClose( );

	if( m_fpDisconnect != NULL )
		m_fpDisconnect( this );
}

void* CTcpServer::DoRecvCall( void *lParam )
{
	fprintf( stderr, "Thread Call\n" );

	CTcpServer* pThis = (CTcpServer*)( ( (SServerRecvParam*)lParam )->pParam );
	int nClient = ( (SServerRecvParam*)lParam )->nClientSock;

	while( 1 )
	{
		if( pThis == NULL )
		{
			fprintf( stderr, "DoRecvCall Error - pThis is NULL" );
		}
		else
		{
			//			pThis->DoRecv( );
			pThis->DoRecv( nClient );
		}
		sleep( 1 ); //110610
	}

	return 0;
}

void CTcpServer::DoRecv( int nClientSock )
{
	int nRecvSize = 0;
	while( isOpen( ) )
	{
		if( m_pRecvBuffer != NULL )
		{
			// m_pRecvBuffer->Reset();
			// recv()�� ȣ������ �� 0�� ���ϵǰų�, send�� ȣ������ �� -1�� ���� �����ϸ鼭 errno�� EPIPE�̸� �����(peer)������ ����� ��
			fprintf( stderr, "pre - recv\n " );
			nRecvSize = recv( nClientSock, m_pRecvBuffer->GetWritePositon( ), m_pRecvBuffer->RemainBuffer( ), 0 );
			fprintf( stderr, "recv sock : %d - Size : %d\n ", nClientSock, nRecvSize );

			if( m_pRecvBuffer == NULL )
				continue;

			if( nRecvSize > 0 )
			{
				m_pRecvBuffer->MoveWritePosition( nRecvSize );
			}
			else if( nRecvSize == 0 ) //X��ư, �������� 
			{
				m_pRecvBuffer->Reset( );
				perror( "CloseSocket()_0_perror!! " );
				CloseSocket( );
			}
			else //�����Ŀ� ���� //Process ����
			{
				m_pRecvBuffer->Reset( );
				perror( "CloseSocket()_1_perror!! " );
				CloseSocket( );
			}

			if( m_pRecvBuffer->GetBufferingSize( ) > 0 )
			{
				m_clientTemp.nClientSock = nClientSock;
				m_clientTemp.pParam = (void*)m_pRecvParam;

				m_fpExternRecv( *m_pRecvBuffer, &m_clientTemp );
			}
		}
	}
}

void CTcpServer::BindConnectionEvent( CONNECT_CALL fpConnection )
{
	m_fpConnect = fpConnection;
}

void CTcpServer::BindDisconnectionEvent( CONNECT_CALL fpDisconnection )
{
	m_fpDisconnect = fpDisconnection;
}

void CTcpServer::CreateThread( )
{
	if( m_bIsCreated[m_nCurClientNum - 1] )
		return;

	int nRet = 0;

	//Thread �켱������ ������ �ȵ�
	pthread_attr_t attr;
	sched_param param;
	pthread_attr_init( &attr );
	nRet = pthread_attr_getschedparam( &attr, &param );
	attr.threadAttrSchedpolicy = PTHREAD_INHERIT_SCHED;
	param.sched_priority = 111;
	nRet = pthread_attr_setschedparam( &attr, &param );

	//Clinet Param
	m_clientParam.pParam = this;
	m_clientParam.nClientSock = m_nClientSock[m_nCurClientNum - 1];

	if( ( nRet = pthread_create( &m_recvThread[m_nCurClientNum - 1], &attr, m_fpDoRecvCall, &m_clientParam ) ) != 0 )
	{
		fprintf( stderr, "RecvThread Create Fail ret : %d, err : %d \n", nRet, errno );
	}

	fprintf( stderr, "RecvThread Create \n " );
	m_bIsCreated[m_nCurClientNum - 1] = true;
}

void CTcpServer::Destroy( )
{
	CloseSocket( );
	for( int i = 0 ; i < m_nCurClientNum ; i++ )
	{
		if( m_bIsCreated[i] )
		{
			pthread_cancel( m_recvThread[i] );
			m_bIsCreated[i] = false;
		}
	}

}

void CTcpServer::SetConnectionParam( void* pParam )
{
	m_pConnectionParam = pParam;
}
