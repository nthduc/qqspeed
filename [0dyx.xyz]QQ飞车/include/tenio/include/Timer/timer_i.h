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
	 * @brief ��ʱ������
	 *
	 * <p>
	 *���� ��ģ���ڲ�ʹ���߳�ʵ�֡�
	 * </p>
	 *
	 * <p>
	 * ��ʱ�������Ͷ�ʱ������ͬһ�������߳��ۼ�ʱ�䡣�������Ե��� SetTimeout()  ����������ʱ�䣬
	 * �����ʱ��������õ�ʱ��, ����� ITimeoutEvent::OnTimeout() ������Ȼ���ڲ�����
	 * ClearTimeout() ֹͣ��ʱ��
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>
	 *   <li>ʹ�� SetTimeout() �������ó�ʱʱ��������ص��¼�ָ��͵��������ָ�룬
	 *       ָ���ص� ITimeoutEvent::OnTimeout() ����ʱʹ�õĲ���</li>
	 *   <li>���¼��ص��ӿ����յ� ITimeoutEvent::OnTimeout() ��Ϣ����ʾ�Ե��� SetTimeout()  ����
	 *       ������ָ��ʱ��</li>
	 *   <li>����ʹ�� ClearTimeout() ����ֹͣ��ʱ��֮��Ͳ����յ� ITimeoutEvent::OnTimeout() ��Ϣ</li>
	 * </ol>
	 * </p>
	 *
	 * @see IInterval, ITimeoutEvent
	 */
	class ITimeout : public IComponent
	{
	public:
		/**
		 * @brief ���ó�ʱ��������������ʼ��ʱ
		 * @param dwMilliSeconds [in] ��ʱʱ��(��λ������)
		 * @param pTimeoutEvent [in] �ص��¼�ָ��
         * @param dwParam [in] ����ָ��ʱ������ ITimeoutEvent::OnTimeout() ʱ����Ĳ���
		 * @param pCreatorObject [in] ָ������������ָ��
		 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
		 *
		 * <p>
	     * ����ú���ִ��ʧ�ܣ���Ҫ������ԭ��
	     *  <ol>
		 *    <li> pTimeoutEvent �� pCreatorObject ����0</li>
	     *	  <li> ִ�� SetTimeout() ʱ�������ϴ����õ�ʱ�䵽��ITimeoutEvent::OnTimeout() ��Ҫ������
		 *         ��ʱ��ֹ���ó�ʱ������������</li>
		 *    <li> �������г�ʱ������ �� ��ʱ������ ���� TIMER_MAX - 1 </li>
	     *  </ol>
	     * </p>
		 * <p>
	     *    �����ᵽ�Ĳ�������Include\\Timer\\timerdef.h �ж���
	     * </p>
		 */
		virtual BOOL SetTimeout(DWORD dwMilliSeconds, ITimeoutEvent* pTimeoutEvent, 
			DWORD dwParam, IComponent* pCreatorObject) = 0;

		/**
		 * @brief ȡ����ʱ���ã�ֹͣ��ʱ
		 */
		virtual void ClearTimeout() = 0;
	};

	/**
	 * @ingroup system
	 * @brief ITimeout ���¼��ص��ӿ�
	 * @see ITimeout
	 */
	class ITimeoutEvent : public IEvent
	{
	public:

		/**
		 * @brief ��ִ�� ITimeout::SetTimeout() �������þ���ָ��ʱ��
		 * @param dwParam [in]  ITimeout::SetTimeout() �д����ͬ������
		 */
		virtual void OnTimeout(DWORD dwParam) = 0;
	};
	
     /**
	 * @ingroup system
	 * @brief ��ʱ��
	 *
	 * <p>
	 *���� ��ģ���ڲ�ʹ���߳�ʵ�֡���
	 * </p>
	 *
	 * <p>
	 * ��ʱ�������Ͷ�ʱ������ͬһ�������߳��ۼ�ʱ�䡣ÿ����һ����ʱ�����ڵ���
	 *  IIntervalEvent::OnInterval() ������
	 * </p>
	 *
	 * <p>
	 * ʹ������
	 * <ol>
	 *   <li>ʹ�� SetInterval() �������ö�ʱ�����ڣ�����ص��¼�ָ��͵������ָ�룬
	 *       ָ���ص� IIntervalEvent::OnInterval() ����ʱʹ�õĲ���</li>
	 *   <li>ÿ����һ����ʱ���������¼��ص��ӿ����յ� IIntervalEvent::OnInterval() ��Ϣ</li>
	 *   <li>����ʹ�� ClearInterval() ȡ����ʱ��</li>
	 * </ol>
	 * </p>
	 *
	 * @see ITimeout, IIntervalEvent
	 */
	class IInterval : public IComponent
	{
	public:
		/**
		 * @brief ���ö�ʱ����������ʼ��ʱ
		 * @param dwMilliSeconds [in] ��ʱ������(��λ������)
		 * @param pIntervalEvent [in] �ص��¼�ָ��
         * @param dwParam [in] ÿ����һ����ʱ�����ڵ��� IIntervalEvent::OnInterval() ʱ����Ĳ���
		 * @param bContinuousMode [in] ����ģʽ��
		 *        TRUE��ÿ����һ����ʱ�����ڵ��� IIntervalEvent::OnInterva() һ�Σ�
		 *        FALSE���Ա�����ִ�к󣬾���һ����ʱ�����ڵ��� IIntervalEvent::OnInterval() һ�Ρ�
		 *               ֮���ٵ��� IIntervalEvent::OnInterval() ��
		 * @param pCreatorObject [in] ָ������������ָ��
		 * @return �ɹ�����TRUE��ʧ�ܷ���FALSE
		 *
		 * <p>
	     * ����ú���ִ��ʧ�ܣ���Ҫ������ԭ��
	     *  <ol>
		 *    <li> pIntervalEvent �� pCreatorObject ����0</li>
	     *	  <li> ִ�� SetInterval() ʱ�������־���һ����ʱ�����ڣ�IIntervalEvent::OnInterval() ��Ҫ������
		 *         ��ʱ��ֹ���ö�ʱ��������</li>
		 *    <li> �������г�ʱ������ �� ��ʱ������ ���� TIMER_MAX - 1 </li>
	     *  </ol>
	     * </p>
		 * <p>
	     *    �����ᵽ�Ĳ�������Include\\Timer\\timerdef.h �ж���
	     * </p>
		 */
		virtual BOOL SetInterval(DWORD dwMilliSeconds, IIntervalEvent* pIntervalEvent, 
			DWORD dwParam, BOOL bContinuousMode, IComponent* pCreatorObject) = 0;

        /**
		 * @brief ȡ����ʱ����ֹͣ��ʱ
		 */
		virtual void ClearInterval() = 0;
	};
	
	/**
	 * @ingroup system
	 * @brief IInterval ���¼��ص��ӿ�
	 * @see IInterval
	 */
	class IIntervalEvent : public IEvent
	{
	public:
        /**
		 * @brief ��ִ�� IInterval::SetInterval() �������þ�����ʱ�����ڵ�������
		 * @param dwParam [in]  IInterval::SetInterval() �д����ͬ������
		 */
		virtual void OnInterval(DWORD dwParam) = 0;
	};
	
};

#endif
