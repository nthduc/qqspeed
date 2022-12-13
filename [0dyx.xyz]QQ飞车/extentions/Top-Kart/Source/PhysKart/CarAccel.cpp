#include "Top-KartPCH.h"
#include "CarAccel.h"
#include "stdlib.h"
#include "Math.h"


CCarAccelControl::CCarAccelControl()
{

	m_fVMax_NormalAccel					=	DescConstant::KarBaseParam[DescConstant::KarType].m_fVMax_NormalAccel		;	
	m_fVMax_UseItemAccel				=	DescConstant::KarBaseParam[DescConstant::KarType].m_fVMax_UseItemAccel	;	

	m_fVTopAccel_NormalAccel			=	DescConstant::KarBaseParam[DescConstant::KarType].m_fVTopAccel_NormalAccel	;
	m_fVTopAccel_UseItemAccel			=	DescConstant::KarBaseParam[DescConstant::KarType].m_fVTopAccel_UseItemAccel	;

	m_fAcel_OriForward					=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_OriForward		;	
	m_fAcel_OriBack						=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_OriBack			;	

	m_fAcelMax_NormalAccel				=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcelMax_NormalAccel	;	
	m_fAcelMax_UseItemAccel				=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcelMax_UseItemAccel	;	
	m_fDeAcelMax						=	DescConstant::KarBaseParam[DescConstant::KarType].m_fDeAcelMax			;	

	m_fAcel_Vfu_NormalAccel 			=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_Vfu_NormalAccel  ;   
	m_fAcel_Vfu_UseItemAccel			=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_Vfu_UseItemAccel;	

	m_fVMin								=	DescConstant::KarBaseParam[DescConstant::KarType].m_fVMin				;		

	m_fAcel_NonePress					=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_NonePress	;		
	m_fAcel_Vfu_NonePress				=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_Vfu_NonePress	;	

	m_fAcel_AcelToNormal				=	DescConstant::KarBaseParam[DescConstant::KarType].m_fAcel_AcelToNormal	;	

	/*
	m_fVMax_NormalAccel			= 20.0f;
	m_fVMax_UseItemAccel		= 25.0f;

	m_fVTopAccel_NormalAccel	= 3.0f ;
	m_fVTopAccel_UseItemAccel	= 3.0f ;

	m_fAcel_OriForward			= 1.0f ;	
	m_fAcel_OriBack				=-3.0f ;		

	m_fAcelMax_NormalAccel		= 6.0f;	
	m_fAcelMax_UseItemAccel		= 12.0f;	
	m_fDeAcelMax				= -16.0f; 

	m_fAcel_Vfu_NormalAccel     = 3.0f;
	m_fAcel_Vfu_UseItemAccel	= 6.0f;

	m_fVMin						= -20.0f;  // 最小速度

	m_fAcel_NonePress			= -0.5f;		 
	m_fAcel_Vfu_NonePress		= 0.5f;

	m_fAcel_AcelToNormal		= -5.0f;
	*/


	m_fRadioUp_NormalAccel		= ( m_fAcelMax_NormalAccel - m_fAcel_OriForward ) / m_fVTopAccel_NormalAccel;
	m_fRadioDown_NormalAccel   = - m_fAcelMax_NormalAccel / ( m_fVMax_NormalAccel -  m_fVTopAccel_NormalAccel);


	m_fRadioUp_UseItemAccel		=  ( m_fAcelMax_UseItemAccel - m_fAcel_OriForward ) / m_fVTopAccel_UseItemAccel;
	m_fRadioDown_UseItemAccel	=  - m_fAcelMax_UseItemAccel / ( m_fVMax_UseItemAccel - m_fVTopAccel_UseItemAccel);


	m_fRadio_DeAccel			= ( m_fDeAcelMax - m_fAcel_OriBack ) / m_fVMax_NormalAccel;


	m_eAcceState				= Normal_Forward;



}

