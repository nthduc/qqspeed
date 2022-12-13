
#include "script.h"

#include "gameui.h"

#include "uiimage.h"

#include "widget.h"

#include "uicontrol.h"

extern const char* UI_Type_Names[];

extern CUIWidget* CreateWidgetByType(const char* ptype);

//根据窗口路径名参数，查找窗口
//index为参数在lua stack中的索引
static CUIWidget *FindUIWidget( lua_State *ls, int index )
{
	std::string idpath( lua_tostring(ls, index) );
	
	if( idpath.empty() )
	{
		MessageBox(0,"widget path is empty","widget not found!",MB_OK);
		return NULL;
	}

	CUIWidget *pwidget = g_pRootWgt->FindRootWidgetByID( idpath );
	
	if(!pwidget) 
	{
		assert(0);
	//	MessageBox(0,idpath.c_str(),"widget not found!",MB_OK);
	}
	
	return pwidget;
}


//////////////////////////编辑器支持///////////////////////////////

//Win_DeleteWidget(pathid);
static int Win_DeleteWidget(lua_State *ls)
{

	CUIWidget *pWgt= FindUIWidget( ls, 1 );
	if ( !pWgt )
		return 0;

	CUIWidget *pFather=pWgt->GetFather();


	if(pWgt&&pFather)
	{
		pWgt->OnDenitRecursive();
		pFather->DelChild( pWgt );
		
		lua_pushboolean(ls,1);
		return 1;
	}

	lua_pushboolean(ls,0);
	return 1;

}


//Win_AddWidget(father,id,type,infostr);
static int Win_AddWidget(lua_State *ls)
{

	CUIWidget *pFather = FindUIWidget( ls, 1 );
	const char* pid=lua_tostring(ls,2);
	const char* ptype=lua_tostring(ls,3);

	//如果父亲为空，或者父亲已经有这个孩子，就不添加，返回失败
	if(!pFather||pFather->FindWidgetByID(pid,0)!=NULL) 
	{
		lua_pushboolean(ls,0);
		return 1;
	}

	//有infostr
	if(lua_gettop(ls) >= 4)
	{
	
		std::string tmpStr=std::string(pid) + "={" + std::string(lua_tostring(ls,4)) + "};" ;

		CScriptState* pState=new CScriptState;
		pState->DoString(tmpStr.c_str());

		CVarTable infoVar(pid);
		infoVar.GetFromScript(pState);
	
		CUIWidget *pChild = LoadUIWidget(pid,&infoVar,pFather);

		delete pState;

		if(!pChild)
		{
			lua_pushboolean(ls,0);
			return 1;
		}
		pChild->ConfirmScripts(&infoVar);
		pChild->SetLoaded(true);
		pFather->AddChild(pChild);

		pChild->Release();
				
		
	}
	

	lua_pushboolean(ls,1);
	return 1;
	
}

////////////////////////Set begin/////////////////////////
//id,info
//info.style
//info.xmoveable
//info.ymoveable
static int Win_SetWidgetInfo(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
    int iTmp[4];

    //style
    iTmp[0]=GetTableElementInt( ls, "style",2);
    if(pwidget) pwidget->SetStyle(iTmp[0]);
	  
	//moveable
    iTmp[0]=GetTableElementInt( ls, "xmoveable",2);
	if(pwidget) pwidget->EnableMove((iTmp[0]==1),true,false);

    iTmp[0]=GetTableElementInt( ls, "ymoveable",2);
    if(pwidget) pwidget->EnableMove((iTmp[0]==1),false,true);
    
	return 0;
	
	
}

//id,info
//info.bkimage
//info.bkimagex
//info.bkimagey
//info.coverpic
//info.coverpicx
//info.coverpicy
static int Win_SetImageInfo(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
    
    int iTmp[4];
    const int iMaxBuf=MAX_PATH;
    char buffer[MAX_PATH];
    
    //back image path
    bool bret = GetTableElement(ls,"bkimage",buffer,iMaxBuf,2);
    if(pwidget && bret) pwidget->SetBackPic(buffer);

    //back image pos
    iTmp[0]=GetTableElementInt( ls, "bkimagex",2);
    iTmp[1]=GetTableElementInt( ls, "bkimagey",2);
    if(pwidget) pwidget->SetBackPicPos(iTmp[0],iTmp[1]);

    
	//cover pic path
    bret=GetTableElement(ls,"coverpic",buffer,iMaxBuf,2);
    if(pwidget && bret) pwidget->SetCoverPic(buffer);
	
    //cover pic pos
    iTmp[0]=GetTableElementInt( ls, "coverpicx",2);
    iTmp[1]=GetTableElementInt( ls, "coverpicy",2);
    if(pwidget) pwidget->SetCoverPicPos(iTmp[0],iTmp[1]);
	
	
	return 0;
	
	
}

//id,info
//info.textstyle
//info.textsize
//info.caption
static int Win_SetTextInfo(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
    
    int iTmp[4];
    const int iMaxBuf=UI_DEFAULT_BUFSIZE;
    char buffer[UI_DEFAULT_BUFSIZE];
    
    /////////////////////////////////text//////////////////////////////
    //textstyle
    iTmp[0]=GetTableElementInt(ls,"textstyle",2);
    if(pwidget) pwidget->SetTextStyle(iTmp[0]);
    
    //textsize
    iTmp[0]=GetTableElementInt(ls,"textsize",2);
    if(pwidget) pwidget->SetTextSize(iTmp[0]);
    
    //caption
    GetTableElement(ls,"caption",buffer,iMaxBuf,2);
    if(pwidget) pwidget->SetCaption(buffer);
    
    
    
    return 0;
    
    
}

////////////////Set end/////////////////////////////////
//id,info
//info.id
//info.fatherid
//info.type
//info.style
//info.xmoveable
//info.ymoveable
static int Win_GetWidgetInfo(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if(!pwidget) 
	{
		lua_pushnil(ls);
		return 1;
	}
	
    lua_newtable( ls );

    //不可改变的参数-begin
    //type	
    SetTableElement( ls, "type", UI_Type_Names[pwidget->GetType()]);

    //id	
    SetTableElement( ls, "id", pwidget->GetWidgetID());

    //fatherid
    if(pwidget->GetFather())
    SetTableElement( ls, "fatherid", pwidget->GetFather()->GetWidgetID());
	else
	SetTableElement( ls, "fatherid", "");

    //不可改变的参数-end


    //style
    SetTableElement( ls, "style", pwidget->GetStyle());
	  
	//moveable
	SetTableElement( ls, "xmoveable",  ((pwidget->IsMovableX())?1:0));
    SetTableElement( ls, "ymoveable",  ((pwidget->IsMovableY())?1:0));


	//layer
    SetTableElement( ls, "initlayer", pwidget->GetInitLayer());
	SetTableElement( ls, "pluslayer", pwidget->GetPlusLayer());
	SetTableElement( ls, "prelayer", pwidget->GetPreLayer());
    
	return 1;
	
	
}


//imagepath
//imageinfo(width,height,frame)
static int Win_GetImageInfo(lua_State *ls)
{

    CUIImage* pImg=new CUIImage(lua_tostring(ls,1),UI_IMG_PRIOR_LOAD_INIT);
	if(!pImg)
	{
		lua_pushnil(ls);    	
		return 1;
	}

	lua_newtable( ls );

	SetTableElement( ls, "width", pImg->GetWidth());
	SetTableElement( ls, "height", pImg->GetHeight());
	SetTableElement( ls, "frame", pImg->GetNumFrame());		

	SAFE_RELEASE(pImg);

	return 1;
	
	
}

//id,info
//info.textstyle
//info.textsize
//info.caption
static int Win_GetTextInfo(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if(!pwidget)
	{
		lua_pushnil(ls);
		return 1;
	}
    
    lua_newtable( ls ); 
    
      
    /////////////////////////////////text//////////////////////////////
    //textstyle
    SetTableElement( ls, "textstyle",  pwidget->GetTextStyle());
    
    //textsize
    SetTableElement( ls, "textsize",  pwidget->GetTextSize());
    
    //caption
    SetTableElement( ls, "caption", pwidget->GetCaption());
    
    
    
    return 1;
    
    
}


