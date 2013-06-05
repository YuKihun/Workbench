#ifndef CTCPCLIENT_H_
#define CTCPCLIENT_H_

#include "CNetwork.h"
#include "pthread.h"

const unsigned int DEFAUL_CLIENT_RECV_BUFFER = 8192; //1048576       // 1024 * 1024 = 1 MegaByte

enum E_CLIENT_CONNECTION_INFO
{
	E_CLIENT_NOACTION, E_CLIENT_CONNECTION_SUCCESS, E_CLIENT_CREATE_ERROR, E_CLIENT_CONNECTION_ERROR, E_CLIENT_SEND_SUCCESS, E_CLIENT_SEND_FAIL,
};

class CTcpClient : public CNetwork
{
	public:
		CTcpClient( );
		virtual ~CTcpClient( );

	private:
		CONNECT_CALL m_fpDisconnect;
		CONNECT_CALL m_fpConnect;
		DO_RECV m_fpDoRecvCall;
		RECV_CALL m_fpExternRecv;

	protected:
		char m_strServerIp[16];
		int m_nPort;

		sockaddr_in m_addrServerSock;
		sockaddr_in m_addrLocal;
		pthread_t m_recvThread; //수신 스레드
		bool m_bIsOpen;
		bool m_bIsCreated;
		void* m_pConnectionParam;
		E_CLIENT_CONNECTION_INFO m_eClientStatus;

	private:
		void CreateThread( );

	protected:
		int CreateSocket( );
		void InitServer( const char * pServerIp, int nPort );
		void DoRecv( void );
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

		int Connect( const char * pLocalIp, int nLocalPort, const char * pServerIp, int nServerPort, RECV_CALL fpExternRecv, void* pRecvParam = NULL );
		//		int Reconnect( void* pRecvParam = NULL );
		int SendData( char* pBuffer, int nBufferLength );
		void CloseSocket( );
		void BindConnectionEvent( CONNECT_CALL fpDisconnection );
		void BindDisconnectionEvent( CONNECT_CALL fpConnection );
		int GetSocket( )
		{
			return m_nSock;
		}
};

#endif /* CTCPCLIENT_H_ */
