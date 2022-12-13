#pragma once

#include "NxPhysics.h"
#include "NxScene.h"

/*-----------------------------------------------------------------------

���˼·��
	
	�Ȳ�����ʹ�ü��ٵ��ߵ������

	����ǰ�ķ����ʱ�� 
		������  ��һ�����ߣ� ���Ա�֤�ٶ�������һ�� S ��״ ���ϵ��ߡ�
	ͨ�׵�˵���ǣ��ոտ�ʼ�����ٶ����ӽϿ죬���ٶ�ֵ�ﵽһ���̶�ʱ�����ٿ�ʼ���䣬��
	�ٶȴﵽ��ֵʱ���ٶ�Ϊ 0  ......

	������ķ����ʱ�� 
		�ٶ�Խ�����Ч��Ӧ�� ԽΪ����.
	
	���ٶ� ���ٶ�֮��Ĺ�ϵ����Ӧ�� ����ͼ��ʾ��

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
	  						|��m_fVTopAccel_NormalAccel                  
	 * 	*	*	*	  *  	*	   *                 *  *
	  			     m_fAcel_OriBack  *              *    *
	  		     			|            *           *      *	m_fAcel_NonePress
	  		  				|               *        *        *
	  	  					|                  *	 *				 
			m_fRadio_DeAccel *   *
	   			     								*  m_fDeAcelMax = m_fRadio_DeAccel * m_fVMax_NormalAccel + m_fAcel_OriBack

													m_fAcel_AcelToNormal


	   ���ʹ�ü��ٵ��� �� 

	   ���ܵı����Ǽ���ͻ�����ٶȷ�ֵ���� �� ��ǰ����� �ɿ�������Ч��������ʧ

	   �������Ƶ������������  

	  1�� ��ֵ�ٶ�  ��ʹ�õ��ߣ���ʹ�õ��ߣ� 
	  
	  NxReal m_fVMax_NormalAccel
	  NxReal m_fVMax_UseItemAccel

	  NxReal m_fVMin; ��С�ٶ�


	  2. ���ٶȲ���
	  m_fAcel_OriForward   ԭ��ǰ����ٶ�
	  m_fAcel_OriBack	   ԭ�������ٶ�
		
	  m_fAcelMax_NormalAccel	����������������ٶ�
	  m_fAcelMax_UseItemAccel	ʹ�ü��ٵ���������������ٶ�

	  3 ���ٱ���
	  m_fRadioUp_NormalAccel  = 2

	  m_fRadioDown_NormalAccel = 1

	  m_fRadioUp_UseItemAccel = 5

	  4 ���ٱ��� ( �ɿ���ǰ�ķ������ �����ķ����)  �˴�������ͬ������
	  m_fRadio_DeAccel = 1




	  5. �ٶ�Ϊ��ʱ���������
	  m_fAcel_Vfu_NormalAccel
	  m_fAcel_Vfu_UseItemAccel




   1. ���ٱ���  �� ʹ�õ��ߣ� �� ��ʹ�õ��ߣ� 




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
		�޸��ˣ�	ray
		���ڣ�	1/31/2007
		����	�ṩ���ٿ���
	*/
	void	StartIntensiveAcce();		//��ʼǿ�Ҽ��٣����߼��٣�
	void	StopIntensiveAcce();		//ֹͣǿ�Ҽ��٣����߼��٣�

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

	NxReal m_fAcel_OriForward  ;			// ԭ��ǰ����ٶ�
	NxReal m_fAcel_OriBack	  ;			//ԭ�������ٶ�

	NxReal m_fAcelMax_NormalAccel;		//����������������ٶ�
	NxReal m_fAcelMax_UseItemAccel;		//ʹ�ü��ٵ���������������ٶ�

	NxReal m_fRadioUp_NormalAccel; 
	NxReal m_fRadioDown_NormalAccel; 
	NxReal m_fRadioUp_UseItemAccel ;
	NxReal m_fRadioDown_UseItemAccel;

	NxReal m_fRadio_DeAccel ;
	NxReal m_fDeAcelMax; 

	NxReal m_fAcel_Vfu_NormalAccel;
	NxReal m_fAcel_Vfu_UseItemAccel;

	NxReal m_fVMin;  // ��С�ٶ�

	NxReal m_fAcel_NonePress;		 //ɶҲû���ļ��ٶ�
	NxReal m_fAcel_Vfu_NonePress;

	NxReal m_fAcel_AcelToNormal;

	/*
		�޸��ˣ�	ray
		���ڣ�	1/31/2007
		����	��ʼ���ٽӿ�
	*/
	eKeyStatus	m_eAcceState;			//����״̬��ǿ�Ҽ��ٻ���ͨ���٣�

};
