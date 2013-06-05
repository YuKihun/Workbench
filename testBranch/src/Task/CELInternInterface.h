/**
 * @file	CELInternInterface.h
 * @author	유기훈
 * @version	
 * @brief	Ethernet Interface 역할을 담당하는 클래스, Client와의 메시지 송/수신을 담당
 *
 * @date	2013.05.01
 */

#ifndef ELINTERN_INTERFACE_H_
#define ELINTERN_INTERFACE_H_
//#include "Common/Network/CTcpClient.h"
#include "../Common/Network/CTcpServer.h"

//class CELInternInterface : public CTcpClient
class CELInternInterface : public CTcpServer
{
	private:
		CELInternInterface( );
		~CELInternInterface( );
	    static void RecvMain( CBuffer& buffer, void * pRecvParam );

	public:
		static CELInternInterface* GetInstance( );
		void startBehavior( ); 
		
		//소켓 정보 //Clinet
//	    bool Begin( const char * pLocalIp, int nLocalPort , const char * pServerIp, int nServerPort ); //, RECV_PROC fpRecvProc );
		bool ServerBegin( const char * pLocalIp, int nLocalPort );
	    
	    void SetSocketSendBufferSize( int dSize );
	    void SetSocketRecvBufferSize( int dSize );
		
	private:
		
};

#define GP_EL_INTERNINTERFACE CELInternInterface::GetInstance()

#endif 
