
#include "gameui.h"
#include "script.h"
#include "widget.h"
#include "variant.h"
#include "uicontrol.h"
#include "uiimage.h"


CUIWidget *LoadUIWidget( const char *pname, CVar *pvar, CUIWidget *pfather );


int  GameUICallSafeScriptEx(short funcid,const char *fmt, ...)
{
	if(g_pRootWgt&&g_pRootWgt->GetCurScreen())
	{
		static char	msg[4096];
		
		va_list	argptr;
		va_start(argptr,fmt);
		vsprintf(msg,fmt,argptr);
		va_end(argptr);

		return g_pRootWgt->GetCurScreen()->CallSafeScript(funcid,msg);	
	}

	return -1;

}

void RegisterUICallBack(UICallbackEvent eventName,CallBackFunc pFunc)
{
	g_vecCallbackFuncs[eventName]=pFunc;
}


bool CreateGameUI( const char *uires, int w, int h, CScriptState *pscript )
{
	

	
	//检查内存泄漏
	if(!pscript) return false;

	
	//读入库变量脚本文件
	if( pscript->DoFile( uires ) == -2 ) return false;

	CVarString uiconfig("UI_CONFIG_FILE");
	uiconfig.GetFromScript( pscript );
	
	CVarString uiroot("UI_ROOT_FILE");
	uiroot.GetFromScript( pscript );

	CVarString uiname("UI_ROOT_NAME");
	uiname.GetFromScript( pscript );
	
	
	//读入界面配置脚本文件
	if( pscript->DoFile(uiconfig.String()) == -2 ) return false;
	
	//读入根窗口脚本文件
	if( pscript->DoFile(uiroot.String()) == -2 ) return false;

	CVarTable alluitmp(uiname.String());
	alluitmp.GetFromScript( pscript );

	
	CVar* pVarIncludes = alluitmp.Element("includes");
	if ( pVarIncludes )
	{
		CVarArray* pArrayIncludes = (CVarArray*)pVarIncludes;
		for ( int i = 0; i < pArrayIncludes->GetNum(); i++ )
		{
			pscript->DoFile((*pArrayIncludes)[i]->String());
		}
	}
	
	//检查内存泄漏
	if(g_pRootWgt) 
		g_pRootWgt->Release();

	g_pRootWgt= new CUIRootWin();
	g_pRootWgt->ClearParameter();
	g_pRootWgt->SetWidgetID(uiname.String());
	g_pRootWgt->SetPosition(0, 0, w, h);	
	g_pRootWgt->SetScriptEnv(pscript);

	//////////////////////载入界面组件////////////////////////////
	
	CVarTable allui(uiname.String());
	allui.GetFromScript( pscript );
	
	assert( allui.Element("children")->GetType() == VAR_TABLE );
	CVarTable *pitems = (CVarTable *)allui.Element("children");

	assert(pitems);

	if(!pitems)
		return false;

	//设置root的属性
	g_pRootWgt->SetParameter( (CVar*)(&allui), NULL );

	//载入根窗口的所有子窗口	
	const char *pchildname;
	CVar *pchildvar;
	pitems->BeginIterate();
	while( pitems->Iterate(pchildname, pchildvar) )
	{
		CUIWidget *pchild = LoadUIWidget( pchildname, pchildvar, g_pRootWgt );//在此设置child参数
	
		g_pRootWgt->AddRootChild( pchild );//添加到root		

		//检查内存泄漏
		if(pchild) 
			pchild->Release();
	}

		
	//设置root的属性
	g_pRootWgt->PostSetParameter( (CVar*)(&allui), NULL );

	//到此,root窗口算是载入完毕
	g_pRootWgt->OnLoadRecursive();
	g_pRootWgt->OnInitRecursive();
	
	return true;

}

void DestroyGameUI()
{
	if(g_pRootWgt)
	{
		g_pRootWgt->Release();
		g_pRootWgt = NULL;
	}
	
}

int  GameUIMessage( UINT msg, WPARAM wparam, LPARAM lparam )
{
	if(g_pRootWgt) return g_pRootWgt->OnWindowMessage( msg, wparam, lparam );
	else return 0;
}

static char s_CurScreen[UI_TEMPORARY_BUFSIZE] = "";
void GameUIGotoScreen( const char *idpath )
{
	strcpy( s_CurScreen, idpath );
	if(g_pRootWgt) g_pRootWgt->GotoScreen( idpath );

}

const char *GameUIGetCurScreen()
{
	return s_CurScreen;
}


