#ifndef __TEXTFILE_I_H__
#define __TEXTFILE_I_H__

#include "teniointerface.h"

namespace Tenio
{
	class ITextFile;

	TENIO_MAKE_TYPEID_MAP(ITextFile, COMPONENTID_TEXTFILE);

	/**
	 * @ingroup system
	 * @brief �ı��ļ�����ģ��
	 * <p>
	 * ʹ�����̣�
	 * <ol>
	 *   <li> ʹ�� OpenFileForRead() �� OpenFileForWrite() ���ļ�</li>
	 *   <li> ִ����ز���</li>
	 *   <li> ʹ�� CloseFile() �ر��ļ�</li>
	 * </ol>
	 * </p>
	 */
	class ITextFile : public IComponent
	{
	public:
		/**
		 *  @brief ���ļ������ں�����ȡ����
		 *  @param lpszRelativePath �ļ�·���������Ǿ���·���������·��
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL OpenFileForRead(LPCTSTR lpszRelativePath) = 0;

		/**
		*  @brief ���ļ������ں���д�����
		*  @param lpszRelativePath �ļ�·���������Ǿ���·���������·��
		*  @param bAppendMode �Ƿ�������ļ�ĩβ׷�����ݵķ�ʽ
		*  @return ����ɹ�, ���� TRUE
		*/
		virtual BOOL OpenFileForWrite(LPCTSTR lpszRelativePath, BOOL bAppendMode) = 0;

		/**
		*  @brief �ر��ļ�
		*  @return ����ɹ�, ���� TRUE
		*/
		virtual BOOL CloseFile() = 0;

		/**
		*  @brief ����һ��
		*  @param pbIsCompletelyRead �����Ƿ������ض�����һ��
		*  @return ���� FALSE ��ʾ�������ļ�ĩβ
		*/
		virtual BOOL ReadLine(LPTSTR lpszBuffer, int nCharCount, BOOL* pbIsCompletelyRead) = 0;

		/**
		*  @brief ��ȡ��һ�г���
		*  @return ��ȡ����
		*/
		virtual int  GetNextLineLength() = 0;
		
		/**
		*  @brief ���ļ�ĩβд��һ��
		*  @return ����ɹ�, ���� TRUE
		*/
		virtual BOOL AppendLine(LPCTSTR lpszLine) = 0;
	
		/**
		*  @brief Format�����ļ�ĩβд��һ��
		*  @return ����ɹ�, ���� TRUE
		*/
		virtual BOOL SmartAppend(LPCTSTR lpszFormat, ...) = 0;
		
		/**
		*  @brief �����ļ�ָ��
		*/
		virtual DWORD SetTextFilePointer(LONG lDistanceToMove, DWORD dwMoveMethod) = 0;
	};
};

#endif