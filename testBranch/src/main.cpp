/**
 * @file	main.h
 * @author	유기훈
 * @version	
 * @brief	프로그램의 시작을 Entry Point
 *
 * @date	2013.05.01
 */

#include <stdio.h>
#include <unistd.h>
#include "Common/MsgQueuePool/CMsgQueuePool.h"
//#include "Task/CELInternInterface.h"
#include "Task/CELExternInterface.h"
#include "Common/common.h"
#include "Task/CELSystemCtrl.h"
#include "Task/CELAcqMngr.h"

#include <pthread.h>
#define _DEBUG_

//void testThread( void * args )
//{
//	while( 1 )
//	{
//		sleep( 1 );
//		printf( "testThread\n" );
//	}
//}

void start( )
{
	printf( "----- Program Start -----\n" );
	if( GP_MESSAGE_QUEUE_POOL->GetInstance( )->initQueue( ) )
	{
#ifdef _DEBUG_
		fprintf( stderr, "----- Task Queue Init Success -----\n" );
#endif
	}
	else
	{
#ifdef _DEBUG_
		fprintf( stderr, "----- task Queue Init Fail -----\n" );
#endif
	}

	//SysCtrl Start
	if ( TASK_ID_ERROR == taskSpawn( "SysCtrl", 5, 0, 1024, (FUNCPTR)CELSystemCtrl::startBehavior, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) )
	{
		fprintf( stderr, "----- taskSpawn Fail -----\n" );
	}
	//Ac Start
	if ( TASK_ID_ERROR == taskSpawn( "SysCtrl", 5, 0, 1024, (FUNCPTR)CELSystemCtrl::startBehavior, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ) )
	{
		fprintf( stderr, "----- taskSpawn Fail -----\n" );
	}
	
	
	//내부 이더넷 인터페이스 시작
	if( GP_EL_EXTERNINTERFACE->ServerBegin( "192.168.200.1", 10001 ) ) 
	{
#ifdef _DEBUG_
		fprintf( stderr, "----- Interface Begin Success -----\n" );
#endif
	}
	else
	{
#ifdef _DEBUG_
		fprintf( stderr, "----- Interface Begin Fail -----\n" );
#endif
	}
	
	//외부 이더넷 인터페이스 시작
//	if( GP_EL_INTERNINTERFACE->ServerBegin( "192.168.200.1", 10002 ) ) //외부 
//	{
//#ifdef _DEBUG_
//		fprintf( stderr, "----- Interface Begin Success -----\n" );
//#endif
//	}
//	else
//	{
//#ifdef _DEBUG_
//		fprintf( stderr, "----- Interface Begin Fail -----\n" );
//#endif
//	}

	while( 1 )
		sleep( 1 );

}
