#ifndef __RESOURCECHECKER_I_H__
#define __RESOURCECHECKER_I_H__

#include "../../teniointerface.h"
#include "resourcestorage_i.h"
namespace Tenio
{
	class IResourceChecker;

	#define ERROR_RESOURCECHECKER_SYNCDOWNLOADERROR						(0x00000001)
	#define ERROR_RESOURCECHECKER_READFILEERROR						(0x00000002)
	
	TENIO_MAKE_TYPEID_MAP(IResourceChecker, COMPONENTID_RESOURCECHECKER);
	/**
	 * @ingroup network
	 * @brief ���汾�����ļ��м�¼��ÿ���ļ������������°汾������
	 *        ע���汾�����ļ�Ĭ����"config\\UpdateVersionFile.xml"
	 *
	 * <p>
	 * Strategy :
	 * 1�������汾�����ļ��е�ÿ���ļ����Ƚϰ汾�š�����ļ��������°汾�������ء�
	 * 2��ÿ���������20���ļ���
	 * 3��ÿ���ļ�֮��û����ϵ��
	 * 4������ͬʱ���ض���ļ��б�
	 * </p>
	 * @param
	 * @return
	 * @see
	 */
	class IResourceChecker : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief ���ݰ汾�����ļ��ļ�¼��ȡ��Դ
		 *
		 * @param pResourceStorage : [in] ��Դ��ȡ�ֿ�, ���ⲿ���ɲ����룬һ��Ϊȫ��Ψһ�Ķ���
		 *
		 * @return  ��������š���1��ʾ����ʧ��, 0��ʾ�����ļ��Ѵ����������°汾��
		 *          1��ʾ�������첽���У����ؽ��ͨ���ص��¼�֪ͨ��
		 *
		 * @see IResourceStorage
		 */
		virtual int CheckAndGetResource(IResourceStorage* pResourceStorage) = 0;

		/**
		 * @ingroup ����Դ��ȡ�ֿ���Ϊ��
		 * @brief 1���ú�����ȡ�����ء�
		 *		  2�����ⲿ��Դ��ȡ�ֿ�����֮ǰ��֪ͨ��������Դ��ȡ�ֿ���Ч
		 *
		 */
		virtual void ResetResourceStorage() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IResourceStorage ���¼��ص��ӿ�
	 * @see IResourceStorage   
	 */
	class IResourceCheckerEvent : public IEvent
	{
	public:		
		
		virtual void OnGetResourceSuccess() = 0;
		virtual void OnGetResourceFail(int nErrorCode) = 0;
	};
	
};

#endif

// InterfaceName:ResourceChecker
// SubFolderName:TRPS\ResourceStorage
