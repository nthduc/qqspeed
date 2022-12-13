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
	 * @brief  命令缓存管理器
	 *
	 */
	class ICommandCacheManager : public IComponent
	{
	public:
		
		/**
		 * @brief 获取命令数目
		 *
		 * @return 命令数目
		 * @see
		 */
		virtual int GetCommandCount() = 0;


		// 之所以引入下面两个函数，是因为:
		// 1 如果所有命令是按顺序从1开始分配ID，则删除一个命令时，必然要将其后的所有命令ID减一，这时
		//   就与内存中已读出的命令ID不符，再删除之后的命令时，出错。
		// 2 如果不要求所有命令的ID是按顺序排列的，则不会出现1的错误，同时提供RearrageCommandInOrder()
		//   函数，让用户自己触发从新按顺序分配ID的操作

		/**
		 * @brief 获得第一个命令
		 * @param pCommandID [Out] 命令ID
		 * @param pVersionResourceList [Out] 文件集
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL ReadFirstCommand(int* pCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;
		
		/**
		 * @brief 获得下一个命令
		 * @param pCommandID [Out] 命令ID
		 * @param pVersionResourceList [Out] 文件集
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL ReadNextCommand(int* pCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief 读取命令
		 * @param pCommandID [In] 命令ID
		 * @param pVersionResourceList [Out] 文件集
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL ReadCommand(int nCommandID, PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief 写命令
		 * @param pVersionResourceList [in] 文件集
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL WriteCommand(PVERSIONRESOURCELIST pVersionResourceList) = 0;

       /**
		 * @brief 删除命令
		 *
		 * @param nCommandID [in] 命令ID
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL DeleteCommand(int nCommandID) = 0;

		/**
		 * @brief 删除命令
		 * 将与本文件集匹配的命令删除
		 *
		 * @param pVersionResourceList [in] 文件集
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL DeleteCommand(PVERSIONRESOURCELIST pVersionResourceList) = 0;

		/**
		 * @brief 按从小到大的顺序为命令赋ID
		 *
		 */
		virtual void RearrageCommandInOrder() = 0;
	};
};

#endif

// InterfaceName:CommandCacheManager
// SubFolderName:CommandCacheManager
