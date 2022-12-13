#ifndef __INIFILESTORAGE_I_H__
#define __INIFILESTORAGE_I_H__

#include "../teniointerface.h"
#include "inistructs.h"

namespace Tenio
{
	class IINIFileStorage;

	TENIO_MAKE_TYPEID_MAP(IINIFileStorage, COMPONENTID_INIFILESTORAGE);

	/**
	 * @ingroup system
	 * @brief INI文件操作模块
	 *
	 * <p>
	 *　　 用于读取、写入、删除INI文件中小节以及关键字内容等操作。
	 * </p>
	 * <p>
	 * 使用流程：
	 * <ol>
	 *   <li> 使用LoadIniSection()加载需要操作的文件</li>
	 *   <li>SetCryptMode()、SetCacheMode() 设置操作模式</li>
	 *   <li>执行相关操作</li>
	 *   <li>如果设置了Cache模式，使用SaveINIFile()保存操作结果</li>
	 * </ol>
	 * </p>
	 */
	class IINIFileStorage : public IComponent
	{
	public:
		/**
		 *  @brief 加载文件以读入 INI 文件的某个节内容
		 *  @param lpszIniFile 文件路径，可以是绝对路径或者相对路径
		 *  @param lpszSectionName 是 INI 文件中节的名字
		 *  @return 如果设置成功, 返回 TRUE
		 */
		virtual BOOL LoadIniSection(LPCTSTR lpszIniFile, LPCTSTR lpszSectionName) = 0;
	
		/**
		 *  @brief 设定配置文件是否使用加密模式(只使用非常简单的加密模式)
		 */
		virtual void SetCryptMode(BOOL bEnable) = 0;

		/**
		 *  @brief 设置后续文件操作是否使用Cache模式.
		 *         如果打开Cache方式, 只执行读取操作一次. 默认关闭 Cache 方式
		 */
		virtual void SetCacheMode(BOOL bEnable) = 0;
		
		/**
		 *  @brief 当前是否处于Cache模式
		 */
		virtual BOOL IsCurrentInCacheMode() = 0;

		/**
		 *  @brief 保存文件 Cache模式才有效.
		 */
		virtual BOOL SaveINIFile() = 0;

		virtual BOOL ReadInt(LPCTSTR lpszKeyName, int* pnValue, int nDefaultValue) = 0;
		virtual BOOL ReadDword(LPCTSTR lpszKeyName, DWORD* pdwValue, DWORD dwDefaultValue) = 0;
		virtual BOOL ReadString(LPCTSTR lpszKeyName, LPTSTR lpszValue, int nCharCount, LPCTSTR lpszDefault) = 0;

		virtual BOOL WriteInt(LPCTSTR lpszKeyName, int nValue) = 0;
		virtual BOOL WriteDword(LPCTSTR lpszKeyName, DWORD dwValue) = 0;
		virtual BOOL WriteString(LPCTSTR lpszKeyName, LPCTSTR lpszValue) = 0;

		/**
		 *  @brief 删除INI文件的一个小节
		 *  @param lpszIniFile 文件路径，可以是绝对路径或者相对路径.
		 *         (如果打开Cache模式加载IniFile，则这里的lpszFile参数无效，可为NULL.)
		 *  @param lpszSectionName 是 INI 文件中节的名字
		 *  @param bDeleteSectionName 是否删除小节标题，TRUE则删除标题
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL DeleteSection(LPCTSTR lpszFile, LPCTSTR lpszSectionName, 
			BOOL bDeleteSectionName) = 0;

		/**
		 *  @brief 删除小节中的Key
		 */
		virtual BOOL DeleteKey(LPCTSTR lpszFile, LPCTSTR lpszSectionName, 
			LPCTSTR lpszKeyName) = 0;
	
		/**
		 *  @brief 把 lpszSourceIniFile 的小节备份到 lpszDestIniFile 中
		 *         如果 lpszDestIniFile 已经有这个小节, 在该小节原来的内容被删除.
		 */
		virtual BOOL BackupSection(LPCTSTR lpszSourceIniFile, LPCTSTR lpszSrcSection, 
			LPCTSTR lpszDestIniFile, LPCTSTR lpszDestSection) = 0;
		
		/**
		 *  @brief 在Cache模式下把 lpszSourceIniFile 的小节备份到 lpszDestIniFile 中
		 *         如果 lpszDestIniFile 已经有这个小节, 在该小节原来的内容被删除.
		 *         需要SaveINIFile()使备份生效
		 *  @see SaveINIFile()
		 */
		virtual BOOL BackupSectionInCacheMode(LPCTSTR lpszSrcSection, 
			LPCTSTR lpszDestSection) = 0; 

		/**
		 *  @brief 枚举指定的小节内容
		 *  @return 如果pKeyInfos==NULL或者nCount<=0, 返回小节内Key的个数\n
		 *          如果 pKeyInfos!=NULL而且nCount>0, 返回实际上拷贝到 pKeyInfos 中的小节个数
		 */
		virtual int EnumKeys(LPCTSTR lpszIniFile, LPCTSTR lpszSectionName,
			INIKEYINFO* pKeyInfos, int nCount) = 0;

		/**
		 *  @brief 枚举文件中的小节
		 *  @return 如果pszSection==NULL或者nCount<=0，返回小节个数\n
		 *          如果pszSection!=NULL而且nCount>0，返回实际拷贝到pszSection中小节个数
		 */
		virtual int EnumSections(LPCTSTR lpszIniFile, SECTIONINFO* pstSectionInfos, int nCount) = 0;

		/**
		 *  @brief 比较两个小节的内容是否一样
		 */
		virtual BOOL IsTwoSectionTheSame(LPCTSTR lpszLeftIni, LPCTSTR lpszLeftSectionName,
			LPCTSTR lpszRightIni, LPCTSTR lpszRightSectionName) = 0;
	};
};

#endif