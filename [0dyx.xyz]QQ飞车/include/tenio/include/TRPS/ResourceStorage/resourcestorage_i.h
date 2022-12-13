
#pragma once

#include "teniointerface.h"
#include "resourcestoragedef.h"
#include "VersionControler/versioncontroler_i.h"
#include "TRPS/Downloader/downloader_i.h"
#include "TRPS/DownloadEngine/commandinterpreter_i.h"
#include "downloadconfigdef.h"

namespace Tenio
{
	class IResourceStorage;
	class IResourceStorageEvent;
	class IDownloadConfig;
	class IResourceChecker;
	
	TENIO_MAKE_TYPEID_MAP(IResourceStorage, COMPONENTID_RESOURCESTORAGE)
	TENIO_MAKE_EVENT_MAP(IResourceStorage, IResourceStorageEvent)
	TENIO_MAKE_TYPEID_MAP(IDownloadConfig, COMPONENTID_DOWNLOADCONFIG)
	TENIO_MAKE_TYPEID_MAP(IResourceChecker, COMPONENTID_RESOURCECHECKER)
	
   /**
	 * @ingroup network
	 * @brief 资源获取仓库 【异步】
	 *
	 * <p>
	 * 如果本地存在最新的文件，则告诉客户，资源已经存在 <BR>
	 * 如果资源不在本地，或是Server有更新版本，那么下载更新版本的资源，成功后通知客户
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>	 
	 *   <li>使用 AddConfig() 设置指定下载类型的底层下载配置参数(可选), 
	 *       否则, 下载类型对应的下载参数从配置文件(config\\Dltype.xml)中读取</li>
	 *   <li>使用 AddVersionControler() 设置指定下载类型的的版本控制器(可选), 
	 *       否则, 下载类型使用默认的版本控制器</li>
	 *   <li>使用 AddCommandInterpreter() 设置指定命令的解释器(可选), 
	 *       否则, 使用默认的解释器</li>
	 *   <li>使用 SetDefaultProxy() 设置代理参数(可选), 
	 *       否则, 下载文件不通过代理</li>
	 *   <li>使用 AddEventHandler() 函数传入回调事件指针，相同的回调事件指针只需调用一次
	 *   <li>使用 GetResource() 启动下载</li>
	 *   <li>如果 GetResource()返回值大于0，则本次下载是异步操作，执行结果通过回调事件通知</li>
	 *   <li>如果 GetResource()返回值等于0，则说明本地已经存在最新版本的资源，
	 *       下载完成，之后不会有回调事件通知</li>
	 *   <li>如果 GetResource()返回值等于-1，则本次下载失败，之后不会有回调事件通知</li>
	 *   <li>如果下载是异步操作，
	 *       在事件回调接口中收到IResourceStorageEvent::OnGetResourceSuccess() 消息，
	 *       表示下载成功完成
	 *       在事件回调接口中收到 IResourceStorageEvent::OnGetResourceFail() 消息，
	 *       表示下载失败</li>
	 *       在事件回调接口中收到 IResourceStorageEvent::OnProgressChanged() 消息，
	 *       表示正在下载，参数指示下载进度</li>
	 *   <li>推荐全局维护一个ResourceStorage对象，程序退出或不再需要下载时，销毁ResourceStorage对象。
	 *       销毁前，调用RemoveConfig()、RemoveCommandInterpreter()、RemoveCommandInterpreter()删除
	 *       前面设置的配置器、版本控制器和命令解释器。调用RemoveEventHandler()注销回调事件指针。
	 * </ol>
	 * </p>
	 *
	 * @see IResourceStorageEvent
	 */
	class IResourceStorage : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 获取资源
		 *        重复申请一个资源, 会提高该资源下载的优先级.
		 *        下载组件管理资源是以提交的lpszFileName或pResourceList为单位的。
		 *
		 * @param lpszDownloadType : [in] 下载类型, 指明本次下载使用的参数。
		 *        如果没用调用AddConfig()设置下载参数，则下载参数在配置文件(config\\Dltype.xml)中定义。
		 *
		 * @param lpszFileName : [in] 下载文件名, 可带路径。路径表示为正斜杠形式。如：test/1.zip
		 *
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 *
		 * @param dwPriority	 [in] 下载优先级：
		 *	  PERIORITY_HIGH :	 高，适合实时下载
		 *    PERIORITY_NORMAL : 中，适合普通下载
		 *    PERIORITY_LOW :	 低，适合后台下载
		 *
		 * @return  下载任务号。－1表示下载启动失败, 0表示最新资源已存在。
		 *          大于0表示下载在异步进行，下载结果通过回调事件通知。
		 *
		 * @see 结构体定义:PRESOURCELIST (即: tagResourceList)
		 *
		 * <p>
	     * 该函数执行失败主要有以下原因：
	     *  <ol>
		 *    <li>lpszDownloadType或lpszFileName为空指针</li>
		 *    <li>lpszDownloadType未定义</li>
	     *	  <li>版本控制器还没准备好</li>
	     *  </ol>
	     * </p>
		 */
		virtual int GetResource(
			LPCTSTR lpszDownloadType, 
			LPCTSTR lpszFileName, 
			DWORD dwMilliSeconds,
			DWORD dwPriority) = 0;

