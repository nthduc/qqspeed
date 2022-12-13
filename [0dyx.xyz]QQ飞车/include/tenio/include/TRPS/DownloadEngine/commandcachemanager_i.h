#ifndef __COMMANDCACHEMANAGER_I_H__
#define __COMMANDCACHEMANAGER_I_H__

#include "../../teniointerface.h"
#include "downloadenginedef.h"

namespace Tenio
{
	class ICommandCacheManager;

	TENIO_MAKE_TYPEID_MAP(ICommandCacheManager, COMPONENTID_COMMANDCACHEMANAGER);

	/**
	 * @ingroup network
	 * @brief  ����������
	 *
	 */
	class ICommandCacheManager : public IComponent
	{
	public:
		
		/**
		 * @brief ��ȡ������Ŀ
		 *
		 * @return ������Ŀ
		 * @see
		 */
		virtual int GetCommandCount() = 0;


		// ֮��������������������������Ϊ:
		// 1 ������������ǰ�˳���1��ʼ����ID����ɾ��һ������ʱ����ȻҪ��������������ID��һ����ʱ
		//   �����ڴ����Ѷ���������ID��������ɾ��֮�������ʱ������
		// 2 �����Ҫ�����������ID�ǰ�˳�����еģ��򲻻����1�Ĵ���ͬʱ�ṩRearrageCommandInOrder()
		//   ���������û��Լ��������°�˳�����ID�Ĳ���

		/**
		 * @brief ��õ�һ������
		 * @param pCommandID [Out] ����ID
		 * @param pVersionResourceList [Out] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL ReadFirstCommand(int* pCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;
		
		/**
		 * @brief �����һ������
		 * @param pCommandID [Out] ����ID
		 * @param pVersionResourceList [Out] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL ReadNextCommand(int* pCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief ��ȡ����
		 * @param pCommandID [In] ����ID
		 * @param pVersionResourceList [Out] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL ReadCommand(int nCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief д����
		 * @param pVersionResourceList [in] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL WriteCommand(PVERSIONRESOURCELIST pVersionResourceList) = 0;

       /**
		 * @brief ɾ������
		 *
		 * @param nCommandID [in] ����ID
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL DeleteCommand(int nCommandID) = 0;

		/**
		 * @brief ɾ������
		 * ���뱾�ļ���ƥ�������ɾ��
		 *
		 * @param pVersionResourceList [in] �ļ���
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 * @see
		 */
		virtual BOOL DeleteCommand(PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief ����С�����˳��Ϊ���ID
		 *
		 */
		virtual void RearrageCommandInOrder() = 0;
	};
};

#endif

// InterfaceName:CommandCacheManager
// SubFolderName:CommandCacheManager
