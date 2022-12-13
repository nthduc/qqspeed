#ifndef __WINTIMER_I_H__
#define __WINTIMER_I_H__

#include "../teniointerface.h"

namespace Tenio
{
#define TIMERID_INVALID	0

	class IWinTimer;
	class IWinTimerEvent;

	TENIO_MAKE_TYPEID_MAP(IWinTimer, COMPONENTID_WINTIMER);

	/**
	 * @ingroup system
	 * @brief 用窗口定时器消息实现的定时器
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	
	class IWinTimer : public IComponent
	{
	public:
		/**
		 *  @brief 设置参数定时器并开始计时
		 *  @param dwMilliSeconds 定时器间隔时间（毫秒）
		 *  @param dwParam 定时器事件回调时返回的参数，由用户给定
		 *	@param pEvent 定时器监听者指针
		 *  @return 返回定时器ID，在本定时器对象中唯一。
		 */		
		virtual DWORD SetTimer(DWORD dwMilliSeconds, DWORD dwParam, IWinTimerEvent* pEvent) = 0;

		/**
		 *  @brief 给某个ID的定时器设置新的间隔时间,定时器将在下一个间隔期计算中改用新的间隔时间
		 *  @param dwTimerID 定时器ID
		 *  @param dwMilliSeconds 新的间隔时间（毫秒）
		 *  @return 成功则返回TRUE，失败返回FALSE，失败的原因通常是该定时器ID不存在
		 */	
		virtual BOOL SetTimer(DWORD dwTimerID, DWORD dwMilliSeconds) = 0;

		/**
		 *  @brief 停止定时器
		 *  @param dwTimerID 定时器ID，由 SetTimer() 函数返回
		 */		
		virtual void KillTimer(DWORD dwTimerID) = 0;

		/**
		 *  @brief 清除本对象中的所有定时器
		 *  @sa 
		 */	
		virtual void ClearAll() = 0;
	};

	/**
	 * @ingroup system
	 * @brief WinTimer事件接口
	 *
	 * <p>
	 *　　 
	 * </p>
	 *
	 * @sa 
	 */	
	class IWinTimerEvent : public IEvent
	{
	public:
		/**
		 *  @brief 定时器事件回调
		 *  @param dwTimerID,定时器ID，由 IWinTimer::SetTimer() 函数返回
		 *  @param dwParam 定时器参数，由 IWinTimer::SetTimer() 函数设置
		 */		
		virtual void OnTimer(DWORD dwTimerID, DWORD dwParam) = 0;
	};
};

#endif

// InterfaceName:WinTimer
// SubFolderName:WinTimer
