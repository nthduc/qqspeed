#ifndef __LAYOUTDEFINE_H__
#define __LAYOUTDEFINE_H__

namespace Tenio
{
#define MAX_NAME_LENGTH		64
#define MAX_VALUE_LENGTH	1024
	
//////////////////////////////////////////////////////////////////////////
//定义配置文件名与路径
//////////////////////////////////////////////////////////////////////////
#define CONFIG_FILE_FOLDER		_T("config/")
#define LAYOUT_FILE_FOLDER		_T("config/layout/")
#define SCRIPT_FILE_FOLDER		_T("config/luascript/")
#define STYLE_FILE_FOLDER		_T("config/style/")
#define UICLASS_NAME2ID_FILE	_T("name2id.xml")
#define UICLASS_CONFIG_FILE		_T("uiclass.xml")
#define UIEVENT_CONFIG_FILE		_T("uievent.xml")
#define PROPERTYCLASS_CONFIG_FILE	_T("property.xml")
#define EDITOR_CONFIG_INIFILE	_T("Editor.ini")
//////////////////////////////////////////////////////////////////////////
// 定义xml文件元素名
//////////////////////////////////////////////////////////////////////////
// layout.xml
#define XMLEN_LAYOUT_ROOT	_T("TenioUILayout")
#define XMLEN_PROPERTIES	_T("Properties")
#define XMLEN_CHILDREN		_T("Children")
#define XMLEN_UIOBJECT		_T("UIObject")
#define XMLEN_PROPERTY		_T("Property")

// uiclass.xml
#define XMLEN_UICLASS_ROOT	_T("UIClassConfig")
#define XMLEN_UICLASS		_T("UIClass")

// uievent.xml
#define XMLEN_UIEVENT_ROOT	_T("UIEventConfig")
#define XMLEN_UIEVENT		_T("UIEvent")

// property.xml
#define XMLEN_PROPERTYCLASS_ROOT	_T("PropertyManager")
#define XMLEN_PROPERTYCLASS			_T("PropertyClass")

// style.xml
#define XMLEN_STYLE_ROOT	_T("UIStyles")
#define XMLEN_STYLE			_T("Style")
#define XMLEN_ITEM			_T("Item")

// name2id.xml
#define XMLEN_COMPONENTID_ROOT	_T("UIComponentID")
//////////////////////////////////////////////////////////////////////////
// 定义xml文件属性名
//////////////////////////////////////////////////////////////////////////
#define XMLAN_NAME			_T("Name")
#define XMLAN_TYPE			_T("Type")
#define XMLAN_SCRIPTFILE	_T("ScriptFile")
#define XMLAN_VALUE			_T("Value")
#define XMLAN_ID			_T("ID")
#define XMLAN_STYLEPATH		_T("StylePath")

#define XMLAN_DEFAULTVALUE	_T("DefaultValue")
#define XMLAN_VALUETYPE		_T("ValueType")
#define XMLAN_ISSTYLE		_T("IsStyle")

//////////////////////////////////////////////////////////////////////////
// 定义name2id.ini里的key
//////////////////////////////////////////////////////////////////////////
//#define INI_KEY_ID			_T("ID")

//////////////////////////////////////////////////////////////////////////
// editor.ini
#define INI_SECTION_CONFIG	_T("config")
#define INI_KEY_RESFOLDER	_T("resfolder")
#define INI_KEY_STYLEFOLDER	_T("stylefolder")



}
#endif