/**
 * @file	CELSystemCtrl.h
 * @author	유기훈
 * @version	
 * @brief	SW의 제어 및 상태를 관리하는 클래스
 *
 * @date	2013.05.01
 */

#ifndef ELSYSTEMCTRL_H_
#define ELSYSTEMCTRL_H_

class CELSystemCtrl
{
	private:
		CELSystemCtrl( );
		~CELSystemCtrl( );

	public:
//		static CELSystemCtrl* GetInstance( );
		static void startBehavior( ); //태스크 시작
	private:
	 
		
};

//#define GP_EL_SYSCTRL CELSystemCtrl::GetInstance()

#endif 