static int Win_SaveWidget(lua_State *ls)
{
	
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if(!pwidget) return 0;
	
    const char *filename = lua_tostring( ls, 2 );

    FILE* fp;
    fp=fopen(filename,"w");
	pwidget->OnSaveRecursive(fp);
    fclose(fp);
	
    return 0;
	
}


//////////////////////////编辑器支持-end///////////////////////////////



//取窗口拖放信息
static int Win_GetDragInfo( lua_State *ls )
{
    
    stDragInfo dragInfo= g_pRootWgt->GetDragInfo();
    
    lua_newtable( ls );
    
    SetTableElement( ls, "type",    dragInfo.type);
    SetTableElement( ls, "index",   dragInfo.index);
	SetTableElement( ls, "itemtype",dragInfo.itemtype);
	
    return 1;
    
}

static int Win_GetCursorInfo(lua_State *ls)
{
    
    int x=0;
    int y=0;
    const char* name=g_pRootWgt->GetCursorInfo(x,y);
    
    lua_newtable(ls);
    
    SetTableElement(ls,"x",x);
    SetTableElement(ls,"y",y);
    SetTableElement(ls,"name",name);	
    
    return 1;
    
}


//查找当前显示的最上层topmost窗口
//可能是根窗口的子窗口或者是当前屏幕的子窗口
//返回该子窗口的path
static int Win_GetFirstTopMost( lua_State *ls )
{

	std::string path="";

	
	if(g_pRootWgt) 
	{
		CUIWidget* pTopChild=g_pRootWgt->GetFirstTopMost();
		
		if((!pTopChild)&&(g_pRootWgt->GetCurScreen())) 
			pTopChild=g_pRootWgt->GetCurScreen()->GetFirstTopMost();
			
		if(pTopChild) 
			path=pTopChild->GetWidgetPath();

	}

	lua_pushstring(ls,path.c_str());
	
	return 1;
	
}


//隐藏屏幕和根窗口的所有topmost子窗口
static int Win_HideTopMost( lua_State *ls )
{
	
	if(g_pRootWgt) 
	{
		g_pRootWgt->HideTopMost();
		
		if(g_pRootWgt->GetCurScreen()) 
			g_pRootWgt->GetCurScreen()->HideTopMost();			

	}	
	
	return 0;
	
}

//显示屏幕和根窗口的所有topmost子窗口
static int Win_ShowTopMost( lua_State *ls )
{
	
	if(g_pRootWgt) 
	{
		g_pRootWgt->ShowTopMost();
		
		if(g_pRootWgt->GetCurScreen()) 
			g_pRootWgt->GetCurScreen()->ShowTopMost();			

	}	
	
	return 0;
	
}



//设置窗口结束
//Win_EndWidget(id,flag):结束窗口，使用指定的结束方式（ok，cancel）
//flag==true:ok
//flag==false:cancel
static int Win_EndWidget( lua_State *ls )
{
	CUIWidget *pwidget = FindUIWidget( ls, 1 );

	bool bFlag=(lua_toboolean(ls,2)!=0);

	if (pwidget)
		pwidget->EndWidget(bFlag);

	return 0;
}


//设置是否显示窗口
static int Win_ShowWidget( lua_State *ls )
{
	assert( lua_gettop(ls) == 2 );

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	bool bshow = (lua_toboolean( ls, 2 )!=0);
	if(pwidget) pwidget->ShowWidget( bshow );

	return 0;
}

static int Win_EnableWidget(lua_State *ls)
{
	
	assert(lua_gettop(ls)==2);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	bool benable=(lua_toboolean(ls,2)!=0);
	if(pWgt) pWgt->EnableWidget(benable);
	
	return 0;
	
}


//设置是否显示窗口
static int Win_ToggleWidget( lua_State *ls )
{

	CUIWidget *pwidget = FindUIWidget( ls, 1 );

	int state=-1;
	if(lua_gettop(ls)>1)
		state=lua_toboolean(ls,2);

	BOOL bshow=FALSE;
	if(pwidget)
	{
		bshow=pwidget->IsVisible(); 			
		pwidget->ShowWidget( !bshow );

		if(!bshow)
		{
			
			if(state==0)
				pwidget->ActiveSelf(true);
			else if(state==1)
				g_pRootWgt->SetFocusWgt(pwidget);

		}

	}

	lua_pushboolean(ls,int(bshow));
	
	return 1;
}


static int Win_IsExist(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	lua_pushboolean(ls,int(pWgt));

	return 1;
	
}

static int Win_IsChildFocused(lua_State *ls)
{
	
	assert(lua_gettop(ls)==1);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	BOOL bFocused=false;

	if(pWgt) bFocused=pWgt->IsChildFocused();
	
	lua_pushboolean(ls,bFocused);

	return 1;
	
}


static int Win_IsDescendFocused(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	BOOL bFocused=false;

	if(pWgt) bFocused=pWgt->IsDescendFocused();
	
	lua_pushboolean(ls,bFocused);

	return 1;
	
}


static int Win_IsChildHot(lua_State *ls)
{
	
	assert(lua_gettop(ls)==1);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	BOOL bHot=false;

	if(pWgt) bHot=pWgt->IsChildHot();
	
	lua_pushboolean(ls,bHot);

	return 1;
	
}


static int Win_IsDescendHot(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	BOOL bHot=false;

	if(pWgt) bHot=pWgt->IsDescendHot();
	
	lua_pushboolean(ls,bHot);

	return 1;
	
}


static int Win_IsInDescend(lua_State *ls)
{
	assert(lua_gettop(ls)==2);
	
	CUIWidget* pFather=FindUIWidget(ls,1);
	CUIWidget* pChild=FindUIWidget(ls,2);
	
	BOOL bHot=false;

	if(pFather&&pChild) 
		bHot=pFather->IsInDescend(pChild);
	
	lua_pushboolean(ls,bHot);

	return 1;
	
}


static int Win_IsAcceptInput(lua_State *ls)
{
	
	CUIWidget* pWgt=g_pRootWgt->GetFocusWgt();

	BOOL bAccept=false;

	if(pWgt) 
		bAccept=pWgt->IsAcceptInput();
	
	lua_pushboolean(ls,bAccept);

	return 1;
	
}


static int Win_IsFocused(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	int bFocused=0;
	CUIWidget* pWgt=FindUIWidget(ls,1);
	if(pWgt) bFocused=pWgt->IsFocused();
	
	lua_pushboolean(ls,bFocused);
	return 1;
	
}


static int Win_IsVisible(lua_State *ls)
{
	assert(lua_gettop(ls)==1);

	int bVisible=0;
	CUIWidget* pWgt=FindUIWidget(ls,1);
	if(pWgt) bVisible=pWgt->IsVisible();
	
	lua_pushboolean(ls,bVisible);
	return 1;

}

static int Win_IsEnabled(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	int bEnable=0;
	CUIWidget* pWgt=FindUIWidget(ls,1);
	if(pWgt) bEnable=pWgt->IsEnabled();

	lua_pushboolean(ls,bEnable);

	return 1;

}


static int Win_IsVisibleEx(lua_State *ls)
{
	assert(lua_gettop(ls)==1);
	
	int bVisible=0;
	CUIWidget* pWgt=FindUIWidget(ls,1);	
	if(pWgt) bVisible=pWgt->IsVisibleEx();
	
	lua_pushboolean(ls,bVisible);
	return 1;

}

static int Win_IsEnabledEx(lua_State *ls)
{
	assert(lua_gettop(ls)==1);

	int bEnable=0;
	CUIWidget* pWgt=FindUIWidget(ls,1);
	if(pWgt) bEnable=pWgt->IsEnabledEx();

	lua_pushboolean(ls,bEnable);

	return 1;

}


