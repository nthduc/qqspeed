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
	 * @brief �ô��ڶ�ʱ����Ϣʵ�ֵĶ�ʱ��
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */	
	class IWinTimer : public IComponent
	{
	public:
		/**
		 *  @brief ���ò�����ʱ������ʼ��ʱ
		 *  @param dwMilliSeconds ��ʱ�����ʱ�䣨���룩
		 *  @param dwParam ��ʱ���¼��ص�ʱ���صĲ��������û�����
		 *	@param pEvent ��ʱ��������ָ��
		 *  @return ���ض�ʱ��ID���ڱ���ʱ��������Ψһ��
		 */		
		virtual DWORD SetTimer(DWORD dwMilliSeconds, DWORD dwParam, IWinTimerEvent* pEvent) = 0;

		/**
		 *  @brief ��ĳ��ID�Ķ�ʱ�������µļ��ʱ��,��ʱ��������һ������ڼ����и����µļ��ʱ��
		 *  @param dwTimerID ��ʱ��ID
		 *  @param dwMilliSeconds �µļ��ʱ�䣨���룩
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE��ʧ�ܵ�ԭ��ͨ���Ǹö�ʱ��ID������
		 */	
		virtual BOOL SetTimer(DWORD dwTimerID, DWORD dwMilliSeconds) = 0;

		/**
		 *  @brief ֹͣ��ʱ��
		 *  @param dwTimerID ��ʱ��ID���� SetTimer() ��������
		 */		
		virtual void KillTimer(DWORD dwTimerID) = 0;

		/**
		 *  @brief ����������е����ж�ʱ��
		 *  @sa 
		 */	
		virtual void ClearAll() = 0;
	};

	/**
	 * @ingroup system
	 * @brief WinTimer�¼��ӿ�
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */	
	class IWinTimerEvent : public IEvent
	{
	public:
		/**
		 *  @brief ��ʱ���¼��ص�
		 *  @param dwTimerID,��ʱ��ID���� IWinTimer::SetTimer() ��������
		 *  @param dwParam ��ʱ���������� IWinTimer::SetTimer() ��������
		 */		
		virtual void OnTimer(DWORD dwTimerID, DWORD dwParam) = 0;
	};
};

#endif

// InterfaceName:WinTimer
// SubFolderName:WinTimer
