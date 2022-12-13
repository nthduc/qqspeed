
#pragma once

#include "teniointerface.h"
#include "downloadconfigdef.h"
namespace Tenio
{
	class IDownloadConfig;
	
	TENIO_MAKE_TYPEID_MAP(IDownloadConfig, COMPONENTID_DOWNLOADCONFIG)
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
		 * @param stDownloadTypeParam [in] 下载参数
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
		 */
		virtual BOOL Init(PDOWNLOADTYPEPARAM pstDownloadTypeParam) = 0;
		virtual BOOL SetCurrentFile(/*LPCTSTR lpszDownloadType, */LPCTSTR lpszFileName) = 0;
		//virtual BOOL GetCurrentFile(LPTSTR lpszDownloadType, 
		//	int nCharCountForDownloadType, LPTSTR lpszFileName, int nCharCountForFileName) = 0;
		/**
		 * @brief 获得带宽
		 * @param nBandWidth [out] 带宽
		 * @return 如果下载配置器记录的带宽大于0，则返回TRUE;
		 *         否则，返回FALSE
		 */
		virtual BOOL GetBandWidth(DWORD& dwBandWidth) = 0;

        /**
         * @brief 获得代理
         * @param pSock5ProxyParam [out]  指向代理信息的结构体的指针
         * @return 如果pSock5ProxyParam != NULL且下载配置器记录的代理IP
		 *         非空，则返回TRUE;否则,返回FALSE
         */
		//virtual BOOL GetProxy(SOCK5PROXYPARAM * pSock5ProxyParam) = 0;	

		/**
		 * @brief 获得下载服务器群组的IP和PORT
		 * @param  pIPPortPackets [out] 指向下载服务器群组的IP和PORT
		 * 的结构体的指针
		 * @return 如果pIPPortPackets != NULL且下载配置器记录的下载服务器
		 * 群组的IP和PORT有效，则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetIPPortPackets(IPPORTPACKETS * pIPPortPackets) = 0;

		/**
		 * @brief 获得P2P配置文件的路径名
		 * @param lpszConfigFile [out] 接收P2P配置文件的路径名的字符串
		 * @return lpszConfigFile != NULL且P2P配置文件的路径名非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetP2PParam(P2PPARAMLIST* pP2PParamList) = 0;

		/**
		 * @brief 获得Server上的文件路径
		 * @param lpszLocalPath [out] 接收Server路径的字符串
		 * @return lpszLocalPath != NULL且Server路径非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetServerPath(LPTSTR lpszServerPath, int nCharCount) = 0;

		/**
		 * @brief 获得下载文件的本地保存路径
		 * @param lpszLocalPath [out] 接收下载文件的本地保存路径的字符串
		 * @return lpszLocalPath != NULL且下载文件的本地保存路径非空，
		 *         则返回TRUE；否则，返回FALSE
		 */
		virtual BOOL GetLocalPath(LPTSTR lpszLocalPath, int nCharCount) = 0;

		virtual BOOL GetTemporaryPath(LPTSTR lpszTemPath, int nCharCount) = 0;

		virtual BOOL GetSubstitutePath(LPTSTR lpszSubstitutePath, int nCharCount) = 0;

		virtual BOOL GetCommand(LPTSTR lpszCommand, int nCharCount) = 0;

		virtual BOOL GetDownloaderType(LPTSTR lpszDownloadType) = 0;
	};
	};