//设置窗口背景文件
static int Win_SetBackPic( lua_State *ls )
{
	assert( lua_gettop(ls) == 2 );
	if ( lua_gettop(ls) < 2 )
	{
		return 0;
	}
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	const char *path = lua_tostring( ls, 2 );
	if(pwidget) pwidget->SetBackPic( path );

	return 0;
}

//判断CUIPicWin是否选中
static int Win_IsChecked( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );

	CUIWidget *pwidget = FindUIWidget( ls, 1 );

	int bChecked=0;
	if(pwidget) bChecked=pwidget->IsChecked();

	lua_pushboolean( ls, bChecked );

	return 1;
}


static int Win_SetCheck( lua_State *ls )
{
	assert( lua_gettop(ls) == 2 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
	bool bcheck = (lua_toboolean(ls,2) != 0);
    if(pwidget) pwidget->SetChecked( bcheck );
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////

//widgetid,channel,index
static int Win_GetValue( lua_State *ls )
{
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
	stUIVar uiVar;
	
	if( lua_gettop(ls) >= 3 )
	{
		uiVar.iChannel=lua_tonumber(ls,2);
		uiVar.iIndex=lua_tonumber(ls,3);
	}
	else if( lua_gettop(ls) >= 2 )
	{
		uiVar.iChannel=lua_tonumber(ls,2);
	}
	else 
	{
		uiVar.iChannel=VALUE_CHANNEL_VALUE;
	}
	
	pwidget->GetVar(uiVar);			

	lua_pushnumber( ls, uiVar.fValue );
	
	return 1;
}



//widgetid,value,channel,index
static int Win_SetValue(lua_State *ls)
{

	CUIWidget* pWgt=FindUIWidget(ls,1);
	if ( !pWgt )
		return 0;

	stUIVar uiVar;
	
	uiVar.fValue=lua_tonumber(ls,2);
	
	if(lua_gettop(ls)>=4)
	{
		uiVar.iChannel=lua_tonumber(ls,3);
		uiVar.iIndex=lua_tonumber(ls,4);		
	}
	else if(lua_gettop(ls)>=3)
	{
		uiVar.iChannel=lua_tonumber(ls,3);		
	}
	else 
	{
		uiVar.iChannel=VALUE_CHANNEL_VALUE;
	}
	
	pWgt->SetVar(uiVar);

	return 0;

}



//widgetid,channel
static int Win_GetText( lua_State *ls )
{
	
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

	stUIVar uiVar;		
		
	if( lua_gettop(ls) == 3 )
	{
		uiVar.iChannel=lua_tonumber(ls,2);
		uiVar.iIndex=lua_tonumber(ls,3);		
	}
	else if( lua_gettop(ls) == 2 )
	{
		uiVar.iChannel=lua_tonumber(ls,2);				
	}
	else 
	{
		uiVar.iChannel=VALUE_CHANNEL_CAPTION;		
	}
	
	pwidget->GetVar(uiVar);

	/*
	//保密密码内容
	if((pwidget->GetType()==WGTTYPE_EDIT)&&(uiVar.iChannel==VALUE_CHANNEL_CAPTION))
	{
		CUILineEdit* pEdit=(CUILineEdit*)pwidget;
		int nMethod=pEdit->GetEditMethod();
		
		if(nMethod&UI_EDITMETHOD_CODER)
		{
			static char coderBuf[UI_DEFAULT_BUFSIZE];
			memset(coderBuf,0,UI_DEFAULT_BUFSIZE);
			memset(coderBuf,'*',strlen(uiVar.sValue.c_str()));			

			uiVar.sValue=coderBuf;
		}
	}
	*/
	lua_pushstring( ls, uiVar.sValue.c_str() );
	
	return 1;
}

//widgetid,text,channel
static int Win_SetText(lua_State *ls)
{
	if ( lua_gettop(ls) < 2 )
	{
		return 0;
	}
	CUIWidget* pWgt=FindUIWidget(ls,1);
	if ( !pWgt )
		return 0;

	stUIVar uiVar;
	
	uiVar.sValue=lua_tostring(ls, 2);	
	
	if(lua_gettop(ls)==4)
	{
		uiVar.iChannel=lua_tonumber(ls,3);
		uiVar.iIndex=lua_tonumber(ls,4);
	}
	else  if(lua_gettop(ls)==3)
	{
		uiVar.iChannel=lua_tonumber(ls,3);		
	}
	else 
	{		
		uiVar.iChannel=VALUE_CHANNEL_CAPTION;
	}

	pWgt->SetVar(uiVar);
	
	return 0;
	
}


static int Win_GetTable(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return NULL;

    int channel=lua_tonumber(ls,2);
	
	int nResult=pwidget->GetTable(channel,ls);

    return nResult;    
    
}


static int Win_SetTable(lua_State *ls)
{
    
    CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

    int channel=lua_tonumber(ls,2);

	int nResult=pwidget->SetTable(channel,ls);    
    
    return nResult;    
    
}



//////////////////////////////////////////////////////////////////////////

//返回一个table(width,height)
static int Win_GetSize( lua_State *ls )
{
	
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
		
	if(pWgt)
	{
		stRect rect=pWgt->GetWinRect();		
	
		lua_pushnumber( ls, rect.width);
		lua_pushnumber( ls, rect.height);		
		
	}
	
	return 2;
}



//(width,height)
static int Win_SetSize(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		
		int width=lua_tonumber(ls,2);
		int height=lua_tonumber(ls,3);
		
		pWgt->SetWidth(width);
		pWgt->SetHeight(height);
	}
	
	return 0;
	
}



//widgetid,channel
//返回一个table(l,t,r,b)
static int Win_GetRect( lua_State *ls )
{
	
	
	CUIWidget* pWgt=FindUIWidget(ls,1);

	int channel=VALUE_CHANNEL_WINRECT;
	if(lua_gettop(ls)>1) channel=lua_tonumber(ls,2);
	
	if(pWgt)
	{
		stRect rect;
		
		if(channel==VALUE_CHANNEL_WINRECT)
			rect=pWgt->GetWinRect();
		else if(channel==VALUE_CHANNEL_CLIENTRECT)
			rect=pWgt->GetClientRect();
		else if(channel==VALUE_CHANNEL_CAPTIONRECT)
			rect=pWgt->GetCaptionRect();
		else if(channel==VALUE_CHANNEL_DRAGRECT)
			rect=pWgt->GetDragRect();
		
		lua_newtable( ls );
		SetTableElement( ls, "x", rect.ox);
		SetTableElement( ls, "y", rect.oy);
		SetTableElement( ls, "w", rect.width);
		SetTableElement( ls, "h", rect.height);		
		
	}
	
	return 1;
}



//widgetid,channel,l,t,r,b
static int Win_SetRect(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int x=lua_tonumber(ls,2);
		int y=lua_tonumber(ls,3);
		int w=lua_tonumber(ls,4);
		int h=lua_tonumber(ls,5);

		int channel=VALUE_CHANNEL_WINRECT;
		if(lua_gettop(ls)>5) channel=lua_tonumber(ls,6);
		
		if(channel==VALUE_CHANNEL_WINRECT)
		{
			pWgt->SetWidth(w);
			pWgt->SetHeight(h);
			pWgt->MoveTo(x,y);
		}
		else if(channel==VALUE_CHANNEL_CLIENTRECT)
			pWgt->SetClientRect(x,y,w,h);
		else if(channel==VALUE_CHANNEL_CAPTIONRECT)
			pWgt->SetCaptionRect(x,y,w,h);
		else if(channel==VALUE_CHANNEL_DRAGRECT)
			pWgt->SetDragRect(x,y,w,h);

	}
	
	return 0;
	
}


