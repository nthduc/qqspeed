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
		// 将property字符串表达的值写入param，简单和复合都可以适用
		virtual void SetPropertyToParam(
			LPCTSTR lpszPropertyName, 
			LPCTSTR lpszPropertyValue, 
			IVariableParam* pParam) = 0;

		// 将复合property转换为字符串
		virtual void SetParamToPropertyString(
			ENMVALUETYPE eValueType, 
			IVariableParam* pParam,
			LPTSTR lpszPropertyValue,
			UINT nMaxLen) = 0;
		
		// 简单property值与字符串的相互转换
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
