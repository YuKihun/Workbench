/**
 * @file	CELAcqMngr.h
 * @author	������
 * @version	
 * @brief	Ž���� ������ ������� ����
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
//		void startAcqCtrl( ); //�������� ����
//		void startHGAcqCtrl( );
//		void startAcquisition( );
//		void cleanUpRelations( );
//		void destroy( );
	public:
		void startBehavior( ); //�½�ũ ����
};
#define GP_EL_ACQMNGR CELAcqMngr::GetInstance()

#endif 