//widgetid,x,y
static int Win_GetPos(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	int x=0;
	int y=0;		

	if(pWgt)
	{
		stRect winRect=pWgt->GetWinRect();
		x=winRect.ox;
		y=winRect.oy;		
	}

	lua_pushnumber(ls,x);
	lua_pushnumber(ls,y);
	
	return 2;
	
}


//widgetid,x,y
static int Win_RelativePos(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int x=lua_tonumber(ls,2);
		int y=lua_tonumber(ls,3);
		
		CUIWidget* pFather=pWgt->GetFather();
		if(pFather)
		{
			pWgt->MoveTo(pFather->GetOriginX()+x,pFather->GetOriginY()+y);
		}
		else 
		{
			pWgt->MoveTo(x,y);		 
		}
		
	}
	
	return 0;
	
}


//widgetid,x,y
static int Win_SetPos(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int x=lua_tonumber(ls,2);
		int y=lua_tonumber(ls,3);
		
		pWgt->MoveTo(x,y);
		
	}
	
	return 0;
	
}



//widgetid,dx,dy
static int Win_MovePos(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int dx=lua_tonumber(ls,2);
		int dy=lua_tonumber(ls,3);
		
		pWgt->Move(dx,dy);
		
	}
	
	return 0;
	
}


static int Win_ExtendTo(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int value=lua_tonumber(ls,2);
		int edgeFlag=lua_tonumber(ls,3);
		int rectFlag=lua_tonumber(ls,4);
		
		pWgt->ExtendTo(value,edgeFlag,rectFlag);
		
	}
	
	return 0;
	
}

static int Win_ExtendEdge(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int addition=lua_tonumber(ls,2);
		int edgeFlag=lua_tonumber(ls,3);
		int rectFlag=lua_tonumber(ls,4);
		
		pWgt->ExtendEdge(addition,edgeFlag,rectFlag);
		
	}
	
	return 0;
	
}


static int Win_GetFreeRect(lua_State *ls)
{

	CUIWidget* pWgt=FindUIWidget(ls,1);
	int width=lua_tonumber(ls,2);
	int height=lua_tonumber(ls,3);

	stRect freeRct(-1,-1,0,0);

	if(pWgt) 
	{
		freeRct=pWgt->GetFreeRect(width,height);
	}

	lua_pushnumber(ls,freeRct.ox);
	lua_pushnumber(ls,freeRct.oy);
	lua_pushnumber(ls,freeRct.width);
	lua_pushnumber(ls,freeRct.height);

	return 4;

}

static int Win_PullIntoScreen(lua_State *ls)
{

	CUIWidget* pWgt=FindUIWidget(ls,1);

	if(pWgt) pWgt->PullIntoScreen();

	return 0;

}

static int Win_IsOutScreen(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
			
	int xflag=UI_TOUCH_NONE;
	int yflag=UI_TOUCH_NONE;


	if(pWgt)
	{
		
		int flag=pWgt->IsOutofScreen();

		if(flag&UI_TOUCH_LEFT_OUT)
			xflag=UI_TOUCH_LEFT_OUT;
		else if(flag&UI_TOUCH_RIGHT_OUT)
			xflag=UI_TOUCH_RIGHT_OUT;

		if(flag&UI_TOUCH_TOP_OUT)
			yflag=UI_TOUCH_TOP_OUT;
		else if(flag&UI_TOUCH_BOTTOM_OUT)
			yflag=UI_TOUCH_BOTTOM_OUT;
		
	}

	
	lua_pushnumber(ls,xflag);
	lua_pushnumber(ls,yflag);

	return 2;
	
}


//////////////////////////////////////////////////////////////////////////
//脚本支持函数

//index->rgba
static int GetColorRGBA( lua_State *ls )
{
	
	int index=lua_tonumber(ls,1);
	stColorQuad color(index);

	lua_newtable( ls );
	SetTableElement( ls, "r", color.r);
	SetTableElement( ls, "g", color.g);
	SetTableElement( ls, "b", color.b);
	SetTableElement( ls, "a", color.a);		
	
	return 1;
}

//rgba->index
static int GetColorIndex( lua_State *ls )
{
	assert( lua_gettop(ls) >= 3 );
	
	stColorQuad color;
	color.r=lua_tonumber(ls,1);
	color.g=lua_tonumber(ls,2);
	color.b=lua_tonumber(ls,3);

	if(lua_gettop(ls) > 3)
	color.a=lua_tonumber(ls,4);
	else 
	color.a=255;
	
	lua_pushnumber( ls, color.c );
	
	return 1;
}

//返回两个参数的按位与
static int GetBitAnd( lua_State *ls )
{
	int src=lua_tonumber(ls,1);
	int dst=lua_tonumber(ls,2);

	lua_pushnumber( ls, src&dst );
	
	return 1;
}


//返回两个参数的按位或
static int GetBitOR( lua_State *ls )
{
	int src=lua_tonumber(ls,1);
	int dst=lua_tonumber(ls,2);

	lua_pushnumber( ls, src|dst );
	
	return 1;
}


//返回两个参数的按位异或
static int GetBitXOR( lua_State *ls )
{
	int src=lua_tonumber(ls,1);
	int dst=lua_tonumber(ls,2);

	lua_pushnumber( ls, src&~dst );
	
	return 1;
}





//////////////////////////////////////////////////////////////////////////



static int Win_GetWidgetNum( lua_State *ls )
{

	int num=g_vecSecuritySet.size();
	lua_pushnumber(ls,num);
	
	return 1;
}

static int Win_DumpImage( lua_State *ls )
{
	std::string srcPath="dump";

	if(lua_gettop(ls)>0)
		srcPath=lua_tostring(ls,1);
	
	DumpImageInfo(srcPath.c_str());	
	
	return 0;
}


static int Win_CancelDrag( lua_State *ls )
{

	GameUICancelDrag();

	return 0;
}


static int Win_AddStyle(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int style=lua_tonumber(ls,2);
		style=pWgt->GetStyle()|style;
		pWgt->SetStyle(style);
	}
	
	
	return 0;
	
}

static int Win_RemoveStyle(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int style=lua_tonumber(ls,2);
		style=pWgt->GetStyle()&~style;
		pWgt->SetStyle(style);
	}
	
	
	return 0;
	
}



static int Win_AddStyleEx(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int style=lua_tonumber(ls,2);
		style=pWgt->GetExtendStyle()|style;
		pWgt->SetExtendStyle(style);
	}
	
	
	return 0;
	
}

static int Win_RemoveStyleEx(lua_State *ls)
{
	
	CUIWidget* pWgt=FindUIWidget(ls,1);
	
	if(pWgt)
	{
		int style=lua_tonumber(ls,2);
		style=pWgt->GetExtendStyle()&~style;
		pWgt->SetExtendStyle(style);
	}
	
	
	return 0;
	
}


