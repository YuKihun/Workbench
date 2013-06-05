/**
 * @file	main.h
 * @author	������
 * @version	
 * @brief	���α׷��� ������ Entry Point
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
	
	
	//���� �̴��� �������̽� ����
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
	
	//�ܺ� �̴��� �������̽� ����
//	if( GP_EL_INTERNINTERFACE->ServerBegin( "192.168.200.1", 10002 ) ) //�ܺ� 
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
