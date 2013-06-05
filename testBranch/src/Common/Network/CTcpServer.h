#ifndef CTCPSERVER_H_
#define CTCPSERVER_H_

#include "CNetwork.h"
#include "pthread.h"

const unsigned int DEFAUL_SERVER_RECV_BUFFER = 8192; //1048576       // 1024 * 1024 = 1 MegaByte
const unsigned int DEFAUL_CLIENT_NUM = 10;

enum E_SERVER_CONNECTION_INFO
{
	E_SERVER_NOACTION, E_SERVER_CONNECTION_SUCCESS, E_SERVER_CREATE_ERROR, E_SERVER_SEND_SUCCESS, E_SERVER_SEND_FAIL,
};

class CTcpServer;

struct SServerRecvParam
{
		void * pParam;
		int nClientSock;
};

class CTcpServer : public CNetwork
{
	public:
		CTcpServer( );
		virtual ~CTcpServer( );

	private:
		CONNECT_CALL m_fpDisconnect;
		CONNECT_CALL m_fpConnect;
		DO_RECV m_fpDoRecvCall;
		RECV_CALL m_fpExternRecv;
		sockaddr_in m_addrClientSock;
		int m_nClientSock[ DEFAUL_CLIENT_NUM ];
		unsigned int m_nCurClientNum;

		SServerRecvParam m_clientParam; //Thread 생성 시, 파라미터 활용
		SServerRecvParam m_clientTemp; //함수포인터 RecvMain에서 활용
		
	protected:
		char m_strServerIp[16];
		int m_nServerPort;

		sockaddr_in m_addrServerSock;
		pthread_t m_recvThread[ DEFAUL_CLIENT_NUM ]; //수신 스레드
		bool m_bIsOpen;
//		bool m_bIsCreated;
		bool m_bIsCreated[ DEFAUL_CLIENT_NUM ];
		void* m_pConnectionParam;
		E_SERVER_CONNECTION_INFO m_eServerStatus;

	private:
		void CreateThread( );

	protected:
		int CreateSocket( );
		void InitServer( const char * pServerIp, int nLocalPort );
		void DoRecv( int nClientSock );
//		static void* DoRecvCall( void *lParam );
		void Destroy( );
		inline void setClose( )
		{
			m_bIsOpen = false;
		}
		inline void setOpen( )
		{
			m_bIsOpen = true;
		}
		void SetConnectionParam( void* pParam );

	public:
		static void* DoRecvCall( void *lParam );
		inline bool isOpen( )
		{
			return m_bIsOpen;
		}
		inline bool isClose( )
		{
			return !m_bIsOpen;
		}
		int Ready( const char * pLocalIp, int nLocalPort, RECV_CALL fpExternRecv, void* pRecvParam = NULL );
		//		int Reconnect( void* pRecvParam = NULL );
		//Client 식별해서 Send 해야되는거야, 나중에 데이터로 구분하겠지.
		int SendData( char* pBuffer, int nBufferLength, int nDestClientSock );
		void CloseSocket( );
		void BindConnectionEvent( CONNECT_CALL fpDisconnection );
		void BindDisconnectionEvent( CONNECT_CALL fpConnection );
		inline int GetSocket( )
		{
			return m_nSock;
		}
		inline unsigned int GetCurClientNum( )
		{ 
			return m_nCurClientNum; 
		}
};

#endif /* CTCPSERVER_H_ */