		/**
		 * @brief 获取资源
		 *        重复申请一个资源, 会提高该资源下载的优先级.
		 *        下载组件管理资源是以提交的lpszFileName或pResourceList为单位的。
		 * @param pResourceList : [in] 下载文件列表, 可带路径。路径表示为正斜杠形式。
		 *
		 * @param dwMilliSeconds [in] 超时时长，单位：毫秒。0表示使用默认值
		 *
		 * @return  下载任务号。－1表示下载启动失败, 0表示最新资源已存在。
		 *          大于0表示下载在异步进行，下载结果通过回调事件通知。
		 *
		 * @see 结构体定义:PRESOURCELIST (即: tagResourceList)
		 *
		 * <p>
	     * 该函数执行失败主要有以下原因：
	     *  <ol>
		 *    <li>pResourceList 等于0</li>
	     *    <li>pResourceList中的下载类型未定义</li>
	     *	  <li>版本控制器还没准备好</li>
	     *  </ol>
	     * </p>
		 */
		virtual int GetResource(PRESOURCELIST pResourceList, DWORD dwMilliSeconds) = 0;

		/**
		 * @brief 取消下载
		 * @param nTaskID [in] 下载任务号
		 * <p>
	     * 该函数执行失败主要有以下原因：
	     *  <ol>
		 *    <li>nTaskID对应的任务不存在</li>
		 *    <li>nTaskID对应的任务已执行结束</li>
	     * </p>
		 */
		virtual BOOL CancelTask(int nTaskID) = 0;
 
		/**
		 * @brief 设置下载类型的版本控制器 [Option]
		 *
		 * <p>
		 * 设置下载类型的版本控制器（可以基于版本描述文件、向Server查询……）
		 * 如果不设置，采用默认的版本控制器：根据文件升级版本比较策略
		 * 必须在调用下载前设置，否则设置无效
		 * </p>
		 *
		 * @param lpszDownloadType [in]下载类型
		 * @param pVersionControler [in] 版本控制器
		 * @return 成功返回TRUE，失败返回FALSE
		 * @see IVersionControler
		 */
		virtual BOOL AddVersionControler(LPCTSTR lpszDownloadType, IVersionControler * pVersionControler) = 0;

		/**
		 * @brief 删除下载类型的版本控制器 [Option]
		 * 底层不作版本控制器的销毁，调用者要自己销毁
		 *
		 * @param lpszDownloadType 下载类型
		 */
		virtual void RemoveVersionControler(LPCTSTR lpszDownloadType) = 0;

		/**
		 * @brief 设置下载类型的版本控制有效(无效) [Option]
		 *
		 * <p>
		 * 默认所有下载类型的版本控制均有效。
		 * 如果要对某个下载类型取消版本控制, 对该下载类型调用本函数
		 * 必须在调用下载前设置，否则设置无效
		 * </p>
		 *
		 * @param lpszDownloadType[in] 下载类型
		 * @param bValid [in]   TRUE : 版本控制有效; FALSE : 版本控制无效
		 * @return 成功返回TRUE，失败返回FALSE
		 * @see IVersionControler
		 */
		virtual BOOL SetVersionFilter(LPCTSTR lpszDownloadType, BOOL bValid) = 0;