//判断孩子中的选择项
static int Win_GetSelect( lua_State *ls )
{
	assert( lua_gettop(ls) == 2 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

	int groupId   = lua_tonumber(ls,2);
	int groupStop = pwidget->GetSelect(groupId);
	lua_pushnumber( ls, groupStop);
	
	return 1;
}


//判断孩子中的选择项
static int Win_SetSelect( lua_State *ls )
{
	assert( lua_gettop(ls) == 3 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

	int groupId   = lua_tonumber(ls,2);
	int groupStop = lua_tonumber(ls,3);
	pwidget->SelectChild(groupId,groupStop);
	
	return 0;
}


//设置孩子中的选择窗口
static int Win_SelectSelf( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;
    pwidget->SelectSelf();
	
	return 0;
}

//设置孩子中的选择窗口
static int Win_ActiveSelf( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

    pwidget->ActiveSelf(true);	
		
	return 0;

}



static int Win_ReAlignChild( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

    pwidget->ReAlignChild();
	
	return 0;
}

static int Win_ArrangeSelf( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

    pwidget->ArrangeChildRect();
	
	return 0;
}


static int Win_AjustSelf( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;
    pwidget->AjustSelf();	
		
	return 0;

}


static int Win_UpdateSelf( lua_State *ls )
{
	assert( lua_gettop(ls) == 1 );

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;
	pwidget->UpdateSelf(0);

	return 0;

}

//////////////////////////窗口对齐////////////////////////////////
//input:width,height,dstWgt,alignType,alignStyle,marginh,marginv
//output:ox,oy
static int Win_AlignRect( lua_State *ls )
{
		
	stRect srcRct;
	
	srcRct.width= lua_tonumber( ls, 1 );
	srcRct.height= lua_tonumber( ls, 2 );

	CUIWidget *pDstWgt = FindUIWidget( ls, 3 );
	if ( !pDstWgt )
		return 0;

	int alignType=lua_tonumber(ls,4);
	int alignStyle=lua_tonumber(ls,5);
	
	int marginH=0;
	int marginV=0;
	
	if( lua_gettop(ls) > 6 )
	{		
		marginH=lua_tonumber(ls,6);
		marginV=lua_tonumber(ls,7);	
	}
	else if( lua_gettop(ls) > 5 )
	{		
		marginH=lua_tonumber(ls,6);
	}

	pDstWgt->AlignRectToSelf(srcRct,alignType,alignStyle,marginH,marginV);

	lua_pushnumber(ls,srcRct.ox);
	lua_pushnumber(ls,srcRct.oy);
	
	return 2;
}


static int Win_AlignWidget( lua_State *ls )
{
		
	CUIWidget *pSrcWgt = FindUIWidget( ls, 1 );
	CUIWidget *pDstWgt = FindUIWidget( ls, 2 );
	if ( !pSrcWgt || !pDstWgt )
		return 0;

	int alignType=lua_tonumber(ls,3);
	int alignStyle=lua_tonumber(ls,4);
	
	int marginH=0;
	int marginV=0;
	
	if( lua_gettop(ls) > 5 )
	{		
		marginH=lua_tonumber(ls,5);
		marginV=lua_tonumber(ls,6);	
	}
	else if( lua_gettop(ls) > 4 )
	{		
		marginH=lua_tonumber(ls,5);
	}

	pSrcWgt->AlignToWidget(pDstWgt,alignType,alignStyle,marginH,marginV);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////



//设置覆盖图片文件
static int Win_SetCoverPic( lua_State *ls )
{
	assert( lua_gettop(ls) == 2 );

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

	pwidget->SetCoverPic( lua_tostring(ls, 2) );

	return 0;
}


//设置当前capture窗口
static int Win_SetCapture( lua_State *ls )
{
	if( lua_gettop(ls) == 0 )
	{

		g_pRootWgt->SetCaptureWgt( 0 );
	}
	else if(lua_gettop(ls) == 1 )
	{
		CUIWidget *pwidget = FindUIWidget( ls, 1 );
		g_pRootWgt->SetCaptureWgt( pwidget );
		
	}
	
	return 0;
}

static int Win_SetHot( lua_State *ls )
{
	if( lua_gettop(ls) == 0 )
	{
		g_pRootWgt->SetHotWgt( 0 );
	}
	else if(lua_gettop(ls) == 1 )
	{
		CUIWidget *pwidget = FindUIWidget( ls, 1 );
		g_pRootWgt->SetHotWgt( pwidget );
		
	}
	
	return 0;
}

static int Win_SetFocus( lua_State *ls )
{
	if( lua_gettop(ls) == 0 )
	{
		g_pRootWgt->SetFocusWgt( 0 );
	}
	else if(lua_gettop(ls) == 1 )
	{
		CUIWidget *pwidget = FindUIWidget( ls, 1 );
		g_pRootWgt->SetFocusWgt( pwidget );
		
	}
	
	return 0;
}


//重新载入屏幕
static int Win_ReloadScreen(lua_State *ls)
{
	
#ifdef _UIEDITMODE
	g_pRootWgt->ReloadScreen();
#endif

	return 0;
}

//显示屏幕调试信息
static int Win_DebugScreen(lua_State *ls)
{
	
#ifdef _UIEDITMODE
	g_pRootWgt->DebugScreen();
#endif

	return 0;
}


//切换编辑模式
static int Win_SetEditMode(lua_State *ls)
{

#ifdef _UIEDITMODE
	int  mode=lua_tonumber(ls, 1);
	g_pRootWgt->SetEditMode(mode);
#endif

	return 0;
}


//得到编辑模式
static int Win_GetEditMode(lua_State *ls)
{

#ifdef _UIEDITMODE
	int mode=g_pRootWgt->GetEditMode();
	lua_pushnumber(ls,mode);
#endif

	return 1;
}


//切换控制台
static int Win_ToggleConsole(lua_State *ls)
{
	
#ifdef _UIEDITMODE
	g_pRootWgt->ToggleConsole();
#endif

	return 0;
}


//显示控制台信息
static int Win_ConsoleInfo(lua_State *ls)
{

	
#ifdef _UIEDITMODE
	assert( lua_gettop(ls) == 1 );
	std::string info( lua_tostring(ls, 1) );
	
	g_pRootWgt->AddConsoleInfo(info.c_str());
#endif

	return 0;

}



//返回：num,table
static int Win_GetTypeNames(lua_State *ls)
{
	
	
	int num=sizeof(UI_Type_Names)/sizeof(void*);
	lua_pushnumber(ls,num);

	lua_newtable(ls);
	for(int i=0;i<num;i++)
	{ 
		SetTableElement(ls,i,UI_Type_Names[i]);
	}

	return 2;

}


//返回：num,table
static int Win_GetSysFuncs(lua_State *ls)
{
		
	int num=sizeof(g_SystemScriptFuncs)/sizeof(void*);
	lua_pushnumber(ls,num);

	lua_newtable(ls);
	for(int i=0;i<num;i++)
	{ 
		SetTableElement(ls,i,g_SystemScriptFuncs[i]);
	}

	return 2;

}

static Win_GetScreens(lua_State *ls)
{
		
	int num=g_pRootWgt->GetNumScreen();
	lua_pushnumber(ls,num);

	lua_newtable(ls);
	for(int i=0;i<num;i++)
	{ 
		SetTableElement(ls,i,g_pRootWgt->GetScreen(i)->GetWidgetID());
	}

	return 2;

}


//scrnid,scrnpath
static int Win_GetCurScreen(lua_State *ls)
{

	CUIWidget* pCurScreen=g_pRootWgt->GetCurScreen();
	
	if(pCurScreen)
	{
		lua_pushstring(ls,pCurScreen->GetWidgetID());
		lua_pushstring(ls,pCurScreen->GetWidgetPath());
	}
	else
	{
		lua_pushstring(ls,"");
		lua_pushstring(ls,"");
	}

	return 2;


}



//用于调试界面
//获取当前鼠标下面的窗口
static int Win_GetMouseWgtID( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetMouseWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPathID());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}

static int Win_GetMouseWgtPath( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetMouseWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPath());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}


//style可以省略
static int Win_RegisterBuddy( lua_State *ls )
{
	assert(lua_gettop(ls)>=3);

	CUIWidget* pWgt=FindUIWidget(ls,1);
	int  type=lua_tonumber(ls, 2);	

	int  style=BUDDY_STYLE_SELF;	
	const char* id="";

	if(lua_gettop(ls)>3)
	{
		style=lua_tonumber(ls, 3);	
		id=lua_tostring(ls, 4);
	}
	else
	{
		id=lua_tostring(ls, 3);
	}

	if(pWgt) pWgt->RegisterBuddyWgt(type,style,id);	


	return 0;
}



//用于调试界面
//获取当前hot窗口
static int Win_GetHotID( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetHotWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPathID());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}

static int Win_GetHotPath( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetHotWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPath());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}

//获取当前focus窗口
static int Win_GetFocusID( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetFocusWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPathID());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}

static int Win_GetFocusPath( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetFocusWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPath());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}



//获取当前capture窗口
static int Win_GetCaptureID( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetCaptureWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPathID());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}

