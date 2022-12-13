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
	 * @brief INI�ļ�����ģ��
	 *
	 * <p>
	 *���� ���ڶ�ȡ��д�롢ɾ��INI�ļ���С���Լ��ؼ������ݵȲ�����
	 * </p>
	 * <p>
	 * ʹ�����̣�
	 * <ol>
	 *   <li> ʹ��LoadIniSection()������Ҫ�������ļ�</li>
	 *   <li>SetCryptMode()��SetCacheMode() ���ò���ģʽ</li>
	 *   <li>ִ����ز���</li>
	 *   <li>���������Cacheģʽ��ʹ��SaveINIFile()����������</li>
	 * </ol>
	 * </p>
	 */
	class IINIFileStorage : public IComponent
	{
	public:
		/**
		 *  @brief �����ļ��Զ��� INI �ļ���ĳ��������
		 *  @param lpszIniFile �ļ�·���������Ǿ���·���������·��
		 *  @param lpszSectionName �� INI �ļ��нڵ�����
		 *  @return ������óɹ�, ���� TRUE
		 */
		virtual BOOL LoadIniSection(LPCTSTR lpszIniFile, LPCTSTR lpszSectionName) = 0;
	
		/**
		 *  @brief �趨�����ļ��Ƿ�ʹ�ü���ģʽ(ֻʹ�÷ǳ��򵥵ļ���ģʽ)
		 */
		virtual void SetCryptMode(BOOL bEnable) = 0;

		/**
		 *  @brief ���ú����ļ������Ƿ�ʹ��Cacheģʽ.
		 *         �����Cache��ʽ, ִֻ�ж�ȡ����һ��. Ĭ�Ϲر� Cache ��ʽ
		 */
		virtual void SetCacheMode(BOOL bEnable) = 0;
		
		/**
		 *  @brief ��ǰ�Ƿ���Cacheģʽ
		 */
		virtual BOOL IsCurrentInCacheMode() = 0;

		/**
		 *  @brief �����ļ� Cacheģʽ����Ч.
		 */
		virtual BOOL SaveINIFile() = 0;

		virtual BOOL ReadInt(LPCTSTR lpszKeyName, int* pnValue, int nDefaultValue) = 0;
		virtual BOOL ReadDword(LPCTSTR lpszKeyName, DWORD* pdwValue, DWORD dwDefaultValue) = 0;
		virtual BOOL ReadString(LPCTSTR lpszKeyName, LPTSTR lpszValue, int nCharCount, LPCTSTR lpszDefault) = 0;

		virtual BOOL WriteInt(LPCTSTR lpszKeyName, int nValue) = 0;
		virtual BOOL WriteDword(LPCTSTR lpszKeyName, DWORD dwValue) = 0;
		virtual BOOL WriteString(LPCTSTR lpszKeyName, LPCTSTR lpszValue) = 0;

		/**
		 *  @brief ɾ��INI�ļ���һ��С��
		 *  @param lpszIniFile �ļ�·���������Ǿ���·���������·��.
		 *         (�����Cacheģʽ����IniFile���������lpszFile������Ч����ΪNULL.)
		 *  @param lpszSectionName �� INI �ļ��нڵ�����
		 *  @param bDeleteSectionName �Ƿ�ɾ��С�ڱ��⣬TRUE��ɾ������
		 *  @return ����ɹ�, ���� TRUE
		 */
		virtual BOOL DeleteSection(LPCTSTR lpszFile, LPCTSTR lpszSectionName, 
			BOOL bDeleteSectionName) = 0;

		/**
		 *  @brief ɾ��С���е�Key
		 */
		virtual BOOL DeleteKey(LPCTSTR lpszFile, LPCTSTR lpszSectionName, 
			LPCTSTR lpszKeyName) = 0;
	
		/**
		 *  @brief �� lpszSourceIniFile ��С�ڱ��ݵ� lpszDestIniFile ��
		 *         ��� lpszDestIniFile �Ѿ������С��, �ڸ�С��ԭ�������ݱ�ɾ��.
		 */
		virtual BOOL BackupSection(LPCTSTR lpszSourceIniFile, LPCTSTR lpszSrcSection, 
			LPCTSTR lpszDestIniFile, LPCTSTR lpszDestSection) = 0;
		
		/**
		 *  @brief ��Cacheģʽ�°� lpszSourceIniFile ��С�ڱ��ݵ� lpszDestIniFile ��
		 *         ��� lpszDestIniFile �Ѿ������С��, �ڸ�С��ԭ�������ݱ�ɾ��.
		 *         ��ҪSaveINIFile()ʹ������Ч
		 *  @see SaveINIFile()
		 */
		virtual BOOL BackupSectionInCacheMode(LPCTSTR lpszSrcSection, 
			LPCTSTR lpszDestSection) = 0; 

		/**
		 *  @brief ö��ָ����С������
		 *  @return ���pKeyInfos==NULL����nCount<=0, ����С����Key�ĸ���\n
		 *          ��� pKeyInfos!=NULL����nCount>0, ����ʵ���Ͽ����� pKeyInfos �е�С�ڸ���
		 */
		virtual int EnumKeys(LPCTSTR lpszIniFile, LPCTSTR lpszSectionName,
			INIKEYINFO* pKeyInfos, int nCount) = 0;

		/**
		 *  @brief ö���ļ��е�С��
		 *  @return ���pszSection==NULL����nCount<=0������С�ڸ���\n
		 *          ���pszSection!=NULL����nCount>0������ʵ�ʿ�����pszSection��С�ڸ���
		 */
		virtual int EnumSections(LPCTSTR lpszIniFile, SECTIONINFO* pstSectionInfos, int nCount) = 0;

		/**
		 *  @brief �Ƚ�����С�ڵ������Ƿ�һ��
		 */
		virtual BOOL IsTwoSectionTheSame(LPCTSTR lpszLeftIni, LPCTSTR lpszLeftSectionName,
			LPCTSTR lpszRightIni, LPCTSTR lpszRightSectionName) = 0;
	};
};

#endif