		/**
		 * @brief 设置下载类型的底层下载配置参数 [Must]
		 *
		 * <p>
		 * 使用本函数设置的下载配置器, 优先级要高于配置文件(config\\Dltype.xml)中定义的下载配置器
		 * 即，如果使用本函数设置某下载类型的配置器DLCFG，则实际下载时, 该下载类型的文件使用配置器DLCFG，
		 * 无论配置文件中是否有定义
		 * 
		 * 必须在调用下载前设置，否则调用下载无效
		 * </p>
		 * @param lpszDownloadType 下载类型
		 * @param pDownloadConfig [in] 下载配置器
		 * @see IDownloadConfig
		 */
		virtual void AddConfig(LPCTSTR lpszDownloadType, IDownloadConfig * pDownloadConfig) = 0;

		/**
		 * @brief 删除下载类型的配置器 [Option]
		 * 底层不作配置器的销毁，调用者要自己销毁
		 *
		 * @param lpszDownloadType [in] 下载类型
		 */
		virtual void RemoveConfig(LPCTSTR lpszDownloadType) = 0;
		

		/**
		 * @brief 设置命令解释器 [Must]
		 *
		 * <p>
		 * 必须在调用下载前设置，否则调用下载无效
		 * </p>
		 * @param lpszCommandType [in] 命令类型
		 * @param pCommandInterpreter [in] 命令解释器
		 * @see ICommandInterpreter
		 */
		virtual void AddCommandInterpreter(
			LPCTSTR lpszCommandType, 
			ICommandInterpreter* pCommandInterpreter) = 0;


		/**
		 * @brief 删除命令解释器 [Option]
		 * 底层不作命令解释器的销毁，调用者要自己销毁
		 *
		 * @param lpszCommandType 命令类型
		 */
		virtual void RemoveCommandInterpreter(
			LPCTSTR lpszCommandType) = 0;

		/**
		 * @brief 设置代理参数 [Option]
		 *
		 * <p>
		 * 必须在调用下载前设置，否则调用下载无效
		 * </p>
		 * @param pSock5ProxyParam [in] 代理信息
		 * @see SOCK5PROXYPARAM
		 */
		virtual void SetDefaultProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;

		/**
		 * @brief 设置总带宽 [Option]
		 *
		 * <p>
		 * 不设置带宽，则使用默认值
		 * 必须在调用下载前设置，否则调用下载无效
		 * </p>
		 * @param nBandWidth [in] 带宽, 单位(KB/S)
		 * @see SOCK5PROXYPARAM
		 */
		virtual void SetTotalBandWidth(int nBandWidth) = 0;

		/**
		 * @brief 检查更新 [Option]
		 *
		 * <p>
		 * 文件下载失败，或下载成功但更新失败，更新命令会写到脚本文件中
		 * 调用本函数，则从脚本文件读出并执行所有命令
		 * </p>
		 * @return  更新结果。
		 *          －1表示更新失败,
		 *			0表示更新成功,
		 *          1表示更新在异步进行，更新结果通过回调事件通知。
		 */
		virtual int CheckUpdate() = 0;