CCarAccelControl::~CCarAccelControl()
{
}

/*
												
							A
							|
							| m_fRadioUp_NormalAccel
							|    |
	m_fAcel_Vfu_NormalAccel	|    | * 
	 *		*	  *  	  *	|     *   *  
			    	    	|    *       *
	 *	    	  		 m_fAcel_OriForward * --- m_fRadioDown_NormalAccel       
m_fAcel_Vfu_NonePress	 	*                 *
	 *   				  	|                    *   m_fVMax_NormalAccel
____ *______________________|_______________________*___________________ V
	  						|　m_fVTopAccel_NormalAccel                  
	 * 	*	*	*	  *  	*	   *                 *  
	  			     m_fAcel_OriBack  *              *    
	  		     			|            *           *      *	m_fAcel_NonePress
	  		  				|               *        *        *
	  	  					|                  *	 *				 
			m_fRadio_DeAccel *   *
	   			     								*  m_fDeAcelMax = m_fRadio_DeAccel * m_fVMax_NormalAccel + m_fAcel_OriBack

													m_fAcel_AcelToNormal

*/


NxReal CCarAccelControl::PeekCarAccel(NxReal vSpeed, NxReal GraviContribute)
{ 
	// 使用 加速道具 状态中 此时一定按下了前向键的 
	if( IsUseItemAcceling())
	{
		if ( vSpeed < m_fVMin )	{
			return __max( 0.0f, m_fAcel_Vfu_UseItemAccel + GraviContribute);
		}
		else if ( vSpeed > m_fVMax_UseItemAccel ) {
			return __min( 0.0f, GraviContribute );
		}
		else {
			return GetUseItemAccel(vSpeed) + GraviContribute;
		}
	}


	// 未使用 加速道具
	if ( vSpeed < m_fVMin )
	{
		
		if ( GetKeyStatue() == Normal_Forward )  
		{
			return __max( 0.0f,m_fAcel_Vfu_NormalAccel + GraviContribute);  // 注意: 下坡后退时 m_fAcel_Vfu_NormalAccel > Max GraviConribute  车才能上坡
		}
		else if ( GetKeyStatue() == Normal_Back ) 
		{
			return __max( 0.0f,GraviContribute ) ;
		}
		else if ( GetKeyStatue() == Normal_None )
		{
			return __max( 0.0f, m_fAcel_Vfu_NonePress + GraviContribute )  ;  
		}
		else
		{ 
			return __max( 0.0f, m_fAcel_Vfu_NonePress + GraviContribute ) ;
		}
	}

	else if ( vSpeed > m_fVMax_NormalAccel )
	{
		// 这里要加入一个逻辑判定 使用加速道具后的递减
		if ( vSpeed - m_fVMax_NormalAccel > 1.5f)
		{
			return __min( m_fAcel_AcelToNormal,GraviContribute + m_fAcel_AcelToNormal);
		}

		if ( GetKeyStatue() == Normal_Forward ) 
		{
			return __min( 0.0f, GraviContribute );
		}
		else if ( GetKeyStatue() == Normal_Back )
		{
			return __min( m_fRadio_DeAccel * m_fVMax_NormalAccel + m_fAcel_OriBack + GraviContribute,
				0.0f );
		}
		else if ( GetKeyStatue() == Normal_None )
		{
			return __min( 0.0f, m_fAcel_NonePress + GraviContribute );
		}
		else
		{
			return __min( 0.0f, m_fAcel_NonePress + GraviContribute );
		}
	}

	else
	{
		return GetNormalAccel(vSpeed) + GraviContribute ;
	}

}


