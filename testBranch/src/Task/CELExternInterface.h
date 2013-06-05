/**
 * @file	CELExternInterface.h
 * @author	유기훈
 * @version	
 * @brief	외부 Ethernet Interface 역할을 담당하는 클래스, Client와의 메시지 송/수신을 담당
 *
 * @date	2013.05.27
 */

#ifndef ELEXTERN_INTERFACE_H_
#define ELEXTERN_INTERFACE_H_

#include "../Common/Network/CTcpServer.h"

class CELExternInterface : public CTcpServer
{
	private:
		CELExternInterface( );
		~CELExternInterface( );
	    static void RecvMain( CBuffer& buffer, void * pRecvParam );

	public:
		static CELExternInterface* GetInstance( );
		void startBehavior( ); 
		
		//소켓 정보
		bool ServerBegin( const char * pLocalIp, int nLocalPort );
	    
	    void SetSocketSendBufferSize( int dSize );
	    void SetSocketRecvBufferSize( int dSize );
		
	private:
		
};

#define GP_EL_EXTERNINTERFACE CELExternInterface::GetInstance()

#endif 
