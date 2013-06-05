#ifndef CUDP_H_
#define CUDP_H_

#include "CNetwork.h"

//#define DEFAUL_UDP_RECV_BUFFER	524288 //1048576      // 1024 * 1024 = 1 MegaByte
class CUdp : public CNetwork
{
//	public:
//
//		CUdp( E_HTNetworkType eNetworkkType );
//		virtual ~CUdp( );
//
//	private:
//		DO_RECV m_fpDoRecvCall;
//		DO_RECV m_fpDoProcCall;
//		RECV_CALL m_fpExternRecv;
//
//	private:
//		char m_strLocalIp[16];
//		char m_strMulticastIp[16];
//		int m_nLocalPort;
//
//		sockaddr_in m_addrLocal;
//		bool m_bIsOpen;
//		int m_nTtl;
//		pthread_t m_nRecvThreadId;
//		pthread_t m_nProcThreadId;
//
//		static pthread_mutex_t m_mutexMsgProc; ///< Mutex°´Ã¼
//		static unsigned int nTest;
//
//	private:
//		void CreateRecvThread( );
//		void CreateProcThread( );
//		void DoRecv( );
//		void DoProc( );
//		static void* DoRecvCall( void *lParam );
//		static void* DoProcCall( void *lParam );
//
//		inline void setOpen( )
//		{
//			m_bIsOpen = true;
//		}
//		inline void setClose( )
//		{
//			m_bIsOpen = false;
//		}
//
//		int SetUdpOption( );
//		int setMulticastInterface( );
//		int setMulticastJoin( );
//		int setBroadcastSend( );
//
//	public:
//		void SetTTL( int nTtl );
//		inline bool isOpen( )
//		{
//			return m_bIsOpen;
//		}
//		inline bool isClose( )
//		{
//			return !m_bIsOpen;
//		}
//
//		virtual int Set( int nLocalPort, RECV_CALL fpExternRecvCall, void* pRecvParam = NULL );
//		virtual int Set( const char* pLocalIp, int nLocalPort, RECV_CALL fpExternRecvCall, void* pRecvParam = NULL );
//		virtual int Set( const char* pLocalIp, int nMulticastPort, const char* pMulticastIp, RECV_CALL fpExternRecvCall, void* pRecvParam = NULL );
//		virtual int Begin( );
//		virtual void End( );
//		virtual int SendData( char* pBuffer, int nBufferLength, const char* pTargetIp, int nTargetPort );
};

#endif /* CUDP_H_ */
