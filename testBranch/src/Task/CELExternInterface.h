/**
 * @file	CELExternInterface.h
 * @author	������
 * @version	
 * @brief	�ܺ� Ethernet Interface ������ ����ϴ� Ŭ����, Client���� �޽��� ��/������ ���
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
		
		//���� ����
		bool ServerBegin( const char * pLocalIp, int nLocalPort );
	    
	    void SetSocketSendBufferSize( int dSize );
	    void SetSocketRecvBufferSize( int dSize );
		
	private:
		
};

#define GP_EL_EXTERNINTERFACE CELExternInterface::GetInstance()

#endif 
