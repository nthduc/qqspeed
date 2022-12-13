#ifndef __TEXTFILE_I_H__
#define __TEXTFILE_I_H__

#include "teniointerface.h"

namespace Tenio
{
	class ITextFile;

	TENIO_MAKE_TYPEID_MAP(ITextFile, COMPONENTID_TEXTFILE);

	/**
	 * @ingroup system
	 * @brief 文本文件操作模块
	 * <p>
	 * 使用流程：
	 * <ol>
	 *   <li> 使用 OpenFileForRead() 或 OpenFileForWrite() 打开文件</li>
	 *   <li> 执行相关操作</li>
	 *   <li> 使用 CloseFile() 关闭文件</li>
	 * </ol>
	 * </p>
	 */
	class ITextFile : public IComponent
	{
	public:
		/**
		 *  @brief 打开文件，用于后续读取操作
		 *  @param lpszRelativePath 文件路径，可以是绝对路径或者相对路径
		 *  @return 如果成功, 返回 TRUE
		 */
		virtual BOOL OpenFileForRead(LPCTSTR lpszRelativePath) = 0;

		/**
		*  @brief 打开文件，用于后续写入操作
		*  @param lpszRelativePath 文件路径，可以是绝对路径或者相对路径
		*  @param bAppendMode 是否采用在文件末尾追加内容的方式
		*  @return 如果成功, 返回 TRUE
		*/
		virtual BOOL OpenFileForWrite(LPCTSTR lpszRelativePath, BOOL bAppendMode) = 0;

		/**
		*  @brief 关闭文件
		*  @return 如果成功, 返回 TRUE
		*/
		virtual BOOL CloseFile() = 0;

		/**
		*  @brief 读入一行
		*  @param pbIsCompletelyRead 返回是否完整地读入了一行
		*  @return 返回 FALSE 表示遇到了文件末尾
		*/
		virtual BOOL ReadLine(LPTSTR lpszBuffer, int nCharCount, BOOL* pbIsCompletelyRead) = 0;

		/**
		*  @brief 获取下一行长度
		*  @return 获取长度
		*/
		virtual int  GetNextLineLength() = 0;
		
		/**
		*  @brief 在文件末尾写入一行
		*  @return 如果成功, 返回 TRUE
		*/
		virtual BOOL AppendLine(LPCTSTR lpszLine) = 0;
	
		/**
		*  @brief Format后在文件末尾写入一行
		*  @return 如果成功, 返回 TRUE
		*/
		virtual BOOL SmartAppend(LPCTSTR lpszFormat, ...) = 0;
		
		/**
		*  @brief 设置文件指针
		*/
		virtual DWORD SetTextFilePointer(LONG lDistanceToMove, DWORD dwMoveMethod) = 0;
	};
};

#endif