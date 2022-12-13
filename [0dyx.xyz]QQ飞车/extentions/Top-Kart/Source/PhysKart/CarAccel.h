#pragma once

#include "NxPhysics.h"
#include "NxScene.h"

/*-----------------------------------------------------------------------

设计思路：
	
	先不考虑使用加速道具的情况：

	按向前的方向键时： 
		加速线  是一条折线， 可以保证速度曲线是一条 S 形状 向上的线。
	通俗点说就是：刚刚开始　加速度增加较快，当速度值达到一定程度时　加速开始回落，当
	速度达到峰值时加速度为 0  ......

	按后项的方向键时： 
		速度越快减速效果应该 越为明显.
	
	加速度 和速度之间的关系曲线应该 如下图所示：

							A
							|
							| m_fRadioUp_NormalAccel
							|    |
	m_fAcel_Vfu_NormalAccel	|    | * 
	 *		*	  *  	  *	|     *   *  
			    	    	|    *       *
	 *	    	  		 m_fAcel_OriForward * --- m_fRadioDown_NormalAccel       
m_fAcel_Vfu_NonePress	 	*  *               *
	 *   				  	|                    *   m_fVMax_NormalAccel
____ *______________________|_______________________*___________________ V
	  						|　m_fVTopAccel_NormalAccel                  
	 * 	*	*	*	  *  	*	   *                 *  *
	  			     m_fAcel_OriBack  *              *    *
	  		     			|            *           *      *	m_fAcel_NonePress
	  		  				|               *        *        *
	  	  					|                  *	 *				 
			m_fRadio_DeAccel *   *
	   			     								*  m_fDeAcelMax = m_fRadio_DeAccel * m_fVMax_NormalAccel + m_fAcel_OriBack

													m_fAcel_AcelToNormal


	   如果使用加速道具 ： 

	   跑跑的表现是加速突增，速度峰值增加 、 但前向方向键 松开，加速效果立刻消失

	   由曲线推导出的所需参数  

	  1。 峰值速度  （使用道具，不使用道具） 
	  
	  NxReal m_fVMax_NormalAccel
	  NxReal m_fVMax_UseItemAccel

	  NxReal m_fVMin; 最小速度


	  2. 加速度参数
	  m_fAcel_OriForward   原点前向加速度
	  m_fAcel_OriBack	   原点后向加速度
		
	  m_fAcelMax_NormalAccel	正常情况最大正向加速度
	  m_fAcelMax_UseItemAccel	使用加速道具情况最大正向加速度

	  3 加速比率
	  m_fRadioUp_NormalAccel  = 2

	  m_fRadioDown_NormalAccel = 1

	  m_fRadioUp_UseItemAccel = 5

	  4 减速比率 ( 松开向前的方向键， 按向后的方向键)  此处可以做同样处理
	  m_fRadio_DeAccel = 1




	  5. 速度为负时的正向加速
	  m_fAcel_Vfu_NormalAccel
	  m_fAcel_Vfu_UseItemAccel




   1. 加速比率  （ 使用道具， 和 不使用道具） 




Create by Hoverzhao 2006

-------------------------------------------------------------------------*/


class CCarAccelControl
{

public:
	CCarAccelControl();
	~CCarAccelControl();

	enum  eKeyStatus
	{
		Normal_Forward,
		Normal_Back,
		Accel_ForWard,
		Normal_None
	};


public:
	bool InitConfig();

public:
	NxReal PeekCarAccel(NxReal vSpeed,NxReal GraviContribute = 0.0f);
	NxReal GetMaxVel() { return m_fVMax_NormalAccel;}

	/*
		修改人：	ray
		日期：	1/31/2007
		描叙：	提供加速开关
	*/
	void	StartIntensiveAcce();		//开始强烈加速（道具加速）
	void	StopIntensiveAcce();		//停止强烈加速（道具加速）

protected:

	NxReal GetUseItemAccel(NxReal vSpeed);

	NxReal GetNormalAccel(NxReal vSpeed);

protected:
	bool IsUseItemAcceling() ;
	int  GetKeyStatue();

private:

	NxReal m_fVMax_NormalAccel;
	NxReal m_fVMax_UseItemAccel;

	NxReal m_fVTopAccel_NormalAccel;
	NxReal m_fVTopAccel_UseItemAccel;

	NxReal m_fAcel_OriForward  ;			// 原点前向加速度
	NxReal m_fAcel_OriBack	  ;			//原点后向加速度

	NxReal m_fAcelMax_NormalAccel;		//正常情况最大正向加速度
	NxReal m_fAcelMax_UseItemAccel;		//使用加速道具情况最大正向加速度

	NxReal m_fRadioUp_NormalAccel; 
	NxReal m_fRadioDown_NormalAccel; 
	NxReal m_fRadioUp_UseItemAccel ;
	NxReal m_fRadioDown_UseItemAccel;

	NxReal m_fRadio_DeAccel ;
	NxReal m_fDeAcelMax; 

	NxReal m_fAcel_Vfu_NormalAccel;
	NxReal m_fAcel_Vfu_UseItemAccel;

	NxReal m_fVMin;  // 最小速度

	NxReal m_fAcel_NonePress;		 //啥也没按的加速度
	NxReal m_fAcel_Vfu_NonePress;

	NxReal m_fAcel_AcelToNormal;

	/*
		修改人：	ray
		日期：	1/31/2007
		描叙：	开始加速接口
	*/
	eKeyStatus	m_eAcceState;			//加速状态（强烈加速或普通加速）

};