static int Win_GetCapturePath( lua_State *ls )
{
	CUIWidget *pwidget=g_pRootWgt->GetCaptureWgt();
	
	if(pwidget) 
	{ 
		lua_pushstring( ls, pwidget->GetWidgetPath());
	}
	else 
	{
		lua_pushstring( ls, "");
	}
	
	return 1;
}


static int Win_OnInsert( lua_State *ls )
{
	CUIWidget *pwidget=NULL;
	const char* pText=NULL;

	if(lua_gettop(ls)>1)
	{
		pwidget=FindUIWidget(ls,1);
		pText=lua_tostring(ls, 2);	
	}
	else
	{
		pwidget=g_pRootWgt->GetFocusWgt();
		pText=lua_tostring(ls, 1);	
	}
	
	if(pwidget) pwidget->OnInsert(pText);
	
	return 0;
}


static int Win_OnPaste( lua_State *ls )
{
	CUIWidget *pwidget=NULL;
	if(lua_gettop(ls)>0)
		pwidget=FindUIWidget(ls,1);
	else
		pwidget=g_pRootWgt->GetFocusWgt();
	
	if(pwidget) pwidget->OnPaste();
	
	return 0;
}

static int Win_OnCut( lua_State *ls )
{
	CUIWidget *pwidget=NULL;
	if(lua_gettop(ls)>0)
		pwidget=FindUIWidget(ls,1);
	else
		pwidget=g_pRootWgt->GetFocusWgt();
	
	if(pwidget) pwidget->OnCut();
	
	return 0;
}

static int Win_OnCopy( lua_State *ls )
{
	CUIWidget *pwidget=NULL;
	if(lua_gettop(ls)>0)
		pwidget=FindUIWidget(ls,1);
	else
		pwidget=g_pRootWgt->GetFocusWgt();
	
	if(pwidget) pwidget->OnCopy();
	
	return 0;
}

static int Win_EditGetFont( lua_State *ls )
{
	CUIWidget* pwidget=FindUIWidget(ls,1);
	if ( !pwidget )
		return 0;
	
	int font=pwidget->GetTextFont();	
	if(pwidget&&(pwidget->GetType()==WGTTYPE_MULTIEDIT))
	{
		font=((CUIMultiEdit*)pwidget)->GetFontPos();		
	}
	lua_pushnumber(ls,font);
	
	return 1;
}


static int Win_EditGetColor( lua_State *ls )
{
	CUIWidget* pwidget=FindUIWidget(ls,1);
	if ( !pwidget )
		return 0;
	int color=pwidget->GetTextFont();	
	if(pwidget&&(pwidget->GetType()==WGTTYPE_MULTIEDIT))
	{
		color=((CUIMultiEdit*)pwidget)->GetColorPos();
	}
	lua_pushnumber(ls,color);
	
	return 1;
}

static int Win_EditSetFont( lua_State *ls )
{
	CUIWidget* pwidget=FindUIWidget(ls,1);
	if ( !pwidget )
		return 0;
	int font=lua_tonumber(ls, 2);	
	if(pwidget&&(pwidget->GetType()==WGTTYPE_MULTIEDIT))
	{
		((CUIMultiEdit*)pwidget)->OnSetFont(font);
	}
	
	return 0;
}


static int Win_EditSetColor( lua_State *ls )
{
	CUIWidget* pwidget=FindUIWidget(ls,1);
	int color=lua_tonumber(ls, 2);	
	if(pwidget&&(pwidget->GetType()==WGTTYPE_MULTIEDIT))
	{
		((CUIMultiEdit*)pwidget)->OnSetColor(color);
	}
	
	return 0;
}


//发送一个键盘消息给指定窗口
static int Win_SendMessage( lua_State *ls )
{
	assert( lua_gettop(ls) == 4 );

	CUIWidget* pwidget=FindUIWidget(ls,1);
	
	unsigned int nMsg=lua_tonumber(ls, 2);
	unsigned int wparam=(unsigned int)lua_tonumber(ls, 3);
	unsigned int lparam=(unsigned int)lua_tonumber(ls, 4);
	
	UIMsgType eMsg=(UIMsgType)nMsg;

	if(pwidget)
	{
		if(MSGTYPE_CHAR==eMsg)
			pwidget->OnChar(wparam,lparam,true);
		else if(MSGTYPE_KEYDOWN==eMsg)
			pwidget->OnKeyDown(wparam,lparam,true);
	}
	
	return 0;
}

//显示消息框
static int Win_ShowMsgBox( lua_State *ls )
{
	
	if( lua_gettop(ls) == 5 )
	{
		
		std::string text( lua_tostring(ls, 1) );
		std::string caption( lua_tostring(ls, 2) );
		int  nMsgId=lua_tonumber(ls, 3);		
		CUIWidget *pwidget = FindUIWidget( ls, 4 );
		int  nHandle=lua_tonumber(ls, 5);		
		g_pRootWgt->ShowMessageBox(UI_MSGBOX_CMDSHOW,text.c_str(),caption.c_str(),nMsgId ,pwidget,nHandle);
		
	}
	else 
	if( lua_gettop(ls) == 4 )
	{
	
		std::string text( lua_tostring(ls, 1) );
		std::string caption( lua_tostring(ls, 2) );
		int  nMsgId=lua_tonumber(ls, 3);		
		CUIWidget *pwidget = FindUIWidget( ls, 4 );
		g_pRootWgt->ShowMessageBox(UI_MSGBOX_CMDSHOW,text.c_str(),caption.c_str(),nMsgId ,pwidget);
		
	}
	else 
	if( lua_gettop(ls) == 3 )
	{	
		
		std::string text( lua_tostring(ls, 1) );
		std::string caption( lua_tostring(ls, 2) );
		int  nMsgId=lua_tonumber(ls, 3);		
		g_pRootWgt->ShowMessageBox(UI_MSGBOX_CMDSHOW,text.c_str(),caption.c_str(),nMsgId );
	}
	else 
	if( lua_gettop(ls) == 2 )
	{	
		
		std::string text( lua_tostring(ls, 1) );
		std::string caption( lua_tostring(ls, 2) );
		g_pRootWgt->ShowMessageBox(UI_MSGBOX_CMDSHOW,text.c_str(),caption.c_str() );
	}
	else 
	if( lua_gettop(ls) == 1 )
	{	
		
		std::string text( lua_tostring(ls, 1) );
		g_pRootWgt->ShowMessageBox(UI_MSGBOX_CMDSHOW,text.c_str());
	}
	
	return 0;

}


//添加模板孩子(个数)
//widget,id
//widget,id,index
//widget,id,start,end
static int Win_AddTemplate( lua_State *ls )
{
		
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if ( !pwidget )
		return 0;

	int  id=lua_tonumber(ls, 2);

	CUIWidget *pchild=NULL;	

	std::string pathid="";


	if( lua_gettop(ls) >= 4 )
	{
		int  start=lua_tonumber(ls, 3);
		int  end=lua_tonumber(ls, 4);
		
		for(int i=start;i<=end;i++)
		{
			pwidget->AddTemplate(id,i); 		
		}
	}
	//添加指定索引的模板
	else if( lua_gettop(ls) >= 3 )
	{
		int  idx=lua_tonumber(ls, 3);
			
		if(pwidget)	
		pchild=pwidget->AddTemplate(id,idx); 		
	}
	//顺序添加一个模板
	else
	{	
		if(pwidget)	
		pchild=pwidget->AddTemplate(id); 		
	}

	if(pchild)
		pathid=pchild->GetWidgetPath();

	lua_pushstring(ls,pathid.c_str());
	
	return 1;
}


//删除所有孩子
static int Win_ClearTemplates( lua_State *ls )
{
	if ( lua_gettop(ls) < 1 )
		return 0;
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	int  id=0;
	if ( lua_gettop(ls) > 1 )
		id = lua_tonumber(ls, 2);
	if(pwidget) pwidget->ClearTemplates(id);
	
	return 0;
}


