#pragma once

namespace Tenio
{
	// ���ڡ��ⲿ���õ����ݽṹ����

	// ģ��汾��Ϣ
	typedef struct tagModuleVersionInfo
	{
		tstring strModuleID; // ģ���Ψһ��־���ڲ�ʹ��
		tstring strModuleName; // ģ����������ƣ�������ʾ
		DWORD dwVersion;  // ���汾��
		DWORD dwPatch;		// �Ѱ�װ���SP��
		DWORD dwKB;  // �Ѵ���󲹶��� 
	}MODULEVERSIONINFO;

	// �ͻ��˰汾��Ϣ
	typedef MODULEVERSIONINFO CLIENTVERSIONINFO;


	
}