int GameUIGetNumScreen()
{

	if(g_pRootWgt)
		return g_pRootWgt->GetNumScreen();
	else 
		return 0;

}

IWgtData* GameUIGetScreen(int i)
{

	if(g_pRootWgt)
		return g_pRootWgt->GetScreen(i);
	else 
		return NULL;

}

IWgtData* GameUIGetWidget(const char* idpath)
{
	if(g_pRootWgt)
		return g_pRootWgt->FindRootWidgetByID(std::string(idpath));
	else 
		return NULL;
}

BOOL GameUIShowWidget(const char* idpath, BOOL isshow)
{
	CUIWidget* pWgt = (CUIWidget*)GameUIGetWidget(idpath);
	if ( !pWgt || ( pWgt->IsVisible() == isshow) )
		return FALSE;
	pWgt->ShowWidget(isshow);
	return TRUE;
}


IWgtData* GameUIGetFocus()
{
	if(g_pRootWgt)
		return g_pRootWgt->GetFocusWgt();
	else 
		return NULL;
}

IWgtData* GameUIGetRoot()
{
	return g_pRootWgt;
}

bool GameUIWidgetExist(IWgtData* pWgt)
{
	return(g_vecSecuritySet.count(pWgt)>0);
}

void GameUIConsoleInfo( const char *fmt, ... )
{
	static char mybuffer[UI_DEFAULT_BUFSIZE];

	va_list		argptr;
	va_start( argptr, fmt );
	vsprintf( mybuffer, fmt, argptr );
	va_end( argptr );
	
	if(g_pRootWgt) g_pRootWgt->AddConsoleInfo( mybuffer );
}

void GameUIShowMsgBox( Msgbox_Cmd nCmdShow,const char* pText/*=""*/,const char* pCaption/*="Message"*/,int nMsgId/*=0*/)
{
	if(g_pRootWgt)
		g_pRootWgt->ShowMessageBox(nCmdShow,pText,pCaption,nMsgId);
		
}


void UpdateGameUI( float deltatime )
{
	if(g_pRootWgt) g_pRootWgt->UpdateRecursive( deltatime );
}

void RenderGameUI()
{
	if(g_pRootWgt) g_pRootWgt->RenderRecursive();

	UpdateUIImage();
}

void GameUISetCursor( const char *pname )
{
	if(g_pRootWgt) g_pRootWgt->SetCurCursor( pname );
}

void GameUISetImeState( bool b )
{
	if(g_pRootWgt)
	{
		CUIWidget* pFocusWgt=g_pRootWgt->GetFocusWgt();

		if(pFocusWgt)
		{
			pFocusWgt->EnableIme(b);
		}
	}

}

void GameUIShowCursor( bool b )
{
	CUICursor::m_bShow = b;
}


stUICoreStyle& GameUIGetCoreStyle()
{
	return g_UICoreStyle;
}


void GameUICancelDrag()
{
		
	g_pRootWgt->CancelDrag();

}

extern const char* UI_Type_Names[];

