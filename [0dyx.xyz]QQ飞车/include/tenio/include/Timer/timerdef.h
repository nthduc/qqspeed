#ifndef __TIMERDEF_I_H
#define __TIMERDEF_I_H

namespace Tenio
{    
	   // 累计时间的周期(单位：毫秒)
   #define TIMER_MIN_SPAN						10
       
	  //  进程中需要被通知的 ITimerEvent 的最大个数 
   #define TIMER_MAX							2048

      //  无效TimerID
   #define TIMER_ID_INVALID					   ((DWORD) -1)

	  //  CTimerObject 使用的消息
   #define WM_TIMER_EX							(WM_USER + 101)
}

#endif	// __TIMERDEF_I_H