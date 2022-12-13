#ifndef __TIMER_I_H__
#define __TIMER_I_H__

#include "../event_i.h"
#include "../component_i.h"
#include "../objecttypeidmap.h"

namespace Tenio
{
	class ITimeout;
	class ITimeoutEvent;
	class IInterval;
	class IIntervalEvent;
	
	TENIO_MAKE_TYPEID_MAP(ITimeout,  COMPOENTID_TIMEOUT )
	TENIO_MAKE_TYPEID_MAP(IInterval, COMPOENTID_INTERVAL)
	
	 /**
	 * @ingroup system
	 * @brief 超时控制器
	 *
	 * <p>
	 *　　 本模块内部使用线程实现。
	 * </p>
	 *
	 * <p>
	 * 超时控制器和定时器依靠同一个独立线程累计时间。它计算自调用 SetTimeout()  以来经过的时间，
	 * 如果该时间等于设置的时长, 则调用 ITimeoutEvent::OnTimeout() 函数，然后内部调用
	 * ClearTimeout() 停止计时。
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>
	 *   <li>使用 SetTimeout() 函数设置超时时长，传入回调事件指针和调用者组件指针，
	 *       指定回调 ITimeoutEvent::OnTimeout() 函数时使用的参数</li>
	 *   <li>在事件回调接口中收到 ITimeoutEvent::OnTimeout() 消息，表示自调用 SetTimeout()  以来
	 *       经过了指定时长</li>
	 *   <li>可以使用 ClearTimeout() 函数停止计时，之后就不会收到 ITimeoutEvent::OnTimeout() 消息</li>
	 * </ol>
	 * </p>
	 *
	 * @see IInterval, ITimeoutEvent
	 */
	class ITimeout : public IComponent
	{
	public:
		/**
		 * @brief 设置超时控制器参数，开始计时
		 * @param dwMilliSeconds [in] 超时时长(单位：毫秒)
		 * @param pTimeoutEvent [in] 回调事件指针
         * @param dwParam [in] 经过指定时长调用 ITimeoutEvent::OnTimeout() 时传入的参数
		 * @param pCreatorObject [in] 指向调用者组件的指针
		 * @return 成功返回TRUE，失败返回FALSE
		 *
		 * <p>
	     * 如果该函数执行失败，主要有以下原因：
	     *  <ol>
		 *    <li> pTimeoutEvent 或 pCreatorObject 等于0</li>
	     *	  <li> 执行 SetTimeout() 时，正好上次设置的时间到，ITimeoutEvent::OnTimeout() 就要被触发
		 *         此时禁止设置超时控制器参数。</li>
		 *    <li> 本进程中超时控制器 和 定时器总数 等于 TIMER_MAX - 1 </li>
	     *  </ol>
	     * </p>
		 * <p>
	     *    以上提到的参数均在Include\\Timer\\timerdef.h 中定义
	     * </p>
		 */
		virtual BOOL SetTimeout(DWORD dwMilliSeconds, ITimeoutEvent* pTimeoutEvent, 
			DWORD dwParam, IComponent* pCreatorObject) = 0;

		/**
		 * @brief 取消超时设置，停止计时
		 */
		virtual void ClearTimeout() = 0;
	};

	/**
	 * @ingroup system
	 * @brief ITimeout 的事件回调接口
	 * @see ITimeout
	 */
	class ITimeoutEvent : public IEvent
	{
	public:

		/**
		 * @brief 自执行 ITimeout::SetTimeout() 以来正好经过指定时长
		 * @param dwParam [in]  ITimeout::SetTimeout() 中传入的同名参数
		 */
		virtual void OnTimeout(DWORD dwParam) = 0;
	};
	
     /**
	 * @ingroup system
	 * @brief 定时器
	 *
	 * <p>
	 *　　 本模块内部使用线程实现。。
	 * </p>
	 *
	 * <p>
	 * 超时控制器和定时器依靠同一个独立线程累计时间。每经过一个定时器周期调用
	 *  IIntervalEvent::OnInterval() 函数。
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>
	 *   <li>使用 SetInterval() 函数设置定时器周期，传入回调事件指针和调用组件指针，
	 *       指定回调 IIntervalEvent::OnInterval() 函数时使用的参数</li>
	 *   <li>每经过一个定时器周期在事件回调接口中收到 IIntervalEvent::OnInterval() 消息</li>
	 *   <li>可以使用 ClearInterval() 取消计时器</li>
	 * </ol>
	 * </p>
	 *
	 * @see ITimeout, IIntervalEvent
	 */
	class IInterval : public IComponent
	{
	public:
		/**
		 * @brief 设置定时器参数，开始计时
		 * @param dwMilliSeconds [in] 定时器周期(单位：毫秒)
		 * @param pIntervalEvent [in] 回调事件指针
         * @param dwParam [in] 每经过一个定时器周期调用 IIntervalEvent::OnInterval() 时传入的参数
		 * @param bContinuousMode [in] 连续模式。
		 *        TRUE：每经过一个定时器周期调用 IIntervalEvent::OnInterva() 一次；
		 *        FALSE：自本函数执行后，经过一个定时器周期调用 IIntervalEvent::OnInterval() 一次。
		 *               之后不再调用 IIntervalEvent::OnInterval() 。
		 * @param pCreatorObject [in] 指向调用者组件的指针
		 * @return 成功返回TRUE，失败返回FALSE
		 *
		 * <p>
	     * 如果该函数执行失败，主要有以下原因：
	     *  <ol>
		 *    <li> pIntervalEvent 或 pCreatorObject 等于0</li>
	     *	  <li> 执行 SetInterval() 时，正好又经过一个定时器周期，IIntervalEvent::OnInterval() 就要被触发
		 *         此时禁止设置定时器参数。</li>
		 *    <li> 本进程中超时控制器 和 定时器总数 等于 TIMER_MAX - 1 </li>
	     *  </ol>
	     * </p>
		 * <p>
	     *    以上提到的参数均在Include\\Timer\\timerdef.h 中定义
	     * </p>
		 */
		virtual BOOL SetInterval(DWORD dwMilliSeconds, IIntervalEvent* pIntervalEvent, 
			DWORD dwParam, BOOL bContinuousMode, IComponent* pCreatorObject) = 0;

        /**
		 * @brief 取消定时器，停止计时
		 */
		virtual void ClearInterval() = 0;
	};
	
	/**
	 * @ingroup system
	 * @brief IInterval 的事件回调接口
	 * @see IInterval
	 */
	class IIntervalEvent : public IEvent
	{
	public:
        /**
		 * @brief 自执行 IInterval::SetInterval() 以来正好经过定时器周期的整数倍
		 * @param dwParam [in]  IInterval::SetInterval() 中传入的同名参数
		 */
		virtual void OnInterval(DWORD dwParam) = 0;
	};
	
};

#endif
