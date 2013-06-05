#include <stdio.h>
#include "CELSystemCtrl.h"
#include "unistd.h"
#include "../Common/MsgQueuePool/CMsgQueuePool.h"
//CELSystemCtrl* CELSystemCtrl::GetInstance( )
//{
//	static CELSystemCtrl Instance;
//	return &Instance;
//}


CELSystemCtrl::CELSystemCtrl( )
{
	
}

CELSystemCtrl::~CELSystemCtrl( )
{
	
}

void CELSystemCtrl::startBehavior( )
{
	fprintf( stderr, "CELSystemCtrl::startBehavior( )\n" );
	int nRecvQData = 0;

	//Task ½ÃÀÛ
	while( 1 )
	{
		msgQReceive( (GP_MESSAGE_QUEUE_POOL->getQueue( TASK_SYSCON )), (char*)&nRecvQData, sizeof(nRecvQData), WAIT_FOREVER );
		switch( nRecvQData )
		{
			case 0x3412:
			{
				msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_ACQMAN ), (char*)&nRecvQData, sizeof( nRecvQData ), NO_WAIT, MSG_PRI_NORMAL );
				fprintf( stderr, "SysControl -> ACQMAN : Request Receiving Control - Q Send \n" );
				break;
			}
			case 0x01234 :
			{
				msgQSend( GP_MESSAGE_QUEUE_POOL->getQueue( TASK_ACQMAN ), (char*)&nRecvQData, sizeof( nRecvQData ), NO_WAIT, MSG_PRI_NORMAL );
				fprintf( stderr, "SysControl -> ACQMAN : Request HighGain Receiving Control - Q Send \n" );
				break;
			}
			default :
				break;
		}
		
		sleep( 1 );
	}
}
