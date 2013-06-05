#include <stdio.h>
#include <unistd.h>
#include "CELAcqMngr.h"
#include "../Common/MsgQueuePool/CMsgQueuePool.h"

//CELAcqMngr* CELAcqMngr::GetInstance( )
//{
//	static CELAcqMngr Instance;
//	return &Instance;
//}


CELAcqMngr::CELAcqMngr( )
{
	
}

CELAcqMngr::~CELAcqMngr( )
{
	
}

void CELAcqMngr::startBehavior( )
{	
	fprintf( stderr, "CELAcqMngr::startBehavior( )\n" );
	int nRecvQData = 0;

	//Task ����
	while( 1 )
	{
		msgQReceive( (GP_MESSAGE_QUEUE_POOL->getQueue( TASK_ACQMAN )), (char*)&nRecvQData, sizeof(nRecvQData), WAIT_FOREVER );
		switch( nRecvQData )
		{
			case 0x3412 :
			{
//				msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_ACQMAN ), (char*)&nRecvQData, sizeof( nRecvQData ), NO_WAIT, MSG_PRI_NORMAL );
				fprintf( stderr, "AcqMngr : Receive Receiving Control\n" );
				//���� Ȯ�� 
				//�߰�/���� ������ ��� Pre_CAL
				//���� ���� 
				fprintf( stderr, "AcqMngr : Processing Receiving Control\n" );
				//
				break;
			}
			case 0x1234 :
			{
//				msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_ACQMAN ), (char*)&nRecvQData, sizeof( nRecvQData ), NO_WAIT, MSG_PRI_NORMAL );
				fprintf( stderr, "AcqMngr : Receive HighGain Antena Control\n" );
				fprintf( stderr, "AcqMngr : Processing HighGain Antena Control\n" );
				break;
			}
			default :
				break;
		}
		
		sleep( 1 );
	}
}