static int Win_GetTemplatesNum( lua_State *ls )
{
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	int  id=lua_tonumber(ls, 2);

	int num=0;

	if(pwidget) 
	{
		num=pwidget->GetTemplatesNum(id);
	}

	lua_pushnumber(ls,num);
	
	return 1;
}

static int Win_GetNumChild( lua_State *ls )
{
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
	int num=0;

	if(pwidget) 
	{
		num=pwidget->GetNumChild();
	}

	lua_pushnumber(ls,num);
	
	return 1;
}


static int Win_GetChildren( lua_State *ls )
{
	
	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
	int num=0;

	if(pwidget) 
		num=pwidget->GetNumChild();

	lua_pushnumber(ls,num);

	lua_newtable( ls );     
    if(pwidget&&(num>0))
	{
		for(int i=0;i<num;i++)
		{ 
			CUIWidget* pChild=pwidget->GetChild(i);
			
			//索引从1开始
			SetTableElement( ls, i+1, pChild->GetWidgetPath() );
		}

	}	
	
	return 2;
}


static int Win_PushPathPoint( lua_State *ls )
{
	

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if(pwidget) 
	{
		stUIPathPoint pt;
		pt.x=lua_tonumber(ls, 2);
		pt.y=lua_tonumber(ls, 3);

		if( lua_gettop(ls) >=4  )
		pt.command=lua_tonumber(ls, 4);

		if( lua_gettop(ls) >=5  )
		pt.alpha=lua_tonumber(ls, 5);
			
		if( lua_gettop(ls) >=6  )
		pt.scale=lua_tonumber(ls, 6);		
	
		pwidget->PushPathPoint(pt);
	}
	
	return 0;
}

static int Win_ClearPathPoint( lua_State *ls )
{

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	if(pwidget) pwidget->ClearPathPoint();

	return 0;
}


//使用custombuffer来绘制sketchmap
//传入一个wgtid
//返回pbuf（一个table）
static int Win_GetCustomBuffer( lua_State *ls )
{

	CUIWidget* pWgt=NULL;

	if(g_pRootWgt)
		pWgt=g_pRootWgt->FindRootWidgetByID(lua_tostring(ls,1));

	assert(pWgt);

		
	//调用客户端自定义函数
	if(pWgt) 
		pWgt->ExecCallBackFunc(UI_Callback_GetCustomBuffer,(void*)ls);
	
	return 1;

}


//使用custombuffer来绘制sketchmap
//传入一个wgtid,pbuf（一个table）
static int Win_SetCustomBuffer( lua_State *ls )
{

	CUIWidget* pWgt=NULL;

	if(g_pRootWgt)
		pWgt=g_pRootWgt->FindRootWidgetByID(lua_tostring(ls,1));

	assert(pWgt);
	

	//调用客户端自定义函数
	if(pWgt) 
		pWgt->ExecCallBackFunc(UI_Callback_SetCustomBuffer,(void*)ls);
	
	return 0;

}

static int Win_ShowTooltip( lua_State *ls )
{

	CUIWidget *pwidget = FindUIWidget( ls, 1 );
	
	if(pwidget)
	{
		int x=0;
		int y=0;

		if(lua_gettop(ls)>2)
		{
			x=lua_tonumber(ls,2);
			y=lua_tonumber(ls,3);
		}
		else 
		{
			const char* name=g_pRootWgt->GetCursorInfo(x,y); 
		}    

		int nIndex=pwidget->ParseDragIndex(x,y,UI_PARSE_EXACT);
		stRect refRect=pwidget->GetWinRect();
		pwidget->ParseTooltip(nIndex,refRect,x,y);
	}
		
	return 0;

}

static int Win_RecoverPathPoint(lua_State * ls)
{
	CUIWidget *pwidget = FindUIWidget( ls, 1 );

	if(pwidget)
	{
		pwidget->OnRecoverPathPoint();
	}
	return 0;
}

static int Win_SetTabCover(lua_State * ls)
{
	if (lua_gettop(ls)< 2)
		return 0;

	CUIWidget * pWidget = FindUIWidget(ls, 1);

	if (pWidget && pWidget->GetType() == WGTTYPE_TABWIN)
	{
		CUITabWin * pTabWin = (CUITabWin*) pWidget;

		const char * pResName = lua_tostring(ls, 2);
		pTabWin->SetTabCover(pResName);
	}

	return 0;
}

