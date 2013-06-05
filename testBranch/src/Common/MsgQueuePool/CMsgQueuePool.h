#ifndef _MSGQUEUEPOOL_H
#define _MSGQUEUEPOOL_H
#include <msgQLib.h>

typedef enum //0_base
{
	TASK_SYSCON = 0, 
	TASK_EXIF = 1, 
	TASK_INIF = 2, 
	TASK_SYSMAN = 3, 
	TASK_DATAMAN = 4, 
	TASK_ACQMAN = 5, 
	TASK_SIGMAN = 6, 
	TASK_SIGPROCMAN = 7
} EQuieuID;

const unsigned int QUEUE_POOL_SIZE = 8;
const unsigned int QUEUE_MSG_NUM = 10;
const unsigned int QUEUE_MSG_SIZE = 4;

class CMsgQueuePool
{
	private:
		CMsgQueuePool( );
		void destroyQueues( );

	public:
		static CMsgQueuePool* GetInstance( );
		MSG_Q_ID  getQueue( EQuieuID eQID );
		BOOL initQueue( );//unsigned int nQueueSize );
		~CMsgQueuePool( );

	private:
		MSG_Q_ID  m_poolQueues[QUEUE_POOL_SIZE];
//		unsigned int m_sizeOfpoolQueues;
};

#define GP_MESSAGE_QUEUE_POOL CMsgQueuePool::GetInstance()
#endif

