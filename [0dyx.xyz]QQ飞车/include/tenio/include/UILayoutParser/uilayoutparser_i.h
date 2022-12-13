/********************************************************************
　created         : 2006/5/24
　filename        : uilayoutparser_i.h
　author          : season
　version         : 1.0
　description     : 读入xml，分析它并生成UI对象结构，提供接口获取单个UI对象。
　history         :
　1 day	          : 2006/5/24
　　author        : season
　　modification: : created	
*********************************************************************/
#ifndef __UILAYOUTPARSER_I_H__
#define __UILAYOUTPARSER_I_H__

#include "../teniointerface.h"
#include "../uilib/uilib_i.h"

namespace Tenio
{
	class IUILayoutParser;
	class IUILayoutParserEvent;
	TENIO_MAKE_TYPEID_MAP(IUILayoutParser, COMPONENTID_UILAYOUTPARSER)
	TENIO_MAKE_EVENT_MAP(IUILayoutParser, IUILayoutParserEvent)

	//////////////////////////////////////////////////////////////////////////
	class IUILayoutParser : 
		public IComponent,
		public IEventProducer
	{
	public:
		virtual BOOL ParserXMLFile(LPCTSTR lpszXMLFileName) = 0;
		virtual BOOL ParserXMLPath(DWORD dwParent, LPCTSTR lpszXMLPath) = 0;
	    virtual BOOL CreateCompositeControlFromXML(DWORD dwParent, LPCTSTR lpszXMLFileName) = 0;
		virtual BOOL SaveToFile(LPCTSTR lpszXMLFileName) = 0;
		virtual void SetUILibHandle(IUILib* pUILib) = 0;
		virtual void ParserProperty(
			LPCTSTR lpszPropertyName, 
			LPCTSTR lpszPropertyValue, 
			IVariableParam* pParam)= 0;

		virtual BOOL AddStart() = 0;
		virtual void AddEnd() = 0;
		virtual void IntoUIObject() = 0;
		virtual void OutofUIObject() = 0;
		virtual void ChildrenBegin() = 0;
		virtual void ChildrenEnd() = 0;
		virtual void PropertiesBegin(LPCTSTR lpszStylePath) = 0;
		virtual void PropertiesEnd() = 0;
		virtual void AddOneUIObject(LPCTSTR lpszTypeName, LPCTSTR lpszName, LPCTSTR lpszScriptFile) = 0;
		virtual void AddOneProperty(LPCTSTR lpszPropertyName, LPCTSTR lpszPropertyValue) = 0;
	};

	class IUILayoutParserEvent : public IEvent
	{
	public:
		virtual void OnParserUIObject(
			LPCTSTR lpszParentName, 
			LPCTSTR lpszClassName, 
			LPCTSTR lpszUIObjectName,
			LPCTSTR lpszScriptFile) = 0;
		virtual void OnParserProperty(
			LPCTSTR lpszUIObjectName,
			LPCTSTR lpszPropertyName,
			LPCTSTR lpszPropertyValue) = 0;
		virtual void OnParserStylePath(
			LPCTSTR lpszUIObjectName, 
			LPCTSTR lpszStylePath) = 0;
		virtual void OnParserPropertiesEnd(LPCTSTR lpszUIObjectName) = 0;
	};
}

#endif