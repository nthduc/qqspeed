#ifndef __FILEOPERATOR_H_I__
#define __FILEOPERATOR_H_I__

#include "../teniointerface.h"
#include "fileoperatorstruct.h"

namespace Tenio
{
	class IFileEnum;
	class IFileOperator;
	
	TENIO_MAKE_TYPEID_MAP(IFileOperator, COMPONENTID_FILEOPERATOR);
	
	class IFileEnum
	{
	public:
		// 枚举文件, 如果返回TRUE, 表示继续进行枚举, 否则停止枚举
		virtual BOOL OnFileEnum(LPCTSTR lpszFullFilePath, LPCTSTR lpszFileName) = 0;
	};

	/**
	 * @ingroup system
	 * @brief 文件操作模块
	 *
	 * <p>
	 *　　 用于文件以及文件夹相关操作。
	 * </p>
	 */
	class IFileOperator : public IComponent
	{
	public:
		/**
		 *  @brief 创建一个目录, 如果该目录的父目录还没有创建, 则先创建父目录
		 *  @param lpszDirectory 目录路径
		 *  @return 如果创建成功, 返回 TRUE
		 */
		virtual BOOL CreateDirectory(LPCTSTR lpszDirectory) = 0;

		/**
		 *  @brief 生成带固定前缀的文件或者目录名称. 例如 "新建文件夹1" "新建文件夹2"
		 *         此函数可以生成在 lpszParentFolder 中查找带有"新建文件夹"前缀的文件或者
		 *         文件夹, 生成一个不会重复的, 由 "前缀"+数字的名字
		 *  @param lpszParentFolder 上级文件夹路径
		 *  @param lpszPrefix 名称前缀
		 *  @param lpszResult 生成的名称
		 *  @param nCharCount 存放lpszResult的Buffer字符长度
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL GetNewSerialFolderName(LPCTSTR lpszParentFolder, 
			LPCTSTR lpszPrefix, LPTSTR lpszResult, int nCharCount) = 0;

		/**
		 *  @brief 修改文件或者文件夹的名字
		 *  @param lpszSourceName 原文件名
		 *  @param lpszChangeTo 修改的文件名
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL ChangeName(LPCTSTR lpszSourceName, LPCTSTR lpszChangeTo) = 0;

		/**
		 *  @brief 把lpszSourcePath原相对于lpszSourceRoot的路径变换为相对lpszDestRoot
		 *         的路径
		 *  @param lpszSourcePath 原文件路径
		 *  @param lpszSourceRoot 原相对的根路径
		 *  @param lpszDestRoot 变换后相对的根路径
		 *  @param lpszResult 变换后的路径
		 *  @param nReaultCharCount 存放lpszResult的Buffer字符长度
		 *  @param lpszRelative 相对路径
		 *  @param nRelativeCharCount 存放lpszRelative的Buffer字符长度
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL TransferRelativePath(LPCTSTR lpszSourcePath, LPCTSTR lpszSourceRoot,
			LPCTSTR lpszDestRoot, LPTSTR lpszResult, int nReaultCharCount, 
			LPTSTR lpszRelative, int nRelativeCharCount) = 0;

		/**
		 *  @brief 拷贝文件, 如果目的文件所在目录不存在, 则建立
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL CopyFile(LPCTSTR lpszSourceFile, LPCTSTR lpszDestFile) = 0;

		/**
		 *  @brief  拷贝文件到指定目录中
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL CopyFileToFolder(LPCTSTR lpszSourceFile, LPCTSTR lpszDestFolder) = 0;

		/**
		 *  @brief  删除文件夹(包括子目录下所有文件)
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL DeleteDirectory(LPCTSTR lpszDirectoryPath) = 0;

		/**
		 *  @brief  移动文件夹到目的文件下
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL MoveDirectory(LPCTSTR lpszSourceFolder, LPCTSTR lpszParentFolder) = 0;
		
		/**
		 *  @brief  复制文件夹到目的文件下
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL CopyDirectory(LPCTSTR lpszSourceFolder, LPCTSTR lpszParentFolder) = 0;

		/**
		 *  @brief 枚举符合 lpszFileFilter 的所有文件和文件夹，如果 pFiles = NULL 或者
		 *         nCount <= 0, 本函数返回该目录下所有文件/文件夹的总个数.
		 *         lpszFileFilter 例子: "C:\\Data\\*.xml" "C:\\*"
		 *  @param lpszFileFilter 通配符描述
		 *  @param pFiles 符合条件的文件列表
         *  @param nCount 文件列表大小
		 *  @param bGetMD5Hash 是否计算文件的MD5值
		 *  @return 枚举的文件夹以及文件个数
		 */
		virtual int EnumFileAndDirectory(LPCTSTR lpszFileFilter, LISTFILEINFO* pFiles, 
			int nCount, BOOL bGetMD5Hash) = 0;

		/**
		 *  @brief 把指定目录下的符合通配符描述的一批文件复制到目的文件夹中.
		 *         如: CopyBatchFiles("C:\\Windows\\*.ini", "D:\\INIBackup", FALSE)
		 *         l上面的语句复制 C:\Windows 目录下所有的 ini 文件. 
		 *  @param lpszFileFilter 通配符描述
		 *  @param lpszDestFolder 目的文件夹
		 *  @param bCopyIncludeSubfolders 表示是否复制目录下的子目录
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL CopyBatchFiles(LPCTSTR lpszFileFilter, LPCTSTR lpszDestFolder,
			BOOL bCopyIncludeSubfolders) = 0;
	
		/**
		 *  @brief  对比两个文件是否相等(二进制比较)
		 *  @return 如果相等, 返回 TRUE
		 */
		virtual BOOL IsTwoFilesTheSame(LPCTSTR lpszLeftFile, LPCTSTR lpszRightFile) = 0;

		/**
		 *  @brief 枚举文件
		 *  @param lpszFolder 文件夹的绝对路径, 要求枚举这个文件夹里的文件 
		 *  @param lpszFilter 过滤条件, 可以由 * ? 通配符组成, 如 "*.cpp" "*.h"
		 *  @param bIncludeSubFolder 是否递归搜索指定文件夹下面的子目录
		 *  @param pFileEnum 每找到一个符合要求的文件, 就调用 pFileEnum 指针内的 OnFileEnum 函数
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL EnumFile(LPCTSTR lpszFolder, LPCTSTR lpszFilter, 
			BOOL bIncludeSubFolder, IFileEnum* pFileEnum) = 0;

		/**
		 *  @brief 获取文件的长度
		 *  @return 文件长度
		 */
		virtual DWORD GetFileSize(LPCTSTR lpszFile) = 0;

		/**
		 *  @brief 检查 lpszFile 是否拥有只读属性
		 *  @return 如果是只读文件, 返回 TRUE
		 */
		virtual BOOL IsFileReadOnly(LPCTSTR lpszFile) = 0;

		/**
		 *  @brief 读文件
		 *  @return 读取文件长度
		 */
		virtual DWORD ReadFromFile(LPTSTR lpszBuffer, int nCharCount, LPCTSTR lpszFile) = 0;
		/**
		 *  @brief 写文件
		 *  @param bAppendMode 是否在文件末尾追加内容
		 *  @return 写入文件长度
		 */
		virtual DWORD WriteToFile(LPCTSTR lpszContent, int nCharCount, LPCTSTR lpszFile, BOOL bAppendMode) = 0;
	};
};

#endif