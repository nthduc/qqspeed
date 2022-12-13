#ifndef __XMLPARSER_I__
#define __XMLPARSER_I__

#include "../component_i.h"
#include "../objecttypeidmap.h"
#include "xmlparserstructs.h"

namespace Tenio
{
	class IXMLParser;

	TENIO_MAKE_TYPEID_MAP(IXMLParser, COMPONENTID_XMLPARSER)
		
	class IXMLParser : public IComponent
	{
	public:
		// Navigate
		virtual BOOL Load(LPCTSTR lpszFileName) = 0;
		virtual BOOL SetDoc(LPCTSTR lpszDoc) = 0;
		virtual BOOL IsWellFormed() = 0;
		virtual BOOL FindElem(LPCTSTR lpszName) = 0;
		virtual BOOL FindChildElem(LPCTSTR lpszName) = 0;
		virtual BOOL IntoElem() = 0;
		virtual BOOL OutOfElem() = 0;
		virtual void ResetChildPos() = 0;
		virtual void ResetMainPos() = 0;
		virtual void ResetPos() = 0;
		virtual BOOL GetTagName(LPTSTR lpszName, int nCharCount) = 0;
		virtual BOOL GetChildTagName(LPTSTR lpszName, int nCharCount) = 0;
		virtual BOOL GetData(LPTSTR lpszData, int nCharCount) = 0;
		virtual BOOL GetChildData(LPTSTR lpszName, int nCharCount) = 0;
		virtual BOOL GetElemContent(LPTSTR lpszElem, int nCharCount) = 0;
		virtual BOOL GetAttrib(LPCTSTR lpszAttribName, LPTSTR lpszAttrib, int nCharCount) = 0;
		virtual BOOL GetChildAttrib(LPCTSTR lpszAttribName, LPTSTR lpszAttrib, int nCharCount) = 0;
		virtual BOOL GetAttribName(int n, LPTSTR lpszAttribName, int nCharCount) = 0;
		virtual int  FindNode(int nType) = 0;
		virtual int  GetNodeType() = 0;
		virtual BOOL SavePos(LPCTSTR lpszPosName) = 0;
		virtual BOOL RestorePos(LPCTSTR lpszPosName) = 0;
		virtual int  GetDocFlags() = 0; 
		virtual void SetDocFlags(int nFlags) = 0;

		virtual BOOL Save(LPCTSTR lpszFileName) = 0;
		virtual void GetDoc(LPTSTR lpszFileName, int nCharCount) = 0;
		virtual BOOL AddElem(LPCTSTR lpszName, LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL InsertElem(LPCTSTR lpszName,LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL AddChildElem(LPCTSTR lpszName,LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL InsertChildElem(LPCTSTR lpszName,LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL AddElem(LPCTSTR lpszName, int nValue, int nFlags ) = 0;
		virtual BOOL InsertElem(LPCTSTR lpszName, int nValue, int nFlags) = 0;
		virtual BOOL AddChildElem(LPCTSTR lpszName, int nValue, int nFlags) = 0;
		virtual BOOL InsertChildElem(LPCTSTR lpszName, int nValue, int nFlags) = 0;
		virtual BOOL AddAttrib(LPCTSTR lpszAttrib,LPCTSTR lpszValue) = 0;
		virtual BOOL AddChildAttrib(LPCTSTR lpszAttrib,LPCTSTR lpszValue) = 0;
		virtual BOOL AddAttrib(LPCTSTR lpszAttrib, int nValue) = 0;
		virtual BOOL AddChildAttrib(LPCTSTR lpszAttrib, int nValue) = 0;
		virtual BOOL AddSubDoc(LPCTSTR lpszSubDoc) = 0;
		virtual BOOL InsertSubDoc(LPCTSTR lpszSubDoc) = 0;
		virtual void GetSubDoc(LPTSTR lpszSubDoc, int nCharCount) = 0;
		virtual BOOL AddChildSubDoc(LPCTSTR lpszSubDoc) = 0;
		virtual BOOL InsertChildSubDoc(LPCTSTR lpszSubDoc) = 0;
		virtual void GetChildSubDoc(LPTSTR lpszChildSubDoc, int nCharCount) = 0;
		virtual BOOL AddNode(int nType, LPCTSTR lpszText) = 0;
		virtual BOOL InsertNode(int nType, LPCTSTR lpszText) = 0;
		
		// Modify
		virtual BOOL RemoveElem() = 0;
		virtual BOOL RemoveChildElem() = 0;
		virtual BOOL RemoveNode() = 0;
		virtual BOOL SetAttrib(LPCTSTR lpszAttrib,LPCTSTR lpszValue) = 0;
		virtual BOOL SetChildAttrib(LPCTSTR lpszAttrib,LPCTSTR lpszValue) = 0;
		virtual BOOL SetAttrib(LPCTSTR lpszAttrib, int nValue) = 0;
		virtual BOOL SetChildAttrib(LPCTSTR lpszAttrib, int nValue) = 0;
		virtual BOOL SetData(LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL SetChildData(LPCTSTR lpszData, int nFlags) = 0;
		virtual BOOL SetData(int nValue) = 0;
		virtual BOOL SetChildData(int nValue) = 0;
		virtual BOOL SetElemContent(LPCTSTR lpszContent) = 0;		
	};
};

#endif