		/**
		 * @ingroup 设置下载任务优先级
		 * @brief 当下载任务数多于允许的最大下载线程数时，下载任务被置于就绪队列。
		 *        使用本函数设置某任务的优先级，可以改变该任务在就绪对列的位置，
		 *        影响该任务的执行时间
		 *
		 * @param  nTaskID [in] 任务号
		 * @param  nPeriority [in] 优先级。
		 *         ADD_READY_1 : 高优先级，只要有空闲线程，该级别任务可以立即执行
		 *         ADD_READY_2 : 中优先级。
		 *         ADD_READY_3 : 低优先级。
		 */
		virtual void SetPriority(int nTaskID, int nPeriority) = 0;
	};
    
	/**
	 * @ingroup network
	 * @brief IResourceStorage 的事件回调接口
	 * @see IResourceStorage   
	 */
	class IResourceStorageEvent : public IEvent
	{
	public:
		/**
		 * @brief 下载成功
		 * @param nTaskID [in] 下载任务号
		 */
		virtual void OnGetResourceSuccess(int nTaskID) = 0;

		/**
		 * @brief 下载失败
		 * @param nTaskID [in] 下载任务号
		 * @param nErrorCode 错误代码
		 */
		virtual void OnGetResourceFail(int nTaskID, int nErrorCode) = 0;

		/**
		 * @brief 下载进度发生变化
		 * @param nTaskID [in] 下载任务号
		 * @param dwFileSize [in]下载文件的总长度，单位(字节)
		 * @param dwDownloadedSize [in] 已下载大小，单位(字节)
		 */
		virtual void OnProgressChanged(
			int nTaskID, DWORD dwFileSize, DWORD dwDownloadedSize) = 0;

		/**
		 * @brief 检查更新执行成功
		 *        只有调用CheckUpdate()时，才可能收到该消息
		 */
		virtual void OnUpdateSuccess() = 0;

		/**
		 * @brief 检查更新执行失败
		 *        只有调用CheckUpdate()时，才可能收到该消息
		 * @param nErrorCode [in] 错误代码
		 */
		virtual void OnUpdateFail(int nErrorID) = 0;
	};

	/**
	 * @ingroup network
	 * @brief 下载配置器 
	 *
	 * <p>
	 * 存放公共下载参数：带宽、代理、下载服务器群组的IP和PORT
	 * P2P配置文件的路径名、下载文件在服务器端的公共路径、
	 * 下载文件在客户端的公共保存路径
	 * </p>
	 */
	class IDownloadConfig : public IComponent
	{
	public:
		
		/**
		 * @brief 初始化配置器，并执行参数有效性检查
		 * @param pstDownloadTypeParam [in] 下载参数
		 * @return 参数有效，则返回TRUE;
		 *         否则，返回FALSE
		 * <p>
	     * 以下参数无效
	     *  <ol>
		 *    <li>服务器端路径为空</li>
		 *    <li>临时保存路径为空</li>
		 *    <li>覆盖路径为空</li>
		 *    <li>服务器IP列表为空</li>
	     *  </ol>
	     * </p>
		 * @see PDOWNLOADTYPEPARAM
		 */
		virtual BOOL Init(PDOWNLOADTYPEPARAM pstDownloadTypeParam) = 0;

		/**
		 * @brief 设置当前文件
		 *        该函数将文件名与相关路径拼凑在一起，形成完整路径
		 *
		 * <p>
		 * 本函数要在下面的GET系列函数之前调用
		 * </p>
		 * @param lpszFileName [in] 文件名，可以带路径
		 * @return 成功返回TRUE,失败返回FALSE
		 */
		virtual BOOL SetCurrentFile(LPCTSTR lpszFileName) = 0;

		/**
		 * @brief 获得下载带宽
		 *
		 * @param dwBandWidth [out] 带宽
		 * @return 带宽有效返回TRUE,无效返回FALSE
		 */
		virtual BOOL GetBandWidth(DWORD& dwBandWidth) = 0;

		/**
		 * @brief 获得下载服务器群组的IP和PORT
		 * @param  pIPPortPackets [out] 指向下载服务器群组的IP和PORT
		 * 的结构体的指针
		 * @return 如果pIPPortPackets != NULL且下载配置器记录的下载服务器
		 * 群组的IP和PORT有效，则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetIPPortPackets(IPPORTPACKETS * pIPPortPackets) = 0;

		/**
		 * @brief 获得P2P配置参数
		 * @param pP2PParamList [out] P2P配置参数
		 * @return pP2PParamList != NULL且P2P参数有效
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetP2PParam(P2PPARAMLIST* pP2PParamList) = 0;

		/**
		 * @brief 获得Server上的完整文件路径
		 * @param lpszLocalPath [out] 接收Server路径的字符串
		 * @return lpszLocalPath != NULL且Server路径非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetServerPath(LPTSTR lpszServerPath, int nCharCount) = 0;

		/**
		 * @brief 获得下载文件的本地保存路径
		 * 文件的下载过程是先下载到LocalPath，再覆盖到SubstitutePath
		 * @param lpszLocalPath [out] 接收下载文件的本地保存路径的字符串
		 * @return lpszLocalPath != NULL且下载文件的本地保存路径非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetLocalPath(LPTSTR lpszLocalPath, int nCharCount) = 0;

		/**
		 * @brief 获得未完成文件的临时文件名
		 
		 * @param lpszTemPath [out] 接收临时文件名的字符串
		 * @return lpszTemPath != NULL且临时文件名非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetTemporaryPath(LPTSTR lpszTemPath, int nCharCount) = 0;

		/**
		 * @brief 获得下载文件的覆盖路径
		 * @param lpszSubstitutePath [out] 接收覆盖路径的字符串
		 * @return lpszSubstitutePath != NULL且下载文件的覆盖路径非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetSubstitutePath(LPTSTR lpszSubstitutePath, int nCharCount) = 0;


		/**
		 * @brief 获得文件下载成功后执行的命令字符串
		 * @param lpszCommand [out] 接收命令的字符串
		 * @return lpszSubstitutePath != NULL且命令非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetCommand(LPTSTR lpszCommand, int nCharCount) = 0;

		/**
		 * @brief 获得使用的下载器名称
		 *
		 * @param lpszDownloadType [out] 接收下载器名称的字符串
		 *
		 * 现在的下载器有下列类型:
		 * 1、HTTPDOWNLOADER  执行http下载
		 * 2、IPGROUPDOWNLOADER 
		 * 先尝试P2P下载，失败后尝试http下载,
		 * 如果有多个IP,则每个ip试过后，才报告下载失败
		 * 3、P2PDOWNLOADER  执行P2P下载
		 *
		 * @return 成功返回TRUE,失败返回FALSE
		 * @see
		 */
		virtual BOOL GetDownloaderType(LPTSTR lpszDownloadType) = 0;
	};

	/**
	 * @ingroup network
	 * @brief 检查版本描述文件中记录的每个文件，若不是最新版本则下载
	 *        注：版本描述文件默认是"config\\UpdateVersionFile.xml"
	 * <p>
	 * Strategy :
	 * 1、读出版本描述文件中的每个文件，比较版本号。如果文件不是最新版本，则下载。
	 * 2、每次最多下载20个文件。
	 * 3、每个文件之间没有联系。
	 * 4、不能同时下载多个文件列表。
	 * </p>
	 * @param
	 * @return
	 * @see
	 */
	class IResourceChecker : public IComponent, public IEventProducer
	{
	public:
		/**
		 * @brief 根据版本描述文件的记录获取资源
		 *
		 * @param pResourceStorage : [in] 资源获取仓库, 由外部生成并传入，一般为全局唯一的对象。
		 *
		 * @return  下载任务号。－1表示下载失败, 0表示所有文件已存在且是最新版本。
		 *          1表示下载在异步进行，下载结果通过回调事件通知。
		 *
		 * @see IResourceStorage
		 */
		virtual int CheckAndGetResource(IResourceStorage* pResourceStorage) = 0;

		/**
		 * @ingroup 将资源获取仓库置为空
		 * @brief 1、该函数可取消下载。
		 *		  2、当外部资源获取仓库析构之前，通知本对象资源获取仓库无效
		 *
		 */
		virtual void ResetResourceStorage() = 0;
	};

	/**
	 * @ingroup network
	 * @brief IResourceStorage 的事件回调接口
	 * @see IResourceStorage   
	 */
	class IResourceCheckerEvent : public IEvent
	{
	public:	
		
		/**
		 * @brief 下载成功
		 */
		virtual void OnGetResourceSuccess() = 0;

		/**
		 * @brief 下载失败
		 * @param nErrorCode 错误代码
		 */
		virtual void OnGetResourceFail(int nErrorCode) = 0;
	};

	};