void CUIRootWin::AddUIScriptFunc()
{
	if(!m_pScript) return;
    
    //编辑器支持-begin
    m_pScript->RegisterFunc( "Win_AddWidget", Win_AddWidget );
	m_pScript->RegisterFunc( "Win_DeleteWidget", Win_DeleteWidget);
    m_pScript->RegisterFunc( "Win_SaveWidget", Win_SaveWidget );

    m_pScript->RegisterFunc( "Win_GetWidgetInfo",   Win_GetWidgetInfo );
    m_pScript->RegisterFunc( "Win_SetWidgetInfo",   Win_SetWidgetInfo );
    


    m_pScript->RegisterFunc( "Win_GetTextInfo",     Win_GetTextInfo );
    m_pScript->RegisterFunc( "Win_GetImageInfo",    Win_GetImageInfo );


    m_pScript->RegisterFunc( "Win_SetTextInfo",     Win_SetTextInfo );
    m_pScript->RegisterFunc( "Win_SetImageInfo",    Win_SetImageInfo );
    //编辑器支持-end


  	m_pScript->RegisterFunc( "Win_RegisterBuddy", Win_RegisterBuddy);    

	m_pScript->RegisterFunc( "Win_EndWidget",  Win_EndWidget );
	m_pScript->RegisterFunc( "Win_ShowWidget", Win_ShowWidget );
	m_pScript->RegisterFunc( "Win_ToggleWidget", Win_ToggleWidget);
	m_pScript->RegisterFunc( "Win_EnableWidget", Win_EnableWidget );
	m_pScript->RegisterFunc( "Win_AlignWidget", Win_AlignWidget );
	m_pScript->RegisterFunc( "Win_AlignRect", Win_AlignRect );


	m_pScript->RegisterFunc( "Win_ShowTopMost", Win_ShowTopMost);
	m_pScript->RegisterFunc( "Win_HideTopMost", Win_HideTopMost);
	m_pScript->RegisterFunc( "Win_GetFirstTopMost", Win_GetFirstTopMost);
	
	m_pScript->RegisterFunc( "Win_IsExist",Win_IsExist);
	m_pScript->RegisterFunc( "Win_IsFocused",Win_IsFocused);
	m_pScript->RegisterFunc( "Win_IsChildFocused", Win_IsChildFocused);
	m_pScript->RegisterFunc( "Win_IsDescendFocused", Win_IsDescendFocused);
	m_pScript->RegisterFunc( "Win_IsChildHot", Win_IsChildHot);
	m_pScript->RegisterFunc( "Win_IsDescendHot", Win_IsDescendHot);
	m_pScript->RegisterFunc( "Win_IsInDescend", Win_IsInDescend);
	m_pScript->RegisterFunc( "Win_IsAcceptInput", Win_IsAcceptInput);
	
	m_pScript->RegisterFunc( "Win_IsVisible",  Win_IsVisible);
	m_pScript->RegisterFunc( "Win_IsEnabled",  Win_IsEnabled);

	m_pScript->RegisterFunc( "Win_IsVisibleEx",  Win_IsVisibleEx);
	m_pScript->RegisterFunc( "Win_IsEnabledEx",  Win_IsEnabledEx);
	
	m_pScript->RegisterFunc( "Win_IsChecked", Win_IsChecked );
	m_pScript->RegisterFunc( "Win_SetCheck",  Win_SetCheck );

	m_pScript->RegisterFunc( "Win_SetBackPic",  Win_SetBackPic );
	m_pScript->RegisterFunc( "Win_SetCoverPic", Win_SetCoverPic );

	m_pScript->RegisterFunc( "Win_SetCapture", Win_SetCapture);
	m_pScript->RegisterFunc( "Win_SetFocus", Win_SetFocus );
	m_pScript->RegisterFunc( "Win_SetHot", Win_SetHot );
	
	m_pScript->RegisterFunc( "Win_ActiveSelf",Win_ActiveSelf );
	m_pScript->RegisterFunc( "Win_AjustSelf",Win_AjustSelf);
	m_pScript->RegisterFunc( "Win_UpdateSelf",Win_UpdateSelf);

	m_pScript->RegisterFunc( "Win_ArrangeSelf",Win_ArrangeSelf);
	m_pScript->RegisterFunc( "Win_ReAlignChild",Win_ReAlignChild);

	m_pScript->RegisterFunc( "Win_SelectSelf",Win_SelectSelf  );
	m_pScript->RegisterFunc( "Win_GetSelect",Win_GetSelect  );
	m_pScript->RegisterFunc( "Win_SetSelect",Win_SetSelect  );

	m_pScript->RegisterFunc( "Win_GetValue",Win_GetValue);
	m_pScript->RegisterFunc( "Win_SetValue",Win_SetValue);

	m_pScript->RegisterFunc( "Win_GetText",Win_GetText);
	m_pScript->RegisterFunc( "Win_SetText",Win_SetText);

	m_pScript->RegisterFunc( "Win_GetTable",Win_GetTable);
	m_pScript->RegisterFunc( "Win_SetTable",Win_SetTable);
	
	m_pScript->RegisterFunc( "Win_GetSize",Win_GetSize);	
    m_pScript->RegisterFunc( "Win_SetSize",Win_SetSize);
	m_pScript->RegisterFunc( "Win_GetRect",Win_GetRect);	
    m_pScript->RegisterFunc( "Win_SetRect",Win_SetRect);
	m_pScript->RegisterFunc( "Win_GetPos",Win_GetPos);
	m_pScript->RegisterFunc( "Win_SetPos",Win_SetPos);
	m_pScript->RegisterFunc( "Win_MovePos",Win_MovePos);
	m_pScript->RegisterFunc( "Win_RelativePos",Win_RelativePos);

	m_pScript->RegisterFunc( "Win_ExtendTo",Win_ExtendTo);
	m_pScript->RegisterFunc( "Win_ExtendEdge",Win_ExtendEdge);
	

	m_pScript->RegisterFunc( "Win_GetDragInfo",Win_GetDragInfo);
	
	
	m_pScript->RegisterFunc( "Win_OnPaste",Win_OnPaste);
	m_pScript->RegisterFunc( "Win_OnCopy",Win_OnCopy);
	m_pScript->RegisterFunc( "Win_OnCut",Win_OnCut);
	m_pScript->RegisterFunc( "Win_OnInsert",Win_OnInsert);
	
	m_pScript->RegisterFunc( "Win_EditSetFont",Win_EditSetFont);
	m_pScript->RegisterFunc( "Win_EditSetColor",Win_EditSetColor);
	m_pScript->RegisterFunc( "Win_EditGetFont",Win_EditGetFont);
	m_pScript->RegisterFunc( "Win_EditGetColor",Win_EditGetColor);
	m_pScript->RegisterFunc( "Win_SendMessage",Win_SendMessage);
		

	m_pScript->RegisterFunc( "Win_AddTemplate",Win_AddTemplate);
	m_pScript->RegisterFunc( "Win_GetTemplatesNum",Win_GetTemplatesNum);
	m_pScript->RegisterFunc( "Win_ClearTemplates",Win_ClearTemplates);

	m_pScript->RegisterFunc( "Win_GetNumChild",Win_GetNumChild);
	m_pScript->RegisterFunc( "Win_GetChildren",Win_GetChildren);
	
	m_pScript->RegisterFunc( "Win_PushPathPoint",Win_PushPathPoint);
	m_pScript->RegisterFunc( "Win_ClearPathPoint",Win_ClearPathPoint);
	
	m_pScript->RegisterFunc( "Win_IsOutScreen", Win_IsOutScreen);
	m_pScript->RegisterFunc( "Win_PullIntoScreen", Win_PullIntoScreen);
	m_pScript->RegisterFunc( "Win_GetFreeRect", Win_GetFreeRect);
	
	
	m_pScript->RegisterFunc( "Win_AddStyle",Win_AddStyle);
	m_pScript->RegisterFunc( "Win_RemoveStyle",Win_RemoveStyle);

	m_pScript->RegisterFunc( "Win_AddStyleEx",Win_AddStyleEx);
	m_pScript->RegisterFunc( "Win_RemoveStyleEx",Win_RemoveStyleEx);
	
	m_pScript->RegisterFunc( "Win_GetCustomBuffer",Win_GetCustomBuffer);
	m_pScript->RegisterFunc( "Win_SetCustomBuffer",Win_SetCustomBuffer);

	m_pScript->RegisterFunc( "Win_ShowTooltip",Win_ShowTooltip);

	//用于调试界面
	m_pScript->RegisterFunc( "Win_SetEditMode",Win_SetEditMode);
	m_pScript->RegisterFunc( "Win_GetEditMode",Win_GetEditMode);

	m_pScript->RegisterFunc( "Win_ToggleConsole",Win_ToggleConsole);
	m_pScript->RegisterFunc( "Win_ReloadScreen",Win_ReloadScreen);
	m_pScript->RegisterFunc( "Win_DebugScreen",Win_DebugScreen);
	m_pScript->RegisterFunc( "Win_ConsoleInfo",Win_ConsoleInfo  );
	m_pScript->RegisterFunc( "Win_ShowMsgBox",Win_ShowMsgBox );

	m_pScript->RegisterFunc( "Win_RecoverPathPoint", Win_RecoverPathPoint);
	
	m_pScript->RegisterFunc( "Win_GetHotID",Win_GetHotID  );
	m_pScript->RegisterFunc( "Win_GetHotPath",Win_GetHotPath );

	m_pScript->RegisterFunc( "Win_GetFocusID",Win_GetFocusID  );
	m_pScript->RegisterFunc( "Win_GetFocusPath",Win_GetFocusPath );

	m_pScript->RegisterFunc( "Win_GetCaptureID",Win_GetCaptureID  );
	m_pScript->RegisterFunc( "Win_GetCapturePath",Win_GetCapturePath );

	m_pScript->RegisterFunc( "Win_GetMouseWgtID",Win_GetMouseWgtID  );
	m_pScript->RegisterFunc( "Win_GetMouseWgtPath",Win_GetMouseWgtPath );

	m_pScript->RegisterFunc( "Win_GetCursorInfo",Win_GetCursorInfo );
	
	m_pScript->RegisterFunc( "Win_GetTypeNames",Win_GetTypeNames);
	m_pScript->RegisterFunc( "Win_GetSysFuncs",Win_GetSysFuncs);
	m_pScript->RegisterFunc( "Win_GetScreens",Win_GetScreens);
	m_pScript->RegisterFunc( "Win_GetCurScreen",Win_GetCurScreen);
	
	m_pScript->RegisterFunc( "Win_SetTabCover", Win_SetTabCover);

		
	//编辑支持函数	
	m_pScript->RegisterFunc( "Win_GetWidgetNum",Win_GetWidgetNum);
	m_pScript->RegisterFunc( "Win_DumpImage",Win_DumpImage);
	m_pScript->RegisterFunc( "Win_CancelDrag",Win_CancelDrag);

	//脚本支持函数	
	m_pScript->RegisterFunc( "GetColorIndex", GetColorIndex);
	m_pScript->RegisterFunc( "GetBitAnd", GetBitAnd);
	m_pScript->RegisterFunc( "GetBitOR",  GetBitOR);
	m_pScript->RegisterFunc( "GetBitXOR", GetBitXOR);		
	
	
}
