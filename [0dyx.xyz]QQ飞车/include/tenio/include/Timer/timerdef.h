#ifndef __TIMERDEF_I_H
#define __TIMERDEF_I_H

namespace Tenio
{    
	   // �ۼ�ʱ�������(��λ������)
   #define TIMER_MIN_SPAN						10
       
	  //  ��������Ҫ��֪ͨ�� ITimerEvent �������� 
   #define TIMER_MAX							2048

      //  ��ЧTimerID
   #define TIMER_ID_INVALID					   ((DWORD) -1)

	  //  CTimerObject ʹ�õ���Ϣ
   #define WM_TIMER_EX							(WM_USER + 101)
}

#endif	// __TIMERDEF_I_H