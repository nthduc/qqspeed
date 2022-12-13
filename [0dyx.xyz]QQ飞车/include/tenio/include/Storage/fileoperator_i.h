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
		// ö���ļ�, �������TRUE, ��ʾ��������ö��, ����ֹͣö��
		virtual BOOL OnFileEnum(LPCTSTR lpszFullFilePath, LPCTSTR lpszFileName) = 0;
	};

	/**
	 * @ingroup system
	 * @brief �ļ�����ģ��
	 *
	 * <p>
	 *���� �����ļ��Լ��ļ�����ز�����
	 * </p>
	 */
	class IFileOperator : public IComponent
	{
	public:
		/**
		 *  @brief ����һ��Ŀ¼, �����Ŀ¼�ĸ�Ŀ¼��û�д���, ���ȴ�����Ŀ¼
		 *  @param lpszDirectory Ŀ¼·��
		 *  @return ��������ɹ�, ���� TRUE
		 */
		virtual BOOL CreateDirectory(LPCTSTR lpszDirectory) = 0;

		/**
		 *  @brief ���ɴ��̶�ǰ׺���ļ�����Ŀ¼����. ���� "�½��ļ���1" "�½��ļ���2"
		 *         �˺������������� lpszParentFolder �в��Ҵ���"�½��ļ���"ǰ׺���ļ�����
		 *         �ļ���, ����һ�������ظ���, �� "ǰ׺"+���ֵ�����
		 *  @param lpszParentFolder �ϼ��ļ���·��
		 *  @param lpszPrefix ����ǰ׺
		 *  @param lpszResult ���ɵ�����
		 *  @param nCharCount ���lpszResult��Buffer�ַ�����
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL GetNewSerialFolderName(LPCTSTR lpszParentFolder, 
			LPCTSTR lpszPrefix, LPTSTR lpszResult, int nCharCount) = 0;

		/**
		 *  @brief �޸��ļ������ļ��е�����
		 *  @param lpszSourceName ԭ�ļ���
		 *  @param lpszChangeTo �޸ĵ��ļ���
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL ChangeName(LPCTSTR lpszSourceName, LPCTSTR lpszChangeTo) = 0;

		/**
		 *  @brief ��lpszSourcePathԭ�����lpszSourceRoot��·���任Ϊ���lpszDestRoot
		 *         ��·��
		 *  @param lpszSourcePath ԭ�ļ�·��
		 *  @param lpszSourceRoot ԭ��Եĸ�·��
		 *  @param lpszDestRoot �任����Եĸ�·��
		 *  @param lpszResult �任���·��
		 *  @param nReaultCharCount ���lpszResult��Buffer�ַ�����
		 *  @param lpszRelative ���·��
		 *  @param nRelativeCharCount ���lpszRelative��Buffer�ַ�����
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL TransferRelativePath(LPCTSTR lpszSourcePath, LPCTSTR lpszSourceRoot,
			LPCTSTR lpszDestRoot, LPTSTR lpszResult, int nReaultCharCount, 
			LPTSTR lpszRelative, int nRelativeCharCount) = 0;

		/**
		 *  @brief �����ļ�, ���Ŀ���ļ�����Ŀ¼������, ����
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL CopyFile(LPCTSTR lpszSourceFile, LPCTSTR lpszDestFile) = 0;

		/**
		 *  @brief  �����ļ���ָ��Ŀ¼��
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL CopyFileToFolder(LPCTSTR lpszSourceFile, LPCTSTR lpszDestFolder) = 0;

		/**
		 *  @brief  ɾ���ļ���(������Ŀ¼�������ļ�)
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL DeleteDirectory(LPCTSTR lpszDirectoryPath) = 0;

		/**
		 *  @brief  �ƶ��ļ��е�Ŀ���ļ���
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL MoveDirectory(LPCTSTR lpszSourceFolder, LPCTSTR lpszParentFolder) = 0;
		
		/**
		 *  @brief  �����ļ��е�Ŀ���ļ���
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL CopyDirectory(LPCTSTR lpszSourceFolder, LPCTSTR lpszParentFolder) = 0;

		/**
		 *  @brief ö�ٷ��� lpszFileFilter �������ļ����ļ��У���� pFiles = NULL ����
		 *         nCount <= 0, ���������ظ�Ŀ¼�������ļ�/�ļ��е��ܸ���.
		 *         lpszFileFilter ����: "C:\\Data\\*.xml" "C:\\*"
		 *  @param lpszFileFilter ͨ�������
		 *  @param pFiles �����������ļ��б�
         *  @param nCount �ļ��б��С
		 *  @param bGetMD5Hash �Ƿ�����ļ���MD5ֵ
		 *  @return ö�ٵ��ļ����Լ��ļ�����
		 */
		virtual int EnumFileAndDirectory(LPCTSTR lpszFileFilter, LISTFILEINFO* pFiles, 
			int nCount, BOOL bGetMD5Hash) = 0;

		/**
		 *  @brief ��ָ��Ŀ¼�µķ���ͨ���������һ���ļ����Ƶ�Ŀ���ļ�����.
		 *         ��: CopyBatchFiles("C:\\Windows\\*.ini", "D:\\INIBackup", FALSE)
		 *         l�������临�� C:\Windows Ŀ¼�����е� ini �ļ�. 
		 *  @param lpszFileFilter ͨ�������
		 *  @param lpszDestFolder Ŀ���ļ���
		 *  @param bCopyIncludeSubfolders ��ʾ�Ƿ���Ŀ¼�µ���Ŀ¼
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL CopyBatchFiles(LPCTSTR lpszFileFilter, LPCTSTR lpszDestFolder,
			BOOL bCopyIncludeSubfolders) = 0;
	
		/**
		 *  @brief  �Ա������ļ��Ƿ����(�����ƱȽ�)
		 *  @return ������, ���� TRUE
		 */
		virtual BOOL IsTwoFilesTheSame(LPCTSTR lpszLeftFile, LPCTSTR lpszRightFile) = 0;

		/**
		 *  @brief ö���ļ�
		 *  @param lpszFolder �ļ��еľ���·��, Ҫ��ö������ļ�������ļ� 
		 *  @param lpszFilter ��������, ������ * ? ͨ������, �� "*.cpp" "*.h"
		 *  @param bIncludeSubFolder �Ƿ�ݹ�����ָ���ļ����������Ŀ¼
		 *  @param pFileEnum ÿ�ҵ�һ������Ҫ����ļ�, �͵��� pFileEnum ָ���ڵ� OnFileEnum ����
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL EnumFile(LPCTSTR lpszFolder, LPCTSTR lpszFilter, 
			BOOL bIncludeSubFolder, IFileEnum* pFileEnum) = 0;

		/**
		 *  @brief ��ȡ�ļ��ĳ���
		 *  @return �ļ�����
		 */
		virtual DWORD GetFileSize(LPCTSTR lpszFile) = 0;

		/**
		 *  @brief ��� lpszFile �Ƿ�ӵ��ֻ������
		 *  @return �����ֻ���ļ�, ���� TRUE
		 */
		virtual BOOL IsFileReadOnly(LPCTSTR lpszFile) = 0;

		/**
		 *  @brief ���ļ�
		 *  @return ��ȡ�ļ�����
		 */
		virtual DWORD ReadFromFile(LPTSTR lpszBuffer, int nCharCount, LPCTSTR lpszFile) = 0;
		/**
		 *  @brief д�ļ�
		 *  @param bAppendMode �Ƿ����ļ�ĩβ׷������
		 *  @return д���ļ�����
		 */
		virtual DWORD WriteToFile(LPCTSTR lpszContent, int nCharCount, LPCTSTR lpszFile, BOOL bAppendMode) = 0;
	};
};

#endif