CUIWidget* CreateWidgetByType(const char* ptype)
{
	CUIWidget* pwidget=NULL;

	if( strcmp(ptype, UI_Type_Names[WGTTYPE_WIDGET]) == 0 )
	{
		pwidget = new CUIWidget;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_CONSOLE]) == 0 )
	{
		pwidget = new CUIConsole;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_LABEL]) == 0 )
	{
		pwidget = new CUILabel;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SCREEN]) == 0 )
	{
		pwidget = new CUIScreen;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_DIALOG]) == 0 )
	{
		pwidget = new CUIDialog;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_PICLABEL]) == 0 )
	{
		pwidget = new CUIPicLabel;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_BUTTON]) == 0 )
	{
		pwidget = new CUIButton;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_CHECK]) == 0 )
	{
		pwidget = new CUICheck;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_TABWIN]) == 0 )
	{
		pwidget = new CUITabWin;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_TABLIST]) == 0 )
	{
		pwidget = new CUITabList;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SPIN]) == 0 )
	{
		pwidget = new CUISpin;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_EDIT]) == 0 )
	{
		pwidget = new CUILineEdit;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_KEYEDIT]) == 0 )
	{
		pwidget = new CUIKeyEdit;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_PROGRESSBAR]) == 0 )
	{
		pwidget = new CUIProgressBar;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_CHANGEBAR]) == 0 )
	{
		pwidget = new CUIChangeBar;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SCROLLBAR]) == 0 )
	{
		pwidget = new CUIScrollBar;
	}	
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_LISTBOX]) == 0 )
	{
		pwidget = new CUIListBox;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_LISTBASE]) == 0 )
	{
		pwidget= new CUIListBase;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_LISTFILE]) == 0 )
	{
		pwidget= new CUIListFile;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_WIDGETLIST]) == 0 )
	{
		pwidget = new CUIWidgetList;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_CONTAINER]) == 0 )
	{
		pwidget = new CUIContainer;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SWITCHER]) == 0 )
	{
		pwidget = new CUISwitcher;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SHAPER]) == 0 )
	{
		pwidget = new CUIShaper;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_PICKER]) == 0 )
	{
		pwidget = new CUIPicker;
	}	
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_TEXTLIST]) == 0 )
	{
		pwidget = new CUITextList;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_TREECTRL]) == 0 )
	{
		pwidget = new CUITreeCtrl;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_RICHEDIT]) == 0 )
	{
		pwidget = new CUIRichEdit;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_MULTIEDIT]) == 0 )
	{
		pwidget = new CUIMultiEdit;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_COMBOBOX]) == 0 )
	{
		pwidget = new CUIComboBox;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_MESSAGEBOX]) == 0 )
	{
		pwidget = new CUIMessageBox;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_SYSDIALOG]) == 0 )
	{
		pwidget = new CUISystemDialog;
	}
	else if( strcmp(ptype, UI_Type_Names[WGTTYPE_MENU]) == 0 )
	{
		pwidget = new CUIMenu;		
	}

	if(pwidget) pwidget->ClearParameter();

	return pwidget;


}


CUIWidget * LoadUIWidget( const char *pname, CVar *pvar, CUIWidget *pfather )
{

	//检查内存泄漏
	if(!pvar)	return NULL;

	CVar* pTypeVar=pvar->Element("type");
	assert(pTypeVar);
	if(!pTypeVar) return NULL;

	const char *ptype = pTypeVar->String();
	
	CUIWidget *pwidget = CreateWidgetByType(ptype);
	

	//检查内存泄漏
	if(pwidget)
	{

		pwidget->SetWidgetID( pname );
		
		pwidget->SetParameter( pvar, pfather );		
		
		//检查所有子窗口
		CVar* pchildren=pvar->Element("children");
		if( pchildren )
		{
			assert( pchildren->GetType() == VAR_TABLE );
			CVarTable *pitems = (CVarTable *)pchildren;

			const char *pchildname;
			CVar *pchildvar;
			pitems->BeginIterate();
			while( pitems->Iterate(pchildname, pchildvar) )
			{
				CUIWidget *pchild = LoadUIWidget( pchildname, pchildvar, pwidget );//在此设置child参数
				
				pwidget->AddChild( pchild );// 在此设置child的m_pFather
				
				//检查内存泄漏
				if(pchild) pchild->Release();
			}
		}	

		pwidget->PostSetParameter( pvar, pfather );

	}
		

	return pwidget;

}



//载入一个界面屏幕
CUIWidget * CUIRootWin::LoadScreen(const char* id)
{
	
	if(!m_pScript) return NULL;

	if(strlen(id)<1)  return NULL;

	if(m_AllScreenDesc.count(std::string(id))<1)  return NULL;

	const stScreenDesc& scrnDesc=m_AllScreenDesc[std::string(id)];

	//指定了fuc文件
	for(int i=0;i<scrnDesc.luaPaths.size();i++)
	{
		const char* pfucpath=scrnDesc.luaPaths[i].c_str();
		if(strlen(pfucpath)>0)
		{
			if( m_pScript->DoFile(pfucpath)==-2 )	
				return NULL;
		}
	}
	
	//载入界面screen
	CVarTable allui(m_ID.c_str());
	allui.GetFromScript( m_pScript );
	
	assert( allui.Element("children")->GetType() == VAR_TABLE );
	CVarTable *pitems = (CVarTable *)allui.Element("children");
	
	if(pitems)
	{

		//检查根窗口的指定子屏幕
		CVar *pScreenVar=pitems->Element(id);
		if(pScreenVar)
		{
			CUIWidget *pScreen = LoadUIWidget( id, pScreenVar, this );
			
			assert(pScreen->GetType()==WGTTYPE_SCREEN);
			
			//重复添加屏幕对象的bug
			//AddScreen( pScreen );	
			//pScreen->Release();
		
			return pScreen;

		}

	}

	return NULL;


}