NxReal CCarAccelControl::GetNormalAccel(NxReal vSpeed)
{
	if ( vSpeed < m_fVMin || vSpeed > m_fVMax_NormalAccel
		|| GetKeyStatue() == Accel_ForWard)
	{
		assert(0);
		return 0;
	}

	if ( GetKeyStatue() == Normal_Forward )
	{
		if( vSpeed >= m_fVMin && vSpeed < 0.0f )
		{
			return m_fAcel_Vfu_NormalAccel ;
		}
		else if ( vSpeed >= 0.0f && vSpeed < m_fVTopAccel_NormalAccel )
		{
			return m_fRadioUp_NormalAccel * vSpeed + m_fAcel_OriForward ;
		}
		else if ( vSpeed >= m_fVTopAccel_NormalAccel && vSpeed <= m_fVMax_NormalAccel )
		{
			NxReal PrepareAccel = m_fRadioUp_NormalAccel * m_fVTopAccel_NormalAccel  
				+ m_fAcel_OriForward + m_fRadioDown_NormalAccel * (vSpeed - m_fVTopAccel_NormalAccel) ;

			if ( abs( PrepareAccel) < 0.01)
				return 0;

			return PrepareAccel ;
		}

	}

	if ( GetKeyStatue() == Normal_Back )
	{
		if( vSpeed >= m_fVMin && vSpeed < 0.0f )
		{
			return m_fAcel_OriBack;
		}
		else if ( vSpeed >= 0.0f && vSpeed <= m_fVMax_NormalAccel )
		{
			return
				m_fRadio_DeAccel * vSpeed + m_fAcel_OriBack  ;
		}
	}

	if ( GetKeyStatue() == Normal_None )
	{
		if( vSpeed >= m_fVMin && vSpeed < 0.0f )
		{
			return m_fAcel_Vfu_NonePress * abs(vSpeed / m_fVMin);
		}
		else if ( vSpeed >= 0.0f && vSpeed <= m_fVMax_NormalAccel )
		{
			return  m_fAcel_NonePress * abs( vSpeed / m_fVMax_NormalAccel) ;
		}
	}
	return 0;

}

NxReal CCarAccelControl::GetUseItemAccel(NxReal vSpeed)
{
	if ( vSpeed < m_fVMin || vSpeed > m_fVMax_UseItemAccel
		|| GetKeyStatue() != Accel_ForWard)
	{
		assert(0);
		return 0;
	}

	if( vSpeed >= m_fVMin && vSpeed < 0.0f )
	{
		return m_fAcel_Vfu_UseItemAccel ;
	}
	else if ( vSpeed >= 0.0f && vSpeed < m_fVTopAccel_UseItemAccel )
	{
		return m_fRadioUp_UseItemAccel * vSpeed + m_fAcel_OriForward;
	}
	else if ( vSpeed >= m_fVTopAccel_UseItemAccel && vSpeed <= m_fVMax_UseItemAccel )
	{
		return m_fRadioUp_UseItemAccel * m_fVTopAccel_UseItemAccel  
			+ m_fAcel_OriForward + m_fRadioDown_UseItemAccel * (vSpeed - m_fVTopAccel_UseItemAccel) ;
	}

	return 0;
}

#define PT(x) InputManager::keys[NiInputKeyboard::x]
//----------------------------------------------------------------------
/*
	修改人：	ray
	日期：	1/31/2007
	描叙：	提供加速开关
*/

int CCarAccelControl::GetKeyStatue()
{
	if( PT(KEY_W) == true )
	{
		return m_eAcceState;
	}

	if ( PT(KEY_S) == true)
	{
		return Normal_Back;
	}

	return Normal_None;

}

bool CCarAccelControl::IsUseItemAcceling()
{
	if( PT(KEY_W) == true )
		if ( m_eAcceState == Accel_ForWard )
			return true;

	return false;
}
//----------------------------------------------------------------------

/*
	修改人：	ray
	日期：	1/31/2007
	描叙：	提供加速开关
*/

void CCarAccelControl::StartIntensiveAcce()
{
	m_eAcceState = Accel_ForWard;
}

void CCarAccelControl::StopIntensiveAcce()
{
	m_eAcceState = Normal_Forward;
}
