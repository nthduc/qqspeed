#ifndef __UIPROPERTY_I_H__
#define __UIPROPERTY_I_H__

#include "../teniointerface.h"
#include "../uilib/variableparam_i.h"
#include "propertyid.h"

namespace Tenio
{
	class IUIProperty;

	TENIO_MAKE_TYPEID_MAP(IUIProperty, COMPONENTID_UIPROPERTY);

	class IUIProperty : public IComponent
	{
	public:
		// ��property�ַ�������ֵд��param���򵥺͸��϶���������
		virtual void SetPropertyToParam(
			LPCTSTR lpszPropertyName, 
			LPCTSTR lpszPropertyValue, 
			IVariableParam* pParam) = 0;

		// ������propertyת��Ϊ�ַ���
		virtual void SetParamToPropertyString(
			ENMVALUETYPE eValueType, 
			IVariableParam* pParam,
			LPTSTR lpszPropertyValue,
			UINT nMaxLen) = 0;
		
		// ��propertyֵ���ַ������໥ת��
		virtual int String2Int(LPCTSTR lpszValue) = 0;
		virtual float String2Float(LPCTSTR lpszValue) = 0;
		virtual BOOL String2Bool(LPCTSTR lpszValue) = 0;
		virtual DWORD String2Hex(LPCTSTR lpszValue) = 0;
		virtual void Int2String(int nValue, LPTSTR lpszValue, UINT nMaxLen) = 0;
		virtual void Float2String(float fValue, LPTSTR lpszValue, UINT nMaxLen) = 0;
		virtual void Bool2String(BOOL bValue, LPTSTR lpszValue, UINT nMaxLen) = 0;
		virtual void Hex2String(DWORD dwValue, LPTSTR lpszValue, UINT nMaxLen) = 0;
	};
};

#endif

// InterfaceName:UIProperty
// SubFolderName:uiproperty
