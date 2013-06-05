/**
 * @file	CELInternInterface.h
 * @author	������
 * @version	
 * @brief	Ethernet Interface ������ ����ϴ� Ŭ����, Client���� �޽��� ��/������ ���
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
		
		//���� ���� //Clinet
//	    bool Begin( const char * pLocalIp, int nLocalPort , const char * pServerIp, int nServerPort ); //, RECV_PROC fpRecvProc );
		bool ServerBegin( const char * pLocalIp, int nLocalPort );
	    
	    void SetSocketSendBufferSize( int dSize );
	    void SetSocketRecvBufferSize( int dSize );
		
	private:
		
};

#define GP_EL_INTERNINTERFACE CELInternInterface::GetInstance()

#endif 
