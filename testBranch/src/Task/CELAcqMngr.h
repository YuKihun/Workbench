/**
 * @file	CELAcqMngr.h
 * @author	유기훈
 * @version	
 * @brief	탐지와 고감도의 수신제어를 관리
 *
 * @date	2013.05.01
 */

#ifndef ELACQMNGR_H_
#define ELACQMNGR_H_

class CELAcqMngr
{
	private:
		CELAcqMngr( );
		~CELAcqMngr( );

	public:
		static CELAcqMngr* GetInstance( );
	
	private:
//		void startAcqCtrl( ); //수집제어 시작
//		void startHGAcqCtrl( );
//		void startAcquisition( );
//		void cleanUpRelations( );
//		void destroy( );
	public:
		void startBehavior( ); //태스크 시작
};
#define GP_EL_ACQMNGR CELAcqMngr::GetInstance()

#endif 
