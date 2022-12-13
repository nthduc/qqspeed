#ifndef __COMMANDINTERPRETER_I_H__
#define __COMMANDINTERPRETER_I_H__

#include "../../teniointerface.h"
#include "TaskManager/taskmanager_i.h"
#include "TaskManager/taskmanagerdef.h"
#include "commandinterpreterdef.h"

namespace Tenio
{

	class ICommandInterpreter;
	class IUnzipInterpreter;

	TENIO_MAKE_TYPEID_MAP(ICommandInterpreter, COMPONENTID_COMMANDINTERPRETER);
	TENIO_MAKE_TYPEID_MAP(IUnzipInterpreter, COMPONENTID_UNZIPINTERPRETER);

	#define OPTION_EXECUTE				0x00000001
	#define OPTION_CLEAR				0x00000002
	#define OPTION_ROLLBACK				0x00000004
	#define OPTION_INVALID				0xF0000000

	/**
	 * @ingroup network
	 * @brief ���������
	 */
	class ICommandInterpreter : public ITaskExecutor
	{
	public:

		/**
		 * @brief ��ʼ���������������ͺͲ�����
		 * ����Ϊһ���������ַ����������������Ҫn����������
		 * �������ַ������Կո���Ϊ�ָ�������ǰn��Token����n������
		 *
		 * @param lpszCommandType [in] ��������
		 * @param lpszParam [in] ����
		 * @return �ɹ�����TRUE,ʧ�ܷ���FALSE
		 */
		virtual BOOL Init(LPCTSTR lpszCommandType, LPCTSTR lpszParam) = 0;

		/**
		 * @brief ���ò������ͣ�
		 * 1 OPTION_EXECUTE ִ������
		 * 2 OPTION_CLEAR   ɾ������
		 * 3 OPTION_ROLLBACK �ع�
		 * �����ִ��˳��:
		 * 1 ִ������
		 * 2 �������ִ�гɹ�����ɾ������
		 * 3 �������ִ��ʧ�ܣ���ع�
		 * <p>
		 * </p>
		 * @param dwOperation [in] ��������
		 * @see
		 */
		virtual BOOL SetOption(DWORD dwOperation) = 0;

		/**
		 * @brief ��ȡ�����ļ���
		 *		  ����ֻ�����ͨ�����COPY����
		 *        Ŀ�ģ���¼COPY����Ӱ����ļ����ڻع���ɾ������ʱ��
		 *        ��SetResourceRecord()�������������
		 * @param pUpdateFileList [in] ִ�и��µ��ļ���¼
		 * @return
		 * @see
		 */
		virtual BOOL GetResourceRecord(UPDATEFILERECORDLIST* pUpdateFileList) = 0;

		/**
		 * @brief ���ø����ļ���������
		 *		  ��ͨ�����COPY���֮ǰ������ļ���
		 *		  ������������ع���ɾ������ʱʹ��
		 *
		 * @param pUpdateFileList [in] ִ�и��µ��ļ���¼
		 * @return
		 * @see
		 */
		virtual void SetResourceRecord(UPDATEFILERECORDLIST* pUpdateFileList) = 0;
	};

	/**
	 * @ingroup network
	 * @brief ICommandInterpreter ���¼��ص��ӿ�
	 * @see IResourceStorage
	 */
	class ICommandInterpreterEvent : public IEvent
	{
	public:
		/**
		 * @brief �첽����ִ�гɹ�
		 * @param nTaskID [in] ���������
		 */
		virtual void OnInterpreteSuccess(int nTaskID) = 0;

		/**
		 * @brief �첽����ִ��ʧ��
		 * @param nTaskID [in] ���������
		 * @param nErrorCode [in] �����
		 * 
		 */
		virtual void OnInterpreteFail(int nTaskID, int nErrorCode) = 0;
	};
	
	/**
	 * @ingroup network
	 * @brief ��ѹ���������
	 */
	class IUnzipInterpreter : public ICommandInterpreter
	{
		
	};
};

#endif

// InterfaceName:CommandInterpreter
// SubFolderName:ResourceStorage
