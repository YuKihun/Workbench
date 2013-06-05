//#include <cmdParser.h>
#include "CMsgQueuePool.h"

CMsgQueuePool* CMsgQueuePool::GetInstance( )
{
	static CMsgQueuePool instance;
	return &instance;
}

CMsgQueuePool::CMsgQueuePool()
{
//	m_sizeOfpoolQueues = QUEUE_POOL_SIZE;
	for( unsigned int i = 0 ; i < QUEUE_POOL_SIZE ; i++ )
		m_poolQueues[i] = NULL;
}

CMsgQueuePool::~CMsgQueuePool()
{
	destroyQueues( );
}

MSG_Q_ID CMsgQueuePool::getQueue( EQuieuID eQID )
{
	MSG_Q_ID getOneQueue = NULL;

	if( QUEUE_POOL_SIZE >= eQID )
	{
		getOneQueue = m_poolQueues[ eQID ];
	}

	return getOneQueue;
}

BOOL CMsgQueuePool::initQueue( )//unsigned int nQueueSize )
{
//	m_sizeOfpoolQueues = nQueueSize;
	for ( int i = 0 ; i < QUEUE_POOL_SIZE ; i ++ )
	{
		m_poolQueues[ i ] = msgQCreate( QUEUE_MSG_NUM, QUEUE_MSG_SIZE, MSG_Q_FIFO | MSG_Q_PRIORITY );
		if( NULL == m_poolQueues[i] )
			return FALSE;
	}
	return TRUE;
}

void CMsgQueuePool::destroyQueues( )
{
	for( unsigned int i = 0 ; i < QUEUE_POOL_SIZE ; i++ )
	{
		if( NULL != m_poolQueues[i] )
		{
			delete m_poolQueues[i];
			m_poolQueues[i] = NULL;
		}
	}
}
