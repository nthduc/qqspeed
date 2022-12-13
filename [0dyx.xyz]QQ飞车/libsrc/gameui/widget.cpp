
#define _WIN32_WINNT 0x400
#include "widget.h"

#include "uicontrol.h"

#include "variant.h"
#include "uiimage.h"
#include "display.h"
#include "script.h"
#include "charstr.h"

#include <algorithm>
#include <deque>
#include <set>
#include <map>
#include "gameui.h"

//��Ҫ��ȫ�ֱ���///////////////////////////////
int UI_MAX_INTEGER=2147483647;//2^31-1,�������
stUICoreStyle g_UICoreStyle;
std::vector<std::string> g_vecScriptFuncs;
CallBackFunc g_vecCallbackFuncs[UI_Callback_MAXNUM] = { NULL };
std::set<IWgtData*> g_vecSecuritySet;
//��Ҫ��ȫ�ֱ���///////////////////////////////
extern float g_fXScale;
extern float g_fYScale;

extern float g_fXUIScale;
extern float g_fYUIScale;

//Ĭ�ϳ���/////////////////////////////////////

const int NORMAL_LAYER_INIT = 100;
const int FOCUS_LAYER_PLUS  = 10000;

const int CONSOLE_LAYER = 0xffff;
const int SYSTOOLTIP_LAYER = 0xfffe;
const int MSGBOX_LAYER = 0xfffd;

const int DEFAULT_TEXT_STYLE=dtCenter|dtVCenter;
const int DEFAULT_TEXT_SIZE=16;
const int DEFAULT_ROW_SPACE=0;

const int DEFAULT_MARGIN_H=0;
const int DEFAULT_MARGIN_V=0;

const int DEFAULT_SNAP_MARGIN_H=10;
const int DEFAULT_SNAP_MARGIN_V=10;

const int DEFAULT_QUERY_INDEX=-1;
const int DEFAULT_DRAG_INDEX=-1;

const double DEFAULT_ALPHA_SPEED=1.0;
const double DEFAULT_PATH_SPEED=10.0;

const double DEFAULT_FRAME_SPEED=0.15;

const int DEFAULT_EDGE_THICK=4;

const int DEFAULT_MOUSE_DELTA=32;

const stColorQuad DEFAULT_DRAW_COLOR=stColorQuad(255,255,255);//��͸������
const stColorQuad DEFAULT_BACK_COLOR=stColorQuad(0,0,0,0);//͸���ڵ�
const stColorQuad DEFAULT_EDGE_COLOR=stColorQuad(0,0,255,0);//͸����ɫ
const stColorQuad DEFAULT_FOCUS_COLOR=stColorQuad(255,255,255);//��͸������
const stColorQuad DEFAULT_MAGIC_COLOR=stColorQuad(0,0,0,255);//��͸���ڱ�
const stColorQuad DEFAULT_LINK_COLOR=stColorQuad(0,0,255,255);//��͸����ɫ
//Ĭ�ϳ���/////////////////////////////////////



void UI_DebugPrint(const char *fmt, ...)
{
	
	char		msg[UI_DEFAULT_BUFSIZE];
	
	va_list		argptr;
	
	va_start (argptr,fmt);
	vsprintf (msg,fmt,argptr);
	va_end (argptr);
	
	OutputDebugString(msg);
	
	g_pRootWgt->AddConsoleInfo(msg);

}


//���ļ�������ģ���ַ���
//ʵ�ʲ���(��һ���ļ�������һ���ַ���)
BOOL LoadTemplateFile(const char *szFileName,std::string& strBuffer)
{
	FILE_V* fp = NULL;
	
	// ���ļ�׼����ȡ
	if(NULL == (fp = vfopen(szFileName, "rb")))
	{
		if(fp) { vfclose(fp); fp = NULL; }
		return FALSE;
	}
	
	size_t length=vfsize(fp);
	char *buffer;
	buffer = new char[length+1];
	vfread(buffer,1,length,fp);	
	buffer[length]=0;
	strBuffer = buffer;	
	delete [] buffer;

	// ��ȡ�ɹ�, �ر��ļ�
	if(fp) { vfclose(fp); fp = NULL;	}	
	return TRUE;
	
	
}





////////////////////////////default consts///////////////////////


extern IDisplay *g_pDisplay;

CUIWidget::CUIWidget()
{
	int i;

	m_Type = WGTTYPE_WIDGET;

	m_pFather = NULL;

	m_pBackPic = NULL;
	m_pCoverPic = NULL;
	m_pEffectPic = NULL;
	m_pCheckCover=NULL;
	m_pHotCover=NULL;
	
	for(i=0;i<PATCHID_NUMBER;i++)
	{
		m_pPatchPic[i] = NULL;
	}

	for(i=0;i<UI_ADORNPIC_NUMBER;i++)
	{
		m_pAdornPic[i] = NULL;
	}

	InsertSecurityPointer(this);
}

CUIWidget::~CUIWidget()
{

	SAFE_RELEASE(m_pBackPic);
	SAFE_RELEASE(m_pCoverPic);
	SAFE_RELEASE(m_pEffectPic);
	SAFE_RELEASE(m_pCheckCover);
	SAFE_RELEASE(m_pHotCover);

	for(int i=0;i<PATCHID_NUMBER;i++)
	{
		SAFE_RELEASE(m_pPatchPic[i]);
	}

	for(int j=0;j<UI_ADORNPIC_NUMBER;j++)
	{
		SAFE_RELEASE(m_pAdornPic[j]);
	}

	m_CustomBuffer.FreeBuffer();

	m_ScriptFuncFlags.clear();
	m_Mornitor.clear();
	m_Accels.clear();	
	m_PathPoints.clear();
	m_vecTemplate.clear();
	
	m_vecFrameScheme.clear();
	m_vecChildRect.clear();
	m_vecWinShape.clear();
	m_vecWakeArea.clear();

	m_BuddyWgts.clear();	
		
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->m_pFather = NULL;
		(*iter)->Release();
	}
	m_Children.clear();

	if(g_pRootWgt) 
		g_pRootWgt->OnDestroyWidget(this);

	EraseSecurityPointer(this);
	
}




void CUIWidget::ClearParameter()
{

	int i;

	m_QueryType=UI_QUERY_TYPE_NONE;
	m_QueryIndex=DEFAULT_QUERY_INDEX;

	m_DragType=UI_DRAG_TYPE_NONE;
	m_DragIndex=DEFAULT_DRAG_INDEX;

	m_HostType=-1;
	m_HostIndex=-1;

	m_AreaId=-1;
	
	m_RectId=-1;

	m_CurFrame = 0;
	m_CurEffectFrame = 0;
	
	for(i=0;i<FRAME_STATE_MAX;i++)
	{
		m_CaptionOffset[i] = Point2D(0,0);
	}

	for(i=0;i<UI_SOUND_MAXNUM;i++)
	{
		m_SoundId[i] = UI_SOUND_NONE;
	}

	
	m_Caption="";

	m_ScriptText="";

	m_TipText="";
	m_TipPos=stRect(0,0,-1,-1);
	m_TipFlag=UI_TIPFLAG_NONE;

	m_WinRect=stRect(0,0,0,0);
	m_DragRect=stRect(0,0,0,0);
	m_ClientRect=stRect(0,0,0,0);
	m_CaptionRect=stRect(0,0,0,0);

	m_BackPicPos=Point2D(0,0);
	m_BackPicPos=Point2D(0,0);
	m_CoverPicPos=Point2D(0,0);
	m_EffectPicPos=Point2D(0,0);
	m_CheckCoverPos=Point2D(0,0);
	m_HotCoverPos=Point2D(0,0);

	m_CursorName=DEFAULT_CURSOR_NAME;
	m_HorizonSizer=DEFAULT_HORIZON_SIZER;
	m_VerticalSizer=DEFAULT_VERTICAL_SIZER;

		
	m_State = WGTSTATE_VISIBLE;
	
	m_Pattern  =  WGTPATTERN_NONE;
	m_Style  =  WGTSTYLE_NONE;
	m_ExtendStyle=  0;
	m_DrawFlag=DRAWFLAG_NONE;

	m_ItemDragFlag=RECT_FLAG_CLIENT;
	m_ItemDropFlag=RECT_FLAG_WINDOW;

	m_CustomState=0;
	m_CustomStyle=0;
	m_CustomId=0;
	m_CustomType=0;
	m_CustomIndex=0;

	m_TemplateId=0;

	m_EdgeCheck=EDGE_CHECK_NONE;

	m_EdgeThick=DEFAULT_EDGE_THICK;
	m_AjustEdge=EDGE_FLAG_NONE;				
	m_AjustFlag=RECT_FLAG_WINDOW;
	m_AjustState=EDGE_FLAG_NONE;

	m_MinHeight=0;
	m_MaxHeight=UI_MAX_INTEGER;
	
	m_MinWidth=0;
	m_MaxWidth=UI_MAX_INTEGER;

	m_MouseDelta=DEFAULT_MOUSE_DELTA;

	//ȱʡ�ɼ�
	m_RenderFactor=1.0;
	
	m_AlphaMin=0.0;
	m_AlphaMax=1.0;
	m_AlphaFactor=1.0;
	m_AlphaState=ALPHA_STATE_PAUSE;


	//ȱʡΪ��ʱ��ʾ�ͼ�ʱ����
	m_AlphaSpeed=DEFAULT_ALPHA_SPEED;

	m_PathSpeed=DEFAULT_PATH_SPEED;

	m_ScaleFactor=1.0;

	//add by lincoln [2006:7:18] [11:54]
	//{{{�����Ч���Ż�
	m_UpdateMuteFlag=UPDATE_MUTE_NONE;//UPDATE_MUTE_VISIBLE;//
	//}}}
	m_bAutoScale = FALSE;
	//idletime:���ھ���tooltip��ʾ�ӳ�ʱ�䣬ȱʡ����һ��
	//mouse inʱ��acceptidle(true);
	//mouse idleʱ������������������
	//idletime>=0:����һ��,acceptidle(false);
	//idletime<0:������,mouse moveʱ��parsetooltip
	m_IdleTime=0.0;

	m_AccumTime=0.0;

	m_TimerSpace=0.0;

	m_PreLayer=0;
	m_InitLayer=NORMAL_LAYER_INIT;
	m_PlusLayer=FOCUS_LAYER_PLUS;
		
	m_GroupId=-1;//belong to none group
	m_GroupStop=-1;//index in a group

	m_TabStop=-1;//none tabstop
	m_ChildStop=-1;//none child fouced
	

	m_AlignStyle = ALIGNSTYLE_NONE;
	m_AlignType = ALIGNTYPE_NONE;
	m_AlignId   = -1;
	m_AlignToId = -1;

	m_MarginH=DEFAULT_MARGIN_H;
	m_MarginV=DEFAULT_MARGIN_V;

	m_SnapH=DEFAULT_SNAP_MARGIN_H;
	m_SnapV=DEFAULT_SNAP_MARGIN_V;

	m_TextFont=0;
	m_TextStyle =DEFAULT_TEXT_STYLE;	
	m_TextSize=DEFAULT_TEXT_SIZE;
	m_RowSpace=DEFAULT_ROW_SPACE;
	m_RichMode=UI_RICHMODE_NONE;
	m_RichFlag=UI_RICHFLAG_NONE;

	m_DrawColor  = DEFAULT_DRAW_COLOR;//��͸������
	m_BackColor    = DEFAULT_BACK_COLOR;//͸���ڵ�
	m_EdgeColor  = DEFAULT_EDGE_COLOR;//͸����ɫ
	m_FocusColor = DEFAULT_FOCUS_COLOR;//͸����ɫ
	m_MagicColor = DEFAULT_MAGIC_COLOR;//��͸���ڱ�
	m_LinkColor  = DEFAULT_LINK_COLOR;//��͸����ɫ

	
	m_CurFrameState=FRAME_STATE_NORMAL;
	m_CurFrameScheme=0;
	m_FrameSpeed=DEFAULT_FRAME_SPEED;
	
	SAFE_RELEASE(m_pBackPic);
	SAFE_RELEASE(m_pCoverPic);
	SAFE_RELEASE(m_pEffectPic);
	SAFE_RELEASE(m_pCheckCover);
	SAFE_RELEASE(m_pHotCover);

	for(i=0;i<PATCHID_NUMBER;i++)
	{
		SAFE_RELEASE(m_pPatchPic[i]);
	}

	for(i=0;i<UI_ADORNPIC_NUMBER;i++)
	{
		SAFE_RELEASE(m_pAdornPic[i]);
	}

	m_CustomBuffer.FreeBuffer();

	m_ScriptFuncFlags.clear();
	m_Mornitor.clear();
	m_Accels.clear();	
	m_PathPoints.clear();
	m_vecTemplate.clear();
	
	m_vecFrameScheme.clear();
	m_vecChildRect.clear();
	m_vecWinShape.clear();
	m_vecWakeArea.clear();
	
	m_BuddyWgts.clear();

	//ʹ�ø����ڵ�tooltip
	RegisterBuddyWgt(BUDDY_WIDGET_TIPWGT, BUDDY_STYLE_ROOT);

	//ʹ����Ļ��infotip
	RegisterBuddyWgt(BUDDY_WIDGET_INFOWGT,BUDDY_STYLE_SCREEN);

	//�����ں���Ļ������Ҫ
	if((m_Type==WGTTYPE_ROOT)||(m_Type==WGTTYPE_SCREEN))
	RegisterBuddyWgt(BUDDY_WIDGET_MSGBOX, BUDDY_STYLE_ROOT);
	
	//��������Ҫ
	if(m_Type==WGTTYPE_ROOT)
	RegisterBuddyWgt(BUDDY_WIDGET_CONSOLE,BUDDY_STYLE_ROOT);
	
}

//�����뺢��֮ǰ�����Լ��Ĳ���
void CUIWidget::SetParameter( CVar *pvar, CUIWidget *pfather )
{

	int tmp[4];
	CVar *pelement=NULL;

	if( (pelement=pvar->Element("initlayer")) )   SetInitLayer( pelement->Integer() );
	if( (pelement=pvar->Element("pluslayer")) )   SetPlusLayer( pelement->Integer() );
	if( (pelement=pvar->Element("prelayer")) )    SetPreLayer( pelement->Integer() );
	if( (pelement=pvar->Element("pattern")) )	  SetPattern( pelement->Integer() );
	if( (pelement=pvar->Element("style")) )		  SetStyle( pelement->Integer() );
	if( (pelement=pvar->Element("extendstyle")) ) SetExtendStyle( pelement->Integer());

	if( (pelement=pvar->Element("customstyle")) ) SetCustomStyle( pelement->Integer() );
	if( (pelement=pvar->Element("customstate")) ) SetCustomState( pelement->Integer());

	if( (pelement=pvar->Element("customid")) )	  SetCustomId( pelement->Integer() );
	if( (pelement=pvar->Element("customtype")) )  SetCustomType( pelement->Integer() );
	if( (pelement=pvar->Element("customindex")) ) SetCustomIndex( pelement->Integer());

	if( (pelement=pvar->Element("customname")) )  SetCustomName( pelement->String() );
	if( (pelement=pvar->Element("customtext")) )  SetCustomText( pelement->String() );
	if( (pelement=pvar->Element("customstring"))) SetCustomString( pelement->String());

	if( (pelement=pvar->Element("edgecheck")) )   SetEdgeCheck( pelement->Integer());
	

	if( (pelement=pvar->Element("soundid")) )	  pelement->IntegerArray(m_SoundId,UI_SOUND_MAXNUM);


	//�����move,size���������style��������
	//��Ϊ���ǻḲ��style������
	//���ұ������captionǰ������,��Ϊlabel��caption���úͷ�����
	if( (pelement=pvar->Element("moveable")) )    EnableMove( pelement->Integer() );
	if( (pelement=pvar->Element("xmoveable")) )   EnableMove( pelement->Integer(),true,false );
	if( (pelement=pvar->Element("ymoveable")) )   EnableMove( pelement->Integer(),false,true );

	if( (pelement=pvar->Element("sizeable")) )    EnableSize( pelement->Integer() );
	if( (pelement=pvar->Element("xsizeable")) )   EnableSize( pelement->Integer(),true,false );
	if( (pelement=pvar->Element("ysizeable")) )   EnableSize( pelement->Integer(),false,true );

	if( (pelement=pvar->Element("pathmove")) )    PausePathMove( pelement->Integer()==0 );
	if( (pelement=pvar->Element("pathflip")) )    FlipPathMove( pelement->Integer()==1 );

	if( (pelement=pvar->Element("editable")) )	  SetEditable( pelement->Integer() );

	if( (pelement=pvar->Element("drawflag")) )	  SetDrawFlag( pelement->Integer() );

	if( (pelement=pvar->Element("bkimage")) )     SetBackPic(  pelement->String() );
	if( (pelement=pvar->Element("coverpic")) )    SetCoverPic( pelement->String() );
	if( (pelement=pvar->Element("effectpic")) )   SetEffectPic( pelement->String() );
	if( (pelement=pvar->Element("checkcover")) )  SetCheckCover( pelement->String() );	
	if( (pelement=pvar->Element("hotcover")) )    SetHotCover( pelement->String() );	
	
	if( (pelement=pvar->Element("captionoffset")) )	  
	{
		
		CVarArray* allOffsets=(CVarArray*)pelement;
		int offNum=allOffsets->GetNum();
		if(offNum>FRAME_STATE_MAX) offNum=FRAME_STATE_MAX;
		
		for(int i=0;i<offNum;i++)
		{ 
			allOffsets->Element(i)->IntegerArray(tmp, 2 );
			SetCaptionOffset( Point2D(tmp[0],tmp[1]), i );
		}
		
	}


	if( (pelement=pvar->Element("patchpic")) )	  
	{
		
		CVarArray* allpics=(CVarArray*)pelement;
		int picNum=allpics->GetNum();
		if(picNum>PATCHID_NUMBER) picNum=PATCHID_NUMBER;
		
		for(int i=0;i<picNum;i++)
		{ 
			SetPatchPic( allpics->Element(i)->String(), i );
		}
		
	}
	else 
	{
	 	if((pelement=pvar->Element("patchupleft")))     SetPatchPic(pelement->String(),PATCHID_UP_LEFT);	
		if((pelement=pvar->Element("patchupright")))    SetPatchPic(pelement->String(),PATCHID_UP_RIGHT);	
		if((pelement=pvar->Element("patchupmiddle")))   SetPatchPic(pelement->String(),PATCHID_UP_MIDDLE);	
		if((pelement=pvar->Element("patchdownleft")))   SetPatchPic(pelement->String(),PATCHID_DOWN_LEFT);	
		if((pelement=pvar->Element("patchdownright")))  SetPatchPic(pelement->String(),PATCHID_DOWN_RIGHT);	
		if((pelement=pvar->Element("patchdownmiddle"))) SetPatchPic(pelement->String(),PATCHID_DOWN_MIDDLE);
		if((pelement=pvar->Element("patchleftmiddle"))) SetPatchPic(pelement->String(),PATCHID_LEFT_MIDDLE);
		if((pelement=pvar->Element("patchrightmiddle")))SetPatchPic(pelement->String(),PATCHID_RIGHT_MIDDLE);
		if((pelement=pvar->Element("patchmiddle")))		SetPatchPic(pelement->String(),PATCHID_MIDDLE);

	}

	if( (pelement=pvar->Element("adornpic")) )	  
	{
		
		CVarArray* allpics=(CVarArray*)pelement;
		int picNum=allpics->GetNum();
		if(picNum>UI_ADORNPIC_NUMBER) picNum=UI_ADORNPIC_NUMBER;
		
		for(int i=0;i<picNum;i++)
		{ 
			SetAdornPic( allpics->Element(i)->String(), i );
		}
		
	}

	//����ڴ�й©
	if( (pelement=pvar->Element("patchrect")) )
	{
		pelement->IntegerArray( tmp, 4 );		
		SetPatchRect( tmp[0], tmp[1], tmp[2], tmp[3] );
	}

	if( (pelement=pvar->Element("adornpicpos")) )   
	{
		CVarArray* allpts=(CVarArray*)pelement;
		int ptNum=allpts->GetNum();
		if(ptNum>UI_ADORNPIC_NUMBER) ptNum=UI_ADORNPIC_NUMBER;
		
		for(int i=0;i<ptNum;i++)
		{ 
			allpts->Element(i)->IntegerArray(tmp, 2 );
			m_AdornPicPos[i].x=tmp[0];
			m_AdornPicPos[i].y=tmp[1];			
		}
	}
	

	if( (pelement=pvar->Element("cursor")) )	  SetCursorName( pelement->String() );
	if( (pelement=pvar->Element("horizonsizer")) )SetHorizonSizer( pelement->String() );
	if( (pelement=pvar->Element("verticalsizer")) )SetVerticalSizer( pelement->String() );

	//������״̬������	
	if( (pelement=pvar->Element("groupid")) )	  SetGroupId( pelement->Integer() );
	if( (pelement=pvar->Element("groupstop")) )   SetGroupStop( pelement->Integer() );
	if( (pelement=pvar->Element("tabstop")) )	  SetTabStop( pelement->Integer() );
	if( (pelement=pvar->Element("childstop")) )   SetChildStop( pelement->Integer() );

	if( (pelement=pvar->Element("querytype")) )	  SetQueryType( pelement->Integer() );	
	if( (pelement=pvar->Element("queryindex")) )  SetQueryIndex( pelement->Integer() );	

	if( (pelement=pvar->Element("dragtype")) )	  SetDragType( pelement->Integer() );	
	if( (pelement=pvar->Element("dragindex")) )   SetDragIndex( pelement->Integer() );	

	if( (pelement=pvar->Element("hosttype")) )	  SetHostType( pelement->Integer() );	
	if( (pelement=pvar->Element("hostindex")) )   SetHostIndex( pelement->Integer() );	

	if( (pelement=pvar->Element("areaid")) )	  SetAreaId( pelement->Integer() );	

	if( (pelement=pvar->Element("visible")) )     
	{
		//add by lincoln [2006:7:20] [14:42]
		//{{{�����Ч���Ż� 
		if(pelement->Integer()==0) m_RenderFactor=0.0f;
		//}}}

		ShowWidget( pelement->Integer() );
	}
	if( (pelement=pvar->Element("enable")) )      EnableWidget( pelement->Integer() );
	if( (pelement=pvar->Element("checked")) )     SetChecked( pelement->Integer()!=0 );
	if( (pelement=pvar->Element("dirty")) )       SetUpdateDirty( pelement->Integer()!=0 );

	if( (pelement=pvar->Element("textstyle")))	  SetTextStyle( pelement->Integer() );
	if( (pelement=pvar->Element("textsize")))	  SetTextSize( pelement->Integer() );
	if( (pelement=pvar->Element("textfont")))	  SetTextFont( pelement->Integer() );
	if( (pelement=pvar->Element("richmode")) )	  SetRichMode( pelement->Integer() );
	if( (pelement=pvar->Element("richflag")) )	  SetRichFlag( pelement->Integer() );
	if( (pelement=pvar->Element("rowspace")))	  SetRowSpace( pelement->Integer() );
	if( (pelement=pvar->Element("marginh")) )	  SetMarginH( pelement->Integer() );
	if( (pelement=pvar->Element("marginv")) )	  SetMarginV( pelement->Integer() );
	if( (pelement=pvar->Element("snaph")) )		  SetSnapH( pelement->Integer() );
	if( (pelement=pvar->Element("snapv")) )		  SetSnapV( pelement->Integer() );
	if( (pelement=pvar->Element("tiptext")) )	  SetTipText( pelement->String() );
	if( (pelement=pvar->Element("tipflag")) )	  SetTipFlag( pelement->Integer() );
	if( (pelement=pvar->Element("tippos")) )   
	{
		pelement->IntegerArray( tmp, 4 );		
		SetTipPos( tmp[0], tmp[1], tmp[2], tmp[3] );
	}

	//add by lincoln [2006:7:19] [11:37]
	//{{{�����Ч���Ż�-��ʼ������͸����
	if( (pelement=pvar->Element("renderfactor")) ) m_RenderFactor=pelement->Double();
	//}}}
	if( (pelement=pvar->Element("alphastate")) )  SetAlphaState(  pelement->Double() );
	if( (pelement=pvar->Element("alphaspeed")) )  SetAlphaSpeed(  pelement->Double() );
	if( (pelement=pvar->Element("alphafactor")) ) SetAlphaFactor( pelement->Double() );
	if( (pelement=pvar->Element("scalefactor")) ) SetScaleFactor( pelement->Double() );
	if( (pelement=pvar->Element("autoscale")) )	  SetAutoScale( pelement->Integer() );

	if( (pelement=pvar->Element("framespeed")) )  SetFrameSpeed( pelement->Double() );
	
	if( (pelement=pvar->Element("pathspeed")) )   SetPathSpeed(pelement->Double());

	if( (pelement=pvar->Element("ajustflag")) )   SetAjustFlag( pelement->Integer() );
	if( (pelement=pvar->Element("ajustedge")) )   SetAjustEdge( pelement->Integer() );
	if( (pelement=pvar->Element("edgethick")) )   SetEdgeThick( pelement->Integer() );

	if( (pelement=pvar->Element("minheight")) )   SetMinHeight( pelement->Integer() );
	if( (pelement=pvar->Element("maxheight")) )   SetMaxHeight( pelement->Integer() );
	if( (pelement=pvar->Element("minwidth")) )    SetMinWidth( pelement->Integer() );
	if( (pelement=pvar->Element("maxwidth")) )    SetMaxWidth( pelement->Integer() );
	
	if( (pelement=pvar->Element("mousedelta")) )  SetMouseDelta( pelement->Integer() );

	//add by lincoln [2006:7:18] [17:58]
	//{{{�����Ч���Ż�
	if( (pelement=pvar->Element("updatemute")) )  SetUpdateMuteFlag( pelement->Integer() );
	//}}}

	if( (pelement=pvar->Element("custombuffer"))) 
	{	
		m_CustomBuffer.FreeBuffer();

		m_CustomBuffer=stCustomBuffer(pelement->Integer());
	}

	if( (pelement=pvar->Element("buffervalue"))) 
	{					
		ExecCallBackFunc(UI_Callback_ParseCustomBuffer,(void*)pelement);
	}	

	if( (pelement=pvar->Element("idletime")) ) 
	{
		SetIdleTime( pelement->Double() );		
	}
	
	//���ڵĶ�������(����Ķ����ţ�ָ�����봰�ڵĶ����ţ��������ͣ�������)
	if( (pelement=pvar->Element("alignid")) )     SetAlignId(pelement->Integer());	
	if( (pelement=pvar->Element("aligntoid")) )   SetAlignToId(pelement->Integer());	
	if( (pelement=pvar->Element("aligntype")) )	  SetAlignType(pelement->Integer());	
	if( (pelement=pvar->Element("alignstyle")) )  SetAlignStyle(pelement->Integer());

	if((pelement=pvar->Element("itemdragflag")))	SetItemDragFlag(pelement->Integer());
	if((pelement=pvar->Element("itemdropflag")))	SetItemDropFlag(pelement->Integer());


	//���ô��ڵ��϶�����
	if( (pelement=pvar->Element("edgeup")) )	
	{		
		CVarArray* pLineArray=(CVarArray*)pelement;		
		for(int i=0;i<pLineArray->GetNum();i++)
		{ 
			pLineArray->Element(i)->IntegerArray( tmp, 2 );			
			m_EdgeLines[EDGE_LINE_UP].m_vecPoints.push_back(Point2D(tmp[0],tmp[1]));
		}
	}

	if( (pelement=pvar->Element("edgedown")) )	
	{		
		CVarArray* pLineArray=(CVarArray*)pelement;		
		for(int i=0;i<pLineArray->GetNum();i++)
		{ 
			pLineArray->Element(i)->IntegerArray( tmp, 2 );			
			m_EdgeLines[EDGE_LINE_DOWN].m_vecPoints.push_back(Point2D(tmp[0],tmp[1]));
		}
	}

	if( (pelement=pvar->Element("edgeleft")) )	
	{		
		CVarArray* pLineArray=(CVarArray*)pelement;		
		for(int i=0;i<pLineArray->GetNum();i++)
		{ 
			pLineArray->Element(i)->IntegerArray( tmp, 2 );			
			m_EdgeLines[EDGE_LINE_LEFT].m_vecPoints.push_back(Point2D(tmp[0],tmp[1]));
		}
	}

	if( (pelement=pvar->Element("edgeright")) )	
	{		
		CVarArray* pLineArray=(CVarArray*)pelement;		
		for(int i=0;i<pLineArray->GetNum();i++)
		{ 
			pLineArray->Element(i)->IntegerArray( tmp, 2 );			
			m_EdgeLines[EDGE_LINE_RIGHT].m_vecPoints.push_back(Point2D(tmp[0],tmp[1]));
		}
	}

	
	if( (pelement=pvar->Element("winshape")) )	
	{
		
		CVarArray* pRectArray=(CVarArray*)pelement;
		
		int pParams[SHAPE_PARAM_MAX+2];
		memset(pParams,0,(SHAPE_PARAM_MAX+2)*sizeof(int));

		for(int i=0;i<pRectArray->GetNum();i++)
		{ 
			pRectArray->Element(i)->IntegerArray( pParams, SHAPE_PARAM_MAX+2 );
			m_vecWinShape.push_back(stShape(pParams[0],(pParams[1]!=0),pParams+2)); 			
		}

	}


	if( (pelement=pvar->Element("childrect")) )	
	{
		
		CVarArray* pRectArray=(CVarArray*)pelement;
		
		for(int i=0;i<pRectArray->GetNum();i++)
		{ 
			pRectArray->Element(i)->IntegerArray( tmp, 4 );
			m_vecChildRect.push_back(stRect(tmp[0],tmp[1],tmp[2],tmp[3])); 			
		}

	}


	//rectid������rect֮ǰ����
	if( (pelement=pvar->Element("rectid")) )     SetRectId(pelement->Integer());	

	//���ڵ����������lua�ű�����ox,oy,width,height��ʽ��
	//�ڳ�����ת��Ϊleft,top,right,bottom��ʽ
	if( (pelement=pvar->Element("rect")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetRelativePos( tmp[0], tmp[1], tmp[0]+tmp[2], tmp[1]+tmp[3], pfather );//ע��˴������ã��ڽű���ʵ���ǿ�Ⱥ͸߶�

		if((tmp[2]<0)||(tmp[3]<0))
			FitBkImageSize();
		
	}
	else if((m_RectId>=0)&&pfather)
	{
		stRect rcChild=pfather->GetChildRect(m_RectId);
		SetRelativePos( rcChild.ox, rcChild.oy, rcChild.ox+rcChild.width, rcChild.oy+rcChild.height, pfather );
	}
	else
	{
	
		//δָ�����ڴ�С,��ʹ�ñ���ͼƬ�Ĵ�С
		FitBkImageSize();
	}
		

	//����ڴ���ԭ����϶�����
	if( (pelement=pvar->Element("dragrect")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetDragRect( tmp[0], tmp[1], tmp[2], tmp[3] );
	}
	else 
		DefaultDragRect();


	if( (pelement=pvar->Element("clientrect")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetClientRect(  tmp[0], tmp[1], tmp[2], tmp[3] );
	}
	else 	
		DefaultClientRect();
	
	
	if( (pelement=pvar->Element("captionrect")))
	{
		pelement->IntegerArray( tmp, 4 );
		SetCaptionRect( tmp[0], tmp[1], tmp[2], tmp[3] );
	}
	else 
		DefaultCaptionRect();

	//////////////////////////////////////////////////////////////////////////

	if( (pelement=pvar->Element("bkimagepos")) )   
	{
		int nSize=pelement->IntegerArray( tmp, 4 );		
		if(nSize>2)
		{
			if(tmp[2]<0) tmp[0]+=m_WinRect.width*0.5;
			if(tmp[3]<0) tmp[1]+=m_WinRect.height*0.5;
		}
			
		SetBackPicPos( tmp[0], tmp[1] );
	}

	if( (pelement=pvar->Element("coverpicpos")) )   
	{
		int nSize=pelement->IntegerArray( tmp, 4 );
		if(nSize>2)
		{
			if(tmp[2]<0) tmp[0]+=m_WinRect.width*0.5;
			if(tmp[3]<0) tmp[1]+=m_WinRect.height*0.5;
		}		

		SetCoverPicPos( tmp[0], tmp[1] );
	}

	if( (pelement=pvar->Element("effectpicpos")) )   
	{
		int nSize=pelement->IntegerArray( tmp, 4 );
		if(nSize>2)
		{
			if(tmp[2]<0) tmp[0]+=m_WinRect.width*0.5;
			if(tmp[3]<0) tmp[1]+=m_WinRect.height*0.5;
		}
		
		SetEffectPicPos( tmp[0], tmp[1] );
	}
	
	if( (pelement=pvar->Element("checkcoverpos")) )  
	{
		int nSize=pelement->IntegerArray( tmp, 4 );
		if(nSize>2)
		{
			if(tmp[2]<0) tmp[0]+=m_WinRect.width*0.5;
			if(tmp[3]<0) tmp[1]+=m_WinRect.height*0.5;
		}

		m_CheckCoverPos.x=tmp[0];
		m_CheckCoverPos.y=tmp[1];
	}

	if( (pelement=pvar->Element("hotcoverpos")) )  
	{
		int nSize=pelement->IntegerArray( tmp, 4 );
		if(nSize>2)
		{
			if(tmp[2]<0) tmp[0]+=m_WinRect.width*0.5;
			if(tmp[3]<0) tmp[1]+=m_WinRect.height*0.5;
		}

		m_HotCoverPos.x=tmp[0];
		m_HotCoverPos.y=tmp[1];
	}


	//////////////////////////////////////////////////////////////////////////
	



	
	if( (pelement=pvar->Element("bkcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetBackColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("drawcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetDrawColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("focuscolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetFocusColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("edgecolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetEdgeColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("magiccolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetMagicColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}

	if( (pelement=pvar->Element("linkcolor")) )
	{
		pelement->IntegerArray( tmp, 4 );
		SetLinkColor( stColorQuad(tmp[0], tmp[1], tmp[2], tmp[3]) );
	}


	//ע��:����label������,caption��Ҫ������rect,captionrect,style,richmode����
	if( (pelement=pvar->Element("caption")) ) SetCaption( pelement->String() );


	if( (pelement=pvar->Element("wakearea")) )	
	{

		//���뾯������
		CVarArray* pRectArray=(CVarArray*)pelement;
		
		for(int i=0;i<pRectArray->GetNum();i++)
		{ 
			pRectArray->Element(i)->IntegerArray( tmp, 4 );
			m_vecWakeArea.push_back(stRect(tmp[0],tmp[1],tmp[2],tmp[3])); 			
		}

	}


	
	if( (pelement=pvar->Element("template")) )	
	{

		//����ģ��
		CVarArray* pTemplArray=(CVarArray*)pelement;
		
		for(int i=0;i<pTemplArray->GetNum();i++)
		{ 
			std::string tmplStr="";
			LoadTemplateFile(pTemplArray->Element(i)->String(),tmplStr);
			m_vecTemplate.push_back(tmplStr);
		}


	}

	//��ӿ�ݼ�
	if( (pelement=pvar->Element("accel")) )	
	{
		
		CVarArray* Accels=(CVarArray*)pelement;
		
		for(int i=0;i<Accels->GetNum();i++)
		{ 
			CVarArray *pAccelVar=(CVarArray*)(Accels->Element(i));		

			stAccelItem accel;
			
			if(pAccelVar->GetNum()>0) sprintf(accel.accel_script,((CVar*)pAccelVar->Element(0))->String());
			if(pAccelVar->GetNum()>1) accel.accel_key=(char)(((CVar*)pAccelVar->Element(1))->Integer());

			if(pAccelVar->GetNum()>2) accel.modifier_shift=(((CVar*)pAccelVar->Element(2))->Integer()!=0);
			if(pAccelVar->GetNum()>3) accel.modifier_ctrl=(((CVar*)pAccelVar->Element(3))->Integer()!=0);
			if(pAccelVar->GetNum()>4) accel.modifier_alt=(((CVar*)pAccelVar->Element(4))->Integer()!=0);

			if(pAccelVar->GetNum()>5) accel.accel_type=((CVar*)pAccelVar->Element(5))->Integer();
			
			m_Accels.push_back(accel);	
			
		}
		
		
		
	}



	if( (pelement=pvar->Element("framescheme")) ) 
	{
		
		
		CVarArray* pAllSchemes=(CVarArray* )pelement;
		
		
		int schemeNum=pAllSchemes->GetNum();
		if(schemeNum>FRAME_SCHEME_MAX) schemeNum=FRAME_SCHEME_MAX;
		
		m_vecFrameScheme.clear();
	
		for(int i=0;i<schemeNum;i++)
		{ 	
			stFrameScheme tmpScheme;
			
			
			CVarArray* pArray=(CVarArray*)(pAllSchemes->Element(i));
			

			//��׼д��
			//framescheme={{0,5,6,8,9,11,12,16,},{17,20,21,28,29,37,38,40,}},
			if(pArray->GetNum()>=FRAME_STATE_MAX*2)
			{
				int schemeValue[FRAME_STATE_MAX*2];
				pArray->IntegerArray(schemeValue,FRAME_STATE_MAX*2);
				
				for(int j=0;j<FRAME_STATE_MAX;j++)
				{ 
					tmpScheme.end[j]=schemeValue[j*2+1];
					tmpScheme.start[j]=schemeValue[j*2];
				}
			}
			//���д��
			//framescheme={{0,1,2,3,},{4,5,6,7,}},
			else if(pArray->GetNum()>=FRAME_STATE_MAX)
			{
				
				int schemeValue[FRAME_STATE_MAX];
				pArray->IntegerArray(schemeValue,FRAME_STATE_MAX);
				
				for(int j=0;j<FRAME_STATE_MAX;j++)
				{ 
					tmpScheme.end[j]=schemeValue[j];
					tmpScheme.start[j]=schemeValue[j];
				}
				

			}
			//���д��
			//framescheme={{0},{1}},
			else
			{
				int schemeValue[1];
				pArray->IntegerArray(schemeValue,1);
				
				for(int j=0;j<FRAME_STATE_MAX;j++)
				{ 
					tmpScheme.end[j]=schemeValue[0];
					tmpScheme.start[j]=schemeValue[0];
				}
			
			}

			
			m_vecFrameScheme.push_back(tmpScheme);
		}
		
		
	}
	else 
	{
	 
			DefaultFrameScheme();
	}

	if( (pelement=pvar->Element("curscheme")) )	
		SetFrameScheme(pelement->Integer());


	if( (pelement=pvar->Element("pathpoints")) ) 
	{
		
		CVarArray* pAllPathPts=(CVarArray*)pelement;
		
		m_PathPoints.clear();
		for(int i=0;i<pAllPathPts->GetNum();i++)
		{ 
		
			CVarArray* pPtVar=((CVarArray*)(pAllPathPts->Element(i)));

			if(pPtVar->GetNum()>4)
			{

				stUIPathPoint pt;

				pt.id=m_PathPoints.size();
			
				pt.x=((CVar*)pPtVar->Element(0))->Integer();
				pt.y=((CVar*)pPtVar->Element(1))->Integer();

				pt.command=((CVar*)pPtVar->Element(2))->Integer();
				pt.alpha=((CVar*)pPtVar->Element(3))->Float();
				pt.scale=((CVar*)pPtVar->Element(4))->Float();
				
				m_PathPoints.push_back(pt);

			}
		}
		
	}
	


	
	//��ӱ��������
	if( (pelement=pvar->Element("mornitor")) )  
	{
		m_Mornitor.clear();

		CVarArray* MornWgts=(CVarArray*)pelement;		
		for( int i=0; i<MornWgts->GetNum(); i++ )
		{
			tmp[0]=tmp[1]=0;

			CVar* pMornVar=MornWgts->Element(i);
			if(pMornVar->GetType()==VAR_FLOAT)
				tmp[0]=pMornVar->Integer();
			else
				pMornVar->IntegerArray( tmp, 2 );


			m_Mornitor[ tmp[0] ]=stUIVar(tmp[0],tmp[1],-1.0,0);		

		}
		
	}	

	//���ÿؼ��ű������ı�־
	ConfirmScripts( pvar );
	

}

//�����뺢��֮�������Լ��Ĳ���
void CUIWidget::PostSetParameter( CVar *pvar, CUIWidget *pfather )
{

	CVar *pelement=NULL;

	//���������鴰��
	if( (pelement=pvar->Element("buddy")) )	
	{
		CVarArray* SysWgts=(CVarArray*)pelement;
		
		for( int i=0; i<SysWgts->GetNum(); i++ )
		{
			CVar *pBuddyVar = (CVar *)(SysWgts->Element(i));

			CVar *pTypeVar=pBuddyVar->Element("type");
			CVar *pStyleVar=pBuddyVar->Element("style");
			CVar *pIdVar=pBuddyVar->Element("id");

			if(pTypeVar&&pStyleVar)
			{
				if(pIdVar)
					RegisterBuddyWgt(pTypeVar->Integer(),pStyleVar->Integer(),pIdVar->String() );
				else
					RegisterBuddyWgt(pTypeVar->Integer(),pStyleVar->Integer());
			}
			
		}
		
	}

	if((pelement=pvar->Element("tipwidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_TIPWGT,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("msgboxwidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_MSGBOX,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("consolewidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_CONSOLE,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("hostwidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_HOST,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("infowidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_INFOWGT,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("buddywidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,pelement->String());
	}
	if((pelement=pvar->Element("buddywidgetsecond")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_BUDDY_SECOND,BUDDY_STYLE_SELF,pelement->String());
	}
	if((pelement=pvar->Element("primaryscroll")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("secondscroll")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,pelement->String());
	}
	
	if((pelement=pvar->Element("blockwidget")))
	{
		RegisterBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON,BUDDY_STYLE_SELF,pelement->String());
	}
	

	//���к���ʹ���Լ���tooltip
	if((pelement=pvar->Element("fathertooltip")))
	{
		std::vector<CUIWidget *>::iterator iter;
		for(iter=m_Children.begin(); iter!=m_Children.end(); iter++)
		{
			(*iter)->RegisterBuddyWgt(BUDDY_WIDGET_TIPWGT,BUDDY_STYLE_FATHER);
		}

	}

	
}


CUIWidget* CUIWidget::GetSafetBuddyWgt(int type)
{

	CUIWidget* pWgt=GetBuddyWgt(type);
	
	if(IsSecurityPointer(pWgt))
		return pWgt;

	return NULL;
	
}


//�������в���,�ҵ��˶�Ӧ����,���Ƿ�������ָ��,û��������.���ظ�ָ��.
//ÿ�ε��øú���ʱ��,��ñ����ָ��
//����buddy�϶�ʱ��,��ʱ.
CUIWidget* CUIWidget::GetBuddyWgt(int type)
{
	//��������ڸ�type��Ӧ��stBuddyInfo,�Ὠ��һ��ȱʡ��stBuddyInfo,����·��,ָ��Ϊ�� 
	stBuddyInfo& buddyInfo=m_BuddyWgts[type];

	//ָ�����
	if(buddyInfo.pBuddy) 
		return buddyInfo.pBuddy;
	
	bool bSelf=(buddyInfo.path!="");
	bool bFather=(buddyInfo.style&BUDDY_STYLE_FATHER)!=0;
	bool bScreen=(buddyInfo.style&BUDDY_STYLE_SCREEN)!=0;
	bool bRoot=(buddyInfo.style&BUDDY_STYLE_ROOT)!=0;

	//·������
	if(bSelf)
	{
		//���Լ��ĺ����в���
		buddyInfo.pBuddy=g_pRootWgt->FindRootWidgetByID(buddyInfo.path);
		
		if(buddyInfo.pBuddy) 
			return buddyInfo.pBuddy;
	}


	if(m_Type==WGTTYPE_SCREEN)
		bScreen=false;

	if(m_Type==WGTTYPE_ROOT)
	{
		bFather=false;
		bScreen=false;
		bRoot=false;
	}
	

	if(m_pFather && bFather)
	{
		//�ڸ��׵ĺ����в���
		buddyInfo.pBuddy=m_pFather->GetSafetBuddyWgt(type);
		
		if(buddyInfo.pBuddy) 
			return buddyInfo.pBuddy;

		if(m_pFather->GetType()==WGTTYPE_SCREEN)
			bScreen=false;

		if(m_pFather->GetType()==WGTTYPE_ROOT)
		{
			bScreen=false;
			bRoot=false;
		}

	}

	CUIWidget* pScreen=g_pRootWgt->GetCurScreen();
	if(pScreen && bScreen)
	{
		//����Ļ�ĺ����в���
		buddyInfo.pBuddy=pScreen->GetSafetBuddyWgt(type);
		
		if(buddyInfo.pBuddy) 
			return buddyInfo.pBuddy;		

	}

	if(g_pRootWgt && bRoot)
	{
		//�ڸ��ĺ����в���
		buddyInfo.pBuddy=g_pRootWgt->GetSafetBuddyWgt(type);
		
		if(buddyInfo.pBuddy) 
			return buddyInfo.pBuddy;

	}
	
	return NULL;
	
}


//ע���Լ��Ļ�鴰��,�Ȳ�����ָ��,ֻ����GetSafetBuddyWgt��ʱ������
//id��������·��id
//����:����root.gameui,����infotip����ע��Ϊ:UI.gameui.infotip
void CUIWidget::RegisterBuddyWgt(int type,int style,const char* pathid/*=""*/,CUIWidget* pBuddy/*=NULL*/)
{
	m_BuddyWgts[type]=stBuddyInfo(style,pathid,pBuddy);
	
}


///////////////////////////////SystemWidget-end//////////////////////////////



void CUIWidget::AddChild( CUIWidget *pwidget )
{
	assert(pwidget);
	if(pwidget)
	{

		m_Children.push_back(pwidget);
		pwidget->IncRefCount();
		pwidget->m_pFather = this;

		//���ϵ���ܱ任
		SetSorted(false);

	}

}

void CUIWidget::DelChild( CUIWidget *pwidget )
{
	assert( pwidget);
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		if( (*iter) == pwidget )
		{
			pwidget->m_pFather = 0;
			pwidget->Release();
			iter=m_Children.erase(iter);
			return;
		}
	}
	assert(0);
}


void CUIWidget::FormatTemplString(int idx,std::string& bufTemplate)
{
		
	
	std::string oldid=DEFAULT_TEMPLATE_KEY;

	char buffer[UI_TEMPORARY_BUFSIZE];
	sprintf(buffer,"%d",idx);
	std::string newid=buffer;
	
	std::string::size_type oldlen=oldid.size();
	std::string::size_type newlen=newid.size();
	
	std::string::size_type endIdx = 0;
	std::string::size_type begIdx = bufTemplate.find(oldid);	
	while ( begIdx!= std::string::npos) 
	{
		bufTemplate.replace(begIdx,oldlen,newid);
		
		endIdx = begIdx+newid.size();
		
		begIdx=bufTemplate.find(oldid,endIdx);
	}
	

}


void CUIWidget::GetTemplString(int id,std::string& resultStr)
{
	if((id<0)||(id>=m_vecTemplate.size()))
		return;

	resultStr=m_vecTemplate[id];

}

int CUIWidget::GetTemplStringNum()
{
	return m_vecTemplate.size();
}

int  CUIWidget::GetTemplatesNum(int id)
{
	int num=0;

	std::vector<CUIWidget *>::iterator iter;
	for(iter=m_Children.begin();iter!=m_Children.end();iter++)
	{
		if(((*iter)->IsTemplateWgt())&&((*iter)->GetTemplateId()==id))
		{
			num++;
		}
	}

	return num;

}


void CUIWidget::ClearTemplates(int id)
{
	
	std::vector<CUIWidget *>::iterator iter;
	iter=m_Children.begin(); 
	while( iter!=m_Children.end() )
	{

		if(((*iter)->IsTemplateWgt())&&((*iter)->GetTemplateId()==id))
		{
			(*iter)->OnDenit();
			
			(*iter)->m_pFather = NULL;

			(*iter)->Release();
			iter=m_Children.erase(iter);
		}
		else 
		{
			iter++;
		}

	}

}


CUIWidget* CUIWidget::AddTemplate(int id)
{
	int num=GetTemplatesNum(id);
	return AddTemplate(id,num);
}


CUIWidget* CUIWidget::AddTemplate(int id,int index)
{
	
	if(GetTemplStringNum()<=0) 
		return NULL;

	std::string bufTemplate="";
	GetTemplString(id,bufTemplate);	
	FormatTemplString(index,bufTemplate);

	if(bufTemplate=="")
		return NULL;

	CScriptState* pScript=g_pRootWgt->GetScriptEnv();

	//ȡ��һ��"="ǰ����ַ���,�����пո�
	std::string childid= bufTemplate.substr(0,bufTemplate.find("="));

	//����Ƿ��Ѿ������������
	CUIWidget *pchild=FindChildById(childid.c_str());

	if(pchild)	
		return pchild;
	

	//�õ�Ҫִ�е������ű��ַ���
	std::string bufScript=this->GetWidgetPathID();
	bufScript+=".children.";

	std::string pathid=bufScript+childid;
	bufScript+=bufTemplate;	
	
	//���촰�ڽű�����
	pScript->DoString(bufScript.c_str());

	//��ȡ�ű�����
	CVar* pchildvar=CVar::CreateVarFromPath( pScript, pathid.c_str());
	
	if(!pchildvar)
		return NULL;

	//�ӽű������봰��
	if(!(pchild = LoadUIWidget( childid.c_str(), pchildvar, this)))
	{
		delete pchildvar;
		return NULL;
	}
	delete pchildvar;

	pchild->SetTemplateId(id);

	//��Ӵ���
	this->AddChild( pchild );
	pchild->OnLoadRecursive();
	pchild->OnInitRecursive();
	pchild->Release();

	return pchild;


}


void CUIWidget::SetCaption( const char *pname )
{
	assert(pname);
	m_Caption = pname;
}

const char * CUIWidget::GetCaption()
{ 
	return m_Caption.c_str(); 
}

void CUIWidget::SetTipPos( int ox, int oy, int alignType, int alignStyle)
{
	m_TipPos=stRect(ox,oy,alignType,alignStyle);
}

void CUIWidget::SetTipText( const char *pTip, int nIndex/*=-1*/)
{
	assert(pTip);
	m_TipText = pTip;
}

const char* CUIWidget::GetTipText(int nIndex)
{
	return m_TipText.c_str(); 
}

void CUIWidget::SetCursorName(const char *pCursor)
{

	assert(pCursor);
	m_CursorName=pCursor;

}

const char* CUIWidget::GetCursorName()
{
	return m_CursorName.c_str();
}

void CUIWidget::SetHorizonSizer(const char *pSizer)
{
	assert(pSizer);
	m_HorizonSizer=pSizer;

}


void CUIWidget::SetVerticalSizer(const char *pSizer)
{
	assert(pSizer);
	m_VerticalSizer=pSizer;

}

const char* CUIWidget::GetHorizonSizer()
{
	return m_HorizonSizer.c_str();
}

const char* CUIWidget::GetVerticalSizer()
{
	return m_VerticalSizer.c_str();
}


void CUIWidget::SetScaleFactor(double factor)
{
	m_ScaleFactor=Clamp(0.0,1.0,factor);
}

void CUIWidget::SetAutoScale(BOOL Value)
{
	m_bAutoScale = Value;
}

void CUIWidget::SetAlphaFactor(double factor)
{
	m_AlphaFactor=Clamp(0.0,1.0,factor);
}

void CUIWidget::SetAlphaMin(double factor)
{
	m_AlphaMin=Clamp(0.0,1.0,factor);
}

void CUIWidget::SetAlphaMax(double factor)
{
	m_AlphaMax=Clamp(0.0,1.0,factor);
}


void CUIWidget::SetAlphaSpeed(double speed)
{
	m_AlphaSpeed=Clamp(0.0,1.0,speed);
}


void CUIWidget::SetAsStaticWidget(BOOL bStatic)
{
	if(bStatic)
		m_Style|=WGTSTYLE_STATIC;
	else 
		m_Style&=~WGTSTYLE_STATIC;
	
}


void CUIWidget::SetAsTemplateWgt(BOOL bTemplate)
{

	if(bTemplate)
		m_Style|=WGTSTYLE_TEMPLATE;
	else 
		m_Style&=~WGTSTYLE_TEMPLATE;
	
}

CUIWidget* CUIWidget::StaticQueryFather()
{
	CUIWidget* pWgt=this;
	while(pWgt) 
	{ 
		if(pWgt->IsVisible()&&pWgt->IsEnabled()&&(!pWgt->IsStaticWidget()))
			return pWgt;
		else
			pWgt=pWgt->GetFather();
	}

	return NULL;

}


void CUIWidget::SetEditable(BOOL bEdit)
{
	if( bEdit ) m_Style|= WGTSTYLE_EDITABLE;
	else m_Style&= ~WGTSTYLE_EDITABLE;
}

void CUIWidget::SetAsTopMost(BOOL bTopMost)
{

	if(bTopMost)
		m_Style|=WGTSTYLE_TOPMOST;
	else 
		m_Style&=~WGTSTYLE_TOPMOST;

}



//�ҵ����ϲ��һ������(�����Ǿ�̬����,��Ч���ںͲ��ɼ�����)
CUIWidget* CUIWidget::GetTopWidget()
{
	
	
	//��������
	if(!IsSorted()) SortSelf();
	
	CUIWidget* pTopChild=NULL;
	
	//�����ϲ�Ŀɼ�����
	for(int i=0;i<m_Children.size();i++)
		if(m_Children[i]->IsVisible()
		   &&m_Children[i]->IsEnabled()
		   &&!m_Children[i]->IsStaticWidget()) 
		{
			pTopChild=m_Children[i];
		}
		
		return pTopChild;
		
}

//�ҵ����ϲ��һ��topmost����(�����Ǿ�̬����,��Ч���ںͲ��ɼ�����)
CUIWidget* CUIWidget::GetFirstTopMost()
{
	
	
	//��������
	if(!IsSorted()) SortSelf();

	CUIWidget* pTopChild=NULL;

	//�����ϲ�Ŀɼ�����
	for(int i=0;i<m_Children.size();i++)
	{
		CUIWidget* pChild=m_Children[i];

		if(pChild->IsTopMost()
		 &&pChild->IsVisible()
		 &&pChild->IsEnabled()
		 &&!pChild->IsStaticWidget()) 
		{
			pTopChild=m_Children[i];
		}
	}
	
		
	return pTopChild;
		
}

//�������е�topmost����
void CUIWidget::HideTopMost()
{

	for(int i=0;i<m_Children.size();i++)
	if(m_Children[i]->IsTopMost()&&m_Children[i]->IsVisible()) 
	{
		m_Children[i]->ShowWidget(false);
	}

}

//��ʾ���е�topmost����
void CUIWidget::ShowTopMost()
{

	for(int i=0;i<m_Children.size();i++)
	if(m_Children[i]->IsTopMost()&& (!m_Children[i]->IsVisible())) 
	{
		m_Children[i]->ShowWidget(true);
	}

}

//�������յ�tab��Ϣʱ,����Լ����ֵ�,���ֵ����л�
//Ϊ������ĳЩ�ֵ�,�����ǲ���tab��focus,ֻ����tabstop>=0���ֵ�
CUIWidget*  CUIWidget::GetNextStopSibling()
{
	
	//û�и���,��Ȼû���ֵ�
	if(!m_pFather) return 0;

	int thisTab=m_TabStop;

	int minTabIndex=-1;
	int nearTabIndex=-1;

		
	int currentTab=0;
	
	int minTab=0xffffff;
	int nearTab=0xffffff;



	for(int index=0;index<m_pFather->GetNumChild();index++)
	{ 
		
		currentTab=m_pFather->GetChild(index)->GetTabStop();
		
		
		//ĳ���ֵܵ�tabstop>0
		if(currentTab>=0)
		{

			//��С����tabstop
			if(currentTab<minTab) 
			{
				minTab=currentTab;
				minTabIndex=index;
			}
			
			//��С�Ĵ��ڵ�ǰtabstop
			if((currentTab>thisTab)&&(currentTab<nearTab))
			{
				nearTab=currentTab;
				nearTabIndex=index;
			}
				

		}
		

	}


		
	CUIWidget* pSibling=0;

	if(nearTabIndex>=0) pSibling=m_pFather->GetChild(nearTabIndex);
	else if(minTabIndex>=0) pSibling=m_pFather->GetChild(minTabIndex);

	return pSibling;
	

				
}

CUIWidget*  CUIWidget::GetNextStopChild()
{
	
	//û�к���
	if(m_Children.size()<=0) return NULL;

		
	int thisTab=m_ChildStop;
	
	int minTabIndex=-1;
	int nearTabIndex=-1;
	
	
	int currentTab=0;
	
	int minTab=0xffffff;
	int nearTab=0xffffff;
	
	
	
	for(int index=0;index<m_Children.size();index++)
	{ 
		
		currentTab=m_Children[index]->GetTabStop();
		
		
		//ĳ�����ӵ�tabstop>0
		if(currentTab>=0)
		{
			
			//��С����tabstop
			if(currentTab<minTab) 
			{
				minTab=currentTab;
				minTabIndex=index;
			}
			
			//��С�Ĵ��ڵ�ǰtabstop
			if((currentTab>thisTab)&&(currentTab<nearTab))
			{
				nearTab=currentTab;
				nearTabIndex=index;
			}
			
			
		}
		
		
	}
	
	
	
	CUIWidget* pSibling=NULL;
	
	if(nearTabIndex>=0) pSibling=m_Children[nearTabIndex];
	else if(minTabIndex>=0) pSibling=m_Children[minTabIndex];
	
	return pSibling;
	
	
				
}


void  CUIWidget::SetVar(stUIVar& uiVar)
{

	switch(uiVar.iChannel) 
	{
	
	case VALUE_CHANNEL_STATE:
		m_State=uiVar.fValue;
		break;

	case VALUE_CHANNEL_PATTERN:
		m_Pattern=uiVar.fValue;
		break;

	case VALUE_CHANNEL_STYLE:
		m_Style=uiVar.fValue;
		break;

	case VALUE_CHANNEL_STYLEEX:
		m_ExtendStyle=uiVar.fValue;
		break;

	case VALUE_CHANNEL_QUERYTYPE:
		m_QueryType=uiVar.fValue;
		break;

	case VALUE_CHANNEL_QUERYINDEX:
		m_QueryIndex=uiVar.fValue;
		break;

	case VALUE_CHANNEL_DRAGTYPE:
		m_DragType=uiVar.fValue;
		break;

	case VALUE_CHANNEL_DRAGINDEX:
		m_DragIndex=uiVar.fValue;
		break;


	case VALUE_CHANNEL_HOSTTYPE:
		m_HostType=uiVar.fValue;
		break;

	case VALUE_CHANNEL_HOSTINDEX:
		m_HostIndex=uiVar.fValue;
		break;


	case VALUE_CHANNEL_AREAID:
		m_AreaId=uiVar.fValue;
		break;


	case VALUE_CHANNEL_RECTID:
		{
			//�Ƿ������ı�λ��
			if(uiVar.iIndex==0)
				SetRectId(uiVar.fValue);
			else
				SetRectId(uiVar.fValue,m_pFather);
		}
		break;
	
	case VALUE_CHANNEL_SOUNDID:
		{
		
			if((uiVar.iIndex>=0)&&(uiVar.iIndex<UI_SOUND_MAXNUM))
				m_SoundId[uiVar.iIndex]=uiVar.fValue;	
		}
		break;

	
	case VALUE_CHANNEL_CUSTOMSTYLE:
		m_CustomStyle=uiVar.fValue;
		break;

	case VALUE_CHANNEL_CUSTOMSTATE:
		m_CustomState=uiVar.fValue;
		break;

	case VALUE_CHANNEL_CUSTOMTYPE:
		m_CustomType=uiVar.fValue;
		break;

	case VALUE_CHANNEL_CUSTOMINDEX:
		m_CustomIndex=uiVar.fValue;
		break;

	case VALUE_CHANNEL_CUSTOMID:
		m_CustomId=uiVar.fValue;
		break;


	case VALUE_CHANNEL_EDGE_CHECK:
		m_EdgeCheck=uiVar.fValue;
		break;


	case VALUE_CHANNEL_TEMPLATEID:
		m_TemplateId=uiVar.fValue;
		break;

	case VALUE_CHANNEL_UPDATEMUTE:
		m_UpdateMuteFlag=uiVar.fValue;
		break;

	case VALUE_CHANNEL_RICHMODE:
		m_RichMode=uiVar.fValue;
		break;

	case VALUE_CHANNEL_RICHFLAG:
		m_RichFlag=uiVar.fValue;
		break;
	
	case VALUE_CHANNEL_TABSTOP:
		m_TabStop=uiVar.fValue;
		break;

	case VALUE_CHANNEL_GROUPSTOP:
		m_GroupStop=uiVar.fValue;
		break;

	case VALUE_CHANNEL_GROUPID:
		m_GroupId=uiVar.fValue;
		break;

	case VALUE_CHANNEL_TEXTSTYLE:
		m_TextStyle=uiVar.fValue;
		break;

	case VALUE_CHANNEL_TEXTSIZE:
		m_TextSize=uiVar.fValue;
		break;

	case VALUE_CHANNEL_TEXTFONT:
		m_TextFont=uiVar.fValue;
		break;

	case VALUE_CHANNEL_SNAPH:
		m_SnapH=uiVar.fValue;
		break;

	case VALUE_CHANNEL_SNAPV:
		m_SnapV=uiVar.fValue;
		break;

	case VALUE_CHANNEL_AJUST_FLAG:
		m_AjustFlag=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_AJUST_EDGE:
		m_AjustEdge=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_EDGE_THICK:
		m_EdgeThick=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_AJUST_STATE:
		m_AjustState=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_MIN_HEIGHT:
		m_MinHeight=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_MAX_HEIGHT:
		m_MaxHeight=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_MIN_WIDTH:
		m_MinWidth=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_MAX_WIDTH:
		m_MaxWidth=uiVar.fValue; 
		break;


	case VALUE_CHANNEL_MOUSEDELTA:
		m_MouseDelta=uiVar.fValue;
		break;

	
	case VALUE_CHANNEL_ALPHASPEED:
		m_AlphaSpeed=uiVar.fValue;
		break;

	case VALUE_CHANNEL_ALPHAFACTOR:
		m_AlphaFactor=uiVar.fValue;
		break;

	case VALUE_CHANNEL_ALPHASTATE:
		m_AlphaState=uiVar.fValue;
		break;

	case VALUE_CHANNEL_SCALEFACTOR:
		m_ScaleFactor=uiVar.fValue;
		break;


	case VALUE_CHANNEL_ALPHAMIN:
		m_AlphaMin=uiVar.fValue;
		break;

	case VALUE_CHANNEL_ALPHAMAX:
		m_AlphaMax=uiVar.fValue;
		break;


	case VALUE_CHANNEL_TIMERSPACE:
		m_TimerSpace=uiVar.fValue;
		break;

	case VALUE_CHANNEL_PATHSPEED:
		m_PathSpeed=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_FRAMESPEED:
		m_FrameSpeed=uiVar.fValue; 
		break;

	case VALUE_CHANNEL_FRAMESCHEME:
		SetFrameScheme((int)uiVar.fValue);
		break;

	case VALUE_CHANNEL_UPDATE_DIRTY:
		SetUpdateDirty(uiVar.fValue!=0); 
		break;

	case VALUE_CHANNEL_ALIGN_DIRTY:
		SetAlignDirty(uiVar.fValue!=0); 
		break;
		

	case VALUE_CHANNEL_SORT_STATE:
		SetSorted(uiVar.fValue!=0);
		break;

	case VALUE_CHANNEL_PATHMOVE_PAUSE:
		PausePathMove(uiVar.fValue!=0);
		break;

	case VALUE_CHANNEL_PATHMOVE_FLIP:
		FlipPathMove(uiVar.fValue!=0);
		break;



	case VALUE_CHANNEL_COLOR_BACK:
		SetBackColor(stColorQuad(uiVar.fValue));
		break;

	case VALUE_CHANNEL_COLOR_DRAW:
		SetDrawColor(stColorQuad(uiVar.fValue));
		break;

	case VALUE_CHANNEL_COLOR_FOCUS:
		SetFocusColor(stColorQuad(uiVar.fValue));
		break;

	case VALUE_CHANNEL_COLOR_EDGE:
		SetEdgeColor(stColorQuad(uiVar.fValue));
		break;

	case VALUE_CHANNEL_COLOR_MAGIC:
		SetMagicColor(stColorQuad(uiVar.fValue));
		break;

	case VALUE_CHANNEL_COLOR_LINK:
		SetLinkColor(stColorQuad(uiVar.fValue));
		break;

	
	case VALUE_CHANNEL_FRAME:
		m_CurFrame=uiVar.fValue;
		break;
	
	
	case VALUE_CHANNEL_EFFECTFRAME:
		m_CurEffectFrame=uiVar.fValue;
		break;
	
	case VALUE_CHANNEL_EDITABLE:
		SetEditable(int(uiVar.fValue)!=0);		
		break;

	case VALUE_CHANNEL_MOVEABLE:
		EnableMove(int(uiVar.fValue)!=0,int(uiVar.iIndex)==0,int(uiVar.iIndex)==1);
		break;

	case VALUE_CHANNEL_CHECKED:
		SetChecked(int(uiVar.fValue)!=0);
		break;


	case VALUE_CHANNEL_ENABLED:
		EnableWidget(int(uiVar.fValue)!=0);
		break;

	case VALUE_CHANNEL_VISIBLE:
		ShowWidget(int(uiVar.fValue)!=0);
		break;

	case VALUE_CHANNEL_DRAWFLAG:
		m_DrawFlag=uiVar.fValue;
		break;
		
	case VALUE_CHANNEL_WINRECT:
		{
			if(uiVar.iIndex==0)	
				MoveToX(uiVar.fValue);
			else if(uiVar.iIndex==1)	
				MoveToY(uiVar.fValue);
			else if(uiVar.iIndex==2)	
				SetWidth(uiVar.fValue);
			else 
				SetHeight(uiVar.fValue);
		}
		break;

	case VALUE_CHANNEL_CLIENTRECT:
		{
			if(uiVar.iIndex==0)	
				m_ClientRect.ox=uiVar.fValue;
			else if(uiVar.iIndex==1)	
				m_ClientRect.oy=uiVar.fValue;
			else if(uiVar.iIndex==2)	
				m_ClientRect.width=uiVar.fValue;
			else 
				m_ClientRect.height=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_CAPTIONRECT:
		{
			if(uiVar.iIndex==0)	
				m_CaptionRect.ox=uiVar.fValue;
			else if(uiVar.iIndex==1)	
				m_CaptionRect.oy=uiVar.fValue;
			else if(uiVar.iIndex==2)	
				m_CaptionRect.width=uiVar.fValue;
			else 
				m_CaptionRect.height=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_DRAGRECT:
		{
			if(uiVar.iIndex==0)	
				m_DragRect.ox=uiVar.fValue;
			else if(uiVar.iIndex==1)	
				m_DragRect.oy=uiVar.fValue;
			else if(uiVar.iIndex==2)	
				m_DragRect.width=uiVar.fValue;
			else 
				m_DragRect.height=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_BACKPICPOS:
		{
			if(uiVar.iIndex==0)	
				m_BackPicPos.x=uiVar.fValue;
			else 
				m_BackPicPos.y=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_COVERPICPOS:
		{
			if(uiVar.iIndex==0)	
				m_CoverPicPos.x=uiVar.fValue;
			else 
				m_CoverPicPos.y=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_EFFECTPICPOS:
		{
			if(uiVar.iIndex==0)	
				m_EffectPicPos.x=uiVar.fValue;
			else 
				m_EffectPicPos.y=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_CHECKCOVERPOS:
		{
			if(uiVar.iIndex==0)	
				m_CheckCoverPos.x=uiVar.fValue;
			else 
				m_CheckCoverPos.y=uiVar.fValue;
		}
		break;

	case VALUE_CHANNEL_HOTCOVERPOS:
		{
			if(uiVar.iIndex==0)	
				m_HotCoverPos.x=uiVar.fValue;
			else 
				m_HotCoverPos.y=uiVar.fValue;
		}
		break;

	//////////////////////////////////text////////////////////////////////////////

	case VALUE_CHANNEL_CAPTION:
		SetCaption(uiVar.sValue.c_str());	
		break;

	case VALUE_CHANNEL_TIPTEXT:
		SetTipText(uiVar.sValue.c_str(),uiVar.fValue);
		break;

	case VALUE_CHANNEL_SCRIPTTEXT:
		SetScriptText(uiVar.sValue.c_str());
		break;

	
	case VALUE_CHANNEL_TIPWGT:
		RegisterBuddyWgt(BUDDY_WIDGET_TIPWGT,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_MSGBOX:
		RegisterBuddyWgt(BUDDY_WIDGET_MSGBOX,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_CONSOLE:
		RegisterBuddyWgt(BUDDY_WIDGET_CONSOLE,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_HOST:
		RegisterBuddyWgt(BUDDY_WIDGET_HOST,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_INFOWGT:
		RegisterBuddyWgt(BUDDY_WIDGET_INFOWGT,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_BUDDY:
		RegisterBuddyWgt(BUDDY_WIDGET_BUDDY,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_PRIMESCROLL:
		RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_SECONDSCROLL:
		RegisterBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_BLOCKBUTTON:
		RegisterBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON,BUDDY_STYLE_SELF,uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_BACKPIC:
		SetBackPic(uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_COVERPIC:
		SetCoverPic(uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_ADORNPIC:
		SetAdornPic(uiVar.sValue.c_str(),uiVar.iIndex);
		break;

	case VALUE_CHANNEL_PATCHPIC:
		SetPatchPic(uiVar.sValue.c_str(),uiVar.iIndex);
		break;

	
	case VALUE_CHANNEL_EFFECTPIC:
		SetEffectPic(uiVar.sValue.c_str());
		break;		

	case VALUE_CHANNEL_CUSTOMNAME:
		SetCustomName(uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_CUSTOMTEXT:
		SetCustomText(uiVar.sValue.c_str());
		break;

	case VALUE_CHANNEL_CUSTOMSTRING:
		SetCustomString(uiVar.sValue.c_str());
		break;

	}



}

void  CUIWidget::GetVar(stUIVar& uiVar)
{

	switch(uiVar.iChannel) 
	{	
	
	case VALUE_CHANNEL_STATE:
		uiVar.fValue = m_State;
		break;

	case VALUE_CHANNEL_PATTERN:
		uiVar.fValue = m_Pattern;
		break;

	case VALUE_CHANNEL_STYLE:
		uiVar.fValue = m_Style;
		break;

	case VALUE_CHANNEL_STYLEEX:
		uiVar.fValue = m_ExtendStyle;
		break;

	case VALUE_CHANNEL_QUERYTYPE:
		uiVar.fValue = m_QueryType;
		break;

	case VALUE_CHANNEL_QUERYINDEX:
		uiVar.fValue = m_QueryIndex;
		break;

	case VALUE_CHANNEL_DRAGTYPE:
		uiVar.fValue = m_DragType;
		break;

	case VALUE_CHANNEL_DRAGINDEX:
		uiVar.fValue = m_DragIndex;
		break;

	case VALUE_CHANNEL_HOSTTYPE:
		uiVar.fValue = m_HostType;
		break;

	case VALUE_CHANNEL_HOSTINDEX:
		uiVar.fValue = m_HostIndex;

		break;

	case VALUE_CHANNEL_AREAID:
		uiVar.fValue = m_AreaId;
		break;

	case VALUE_CHANNEL_RECTID:
		uiVar.fValue = m_RectId;
		break;

	case VALUE_CHANNEL_SOUNDID:
		{
			if((uiVar.iIndex>=0)&&(uiVar.iIndex<UI_SOUND_MAXNUM))
				uiVar.fValue = m_SoundId[uiVar.iIndex];	
		}
		break;

	case VALUE_CHANNEL_CUSTOMSTYLE:
		uiVar.fValue = m_CustomStyle;
		break;

	case VALUE_CHANNEL_CUSTOMSTATE:
		uiVar.fValue = m_CustomState;
		break;

	case VALUE_CHANNEL_CUSTOMTYPE:
		uiVar.fValue = m_CustomType;
		break;

	case VALUE_CHANNEL_CUSTOMINDEX:
		uiVar.fValue = m_CustomIndex;
		break;

	case VALUE_CHANNEL_CUSTOMID:
		uiVar.fValue = m_CustomId;
		break;

	case VALUE_CHANNEL_EDGE_CHECK:
		uiVar.fValue = m_EdgeCheck;
		break;

	case VALUE_CHANNEL_TEMPLATEID:
		uiVar.fValue = m_TemplateId;
		break;

	case VALUE_CHANNEL_UPDATEMUTE:
		uiVar.fValue=m_UpdateMuteFlag;
		break;
	case VALUE_CHANNEL_RICHMODE:
		uiVar.fValue=m_RichMode;
		break;

	case VALUE_CHANNEL_RICHFLAG:
		uiVar.fValue=m_RichFlag;
		break;

	case VALUE_CHANNEL_TABSTOP:
		uiVar.fValue = m_TabStop;
		break;

	case VALUE_CHANNEL_GROUPSTOP:
		uiVar.fValue = m_GroupStop;
		break;

	case VALUE_CHANNEL_GROUPID:
		uiVar.fValue = m_GroupId;
		break;

	case VALUE_CHANNEL_TEXTSTYLE:
		uiVar.fValue = m_TextStyle;
		break;

	case VALUE_CHANNEL_TEXTSIZE:
		uiVar.fValue = m_TextSize;
		break;

	case VALUE_CHANNEL_TEXTFONT:
		uiVar.fValue = m_TextFont;
		break;

	case VALUE_CHANNEL_SNAPH:
		uiVar.fValue = m_SnapH;
		break;

	case VALUE_CHANNEL_SNAPV:
		uiVar.fValue = m_SnapV;
		break;

	case VALUE_CHANNEL_AJUST_FLAG:
		uiVar.fValue = m_AjustFlag; 
		break;

	case VALUE_CHANNEL_AJUST_EDGE:
		uiVar.fValue = m_AjustEdge; 
		break;

	case VALUE_CHANNEL_EDGE_THICK:
		uiVar.fValue = m_EdgeThick; 
		break;

	case VALUE_CHANNEL_AJUST_STATE:
		uiVar.fValue = m_AjustState; 
		break;

	case VALUE_CHANNEL_MIN_HEIGHT:
		uiVar.fValue = m_MinHeight; 
		break;

	case VALUE_CHANNEL_MAX_HEIGHT:
		uiVar.fValue = m_MaxHeight; 
		break;

	case VALUE_CHANNEL_MIN_WIDTH:
		uiVar.fValue = m_MinWidth; 
		break;

	case VALUE_CHANNEL_MAX_WIDTH:
		uiVar.fValue = m_MaxWidth; 
		break;

	case VALUE_CHANNEL_FRAME:
		uiVar.fValue = m_CurFrame;
		break;

	case VALUE_CHANNEL_EFFECTFRAME:
		uiVar.fValue= m_CurEffectFrame;
		break;

	case VALUE_CHANNEL_MOUSEDELTA:
		uiVar.fValue = m_MouseDelta;
		break;

	case VALUE_CHANNEL_CAPTIONSIZE:
		{
			const char* pCaption=GetCaption();
			uiVar.fValue = strlen(pCaption);
		}
		break;

	case VALUE_CHANNEL_ALPHASPEED:
		uiVar.fValue = m_AlphaSpeed;
		break;

	case VALUE_CHANNEL_ALPHAFACTOR:
		uiVar.fValue = m_AlphaFactor;	
		break;

	case VALUE_CHANNEL_ALPHASTATE:
		uiVar.fValue = m_AlphaState;
		break;

	case VALUE_CHANNEL_SCALEFACTOR:
		uiVar.fValue = m_ScaleFactor;
		break;

	case VALUE_CHANNEL_ALPHAMIN:
		uiVar.fValue = m_AlphaMin;
		break;

	case VALUE_CHANNEL_ALPHAMAX:
		uiVar.fValue = m_AlphaMax;
		break;

	case VALUE_CHANNEL_TIMERSPACE:
		uiVar.fValue = m_TimerSpace;	
		break;

	case VALUE_CHANNEL_PATHSPEED:
		uiVar.fValue = m_PathSpeed;
		break;

	case VALUE_CHANNEL_FRAMESPEED:
		uiVar.fValue = m_FrameSpeed; 
		break;

	case VALUE_CHANNEL_FRAMESCHEME:
		uiVar.fValue = m_CurFrameScheme;
		break;

	case VALUE_CHANNEL_UPDATE_DIRTY:
		uiVar.fValue= (IsUpdateDirty()?1.0:0.0);
		break;

	case VALUE_CHANNEL_PATHMOVE_PAUSE:
		uiVar.fValue = (IsPathMovePaused()?1.0:0.0);
		break;

	case VALUE_CHANNEL_PATHMOVE_FLIP:
		uiVar.fValue = (IsPathMoveFliped()?1.0:0.0);
		break;

	case VALUE_CHANNEL_COLOR_BACK:
		uiVar.fValue = m_BackColor.c;
		break;

	case VALUE_CHANNEL_COLOR_DRAW:
		uiVar.fValue = m_DrawColor.c;
		break;

	case VALUE_CHANNEL_COLOR_FOCUS:
		uiVar.fValue = m_FocusColor.c;
		break;

	case VALUE_CHANNEL_COLOR_EDGE:
		uiVar.fValue = m_EdgeColor.c;
		break;

	case VALUE_CHANNEL_COLOR_MAGIC:
		uiVar.fValue = m_MagicColor.c;
		break;

	case VALUE_CHANNEL_COLOR_LINK:
		uiVar.fValue = m_LinkColor.c;
		break;

	case VALUE_CHANNEL_MOUSEOVER:
		uiVar.fValue =((IsMouseOver())?1.0:0.0);
		break;

	case VALUE_CHANNEL_CLICKDOWN:
		uiVar.fValue =((IsClickDown())?1.0:0.0);
		break;

	case VALUE_CHANNEL_CAPTURED:
		uiVar.fValue =((g_pRootWgt->GetCaptureWgt()==this)?1.0:0.0);
		break;

	case VALUE_CHANNEL_FOCUSED:
		uiVar.fValue =((IsFocused())?1.0:0.0);
		break;
	
	case VALUE_CHANNEL_CHILDFOCUSED:
		uiVar.fValue =((IsChildFocused())?1.0:0.0);
		break;

	case VALUE_CHANNEL_DESCENDFOCUSED:
		uiVar.fValue =((IsDescendFocused())?1.0:0.0);
		break;
	
	case VALUE_CHANNEL_ENABLED:
		uiVar.fValue =((IsEnabled())?1.0:0.0);
		break;

	case VALUE_CHANNEL_CHECKED:
		uiVar.fValue =((IsChecked())?1.0:0.0);
		break;

	case VALUE_CHANNEL_DISCARDED:
		uiVar.fValue =((IsDiscarded())?1.0:0.0);
		break;

	case VALUE_CHANNEL_DRAGGING:
		uiVar.fValue =((IsDragging())?1.0:0.0);
		break;

	case VALUE_CHANNEL_VISIBLE:
		uiVar.fValue =((IsVisible())?1.0:0.0);
		break;

	case VALUE_CHANNEL_ACTIVED:
		uiVar.fValue =((IsActived())?1.0:0.0);
		break;

	case VALUE_CHANNEL_EDITABLE:
		uiVar.fValue =((IsEditable())?1.0:0.0);
		break;

	case VALUE_CHANNEL_MOVEABLE:
		uiVar.fValue =((IsMovable())?1.0:0.0);
		break;


	case VALUE_CHANNEL_DRAWFLAG:
		uiVar.fValue =m_DrawFlag;
		break;

	case VALUE_CHANNEL_WINRECT:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue =m_WinRect.ox;
			else if(uiVar.iIndex==1)	
				uiVar.fValue =m_WinRect.oy;
			else if(uiVar.iIndex==2)	
				uiVar.fValue =m_WinRect.width;
			else 
				uiVar.fValue =m_WinRect.height;
		}
		break;

	case VALUE_CHANNEL_CLIENTRECT:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue =m_ClientRect.ox;
			else if(uiVar.iIndex==1)	
				uiVar.fValue =m_ClientRect.oy;
			else if(uiVar.iIndex==2)	
				uiVar.fValue =m_ClientRect.width;
			else 
				uiVar.fValue =m_ClientRect.height;
		}
		break;

	case VALUE_CHANNEL_CAPTIONRECT:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue =m_CaptionRect.ox;
			else if(uiVar.iIndex==1)	
				uiVar.fValue =m_CaptionRect.oy;
			else if(uiVar.iIndex==2)	
				uiVar.fValue =m_CaptionRect.width;
			else 
				uiVar.fValue =m_CaptionRect.height;
		}
		break;

	case VALUE_CHANNEL_DRAGRECT:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue =m_DragRect.ox;
			else if(uiVar.iIndex==1)	
				uiVar.fValue =m_DragRect.oy;
			else if(uiVar.iIndex==2)	
				uiVar.fValue =m_DragRect.width;
			else 
				uiVar.fValue =m_DragRect.height;
		}
		break;
	
	case VALUE_CHANNEL_BACKPICPOS:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue=m_BackPicPos.x;
			else 
				uiVar.fValue=m_BackPicPos.y;
		}
		break;

	case VALUE_CHANNEL_COVERPICPOS:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue=m_CoverPicPos.x;
			else 
				uiVar.fValue=m_CoverPicPos.y;
		}
		break;

	case VALUE_CHANNEL_EFFECTPICPOS:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue=m_EffectPicPos.x;
			else 
				uiVar.fValue=m_EffectPicPos.y;
		}
		break;

	case VALUE_CHANNEL_CHECKCOVERPOS:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue=m_CheckCoverPos.x;
			else 
				uiVar.fValue=m_CheckCoverPos.y;
		}
		break;

	case VALUE_CHANNEL_HOTCOVERPOS:
		{
			if(uiVar.iIndex==0)	
				uiVar.fValue=m_HotCoverPos.x;
			else 
				uiVar.fValue=m_HotCoverPos.y;
		}
		break;

	//text////////////////////////////////////////////////////////////////////////
		
	case VALUE_CHANNEL_CAPTION:
		uiVar.sValue = GetCaption();
		break;

	case VALUE_CHANNEL_TIPTEXT:
		uiVar.sValue = GetTipText(uiVar.fValue);
		break;

	case VALUE_CHANNEL_SCRIPTTEXT:
		uiVar.sValue = GetScriptText();
		break;

	case VALUE_CHANNEL_TYPENAME:
		uiVar.sValue = (UI_Type_Names[m_Type]);
		break;

	case VALUE_CHANNEL_TIPWGT:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_TIPWGT);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_MSGBOX:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_MSGBOX);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_CONSOLE:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_CONSOLE);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_HOST:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_HOST);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_INFOWGT:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_INFOWGT);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_BUDDY:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_PRIMESCROLL:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_PRIMARY);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_SECONDSCROLL:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_SCROLL_SECONDARY);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

	case VALUE_CHANNEL_BLOCKBUTTON:
		{
			CUIWidget* pWgt=GetSafetBuddyWgt(BUDDY_WIDGET_BLOCK_BUTTON);
			if(pWgt)
				uiVar.sValue = pWgt->GetWidgetPath();
			else 
				uiVar.sValue = "";
		}
		break;

		
	case VALUE_CHANNEL_BACKPIC:
		{
			CUIImage* pImg=GetBackPic();
			if(pImg) 
			{
				uiVar.sValue=pImg->GetFilePath();
			}
		}
		break;

	case VALUE_CHANNEL_COVERPIC:
		{
			CUIImage* pImg=GetCoverPic();
			if(pImg) 
			{
				uiVar.sValue=pImg->GetFilePath();
			}
		}
		break;

	case VALUE_CHANNEL_ADORNPIC:
		{
			CUIImage* pImg=GetAdornPic(uiVar.fValue);
			if(pImg) 
			{
				uiVar.sValue=pImg->GetFilePath();
			}
		}
		break;

	case VALUE_CHANNEL_PATCHPIC:
		{
			CUIImage* pImg=GetPatchPic(uiVar.fValue);
			if(pImg) 
			{
				uiVar.sValue=pImg->GetFilePath();
			}
		}
		break;

	
	case VALUE_CHANNEL_EFFECTPIC:
		{
			CUIImage* pImg=GetEffectPic();
			if(pImg) 
			{
				uiVar.sValue=pImg->GetFilePath();
			}
		}
		break;		

	case VALUE_CHANNEL_CUSTOMNAME:
		uiVar.sValue = GetCustomName();
		break;

	case VALUE_CHANNEL_CUSTOMTEXT:
		uiVar.sValue = GetCustomText();
		break;

	case VALUE_CHANNEL_CUSTOMSTRING:
		uiVar.sValue = GetCustomString();
		break;

	default:
		{
			uiVar.fValue = -1.0;
			uiVar.sValue = "";
		}

	}

}


int	CUIWidget::GetTable(int channel,lua_State *ls)
{
	return 0;
}

int CUIWidget::SetTable(int channel,lua_State *ls)
{
	return 0;
}


stRect CUIWidget::GetChildRect(int index)
{
	stRect rcChild(-1,-1,-1,-1);

	if((index>=0)&&(index<m_vecChildRect.size()))
		rcChild=m_vecChildRect[index];

	return rcChild;	

}

void   CUIWidget::SetChildRect(int index,stRect rcChild)
{
	if((index>=0)&&(index<m_vecChildRect.size()))
		m_vecChildRect[index]=rcChild;
}

void CUIWidget::SetRectId(int id,CUIWidget* pfather)
{
	SetRectId(id);

	if(pfather)
	{
		stRect rcChild=pfather->GetChildRect(id);
		stRect rcFather=pfather->GetWinRect();
		rcChild.ox+=rcFather.ox;
		rcChild.oy+=rcFather.oy;

		MoveTo(rcChild.ox,rcChild.oy);

		if(rcChild.width>=0)
			SetWidth(rcChild.width);

		if(rcChild.height>=0)
			SetHeight(rcChild.height);
	}

}

void CUIWidget::SetWinRect( int ox, int oy, int width, int height )
{
	m_WinRect.ox = ox;
	m_WinRect.oy = oy;
	m_WinRect.width = width;
	m_WinRect.height = height;
	
}

stRect CUIWidget::GetRelativeWinRect()
{
	stRect rcFather(0,0,0,0);
	
	if(m_pFather)	
		rcFather=m_pFather->GetWinRect();

	stRect rcResult=m_WinRect;
	rcResult.ox=m_WinRect.ox-rcFather.ox;
	rcResult.oy=m_WinRect.oy-rcFather.oy;
	
	
	return rcResult;

}

//���ô���λ��Ϊ�����ĳ�����ڵ�λ�ã���һ���Ǹ��ף�
//�����ָ�����ڲ���Ϊ�գ�������Ϊ����λ��
void CUIWidget::SetRelativePos( int left, int top, int right, int bottom, CUIWidget *pfather ) //���õ�����������ڸ����ڵ�
{
	int ox(0), oy(0);


	if( pfather )
	{
		ox = pfather->GetOriginX();
		oy = pfather->GetOriginY();
	}

	SetPosition( left+ox, top+oy, right+ox, bottom+oy );
}

void CUIWidget::SetPosition( int left, int top, int right, int bottom )
{
	m_WinRect.ox = left;
	m_WinRect.oy = top;
	m_WinRect.width = right-left;
	m_WinRect.height = bottom-top;	
}

void CUIWidget::SetCaptionRect( int ox, int oy, int width, int height )
{
	m_CaptionRect.ox = ox;
	m_CaptionRect.oy = oy;
	m_CaptionRect.width = width;
	m_CaptionRect.height = height;
	
}

//ȱʡ�ı�����ռ����������
void CUIWidget::DefaultCaptionRect()
{
	m_CaptionRect.ox = 0;
	m_CaptionRect.oy = 0;
	m_CaptionRect.width = m_WinRect.width;
	m_CaptionRect.height = m_WinRect.height;
	
}

void CUIWidget::AlignRectToSelf(stRect& srcRct,int nAlignType,int nAlignStyle,int nMarginH/*=0*/,int nMarginV/*=0*/)
{

	stRect dstRct;

	if(nAlignType&ALIGNTYPE_WINRECT)     
		dstRct=GetWinRect();
	else 
	{
		if(nAlignType&ALIGNTYPE_CLIENTRECT)  
			dstRct=GetClientRect();			
		else 
		if(nAlignType&ALIGNTYPE_DRAGRECT)    
			dstRct=GetDragRect();
		else 
		if(nAlignType&ALIGNTYPE_CAPTIONRECT) 
			dstRct=GetCaptionRect();
				
			dstRct.ox+=GetWinRect().ox;
			dstRct.oy+=GetWinRect().oy;
	}
	
	AlignRect(dstRct,srcRct,nAlignStyle,nMarginH,nMarginV);
								
}

void CUIWidget::AlignToWidget(CUIWidget* pDstWgt,int nAlignType,int nAlignStyle,int nMarginH,int nMarginV)
{
	
	if(pDstWgt) 
	{ 
		stRect dstRct;

		if(nAlignType&ALIGNTYPE_WINRECT)     
			dstRct=pDstWgt->GetWinRect();
		else 
		{
			if(nAlignType&ALIGNTYPE_CLIENTRECT)  
				dstRct=pDstWgt->GetClientRect();			
			else 
			if(nAlignType&ALIGNTYPE_DRAGRECT)    
				dstRct=pDstWgt->GetDragRect();
			else 
			if(nAlignType&ALIGNTYPE_CAPTIONRECT) 
				dstRct=pDstWgt->GetCaptionRect();
					
				dstRct.ox+=pDstWgt->GetWinRect().ox;
				dstRct.oy+=pDstWgt->GetWinRect().oy;
		}
		
		AlignToRect(dstRct,nAlignStyle,nMarginH,nMarginV);
								
	}

	
}


//��srcRect���뵽dstRect
void CUIWidget::AlignRect(stRect dstRect,stRect& srcRect,int nAlignStyle,int nMarginH,int nMarginV)
{
	
		
	if(nAlignStyle==ALIGNSTYLE_NONE) return;	
	
	
	if(nAlignStyle&ALIGNSTYLE_TOP_IN) 
		srcRect.oy=(dstRect.oy+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_BOTTOM_IN) 
		srcRect.oy=(dstRect.oy+dstRect.height-srcRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_TOP_OUT) 
		srcRect.oy=(dstRect.oy-srcRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_BOTTOM_OUT) 
		srcRect.oy=(dstRect.oy+dstRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VCENTER) 
		srcRect.oy=(dstRect.oy+dstRect.height*0.5-srcRect.height*0.5+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VMID_TOP) 
		srcRect.oy=(dstRect.oy+dstRect.height*0.5-srcRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VMID_BOTTOM) 
		srcRect.oy=(dstRect.oy+dstRect.height*0.5+nMarginV);

	
	if(nAlignStyle&ALIGNSTYLE_LEFT_IN) 
		srcRect.ox=(dstRect.ox+nMarginH);
	else
	if(nAlignStyle&ALIGNSTYLE_RIGHT_IN) 
		srcRect.ox=(dstRect.ox+dstRect.width-srcRect.width+nMarginH);	
	else
	if(nAlignStyle&ALIGNSTYLE_LEFT_OUT) 
		srcRect.ox=(dstRect.ox-srcRect.width+nMarginH);
	else
	if(nAlignStyle&ALIGNSTYLE_RIGHT_OUT) 
		srcRect.ox=(dstRect.ox+dstRect.width+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HCENTER) 
		srcRect.ox=(dstRect.ox+dstRect.width*0.5-srcRect.width*0.5+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HMID_LEFT) 
		srcRect.ox=(dstRect.ox+dstRect.width*0.5-srcRect.width+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HMID_RIGHT) 
		srcRect.ox=(dstRect.ox+dstRect.width*0.5+nMarginH);
			
}


//���Լ���winrect����alignrect��ָ��rect
void CUIWidget::AlignToRect(stRect dstRct,int nAlignStyle,int nMarginH,int nMarginV)
{

	if(nAlignStyle==ALIGNSTYLE_NONE) return;	
	
	
	if(nAlignStyle&ALIGNSTYLE_TOP_IN) 
		MoveToY(dstRct.oy+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_BOTTOM_IN) 
		MoveToY(dstRct.oy+dstRct.height-m_WinRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_TOP_OUT) 
		MoveToY(dstRct.oy-m_WinRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_BOTTOM_OUT) 
		MoveToY(dstRct.oy+dstRct.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VCENTER) 
		MoveToY(dstRct.oy+dstRct.height*0.5-m_WinRect.height*0.5+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VMID_TOP) 
		MoveToY(dstRct.oy+dstRct.height*0.5-m_WinRect.height+nMarginV);
	else
	if(nAlignStyle&ALIGNSTYLE_VMID_BOTTOM) 
		MoveToY(dstRct.oy+dstRct.height*0.5+nMarginV);

	
	if(nAlignStyle&ALIGNSTYLE_LEFT_IN) 
		MoveToX(dstRct.ox+nMarginH);
	else
	if(nAlignStyle&ALIGNSTYLE_RIGHT_IN) 
		MoveToX(dstRct.ox+dstRct.width-m_WinRect.width+nMarginH);	
	else
	if(nAlignStyle&ALIGNSTYLE_LEFT_OUT) 
		MoveToX(dstRct.ox-m_WinRect.width+nMarginH);
	else
	if(nAlignStyle&ALIGNSTYLE_RIGHT_OUT) 
		MoveToX(dstRct.ox+dstRct.width+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HCENTER) 
		MoveToX(dstRct.ox+dstRct.width*0.5-m_WinRect.width*0.5+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HMID_LEFT) 
		MoveToX(dstRct.ox+dstRct.width*0.5-m_WinRect.width+nMarginH);
	else	
	if(nAlignStyle&ALIGNSTYLE_HMID_RIGHT) 
		MoveToX(dstRct.ox+dstRct.width*0.5+nMarginH);
	
		
		
}

//ȱʡ�ı�����ռ����������
void CUIWidget::DefaultDragRect()
{
	m_DragRect.ox = 0;
	m_DragRect.oy = 0;
	m_DragRect.width = m_WinRect.width;
	m_DragRect.height= m_WinRect.height;
	
}

void CUIWidget::SetDragRect( int ox, int oy, int width, int height )
{
	m_DragRect.ox = ox;
	m_DragRect.oy = oy;
	m_DragRect.width = width;
	m_DragRect.height = height;
}

void CUIWidget::SetClientRect( int ox, int oy, int width, int height )
{
	m_ClientRect.ox = ox;
	m_ClientRect.oy = oy;
	m_ClientRect.width = width;
	m_ClientRect.height = height;
}

//ȱʡ�ͻ���ռ������������
void CUIWidget::DefaultClientRect()
{

	m_ClientRect.ox = 0;
	m_ClientRect.oy = 0;
	m_ClientRect.width = m_WinRect.width;
	m_ClientRect.height = m_WinRect.height;
	
}

//��pSrc������ĳ��λ��ʱ,����Ƿ�Ҫsnap����pDst��ĳ��λ��
int CUIWidget::SnapRect(stRect dstRect,stRect& srcRect,int nSnapH,int nSnapV)
{

	int i;

	int snaped=0;

	int xmargins[4];
	int ymargins[4];
	int minValue;
	int minIndex;

	bool bSnapX=true;
	bool bSnapY=true;


	/////////////////Calculate Margins///////////////////////////
	
	xmargins[0]=dstRect.ox-srcRect.ox;//��ߵ���ߵ����λ��
	xmargins[1]=(dstRect.ox+dstRect.width)-(srcRect.ox+srcRect.width);//�ұߵ��ұߵ����λ��
	xmargins[2]=(dstRect.ox+dstRect.width)-srcRect.ox;//��ߵ��ұߵ����λ��
	xmargins[3]=dstRect.ox-(srcRect.ox+srcRect.width);//�ұߵ���ߵ����λ��
	
	ymargins[0]=(dstRect.oy-srcRect.oy);
	ymargins[1]=((dstRect.oy+dstRect.height)-(srcRect.oy+srcRect.height));
	ymargins[2]=((dstRect.oy+dstRect.height)-srcRect.oy);
	ymargins[3]=(dstRect.oy-(srcRect.oy+srcRect.height));
	
	if(xmargins[2]<0||xmargins[3]>0)
	bSnapY=false;

	if(ymargins[2]<0||ymargins[3]>0)
	bSnapX=false;

	
	////////////////////snap x////////////////////

	if(bSnapX)
	{

		xmargins[0]=abs(xmargins[0]);
		xmargins[1]=abs(xmargins[1]);
		xmargins[2]=abs(xmargins[2]);
		xmargins[3]=abs(xmargins[3]);

		minValue=10000;
		minIndex=-1;
		for(i=0;i<4;i++)
		{ 
			if(xmargins[i]<minValue) 
			{ 
				minValue=xmargins[i];
				minIndex=i;			
			}
		}
		
		//��Сֵ�㹻С������snap
		if(minValue<nSnapH)
		{
			if(minIndex==0) 	//�������ڵ�����㹻����
				AlignRect(dstRect,srcRect,ALIGNSTYLE_LEFT_IN);
			
			else if(minIndex==1) //�������ڵ��ұ��㹻����
				AlignRect(dstRect,srcRect,ALIGNSTYLE_RIGHT_IN);
			
			else if(minIndex==2) //src������㹻����dst���ұ�
				AlignRect(dstRect,srcRect,ALIGNSTYLE_RIGHT_OUT);
			
			else if(minIndex==3) //src���ұ��㹻����dst�����
				AlignRect(dstRect,srcRect,ALIGNSTYLE_LEFT_OUT);
			
			if(minIndex!=-1)	snaped=1;



			
		}
		
	}



	////////////////////snap y////////////////////
	
	if(bSnapY)
	{
		
		ymargins[0]=abs(ymargins[0]);
		ymargins[1]=abs(ymargins[1]);
		ymargins[2]=abs(ymargins[2]);
		ymargins[3]=abs(ymargins[3]);

		minValue=10000;
		minIndex=-1;
		for(i=0;i<4;i++)
		{ 
			if(ymargins[i]<minValue) 
			{ 
				minValue=ymargins[i];
				minIndex=i;			
			}
		}
		
		//��Сֵ�㹻С������snap
		if(minValue<nSnapV)
		{
			if(minIndex==0) 	//�������ڵ��ϱ��㹻����
				AlignRect(dstRect,srcRect,ALIGNSTYLE_TOP_IN);
			
			else if(minIndex==1) //�������ڵ��±��㹻����
				AlignRect(dstRect,srcRect,ALIGNSTYLE_BOTTOM_IN);
			
			else if(minIndex==2) //src���ϱ��㹻����dst���±�
				AlignRect(dstRect,srcRect,ALIGNSTYLE_BOTTOM_OUT);
			
			else if(minIndex==3) //src���±��㹻����dst���ϱ�
				AlignRect(dstRect,srcRect,ALIGNSTYLE_TOP_OUT);
			
			if(minIndex!=-1)	snaped=1;
			
	
		}
		
		
	}


	return snaped;//��û��snap


}

//����ֵ��ʾ�Ƿ��޸���x��y
int  CUIWidget::ChildQueryMoveTo(int &x,int &y,CUIWidget* pChild)
{


	stRect srcRect=pChild->GetWinRect();
	srcRect.ox=x;
	srcRect.oy=y;

	//ȱʡ�Ĳ���:�������ƶ�ʱ,ȷ���Ƿ�snap���Լ����������ֵ�
	int snaped=0;
	snaped=SnapRect(m_WinRect,srcRect,m_SnapH,m_SnapV);

	for(int i=0;i<m_Children.size();i++)
	{
		CUIWidget* pWgt=m_Children[i];

		if((pWgt!=pChild)&&(pWgt->IsVisible()))//ֻsnap���ɼ��ֵ�
		{
			if(SnapRect(pWgt->GetWinRect(),srcRect,pWgt->GetSnapH(),pWgt->GetSnapV()))
			snaped=1;
		}

	}
	
	if(snaped)
	{
		x=srcRect.ox;
		y=srcRect.oy;
	}
	
	return snaped;

}

//�������
int CUIWidget::AjustWidth(int addition,int rectFlag)
{
	
	//RECT_FLAG_WINDOW���Ǵ���////////////////////////////////////////////////
	
	if((m_WinRect.width+addition)<m_MinWidth)
		addition=m_MinWidth-m_WinRect.width;			
	
	if((m_WinRect.width+addition)>m_MaxWidth)
		addition=m_MaxWidth-m_WinRect.width;		

	m_WinRect.width+=addition;		
	

	//rectFlag��������//////////////////////////////////////////////////////////

	
	if(rectFlag&RECT_FLAG_CLIENT)
	{
		if((m_ClientRect.width+addition)>0)
			m_ClientRect.width+=addition;
		else 
			m_ClientRect.width=0;
	}
	
	if(rectFlag&RECT_FLAG_CAPTION)
	{
		if((m_CaptionRect.width+addition)>0)
			m_CaptionRect.width+=addition;
		else 
			m_CaptionRect.width=0;
	}
	
	if(rectFlag&RECT_FLAG_DRAG)
	{
		if((m_DragRect.width+addition)>0)
			m_DragRect.width+=addition;
		else 
			m_DragRect.width=0;
	}

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++)
	{
		if((*iter)->GetStyle()&WGTSTYLE_AJUSTFOLLOW)
		{
			(*iter)->AjustWidth(addition,(*iter)->GetAjustFlag());
		}	
	}		

	SetAlignDirty(true);

	SetUpdateDirty(true);

	CallSafeScriptEx(SCRIPT_FUNC_ON_EDGEAJUST,"%d,%d,%d,%d",m_WinRect.ox,m_WinRect.oy,m_WinRect.width,m_WinRect.height);

	return addition;

}

//�����߶�
int CUIWidget::AjustHeight(int addition,int rectFlag)
{

	//RECT_FLAG_WINDOW���Ǵ���////////////////////////////////////////////////

	if((m_WinRect.height+addition)<m_MinHeight)
		addition=m_MinHeight-m_WinRect.height;
	
	if((m_WinRect.height+addition)>m_MaxHeight)
		addition=m_MaxHeight-m_WinRect.height;
				

	m_WinRect.height+=addition;		

	//rectFlag��������//////////////////////////////////////////////////////////
	
	if(rectFlag&RECT_FLAG_CLIENT)
	{
		if((m_ClientRect.height+addition)>0)
			m_ClientRect.height+=addition;
		else
			m_ClientRect.height=0;		
	}
	
	if(rectFlag&RECT_FLAG_CAPTION)
	{
		if((m_CaptionRect.height+addition)>0)
			m_CaptionRect.height+=addition;
		else 
			m_CaptionRect.height=0;
	}
	
	if(rectFlag&RECT_FLAG_DRAG)
	{
		if((m_DragRect.height+addition)>0)
			m_DragRect.height+=addition;
		else 
			m_DragRect.height=0;
	}

	
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++)
	{
		if((*iter)->GetStyle()&WGTSTYLE_AJUSTFOLLOW)
		{
			(*iter)->AjustHeight(addition,(*iter)->GetAjustFlag());
		}	
	}

	SetAlignDirty(true);

	SetUpdateDirty(true);

	CallSafeScriptEx(SCRIPT_FUNC_ON_EDGEAJUST,"%d,%d,%d,%d",m_WinRect.ox,m_WinRect.oy,m_WinRect.width,m_WinRect.height);


	return addition;
	
}

//������չĳ����һ�����루ע�⣺һ��ֻ����չһ���ߣ�
void CUIWidget::ExtendEdge( int addition,int edgeFlag,int rectFlag)
{
	if(edgeFlag==EDGE_FLAG_UP)
	{
		int nAjust=AjustHeight(addition,rectFlag);

		if(nAjust!=0)
			Move(0,-nAjust);	
		
	}
	else if(edgeFlag==EDGE_FLAG_DOWN)
	{		
		AjustHeight(addition,rectFlag);
	}
	else if(edgeFlag==EDGE_FLAG_LEFT)
	{
		int nAjust=AjustWidth(addition,rectFlag);

		if(nAjust!=0)
			Move(-nAjust,0);
		
	}
	else if(edgeFlag==EDGE_FLAG_RIGHT)
	{
		AjustWidth(addition,rectFlag);		
	}

	
}

//��չĳ���ߵ�ָ�����꣨ע�⣺һ��ֻ����չһ���ߣ�
//value:��չ����λ��
//edgeFlag:��չ�Ǹ���
//rectFlag:�Ƿ���չ�ͻ���,�ı���,�Ϸ���,
void CUIWidget::ExtendTo( int value,int edgeFlag,int rectFlag)
{

	if(edgeFlag==EDGE_FLAG_UP)
	{
		ExtendEdge(m_WinRect.oy-value,edgeFlag,rectFlag);
	}
	else if(edgeFlag==EDGE_FLAG_DOWN)
	{		
		ExtendEdge(value-(m_WinRect.oy+m_WinRect.height),edgeFlag,rectFlag);
	}
	else if(edgeFlag==EDGE_FLAG_LEFT)
	{
		ExtendEdge(m_WinRect.ox-value,edgeFlag,rectFlag);
	}
	else if(edgeFlag==EDGE_FLAG_RIGHT)
	{
		ExtendEdge(value-(m_WinRect.ox+m_WinRect.width),edgeFlag,rectFlag);
	}

}



//ͬʱ���ݹ��������
void CUIWidget::MoveTo( int x, int y )
{
	int dx = x - m_WinRect.ox;
	int dy = y - m_WinRect.oy;
	Move( dx, dy );
}

//ͬʱ���ݹ��������
void CUIWidget::MoveToX( int x )
{
	int dx = x - m_WinRect.ox;
	Move( dx, 0 );
}

//ͬʱ���ݹ��������
void CUIWidget::MoveToY( int y )
{
	int dy = y - m_WinRect.oy;
	Move( 0, dy );
}

//ͬʱ���ݹ��������
void CUIWidget::Move( int dx, int dy )
{
	m_WinRect.ox += dx;
	m_WinRect.oy += dy;
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->Move( dx,dy);
	}
}

	
//���²��ֺ���
void CUIWidget::ReAlignChild()
{		
	
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		CUIWidget* pWgt=(*iter);
		
		if(pWgt->GetStyle()&WGTSTYLE_ALIGNFOLLOW)
		{
			pWgt->AlignSelf();

			pWgt->ReAlignChild();
		}
	}

}

//��extendto����extendedge�󣬴��ڳ���aligndirty״̬
//��aligndirtyʱ����postupdateself�е��ã��Ծ����Ƿ����²����Ӵ���
void CUIWidget::OnAlignDirty()
{
		
	//���²��ֺ���
	ReAlignChild();

}

void CUIWidget::ClearPathPoint()
{
	m_PathPoints.clear();
}

void CUIWidget::PushPathPoint(stUIPathPoint pt)
{
	if(IsPathMoveFliped())
		m_PathPoints.push_front(pt);
	else 
		m_PathPoints.push_back(pt);
}

stRect CUIWidget::GetAbsoluteDragRect()
{
	stRect rect;
	rect.ox=m_WinRect.ox+m_DragRect.ox;
	rect.oy=m_WinRect.oy+m_DragRect.oy;
	rect.width=m_DragRect.width;
	rect.height=m_DragRect.height;

	return rect;
}

stRect CUIWidget::GetAbsoluteClientRect()
{

	stRect rect;
	rect.ox=m_WinRect.ox+m_ClientRect.ox;
	rect.oy=m_WinRect.oy+m_ClientRect.oy;
	rect.width=m_ClientRect.width;
	rect.height=m_ClientRect.height;
	
	return rect;
}

stRect CUIWidget::GetAbsoluteCaptionRect()
{

	stRect rect;
	rect.ox=m_WinRect.ox+m_CaptionRect.ox;
	rect.oy=m_WinRect.oy+m_CaptionRect.oy;
	rect.width=m_CaptionRect.width;
	rect.height=m_CaptionRect.height;
	
	return rect;

}


//��ʾ�������ȷ��ڴ��ڵ�����,����Ϊ���λ�õ��Ҳ�һ��
//�������ռ䲻��,��������,����Ҳ�ռ䲻��,�������һ��
Point2D CUIWidget::GetBestTooltipPos(int xcursor,int ycursor,int w,int h,stRect refRect)
{
	//Ĭ��tooltip�����λ��
	Point2D ptResult(xcursor,ycursor);

	//��Ļ�ߴ�
	int width=g_pRootWgt->GetWidth();
	int height=g_pRootWgt->GetHeight();	

	Point2D ptHotSpot=g_pRootWgt->GetCurCursor()->GetHotSpot();
	int cursorW=g_pRootWgt->GetCurCursor()->GetWidth();
	int cursorH=g_pRootWgt->GetCurCursor()->GetHeight();
	
	if(m_TipFlag==UI_TIPFLAG_INREFWIN)//tooltip�ڲο�����֮��
	{
		//�����趨�ο�����Ϊ������
		refRect.ox=xcursor-ptHotSpot.x;
		refRect.oy=ycursor-ptHotSpot.y;
		refRect.width=cursorW;
		refRect.height=cursorH;	 
	}
	else 
	{
		//�����趨�ο�����Ϊԭ�ο����κ����Ĳ���
		refRect=GetUnifyRect(refRect,stRect(xcursor-ptHotSpot.x,ycursor-ptHotSpot.y,cursorW,cursorH));	 	 
	}

	//refRect������Ļ���Ŀհ�
	int leftmargin=refRect.ox;

	//refRect������Ļ�Ҳ�Ŀհ�
	int rightmargin=width-(refRect.ox+refRect.width);

	//refRect������Ļ�²�Ŀհ�
	int downmargin=height-(refRect.oy+refRect.height);

	//refRect������Ļ�ϲ�Ŀհ�
	int upmargin=refRect.oy;


	if(upmargin>h)
	{
		ptResult.y=refRect.oy-h;
	}
	else if(downmargin>h)
	{
		ptResult.y=refRect.oy+refRect.height;
	}
	else
	{
		if(upmargin>downmargin)
		{
			ptResult.y=refRect.oy-h;
		}		
		else 
		{
			ptResult.y=refRect.oy+refRect.height;		 
		}

		if(leftmargin>rightmargin)
		{
			ptResult.x=refRect.ox-w;
		}
		else 
		{
			ptResult.x=refRect.ox+refRect.width;
		}

	}


	return ptResult;

}


void CUIWidget::SetActived( bool bActive)
{ 

	if( bActive ) 
	{
		m_State |= WGTSTATE_ACTIVED;
	}
	else 
	{
		m_State &= ~WGTSTATE_ACTIVED;
	}
}


void CUIWidget::SetChecked( bool bCheck )
{ 

	bool bOldCheck=((m_State&WGTSTATE_CHECKED)!=0);


	if(bCheck)
		m_State |= WGTSTATE_CHECKED;
	else 
		m_State &= ~WGTSTATE_CHECKED;
		
	CallSafeScriptEx(SCRIPT_FUNC_ON_CHECKED,"%d,%d",(int)bCheck,(int)bOldCheck);


}

void CUIWidget::SetFocused(bool bFocus)
{ 
	

	if( bFocus ) 
	{
		m_State |= WGTSTATE_FOCUSED;
		
		if(m_pFather) 
			m_pFather->SetChildStop(m_TabStop);

	}
	else 
	{
		m_State &= ~WGTSTATE_FOCUSED;
		
		m_State &=~WGTSTATE_CLICKDOWN;
		m_State &=~WGTSTATE_MOUSEOVER;
		m_State &=~WGTSTATE_DRAGGING;

	}
}

void CUIWidget::OnGainFocus()
{

	if(IsAcceptInput())
	{
		g_pRootWgt->SetHotWgt(this);
	}

	int bIme=0;
	bIme=int(IsAcceptIme()&&IsImeEnabled());
	ExecCallBackFunc(UI_Callback_EnableIME,(void*)&bIme);	

	SetFocused(true);
	ActiveRecursive(true);
	OnMessage( MSGTYPE_GAINFOCUS, 0, 0 );

}

void CUIWidget::OnLostFocus()
{
	if(IsAcceptInput())
	{
		if((m_ExtendStyle&EDIT_STYLE_SMARTCURSOR)==0)
			g_pRootWgt->SetHotWgt(NULL);
	}
	
	int bIme=0;
	ExecCallBackFunc(UI_Callback_EnableIME,(void*)&bIme);

	SetFocused(false);		
	ActiveRecursive(false);
	OnMessage( MSGTYPE_LOSTFOCUS, 0, 0 );	
	
}


void CUIWidget::SetCaptionOffset( Point2D offsetPt ,int index)
{
	if((index>=0)&&(index<FRAME_STATE_MAX))
	{
		m_CaptionOffset[index]=offsetPt;
	}
}

Point2D CUIWidget::GetCaptionOffset(int index)
{

	if((index>=0)&&(index<FRAME_STATE_MAX))
	{
		return m_CaptionOffset[index];
	}

	return Point2D(0xffffffff,0xffffffff);

}


void CUIWidget::SetPatchPic( CUIImage *pimg ,int index)
{
	if((index<0)||(index>=PATCHID_NUMBER)) return;
	
	if( pimg) pimg->IncRefCount();
	if( m_pPatchPic[index] ) m_pPatchPic[index]->Release();
	m_pPatchPic[index] = pimg;
	
}

void CUIWidget::SetPatchPic( const char *filename ,int index)
{
	if((index<0)||(index>=PATCHID_NUMBER)) return;

	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetPatchPic( pimg ,index);
	if( pimg ) pimg->Release();

}

CUIImage* CUIWidget::GetPatchPic(int index)
{
	if((index<0)||(index>=PATCHID_NUMBER)) 
		return NULL;
	else
		return m_pPatchPic[index];
}

void CUIWidget::SetBackPic( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pBackPic ) m_pBackPic->Release();
	m_pBackPic = pimg;
}

void CUIWidget::SetBackPic( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetBackPic( pimg );
	if( pimg ) pimg->Release();
}


void CUIWidget::SetBackPicPos(int x,int y)
{
	m_BackPicPos.x=x;
	m_BackPicPos.y=y;
}


void CUIWidget::SetCoverPic( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetCoverPic( pimg );
	if( pimg ) pimg->Release();
}


void CUIWidget::SetCoverPic( CUIImage *pimg)
{
	if( pimg) pimg->IncRefCount();
	if( m_pCoverPic ) m_pCoverPic->Release();
	m_pCoverPic = pimg;
}


void CUIWidget::SetCoverPicPos(int x,int y)
{
	m_CoverPicPos.x=x;
	m_CoverPicPos.y=y;
}

void CUIWidget::SetEffectPic( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetEffectPic( pimg );
	if( pimg ) pimg->Release();
}


void CUIWidget::SetEffectPic( CUIImage *pimg)
{
	if( pimg) pimg->IncRefCount();
	if( m_pEffectPic ) m_pEffectPic->Release();
	m_pEffectPic = pimg;
}


void CUIWidget::SetEffectPicPos(int x,int y)
{
	m_EffectPicPos.x=x;
	m_EffectPicPos.y=y;
}



void CUIWidget::SetAdornPic( CUIImage *pimg ,int index)
{
	if((index<0)||(index>=UI_ADORNPIC_NUMBER)) return;
	
	if( pimg) pimg->IncRefCount();
	if( m_pAdornPic[index] ) m_pAdornPic[index]->Release();
	m_pAdornPic[index] = pimg;
	
}

void CUIWidget::SetAdornPic( const char *filename ,int index)
{
	if((index<0)||(index>=UI_ADORNPIC_NUMBER)) return;

	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetAdornPic( pimg ,index);
	if( pimg ) pimg->Release();

}

CUIImage* CUIWidget::GetAdornPic(int index)
{
	if((index<0)||(index>=UI_ADORNPIC_NUMBER)) 
		return NULL;
	else
		return m_pAdornPic[index];
}

void CUIWidget::SetCheckCover( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pCheckCover) m_pCheckCover->Release();
	m_pCheckCover = pimg;
}

void CUIWidget::SetCheckCover( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetCheckCover( pimg );
	if( pimg ) pimg->Release();
}

void CUIWidget::SetHotCover( CUIImage *pimg )
{
	if( pimg) pimg->IncRefCount();
	if( m_pHotCover) m_pHotCover->Release();
	m_pHotCover = pimg;
}

void CUIWidget::SetHotCover( const char *filename )
{
	CUIImage *pimg = 0;
	if( filename) pimg = new CUIImage(filename);
	SetHotCover( pimg );
	if( pimg ) pimg->Release();
}




void CUIWidget::OnCmdOk()
{
	CallSafeScript(SCRIPT_FUNC_ON_OK);
}

void CUIWidget::OnCmdCancel()
{
	CallSafeScript(SCRIPT_FUNC_ON_CANCEL);
}

void CUIWidget::EndWidget(bool bFlag)
{

	if((m_Style&WGTSTYLE_CUSTOMCLOSE)==0)
		ShowWidget(false);

	if(bFlag)
		OnCmdOk();
	else 
		OnCmdCancel();

}

void CUIWidget::CheckWidgetEdge()
{
	//û�г�����Ļ
	if(IsOutofScreen()==UI_TOUCH_NONE) 
		return;

	//������Զ���������
	if(m_EdgeCheck&EDGE_CHECK_AUTO)
		PullIntoScreen();
	else 
		CallSafeScript(SCRIPT_FUNC_ON_OUT_SCREEN);
}

void CUIWidget::ShowWidget( BOOL isshow )
{

	bool bOldVis=(m_State&WGTSTATE_VISIBLE)!=0;

	//���ÿɼ�
	if( isshow ) 
	{
		//�����ǰ���ɼ�,��ʾ����
		if(!bOldVis)
		{
		
			m_State |= WGTSTATE_VISIBLE;

			m_AlphaState=ALPHA_STATE_INCREASE;

			if(IsPopup())	
				g_pRootWgt->SetPopupWgt( this );
		}
		
		//��������
		stUISound sndData;
		sndData.id=m_SoundId[UI_SOUND_SHOW];
		sndData.cmd=UI_SOUND_SHOW;
		sndData.mode=0;
		ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);

		//��SelfShow��ʱ��RenderBuffer��׼������
		if(m_Style&WGTSTYLE_RENDERBUFFER)
			ExecCallBackFunc(UI_Callback_CustomBufferSelfShow, &m_CustomBuffer);

		//���ܵ�ǰ�Ƿ�ɼ�,���ǵ��ýű�
		CallSafeScriptEx(SCRIPT_FUNC_ON_SELFSHOW,"%d",int(bOldVis));		

		if(m_EdgeCheck&EDGE_CHECK_SHOW)
			CheckWidgetEdge();

		
	}
	//���ò��ɼ�
	else
	{
		//�����ǰ�ɼ�,���ش���
		if(bOldVis)
		{

			m_State &= ~WGTSTATE_VISIBLE;
			
			m_State &=~WGTSTATE_CLICKDOWN;
			m_State &=~WGTSTATE_MOUSEOVER;
			m_State &=~WGTSTATE_DRAGGING;
			
			m_AlphaState=ALPHA_STATE_DECREASE;
			
			
			//����������,��ȡ����capture����
			if(g_pRootWgt->GetCaptureWgt()==this)
				g_pRootWgt->SetCaptureWgt(NULL);
			
			//����������,��ȡ����focus����
			if(g_pRootWgt->GetFocusWgt()==this)
				g_pRootWgt->SetFocusWgt(NULL);
			
			if(g_pRootWgt->GetPopupWgt()==this)	
				g_pRootWgt->SetPopupWgt(NULL);		

		}

		//��������
		stUISound sndData;
		sndData.id=m_SoundId[UI_SOUND_HIDE];
		sndData.cmd=UI_SOUND_HIDE;
		sndData.mode=0;

		ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);


		//���ܵ�ǰ�Ƿ�ɼ�,���ǵ��ýű�
		CallSafeScriptEx(SCRIPT_FUNC_ON_SELFHIDE,"%d",int(bOldVis));

	}

	if(m_pFather) 
		m_pFather->OnNotify(NTFTYPE_CHILD_SHOWHIDE,0,(unsigned int)this);


}


void CUIWidget::EnableWidget( BOOL bEnable )
{

	if(bEnable) 
		m_State &= ~WGTSTATE_DISABLE;
	else 
	{
		m_State |= WGTSTATE_DISABLE;

		if(IsFocused()) //focus����,���m_pFather==0,���focus������
		g_pRootWgt->SetFocusWgt(m_pFather);

		m_State &=~WGTSTATE_CLICKDOWN;
		m_State &=~WGTSTATE_MOUSEOVER;
		m_State &=~WGTSTATE_DRAGGING;
		

	}
	
}


void CUIWidget::EnableSize( BOOL cansize, BOOL effect_x, BOOL effect_y )
{
	
	if( cansize )
	{
		if( effect_x ) m_Style |= WGTSTYLE_SELF_XSIZE;
		if( effect_y ) m_Style |= WGTSTYLE_SELF_YSIZE;
	}
	else
	{
		if( effect_x ) m_Style &= ~WGTSTYLE_SELF_XSIZE;
		if( effect_y ) m_Style &= ~WGTSTYLE_SELF_YSIZE;
	}
}

void CUIWidget::EnableMove( BOOL canmove, BOOL effect_x, BOOL effect_y )
{
	
	if( canmove )
	{
		if( effect_x ) m_Style |= WGTSTYLE_SELF_XMOVE;
		if( effect_y ) m_Style |= WGTSTYLE_SELF_YMOVE;
	}
	else
	{
		if( effect_x ) m_Style &= ~WGTSTYLE_SELF_XMOVE;
		if( effect_y ) m_Style &= ~WGTSTYLE_SELF_YMOVE;
	}
}

//�Ƿ������������
BOOL CUIWidget::IsAcceptIme()
{
	return FALSE;
}


//�Ƿ�����ַ�����
BOOL CUIWidget::IsAcceptInput()
{
	return FALSE;
}



//�����Ƿ����Լ����ߺ���֮��
BOOL CUIWidget::IsPointInDescendant( int x, int y )
{
	return (this->FindWidgetOnPoint(x,y)!=NULL);
}

double  CUIWidget::GetRenderFactor()
{
	return m_RenderFactor;
}

stColorQuad CUIWidget::GetRenderColor(stColorQuad srcColor)
{
	srcColor.a*=m_RenderFactor;

	return srcColor;
}

//����0:����Ҫ�����ݼ�
//����1:��Ҫ�����ݼ�
//����-1:�ű�û�з���ֵ,���߳���
int CUIWidget::ProcessAccelerator(const stAccelItem& accel, WPARAM wparam, LPARAM lparam)
{
	
	if(m_Accels.empty()) return 0;
	
	for(int i=0;i<m_Accels.size();i++)
	{ 
		if(m_Accels[i]==accel)
		{
			char buf[128];
			memset(buf,0,128);
			sprintf(buf,"%d,%d",wparam,lparam);
			if(CallScript(m_Accels[i].accel_script,buf)==1)
			return  1;			
		}
	}
	
	
	return 0;
	
	
}


//���ҽ��ܵ�ǰ���������widget
//�ȿ��ÿ�ݼ��Ƿ����Լ������д���Ȼ���Ƿ����Լ����ϼ������д���
//self->father->grapha->...->screen->root 
CUIWidget *CUIWidget::FindWidgetForAccel( const stAccelItem& accel )
{
	
	CUIWidget *pWgt=this;
	while(pWgt) 
	{ 
		if( pWgt->IsAccelIn(accel) )
			return pWgt;
		
		pWgt=pWgt->GetFather();
	}

	return NULL;
	
}

std::vector<stAccelItem>& CUIWidget::GetWgtAccels()
{
	return m_Accels;
}

//�жϴ����Ƿ��ܹ�����ָ���Ŀ�ݼ�
BOOL CUIWidget::IsAccelIn(const stAccelItem& accel)
{
	if(m_Accels.empty()) return FALSE;
	
	for(int i=0;i<m_Accels.size();i++)
	{ 
		if(m_Accels[i]==accel)
		return  TRUE;			
	}	
	
	return FALSE;
}

//�Ƿ�ĳ������focus��
BOOL CUIWidget::IsChildFocused()
{

	bool bResult=false;
	
	CUIWidget* pWgt=g_pRootWgt->GetFocusWgt();
	if(pWgt&&(pWgt->GetFather()==this)) bResult=true;

	return bResult;

}

//�Ƿ��ӻ��ߺ��ӵ��¼�����focus��
BOOL CUIWidget::IsDescendFocused()
{

	bool bResult=false;

	CUIWidget* pWgt=g_pRootWgt->GetFocusWgt();
	while (pWgt)
	{
		pWgt=pWgt->GetFather();

		if(pWgt==this) 
		{
			bResult=true;
			break;
		}

	}	

	return bResult;

}



//�Ƿ�ĳ������hot��
BOOL CUIWidget::IsChildHot()
{

	bool bResult=false;
	
	CUIWidget* pWgt=g_pRootWgt->GetHotWgt();
	if(pWgt&&(pWgt->GetFather()==this)) bResult=true;

	return bResult;

}

//�Ƿ��ӻ��ߺ��ӵ��¼�����hot��
BOOL CUIWidget::IsDescendHot()
{

	bool bResult=false;

	CUIWidget* pWgt=g_pRootWgt->GetHotWgt();
	while (pWgt)
	{
		pWgt=pWgt->GetFather();

		if(pWgt==this) 
		{
			bResult=true;
			break;
		}

	}	

	return bResult;

}


BOOL CUIWidget::IsInDescend(CUIWidget* pWgt)
{
	
	bool bResult=false;

	while (pWgt)
	{
		pWgt=pWgt->GetFather();

		if(pWgt==this) 
		{
			bResult=true;
			break;
		}

	}	

	return bResult;

}

//x,yΪ����ڴ���ԭ�������
stColorQuad CUIWidget::GetPointColor( int x, int y )
{

	//�߼���������,��ʾ��ʹ��callback��ɫ�ж��϶�
	if(m_Style&WGTSTYLE_COLORTEST)
	{

		stItemPixel theItem;
		theItem.type=m_DragType;
		theItem.index=m_DragIndex;
		theItem.hosttype=m_HostType;
		theItem.hostindex=m_HostIndex;
		theItem.x=x;
		theItem.y=y;

		ExecCallBackFunc(UI_Callback_GetItemPixel,(void*)&theItem);		
		return theItem.color;

	}
	

	//ֻ���Ǳ����͹̶�ƴͼ(װ��ͼƬ)
	stColorQuad color;
	
	if(m_pBackPic)
		color=m_pBackPic->GetPointColor(x-m_BackPicPos.x,y-m_BackPicPos.y,m_CurFrame);

	//�����ڸõ㲻͸��
	if(color.a!=0)
		return color;

	
	for(int i=0;i<UI_ADORNPIC_NUMBER;i++)
	{ 
		if(m_pAdornPic[i])
		{
			color=m_pAdornPic[i]->GetPointColor(x-m_AdornPicPos[i].x,y-m_AdornPicPos[i].y,m_CurFrame);
			
			//װ��ͼƬ�ڸõ㲻͸��
			if(color.a!=0)
				return color;
		}
		
	}

	return color;

}

BOOL CUIWidget::IsPointIn( int x, int y )
{
	//������WinRect,ExWinRect��
	if(!IsPointInWinRect(x,y))
		return FALSE;

	//�����ڱ���ͼƬ�ķ�͸������
	if(m_Style&WGTSTYLE_ALPHAWIN)
	{
		if(GetPointColor(x-m_WinRect.ox,y-m_WinRect.oy).a==0)
		return FALSE;
	}

	return TRUE;		 
	
}


BOOL CUIWidget::IsRawInDragRect( int x, int y )
{

	if( x>m_WinRect.ox+m_DragRect.ox && x<m_WinRect.ox+m_DragRect.ox+m_DragRect.width &&
		y>m_WinRect.oy+m_DragRect.oy && y<m_WinRect.oy+m_DragRect.oy+m_DragRect.height )
		return TRUE;
	else 
		return FALSE;	

}

BOOL CUIWidget::IsPointInDragRect( int x, int y )
{

	//��ת�϶�����
	if(m_Style&WGTSTYLE_FLIPDRAG)
		return (!IsRawInDragRect(x,y));
	else 
		return IsRawInDragRect(x,y);

}

BOOL CUIWidget::IsPointInWinRect( int x, int y )
{
	if( x>m_WinRect.ox && x<m_WinRect.ox+m_WinRect.width &&
		y>m_WinRect.oy && y<m_WinRect.oy+m_WinRect.height )
	{
		//������edgeline��������δ���������
		return IsPointInsideWinShape(x,y);
	}
	else return FALSE;
}

BOOL CUIWidget::IsPointInClientRect( int x, int y )
{
	if( x>m_WinRect.ox+m_ClientRect.ox && x<m_WinRect.ox+m_ClientRect.ox+m_ClientRect.width &&
		y>m_WinRect.oy+m_ClientRect.oy && y<m_WinRect.oy+m_ClientRect.oy+m_ClientRect.height )
		return TRUE;
	else return FALSE;
}

BOOL CUIWidget::IsPointInCaptionRect( int x, int y )
{
	if( x>m_WinRect.ox+m_CaptionRect.ox && x<m_WinRect.ox+m_CaptionRect.ox+m_CaptionRect.width &&
		y>m_WinRect.oy+m_CaptionRect.oy && y<m_WinRect.oy+m_CaptionRect.oy+m_CaptionRect.height )
		return TRUE;
	else return FALSE;
}

BOOL CUIWidget::IsInItemDragRect( int x, int y )
{
	if(m_ItemDragFlag<0)	return FALSE;

	if(m_ItemDragFlag==RECT_FLAG_WINDOW)
	{
		return IsPointInWinRect(x,y);
	}
	else if(m_ItemDragFlag==RECT_FLAG_CLIENT)
	{
		return IsPointInClientRect(x,y);
	}
	else if(m_ItemDragFlag==RECT_FLAG_CAPTION)
	{
		return IsPointInCaptionRect(x,y);
	}
	else if(m_ItemDragFlag==RECT_FLAG_DRAG)
	{
		return IsPointInDragRect(x,y);
	}
	else
	{
		return TRUE;
	}

	
}



BOOL CUIWidget::IsInItemDropRect( int x, int y )
{
	
	if(m_ItemDropFlag<0)	return FALSE;

	if(m_ItemDropFlag==RECT_FLAG_WINDOW)
	{
		return IsPointInWinRect(x,y);
	}
	else if(m_ItemDropFlag==RECT_FLAG_CLIENT)
	{
		return IsPointInClientRect(x,y);
	}
	else if(m_ItemDropFlag==RECT_FLAG_CAPTION)
	{
		return IsPointInCaptionRect(x,y);
	}
	else if(m_ItemDropFlag==RECT_FLAG_DRAG)
	{
		return IsPointInDragRect(x,y);
	}
	else
	{
		return TRUE;
	}
}


bool CUIWidget::IsPointInsideWinShape(int x,int y)
{

	//û�ж�����չ���ھ���
	if(m_vecWinShape.empty()) return TRUE;

	Point2D pt(x-m_WinRect.ox,y-m_WinRect.oy);

	bool bIncludeShape=false;//����û�а�Χshape
	bool bInside=false;//��ĳ����Χshape֮��
	for(int i=0;i<m_vecWinShape.size();i++)
	{ 
		//����һ����Χshape
		if(m_vecWinShape[i].bFlag)
			bIncludeShape=true;
		
		if(m_vecWinShape[i].IsPointInside(pt.x,pt.y))
		{
			//����һ���ų�shape
			if(!m_vecWinShape[i].bFlag)
				return false;
			else 
				bInside=true;
		}
		
	}

	if(bIncludeShape)
		return bInside;
	else//���һ����Χshape��û�У�ֻ���ų�shape
		return true;
}

int CUIWidget::IsPointInAjustRect(int x,int y)
{

	if(m_RenderFactor<=0.0f) return EDGE_FLAG_NONE;

	int flag=EDGE_FLAG_NONE;
	

	//ת��Ϊ��Դ���ԭ�������
	Point2D pt(x-m_WinRect.ox,y-m_WinRect.oy);

	//����ÿ�����ߣ��������ͱ��ߵľ���
	for(int i=0;i<EDGE_LINE_MAX;i++)
	{ 
		switch(i) 
		{
		case EDGE_LINE_UP:
			{
				int dis=m_EdgeLines[i].PointSegDisV(pt);
				
				if((dis<0)||(dis>m_EdgeThick))	break;
				
				return EDGE_FLAG_UP;

			}
			break;

		case EDGE_LINE_DOWN:
			{
				int dis=m_EdgeLines[i].PointSegDisV(pt);
				
				if((dis<0)||(dis>m_EdgeThick))	break;
				
				return EDGE_FLAG_DOWN;

			}
			break;

		case EDGE_LINE_LEFT:
			{
				int dis=m_EdgeLines[i].PointSegDisH(pt);
				
				if((dis<0)||(dis>m_EdgeThick))	break;
				
				return EDGE_FLAG_LEFT;

			}
			break;

		case EDGE_LINE_RIGHT:
			{
				int dis=m_EdgeLines[i].PointSegDisH(pt);
				
				if((dis<0)||(dis>m_EdgeThick))	break;
				
				return EDGE_FLAG_RIGHT;

			}
			break;

		default:
			break;
		}		
	}

	if(m_EdgeLines[EDGE_LINE_UP].m_vecPoints.empty() 
	  &&(pt.x>=0)
	  &&(pt.x<=m_WinRect.width)
	  &&(pt.y>=0)
	  &&(pt.y<=m_EdgeThick))
	    flag|=EDGE_FLAG_UP;


	if(m_EdgeLines[EDGE_LINE_DOWN].m_vecPoints.empty() 
	  &&(pt.x>=0)
	  &&(pt.x<=m_WinRect.width)
	  &&(pt.y<=m_WinRect.height)
	  &&(pt.y>=(m_WinRect.height-m_EdgeThick)) )
		flag|=EDGE_FLAG_DOWN;

	if(m_EdgeLines[EDGE_LINE_LEFT].m_vecPoints.empty() 
	  &&(pt.y>=0)
	  &&(pt.y<=m_WinRect.height) 
	  &&(pt.x>=0)
	  &&(pt.x<=m_EdgeThick) )
		flag|=EDGE_FLAG_LEFT;
	
	
	if(m_EdgeLines[EDGE_LINE_RIGHT].m_vecPoints.empty() 
	  &&(pt.y>=0)
	  &&(pt.y<=m_WinRect.height) 
	  &&(pt.x<=m_WinRect.width)
	  &&(pt.x>=(m_WinRect.width-m_EdgeThick)) )
		flag|=EDGE_FLAG_RIGHT;
	
	
	return (flag&m_AjustEdge);

}


int CUIWidget::IsOutofScreen()
{

	int flag=UI_TOUCH_NONE;

	if(g_pRootWgt&&(g_pRootWgt->GetCurScreen()))
	{

		stRect scrnRc=g_pRootWgt->GetCurScreen()->GetWinRect();
		
		if(m_WinRect.ox < scrnRc.ox)
			flag|=UI_TOUCH_LEFT_OUT;
		else if((m_WinRect.ox+m_WinRect.width)>(scrnRc.ox+scrnRc.width))
			flag|=UI_TOUCH_RIGHT_OUT;
		
		if(m_WinRect.oy < scrnRc.oy)
			flag|=UI_TOUCH_TOP_OUT;
		else if((m_WinRect.oy+m_WinRect.height)>(scrnRc.oy+scrnRc.height))
			flag|=UI_TOUCH_BOTTOM_OUT;
	}

	return flag;
	
}

void CUIWidget::PullIntoScreen()
{

	if( !g_pRootWgt || !(g_pRootWgt->GetCurScreen()) ) return; 

	stRect scrnRc=g_pRootWgt->GetCurScreen()->GetWinRect();
	
	if(m_WinRect.ox < scrnRc.ox)
		AlignToRect(scrnRc,ALIGNSTYLE_LEFT_IN);
	else if((m_WinRect.ox+m_WinRect.width)>(scrnRc.ox+scrnRc.width))
		AlignToRect(scrnRc,ALIGNSTYLE_RIGHT_IN);
	
	if(m_WinRect.oy < scrnRc.oy)
		AlignToRect(scrnRc,ALIGNSTYLE_TOP_IN);
	else if((m_WinRect.oy+m_WinRect.height)>(scrnRc.oy+scrnRc.height))
		AlignToRect(scrnRc,ALIGNSTYLE_BOTTOM_IN);

}

//�õ������У�ָ����ȣ��߶ȵ���������
//����������ָ��ǰû�б��ɼ��Ӵ��ڸ��ǵ�����
//���Ҳ���ָ����ȸ߶ȵ���������ʱ����С��������ķ�Χ
stRect CUIWidget::GetFreeRect(int width,int height)
{
	//Ĭ����Ч(���ڿ�ȣ��߶�Ϊ0����㳬����Ļ)
	stRect freeRct(-1,-1,0,0);

	//�㷨����ʵ��	

	return freeRct;

}

//�ۺ�visible,rawvisible,enable����Ϣ
//�жϴ����Ƿ���Խ���
BOOL CUIWidget::IsInteractive()
{
	if(!IsVisible())	return FALSE;
	if(!IsEnabled())	return FALSE;
	
	
	if(m_Pattern&WGTPATTERN_RAWVISIBLE)
	{
		if(!IsRawVisible())	return FALSE;	
	}
	

	return TRUE;
}

//�Ӿ����Ƿ�ɼ�
//ÿ�����ڶ��ǵ��뵭�����ڣ�ֻ�ǵ��뵭�����ٶȲ�ͬ
//�߼��ϲ��ɼ�,�Ӿ���Ҳ���ܿɼ�
//�޸�:�߼��Ͽɼ�,�Ӿ���Ҳ���ܲ��ɼ�
//(ԭ��:ֻҪ�߼��Ͽɼ����Ӿ��ϾͿɼ�)
BOOL CUIWidget::IsRawVisible()
{

	//add by lincoln [2006:7:19] [10:47]
	//{{{�����Ч���Ż�
	if(m_State&WGTSTATE_VISIBLE)
	{
		//WGTPATTERN_ALPHAVISIBLE:�����ȫ͸�����Ӿ��ϾͲ��ɼ����Ͳ�������
		if(m_Pattern&WGTPATTERN_ALPHAVISIBLE)
		{
			if(m_RenderFactor<=0.0) return FALSE;
		}
	}
	else 
	{
		if(m_RenderFactor<=0.0) return FALSE;
	}
	//}}}
	//�������ڲü�,��ʵ������Բ�����
	if(m_pFather)
	{
		int nStyle=m_pFather->GetStyle();
		stRect absWinRct=m_pFather->GetWinRect();
		stRect absClientRct=m_pFather->GetAbsoluteClientRect();
		
		if(nStyle&WGTSTYLE_CLIPCHILD_WINDOW) 
		{
			if(!IsRectIntersect(m_WinRect,absWinRct)) return FALSE;
		}
		else if(nStyle&WGTSTYLE_CLIPCHILD_CLIENT) 
		{
			if(!IsRectIntersect(m_WinRect,absClientRct)) return FALSE;
		}

	}

	//add by lincoln [2006:7:19] [10:47]
	//{{{�����Ч���Ż�
	/*
	if(m_State&WGTSTATE_VISIBLE)
	{
		if(m_Pattern&WGTPATTERN_ALPHAVISIBLE)
			return (m_RenderFactor>0.0);
		else 
			return TRUE;
		
	}
	else 
	{
		return (m_RenderFactor>0.0);	
	}
	*/
	return TRUE;	
	//}}}



}

BOOL CUIWidget::IsVisibleEx()
{
	CUIWidget* pWgt=this;
	while(pWgt) 
	{ 
		if(!pWgt->IsVisible()) 
			return FALSE;

		pWgt=pWgt->GetFather();
	}

	return TRUE;
}


BOOL CUIWidget::IsEnabledEx()
{
	CUIWidget* pWgt=this;
	while(pWgt) 
	{ 
		if(!pWgt->IsEnabled()) 
			return FALSE;

		pWgt=pWgt->GetFather();
	}

	return TRUE;
}

//static���ں�disable���ڵ���������:
//����static�����Ƿ��ڸ�����֮��
//����϶�static����,���൱�ڶԸ����ڲ���
//������϶�disable���ڲ���
//����ָ���㴦��widget
//�ȿ��õ��Ƿ����Լ��ĺ������棬Ȼ���Ƿ����Լ�����
CUIWidget *CUIWidget::FindWidgetOnPoint( int x, int y )
{

	if( IsInteractive() )
	{		

		//ȷ�����к��Ӱ��ջ���˳������
		if(!IsSorted()) SortSelf();


		bool bClipped=false;
		
		if(m_Style&WGTSTYLE_CLIPCHILD_WINDOW) 
		{
			if(!IsPointInWinRect(x,y)) bClipped=true;
		}
		else if(m_Style&WGTSTYLE_CLIPCHILD_CLIENT) 
		{
			if(!IsPointInClientRect(x,y)) bClipped=true;				
		}

		if(m_Type==WGTTYPE_TABWIN)
		{
			if(!IsChecked())
				bClipped=true;
		}

		//������,��Խ�ߵ�Խ������,����,Ҫ�Ӻ��濪ʼ
		//Ҳ�����ò�ϸߵ��ֵ�����
		if(!bClipped)
		for( int i=m_Children.size()-1; i>=0; i-- )
		{
			CUIWidget *pwin = m_Children[i]->FindWidgetOnPoint(x, y);		

			if( pwin) return pwin;
		}
	}

	//����static�����Ƿ��ڸ�����֮��
	//����϶�static����,���൱�ڶԸ����ڲ���
	if( !IsPointIn(x, y) || !IsInteractive() ) 
		return NULL;
	else if(IsStaticWidget())
	{
		return StaticQueryFather();
	}
	else 
		return this;
		
}





void CUIWidget::AjustChildren()
{

	//�����Լ��ĺ��Ӵ�С
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->AjustRecursive();
	}

}


void CUIWidget::AjustRecursive()
{
	
	AjustChildren();
	
	
	AjustSelf();//�����Լ�	

}

void CUIWidget::AlignChildren()
{
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->AlignRecursive();
	}
}

void CUIWidget::AlignRecursive()
{
	
	AlignSelf();
	
	AlignChildren();
	
	
}


void CUIWidget::ArrangeChildRect()
{
	if(m_vecChildRect.empty())
		return;

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		CUIWidget* pChild=(*iter);
		int rectId=pChild->GetRectId();
		
		if(rectId>=0)
			pChild->SetRectId(rectId);
		
	}
}



void CUIWidget::SortByGroupStop( )
{
	std::sort( m_Children.begin(),m_Children.end(), SortWidgetByGroupStop );
	SetSorted(false);//�ı��˴���,��Ҫ���°���layer����
}

void CUIWidget::SortByTabStop( )
{
	std::sort( m_Children.begin(),m_Children.end(), SortWidgetByTabStop );
	SetSorted(false);//�ı��˴���,��Ҫ���°���layer����
}

void CUIWidget::SortByAlignId( )
{
	std::sort( m_Children.begin(),m_Children.end(), SortWidgetByAlignId );
	SetSorted(false);//�ı��˴���,��Ҫ���°���layer����
}

//sort self by layer
void CUIWidget::SortSelf( )
{
	std::sort( m_Children.begin(),m_Children.end(), SortWidgetByLayer );
	SetSorted(true);
}

//ǿ���������º��ӵ�prelayer
void CUIWidget::UpdateChildPreLayer()
{

	int preLayer;
	std::vector<CUIWidget *>::iterator iter;
	for( preLayer=0,iter=m_Children.begin(); iter!=m_Children.end(); iter++,preLayer++ )
	{
		(*iter)->SetPreLayer(preLayer);		
	}

}


void CUIWidget::AjustSelf()
{
	
	SetAjusted(true);
}

void CUIWidget::FitBkImageSize()
{

	if(m_pBackPic)
	{
		m_WinRect.width=m_pBackPic->GetWidth();
		m_WinRect.height=m_pBackPic->GetHeight();
	}
	
}


void CUIWidget::AlignSelf()
{
		
	CUIWidget* pAlignWdgt=0;
	stRect AlignRect;
	
	if(m_AlignType&ALIGNTYPE_NONE) 
		pAlignWdgt=0;		
	if(m_AlignType&ALIGNTYPE_FATHER) 
		pAlignWdgt=GetFather();			
	else
	if(m_AlignType&ALIGNTYPE_GRANDPA) 
		pAlignWdgt=GetGrandpa();
	else
	if(m_AlignType&ALIGNTYPE_UNCLE) 
		pAlignWdgt=GetUncle(m_AlignToId);
	else
	if(m_AlignType&ALIGNTYPE_BROTHER) 
		pAlignWdgt=GetBrother(m_AlignToId);
				
				
				
	if(pAlignWdgt) 
	{ 
					
		if(m_AlignType&ALIGNTYPE_WINRECT)     
			AlignRect=pAlignWdgt->GetWinRect();
		else 
		{
			if(m_AlignType&ALIGNTYPE_CLIENTRECT)  
				AlignRect=pAlignWdgt->GetClientRect();			
			else 
			if(m_AlignType&ALIGNTYPE_DRAGRECT)    
				AlignRect=pAlignWdgt->GetDragRect();
			else 
			if(m_AlignType&ALIGNTYPE_CAPTIONRECT) 
				AlignRect=pAlignWdgt->GetCaptionRect();

			AlignRect.ox+=pAlignWdgt->GetWinRect().ox;
			AlignRect.oy+=pAlignWdgt->GetWinRect().oy;
		}
		
		AlignToRect(AlignRect,m_AlignStyle,m_MarginH,m_MarginV);
								
	}


}

//���ڵ��Եĺ���
void CUIWidget::DrawDebugBarEdge()
{

	g_pDisplay->PushClipState();
	g_pDisplay->EnableClipRect(false);

	if(g_pRootWgt->GetMouseWgt()==this)
	{

		//��ɫ�ı�������Ե
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_CaptionRect.ox, 
		m_WinRect.oy+m_CaptionRect.oy, 
		m_CaptionRect.width, 
		m_CaptionRect.height,
		stColorQuad(255,255,255,255).c );
	

		//��ɫ���϶�����Ե
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_DragRect.ox, 
		m_WinRect.oy+m_DragRect.oy, 
		m_DragRect.width, 
		m_DragRect.height,
		stColorQuad(255,0,0,255).c );

	}
	else 
	{

		//��ɫ�Ŀͻ�����Ե
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_ClientRect.ox, 
		m_WinRect.oy+m_ClientRect.oy, 
		m_ClientRect.width, 
		m_ClientRect.height,
		stColorQuad(0,0,255,255).c );
	
		//��ɫ���ڱ�Ե
		g_pDisplay->DrawBox( 
		m_WinRect.ox, 
		m_WinRect.oy, 
		m_WinRect.width, 
		m_WinRect.height, 
		stColorQuad(255,255,255,255).c );

	} 
			

	int i,j;

	//����·����
	int sx=m_WinRect.ox+m_WinRect.width*0.5;
	int sy=m_WinRect.oy+m_WinRect.height-90;
	
	if(m_PathPoints.size()>0)
	g_pDisplay->DrawBar(sx,sy,8,8,stColorQuad(255,0,0,255).c);
		
	for(i=0;i<m_PathPoints.size();i++)
	{ 
		stUIPathPoint pt=m_PathPoints[i];


		sx=pt.x+m_WinRect.width*0.5;
		sy=pt.y+m_WinRect.height-90;

		g_pDisplay->DrawBar(sx,sy,8,8,stColorQuad(255,0,0,255).c);
	}


	//���ƴ�������
	std::vector<stRect>::iterator iter;
	for(iter=m_vecWakeArea.begin();iter!=m_vecWakeArea.end();iter++)
	{
		g_pDisplay->DrawBar(iter->ox+m_WinRect.ox,iter->oy+m_WinRect.oy,iter->width,iter->height,stColorQuad(0,255,255,255).c);
	}		


	//���Ƶ�������	
	for(i=0;i<EDGE_LINE_MAX;i++)
	{ 
		
		for(j=0;j<m_EdgeLines[i].m_vecPoints.size();j++)
		{
			DWORD dwColor=stColorQuad(128,128,0,255).c;
			if(i==EDGE_LINE_UP) dwColor=stColorQuad(255,0,0,255).c;
			else if(i==EDGE_LINE_DOWN) dwColor=stColorQuad(0,255,0,255).c;
			else if(i==EDGE_LINE_LEFT) dwColor=stColorQuad(0,0,255,255).c;

			g_pDisplay->DrawBar(m_EdgeLines[i].m_vecPoints[j].x+m_WinRect.ox,m_EdgeLines[i].m_vecPoints[j].y+m_WinRect.oy,8,8,dwColor);
		}
		
	}

	//�������δ���
	for(i=0;i<m_vecWinShape.size();i++)
	{
		stColorQuad color(255,0,0,128);
		if(!m_vecWinShape[i].bFlag) 
			color=stColorQuad(0,0,255,128);

		stRect rctDraw;
		

		if(m_vecWinShape[i].nType==SHAPE_TYPE_TRIANGLE)
		{
			
			g_pDisplay->DrawLine(
				m_vecWinShape[i].vecParams[0]+m_WinRect.ox,m_vecWinShape[i].vecParams[1]+m_WinRect.oy,
				m_vecWinShape[i].vecParams[2]+m_WinRect.ox,m_vecWinShape[i].vecParams[3]+m_WinRect.oy,
				color.c
				);

			g_pDisplay->DrawLine(
				m_vecWinShape[i].vecParams[2]+m_WinRect.ox,m_vecWinShape[i].vecParams[3]+m_WinRect.oy,
				m_vecWinShape[i].vecParams[4]+m_WinRect.ox,m_vecWinShape[i].vecParams[5]+m_WinRect.oy,
				color.c
				);

			g_pDisplay->DrawLine(
				m_vecWinShape[i].vecParams[4]+m_WinRect.ox,m_vecWinShape[i].vecParams[5]+m_WinRect.oy,
				m_vecWinShape[i].vecParams[0]+m_WinRect.ox,m_vecWinShape[i].vecParams[1]+m_WinRect.oy,
				color.c
				);

			
		}
		else
		{
			rctDraw.ox=m_vecWinShape[i].vecParams[0];
			rctDraw.oy=m_vecWinShape[i].vecParams[1];
			rctDraw.width=m_vecWinShape[i].vecParams[2];
			rctDraw.height=m_vecWinShape[i].vecParams[3];

			g_pDisplay->DrawBar(
			rctDraw.ox+m_WinRect.ox,
			rctDraw.oy+m_WinRect.oy,
			rctDraw.width,
			rctDraw.height,
			color.c);
		}
		
		
	
	}
	

	g_pDisplay->PopClipState();


}

void CUIWidget::DrawBarEdge()
{
	
	stColorQuad drawColor=GetRenderColor(m_DrawColor);
	stColorQuad edgeColor=GetRenderColor(m_EdgeColor);
	stColorQuad backColor=GetRenderColor(m_BackColor);
	stColorQuad focusColor=GetRenderColor(m_FocusColor);


	//���Ʊ�����ɫ��ʵ�Ŀ�
	if( (m_DrawFlag&DRAWFLAG_WIN_FILL) && (backColor.a > 0) ) 
		g_pDisplay->DrawBar( m_WinRect.ox, m_WinRect.oy, m_WinRect.width, m_WinRect.height, backColor.c );

	if( (m_DrawFlag&DRAWFLAG_CLIENT_FILL) && (backColor.a > 0)  )
		g_pDisplay->DrawBar( 
		m_WinRect.ox+m_ClientRect.ox, 
		m_WinRect.oy+m_ClientRect.oy, 
		m_ClientRect.width,
		m_ClientRect.height,
		backColor.c );

	if( (m_DrawFlag&DRAWFLAG_CAPTION_FILL) && (backColor.a > 0)  )
		g_pDisplay->DrawBar( 
		m_WinRect.ox+m_CaptionRect.ox, 
		m_WinRect.oy+m_CaptionRect.oy, 
		m_CaptionRect.width, 
		m_CaptionRect.height,
		backColor.c );

	if( (m_DrawFlag&DRAWFLAG_DRAG_FILL) && (backColor.a > 0)  )
		g_pDisplay->DrawBar( 
		m_WinRect.ox+m_DragRect.ox, 
		m_WinRect.oy+m_DragRect.oy, 
		m_DragRect.width, 
		m_DragRect.height,
		backColor.c );


	
	//���Ʊ�����ɫ�ı�Ե
	if( (m_DrawFlag&DRAWFLAG_WIN_EDGE) && (edgeColor.a > 0) ) 
			g_pDisplay->DrawBox( m_WinRect.ox, m_WinRect.oy, m_WinRect.width, m_WinRect.height, edgeColor.c );
		
	if( (m_DrawFlag&DRAWFLAG_CLIENT_EDGE) && (edgeColor.a > 0) )
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_ClientRect.ox, 
		m_WinRect.oy+m_ClientRect.oy, 
		m_ClientRect.width, 
		m_ClientRect.height,
		edgeColor.c );
	
	if( (m_DrawFlag&DRAWFLAG_CAPTION_EDGE) && (edgeColor.a > 0) )
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_CaptionRect.ox, 
		m_WinRect.oy+m_CaptionRect.oy, 
		m_CaptionRect.width, 
		m_CaptionRect.height,
		edgeColor.c );
	
	if( (m_DrawFlag&DRAWFLAG_DRAG_EDGE) && (edgeColor.a > 0) )
		g_pDisplay->DrawBox( 
		m_WinRect.ox+m_DragRect.ox, 
		m_WinRect.oy+m_DragRect.oy, 
		m_DragRect.width, 
		m_DragRect.height,
		edgeColor.c );
}

stColorQuad CUIWidget::GetCaptionColor()
{

	stColorQuad drawColor;
	
	if((m_DrawFlag&DRAWFLAG_HOTCAPTION)&&(IsMouseOver()))
	{
		drawColor=GetRenderColor(m_FocusColor);
	}
	else if((m_DrawFlag&DRAWFLAG_FOCUSCAPTION)&&(IsFocused()))
	{
		drawColor=GetRenderColor(m_FocusColor);
	}
	else 
	{
		drawColor=GetRenderColor(m_DrawColor);
	}

	return drawColor;
}

void CUIWidget::DrawCaption()
{

	if( m_Caption.empty() ) return;

	
	stRect captionRect=GetAbsoluteCaptionRect();

	stColorQuad drawColor=GetCaptionColor();
	
	Point2D captionOffset(0,0);

	if((m_CurFrameState>=0)&&(m_CurFrameState<FRAME_STATE_MAX))
	{
		captionOffset=m_CaptionOffset[m_CurFrameState];
	}

	g_pDisplay->DrawTextEx( 
		captionRect.ox+captionOffset.x, 
		captionRect.oy+captionOffset.y, 
		captionRect.width, 
		captionRect.height, 
		m_Caption.c_str(), 
		m_TextSize,m_RowSpace, 
		drawColor.c, 
		m_TextStyle );
	

}

//��ָ��ͼƬ��ָ��֡���һ������
//���ˮƽ������ֱ�������Ͳü�
//bClip:�Ƿ�ü����������ͼ��
void CUIWidget::DrawImageArea(stRect area,CUIImage* pImge,DWORD color,int frame/*=0*/,bool bClip/*=TRUE*/)
{

	if(area.width<0) return;
	if(area.height<0) return;

	if(frame<0) return ;
	if(!pImge) return;


	int sx;
	int sy;

	int endx=(area.ox+area.width);
	int endy=(area.oy+area.height);

	int sizex=pImge->GetWidth();
	int sizey=pImge->GetHeight();

	if (sizex == 0 || sizey == 0) 
	{
		return;
	}


	sy=area.oy;
	while(sy<endy)
	{

		sx=area.ox;
		while(sx<endx)
		{
			if((sx+sizex)>endx ||(sy+sizey)>endy)
				g_pDisplay->PushClipRect(sx,sy,endx-sx,endy-sy);

			
			pImge->Draw(sx,sy,frame,fxTexMul,color);

			if((sx+sizex)>endx ||(sy+sizey)>endy)
				g_pDisplay->PopClipState();


			//����ʹ��
			//g_pDisplay->DrawBox(sx,sy,endx-sx,endy-sy,color);

			sx+=sizex;
		}

		sy+=sizey;

	}


}



//��ָ��ͼƬ��ָ��֡ˮƽ���һ������
//���ˮƽ�������Ͳü�
//bClip:�Ƿ�ü����������ͼ��
void CUIWidget::DrawImageAreaH(stRect area,CUIImage* pImge,DWORD color,int frame/*=0*/,bool bClip/*=TRUE*/)
{

	if(area.width<0) return;
	if(area.height<0) return;

	if(frame<0) return ;
	if(!pImge) return;


	//����֮��հ�����֡��
	int begin=area.ox;
	int end=area.ox+area.width;
	int size=pImge->GetWidth();
	
	int num=0;
	
	if(size>0) 
		num=ceil(area.width*1.0/size);
	
	//���Ƴ����һ������м�֡
	for(int i=0;i<num;i++)
	{
		if(((end-begin)<size)&&bClip)
			g_pDisplay->PushClipRect(begin,area.oy,(end-begin),area.height);

		pImge->Draw(begin,area.oy,frame,fxTexMul,color);
		
		if(((end-begin)<size)&&bClip)
			g_pDisplay->PopClipState();
		//����ʹ��
		//g_pDisplay->DrawBox(begin,area.oy,(end-begin),area.height,color);


		begin+=size;

	}


}

//��ָ��ͼƬ��ָ��֡��ֱ���һ������
//�����ֱ�������Ͳü�
//bClip:�Ƿ�ü����������ͼ��
void CUIWidget::DrawImageAreaV(stRect area,CUIImage* pImge,DWORD color,int frame/*=0*/,bool bClip/*=TRUE*/)
{

	if(area.width<0) return;
	if(area.height<0) return;

	if(frame<0) return ;
	if(!pImge) return;

	//����֮��հ�����֡��
	int begin=area.oy;
	int end=area.oy+area.height;
	int size=pImge->GetHeight();
	
	int num=0;
	
	if(size>0) 
		num=ceil(area.height*1.0/size);
	
	//���Ƴ����һ������м�֡
	for(int i=0;i<num;i++)
	{
		if(((end-begin)<size)&&bClip)
			g_pDisplay->PushClipRect(area.ox,begin,area.width,(end-begin));

		pImge->Draw(area.ox,begin,frame,fxTexMul,color);
		
		if(((end-begin)<size)&&bClip)
			g_pDisplay->PopClipState();

		//����ʹ��
		//g_pDisplay->DrawBox(area.ox,begin,area.width,(end-begin),color);

		begin+=size;

	}

}

void CUIWidget::DrawPatchPic()
{

	stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));

	int x=0;
	int y=0;

	//�������ϻ��Ƶ�ƴ��ͼ�ߴ�
	int upleft=0;
	int upright=0;
	int downleft=0;
	int downright=0;

	int leftup=0;
	int rightup=0;
	int leftdown=0;
	int rightdown=0;		


	//�ĸ���////////////////////////////////////////////////////////////////////////
	
	if(m_pPatchPic[PATCHID_UP_LEFT])
	{
		x=m_WinRect.ox;
		y=m_WinRect.oy;
		m_pPatchPic[PATCHID_UP_LEFT]->Draw(x,y,m_CurFrame,fxTexMul,drawColor.c);

		upleft=m_pPatchPic[PATCHID_UP_LEFT]->GetWidth();
		leftup=m_pPatchPic[PATCHID_UP_LEFT]->GetHeight();

	}
	
	if(m_pPatchPic[PATCHID_UP_RIGHT])
	{
		x=m_WinRect.ox+m_WinRect.width-m_pPatchPic[PATCHID_UP_RIGHT]->GetWidth();
		y=m_WinRect.oy;
		m_pPatchPic[PATCHID_UP_RIGHT]->Draw(x,y,m_CurFrame,fxTexMul,drawColor.c);

		upright=m_pPatchPic[PATCHID_UP_RIGHT]->GetWidth();
		rightup=m_pPatchPic[PATCHID_UP_RIGHT]->GetHeight();
	}

	if(m_pPatchPic[PATCHID_DOWN_LEFT])
	{
		x=m_WinRect.ox;
		y=m_WinRect.oy+m_WinRect.height-m_pPatchPic[PATCHID_DOWN_LEFT]->GetHeight();
		m_pPatchPic[PATCHID_DOWN_LEFT]->Draw(x,y,m_CurFrame,fxTexMul,drawColor.c);

		downleft=m_pPatchPic[PATCHID_DOWN_LEFT]->GetWidth();
		leftdown=m_pPatchPic[PATCHID_DOWN_LEFT]->GetHeight();

	}

	if(m_pPatchPic[PATCHID_DOWN_RIGHT])
	{
		x=m_WinRect.ox+m_WinRect.width-m_pPatchPic[PATCHID_DOWN_RIGHT]->GetWidth();
		y=m_WinRect.oy+m_WinRect.height-m_pPatchPic[PATCHID_DOWN_RIGHT]->GetHeight();
		m_pPatchPic[PATCHID_DOWN_RIGHT]->Draw(x,y,m_CurFrame,fxTexMul,drawColor.c);

		downright=m_pPatchPic[PATCHID_DOWN_RIGHT]->GetWidth();
		rightdown=m_pPatchPic[PATCHID_DOWN_RIGHT]->GetHeight();
	}
	
	//�м�հ�ƴͼ////////////////////////////////////////////////////////////////////////
	
	if(m_pPatchPic[PATCHID_MIDDLE])
	{		
		int maxleft=Max(upleft,downleft);
		int maxup=Max(leftup,rightup);
		int maxright=Max(upright,downright);
		int maxdown=Max(leftdown,rightdown);

		stRect area;
		area.ox=m_WinRect.ox+maxleft;
		area.oy=m_WinRect.oy+maxup;
		area.width=m_WinRect.width-maxleft-maxright;
		area.height=m_WinRect.height-maxup-maxdown;
		DrawImageArea(area,m_pPatchPic[PATCHID_MIDDLE],drawColor.c,m_CurFrame);
	}
	else if((m_PatchRect.width>0)&&(m_PatchRect.height>0))
	{
		stRect area;
		area.ox=m_WinRect.ox+m_PatchRect.ox;
		area.oy=m_WinRect.oy+m_PatchRect.oy;
		area.width=m_WinRect.width-m_PatchRect.ox-m_PatchRect.width;
		area.height=m_WinRect.height-m_PatchRect.oy-m_PatchRect.height;

		stColorQuad patchColor=GetRenderColor(m_BackColor);
		g_pDisplay->DrawBar(area.ox,area.oy,area.width,area.height,patchColor.c);	 
	}


	//�ĸ���ƴͼ////////////////////////////////////////////////////////////////////////
	
	
	if(m_pPatchPic[PATCHID_UP_MIDDLE])
	{
		stRect area;
		area.ox=m_WinRect.ox+upleft;
		area.oy=m_WinRect.oy;
		area.width=m_WinRect.width-upleft-upright;
		area.height=m_pPatchPic[PATCHID_UP_MIDDLE]->GetHeight();
		DrawImageAreaH(area,m_pPatchPic[PATCHID_UP_MIDDLE],drawColor.c,m_CurFrame);
	}

	if(m_pPatchPic[PATCHID_DOWN_MIDDLE])
	{
		stRect area;
		area.ox=m_WinRect.ox+downleft;
		area.oy=m_WinRect.oy+m_WinRect.height-m_pPatchPic[PATCHID_DOWN_MIDDLE]->GetHeight();
		area.width=m_WinRect.width-downleft-downright;
		area.height=m_pPatchPic[PATCHID_DOWN_MIDDLE]->GetHeight();
		DrawImageAreaH(area,m_pPatchPic[PATCHID_DOWN_MIDDLE],drawColor.c,m_CurFrame);
	}

	if(m_pPatchPic[PATCHID_LEFT_MIDDLE])
	{
		stRect area;
		area.ox=m_WinRect.ox;
		area.oy=m_WinRect.oy+leftup;
		area.width=m_pPatchPic[PATCHID_LEFT_MIDDLE]->GetWidth();
		area.height=m_WinRect.height-leftup-leftdown;
		DrawImageAreaV(area,m_pPatchPic[PATCHID_LEFT_MIDDLE],drawColor.c,m_CurFrame);
	}

	if(m_pPatchPic[PATCHID_RIGHT_MIDDLE])
	{
		stRect area;
		area.ox=m_WinRect.ox+m_WinRect.width-m_pPatchPic[PATCHID_RIGHT_MIDDLE]->GetWidth();
		area.oy=m_WinRect.oy+rightup;
		area.width=m_pPatchPic[PATCHID_RIGHT_MIDDLE]->GetWidth();
		area.height=m_WinRect.height-rightup-rightdown;
		DrawImageAreaV(area,m_pPatchPic[PATCHID_RIGHT_MIDDLE],drawColor.c,m_CurFrame);
	}
	


}



//���Ʊ���ͼ
void CUIWidget::DrawBackPic()
{

	if( !m_pBackPic ) return;
	
	stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));
	
	if ( m_bAutoScale )
	{
		float xScale = m_WinRect.width / (1.0 * m_pBackPic->GetWidth());
		float yScale = m_WinRect.height / (1.0 * m_pBackPic->GetHeight());
		g_pDisplay->SetImageScale(xScale, yScale);
		m_pBackPic->Draw( (m_WinRect.ox+m_BackPicPos.x), (m_WinRect.oy+m_BackPicPos.y), m_CurFrame, fxTexMul | fxGeoScale, drawColor.c );
		g_pDisplay->SetImageScale(1.0, 1.0);
	}
	else
	{		
		m_pBackPic->Draw( (m_WinRect.ox+m_BackPicPos.x), (m_WinRect.oy+m_BackPicPos.y), m_CurFrame, fxTexMul, drawColor.c );
	}
		
}

//����װ��ͼ
void CUIWidget::DrawAdornPic()
{
	stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));
	
	for(int i=0;i<UI_ADORNPIC_NUMBER;i++)
	{ 
		if(m_pAdornPic[i])
			m_pAdornPic[i]->Draw( m_WinRect.ox+m_AdornPicPos[i].x, m_WinRect.oy+m_AdornPicPos[i].y, m_CurFrame, fxTexMul, drawColor.c );
	}
	
}

//������Чͼ
void CUIWidget::DrawEffectPic()
{
	
	if( !m_pEffectPic ) return;
	
	//û��ָ�����ǻ���
	if((m_DrawFlag&DRAWFLAG_DRAW_EFFECT)==0)
	{
		//ָ����ֻ��focus��ʱ�����
		if(m_DrawFlag&DRAWFLAG_FOCUS_EFFECT)
		{
			if(!IsMouseOver())	return;
		}

		//ָ����ֻ��enable��ʱ�����
		if(m_DrawFlag&DRAWFLAG_ENABLE_EFFECT)
		{
			if(!IsEnabled())	return;
		}

		//ָ����ֻ��check��ʱ�����
		if(m_DrawFlag&DRAWFLAG_CHECK_EFFECT)
		{
			if(!IsChecked())	return;
		}
		
	}
	
	stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));
	
	m_pEffectPic->Draw( m_WinRect.ox+m_EffectPicPos.x, m_WinRect.oy+m_EffectPicPos.y, m_CurEffectFrame, fxTexMul, drawColor.c );		
	
#ifdef _UIEDITMODE
	if(g_pRootWgt->GetEditMode()==UI_MODE_DEBUG) 
		g_pDisplay->DrawBox(m_WinRect.ox+m_EffectPicPos.x,m_WinRect.oy+m_EffectPicPos.y,m_pEffectPic->GetWidth(),m_pEffectPic->GetHeight(),drawColor.c);
#endif	
	
	
}

//�����к��ӻ���֮ǰ����
//ȱʡ�Ļ��ƺ���:���Ʊ����Ͳ�͸���п�
void CUIWidget::RenderSelf()
{
	
	bool bPriorBackDraw = (m_DrawFlag&DRAWFLAG_BACKPRIOR);
	if (bPriorBackDraw)
	{
		if(m_Style&WGTSTYLE_RENDERITEM)
			RenderCustomItem();

		if(m_Style&WGTSTYLE_RENDERBUFFER)
			RenderCustomBuffer();
	}

	//���ű���ͼ
	DrawBackPic();

	if(!bPriorBackDraw && (m_DrawFlag&DRAWFLAG_CLIENTPRIOR))
	{
	
		if(m_Style&WGTSTYLE_RENDERITEM)
			RenderCustomItem();

		if(m_Style&WGTSTYLE_RENDERBUFFER)
			RenderCustomBuffer();

	}
	
	//����ƴͼ
	DrawPatchPic();

	//�̶�����ƴͼ
	DrawAdornPic();


	//���Ʊ��ߺͱ���ɫ
	DrawBarEdge();


	if(!bPriorBackDraw && !(m_DrawFlag&DRAWFLAG_CLIENTPRIOR))
	{
	
		if(m_Style&WGTSTYLE_RENDERITEM)
			RenderCustomItem();

		if(m_Style&WGTSTYLE_RENDERBUFFER)
			RenderCustomBuffer();

	}

	
	DrawEffectPic();


#ifdef _UIEDITMODE
	if(g_pRootWgt->GetEditMode()==UI_MODE_DEBUG)
		DrawDebugBarEdge();
#endif

}


//�����к��ӻ�����֮�����
void CUIWidget::PostRenderSelf()
{
	
	stColorQuad drawColor=GetRenderColor(stColorQuad(255,255,255,255));
	
	
	if(m_pCheckCover&&IsChecked())
		m_pCheckCover->Draw( m_WinRect.ox+m_CheckCoverPos.x, m_WinRect.oy+m_CheckCoverPos.y, m_CurFrame, fxTexMul, drawColor.c );

	if(m_pHotCover&&IsMouseOver())
		m_pHotCover->Draw( m_WinRect.ox+m_HotCoverPos.x, m_WinRect.oy+m_HotCoverPos.y, m_CurFrame, fxTexMul, drawColor.c );

	if(m_pCoverPic)
		m_pCoverPic->Draw( m_WinRect.ox+m_CoverPicPos.x, m_WinRect.oy+m_CoverPicPos.y, m_CurFrame, fxTexMul, drawColor.c );
	
	
}

//������UpdateChildPreLayer�еĲ���
void CUIWidget::RenderChild()
{

	int preLayer;
	std::vector<CUIWidget *>::iterator iter;
	for( preLayer=0,iter=m_Children.begin(); iter!=m_Children.end(); iter++,preLayer++ )
	{
		(*iter)->RenderRecursive();
		(*iter)->SetPreLayer(preLayer);		
	}

}



void CUIWidget::RenderRecursive()
{
	
	//�����뵭��
	UpdateAlpha();	
	//�Ƿ��Ӿ��Ͽɼ�
	if( !IsRawVisible() ) return;
	assert(IsVisible()||(m_RenderFactor>0.0f));	

	
	if(m_Style&WGTSTYLE_CLIPSELF) 
	{
		g_pDisplay->PushClipRect(m_WinRect.ox,m_WinRect.oy,m_WinRect.width,m_WinRect.height);

	}

	g_pDisplay->SetFont(m_TextFont);
	RenderSelf();

	if(m_Style&WGTSTYLE_CLIPSELF) 
	{
		
		g_pDisplay->PopClipState();
	}

	if(m_Style&WGTSTYLE_CLIPCHILD_WINDOW) 
	{
		g_pDisplay->PushClipRect(m_WinRect.ox,m_WinRect.oy,m_WinRect.width,m_WinRect.height);

	}
	else if(m_Style&WGTSTYLE_CLIPCHILD_CLIENT) 
	{
		stRect absRc=GetAbsoluteClientRect();
		g_pDisplay->PushClipRect(absRc.ox,absRc.oy,absRc.width,absRc.height);
	}	

	RenderChild();

	if((m_Style&WGTSTYLE_CLIPCHILD_WINDOW)||(m_Style&WGTSTYLE_CLIPCHILD_CLIENT))
	{
		g_pDisplay->PopClipState();
	}
		

	if(m_Style&WGTSTYLE_CLIPSELF) 
	{
		g_pDisplay->PushClipRect(m_WinRect.ox,m_WinRect.oy,m_WinRect.width,m_WinRect.height);
	}
	
	g_pDisplay->SetFont(m_TextFont);
	PostRenderSelf();
	
	if(m_Style&WGTSTYLE_CLIPSELF) 
	{
		g_pDisplay->PopClipState();
	}


}

void CUIWidget::UpdateWakeArea()
{
	if(m_Style&WGTSTYLE_AREAWAKE==0)	return;

	//add by lincoln [2006:7:18] [11:54]
	//{{{�����Ч���Ż�
	if(m_vecWakeArea.empty()) return;
	//}}}

	CUIWidget* pMouseWgt=g_pRootWgt->GetMouseWgt();
	if(!pMouseWgt)	return;

	short cFlag=-1;	
	if(pMouseWgt==this)
	{
		cFlag=0;
	}
	else if(IsInDescend(pMouseWgt))
	{
		cFlag=1;
	}

	short cIndex=-1;

	int rx=CUICursor::m_Pos.x-m_WinRect.ox;
	int ry=CUICursor::m_Pos.y-m_WinRect.oy;
	std::vector<stRect>::iterator iter;
	for(iter=m_vecWakeArea.begin();iter!=m_vecWakeArea.end();iter++)
	{
		if(iter->IsPointIn(rx,ry))
		{
			cIndex=int(iter-m_vecWakeArea.begin());
			break;
		}

	}

	int areaId=MAKELONG(cFlag,cIndex); 
	if(areaId!=m_AreaId)
	{
		short oldIndex=LOWORD(m_AreaId);
		CallSafeScriptEx(SCRIPT_FUNC_ON_AREAWAKE,"%d,%d,%d",cIndex,oldIndex,cFlag);
		m_AreaId=areaId;
	}


}

//Ŀǰֻ�����ֵ�ĸı�,������չΪ����ı��ĸı�
void CUIWidget::UpdateMornitor()
{

	//add by lincoln [2006:7:18] [11:54]
	//{{{�����Ч���Ż�
	if(m_Mornitor.empty()) return;
	//}}}
	//�Ƿ��б���ر����ı�
	bool bChanged=false;
	
		std::map<int,stUIVar>::iterator iter;
		for(iter=m_Mornitor.begin();iter!=m_Mornitor.end();iter++)
		{ 

			stUIVar dstVar=iter->second;
			
			stUIVar srcVar(dstVar.iChannel,dstVar.iIndex,-1.0,0);
			GetVar(srcVar);

			if(!(dstVar==srcVar))
			{
				bChanged=true;
				iter->second=srcVar;
			}
			
		
	}
	
	if(bChanged)
	{
		CallSafeScript(SCRIPT_FUNC_ON_UPDATE);
	}


}

void CUIWidget::UpdateMove(float deltatime)
{
	
	if(m_PathPoints.empty()||IsPathMovePaused()) return;

	//add by lincoln [2006:8:17] [19:06]
	//{{{��������ܸ���
	//if(deltatime<0.02f) deltatime=0.02f;
	//else if(deltatime>0.06f) deltatime=0.06f;
	
	int moveLen=m_PathSpeed*deltatime;	
	//}}}
	
	stUIPathPoint pt;

	if(IsPathMoveFliped())
		pt=m_PathPoints[m_PathPoints.size()-1];
	else 
		pt=m_PathPoints[0];

	//����ƶ�
	if((pt.command&UI_PATHCMD_RELATIVE)&&m_pFather)
	{
		pt.x+=m_pFather->GetWinRect().ox;
		pt.y+=m_pFather->GetWinRect().oy;
	}

	
	//��õ��ƶ�
	Point2D vecPt=Point2D(pt.x,pt.y)-Point2D(m_WinRect.ox,m_WinRect.oy);
	float vecLen=vecPt.Length();
	
	if(vecLen>0) 
	{

		if(vecLen<=moveLen) 
		{
			MoveTo(pt.x,pt.y);
			
			OnPathArrive(pt);
			OnPathCommand(pt);
		}
		else 
		{
			float moveFactor=moveLen/vecLen;
			Move(vecPt.x*moveFactor,vecPt.y*moveFactor);
			
			OnPathMove(pt,moveFactor);
			
		}


	}
	//��ǰ�Ѿ��ڸõ����ˣ��Ͳ���ִ��������
	else 
	{
		OnPathArrive(pt);
	}
	
	
}

void CUIWidget::UpdateAlpha()
{

	//���뵭��û����ͣ
	//��ǰ����û�б�������С
	if( !(m_AlphaState==ALPHA_STATE_PAUSE) && (m_AjustState==EDGE_FLAG_NONE) )
	{

		//��ǰ�Ƿ�������alpha����
		if(m_AlphaState==ALPHA_STATE_INCREASE)
		{
			m_AlphaFactor+=m_AlphaSpeed;

			//�ﵽ���͸����,ֹͣ����alpha
			if(m_AlphaFactor>m_AlphaMax)
			{
				m_AlphaFactor=m_AlphaMax;
				m_AlphaState=ALPHA_STATE_PAUSE;
			}
			
		}

		else
		{
			
			m_AlphaFactor-=m_AlphaSpeed;

			//�ﵽ��С͸����,ֹͣ����alpha
			if(m_AlphaFactor<m_AlphaMin) 
			{
				m_AlphaFactor=m_AlphaMin;
				m_AlphaState=ALPHA_STATE_PAUSE;
			}
		}

	}

	//����͸���ȵĴ���
	if(m_pFather&&((m_Style&WGTSTYLE_CUSTOMALPHA)==0))	m_RenderFactor=m_pFather->GetRenderFactor()*m_AlphaFactor;
	else m_RenderFactor=m_AlphaFactor;

}

//ע�⣺�ڸ����ڵ�UpdateSelf��PostUpdateSelf�У�������Ҫʹ��updatedirty����
//��Ϊupdatedirty�������update�����п��ܱ��ı�
//��������������updateself��Ȼ����updateself�Ĺ���
void CUIWidget::UpdateSelf( float deltatime )
{	

	if(m_QueryType>UI_QUERY_TYPE_NONE)
	{
		//��ѯ����
		stQueryDirty queryDirty;
		queryDirty.querytype=m_QueryType;
		queryDirty.queryindex=m_QueryIndex;

		ExecCallBackFunc(UI_Callback_QueryDirty,(void*)&queryDirty);

		if(queryDirty.bDirty)
		{
			CallSafeScriptEx(SCRIPT_FUNC_ON_QUERYDIRTY,"%d",queryDirty.nChannel);			
		}

	}
	
	///////////////����·���ƶ�///////////////////////

	if((!(m_PathPoints.empty()))&&(!(IsPathMovePaused())))
	{

		stUIPathPoint pt;

		if(IsPathMoveFliped())
			pt=m_PathPoints[m_PathPoints.size()-1];
		else 
			pt=m_PathPoints[0];

		//����ƶ�
		if((pt.command&UI_PATHCMD_RELATIVE)&&m_pFather)
		{
			pt.x+=m_pFather->GetWinRect().ox;
			pt.y+=m_pFather->GetWinRect().oy;
		}


		//��õ��ƶ�
		Point2D vecPt=Point2D(pt.x,pt.y)-Point2D(m_WinRect.ox,m_WinRect.oy);
		float vecLen=vecPt.Length();

		if(vecLen>0) 
		{

			if(vecLen<=m_PathSpeed) 
			{
				MoveTo(pt.x,pt.y);

				OnPathArrive(pt);
				OnPathCommand(pt);
			}
			else 
			{
				int moveLen=m_PathSpeed*deltatime;	
				float speed=m_PathSpeed/vecLen;
				float moveFactor=moveLen/vecLen;
				Move(vecPt.x*speed,vecPt.y*speed);

				OnPathMove(pt, moveFactor);

			}


		}
		//��ǰ�Ѿ��ڸõ����ˣ��Ͳ���ִ��������
		else 
		{
			OnPathArrive(pt);
		}


	}

	
	//�����˼�ʱ��
	if(m_TimerSpace>0.0)
	{
		m_TimerSpace-=deltatime;
		
		if(m_TimerSpace<=0.0) 
			CallSafeScript(SCRIPT_FUNC_ON_TIMER);

	}
	

}

//��ִ�к��ӵ�PostUpdateSelf����ִ���Լ���PostUpdateSelf
void CUIWidget::PostUpdateSelf( float deltatime )
{

	UpdateMornitor();

	//add by lincoln [2006:7:19] [9:58]
	//{{{�����Ч���Ż�
	if(!IsVisible()) return;
	//}}}
	if(IsAlignDirty()) 
	{
		OnAlignDirty();

		SetAlignDirty(false);

	}


	//ȷ�����к��Ӱ��ջ���˳������
	if(!IsSorted()) SortSelf();		

	UpdateWakeArea();
	
	if(m_EdgeCheck&EDGE_CHECK_UPDATE)
	{
		CheckWidgetEdge();
	}

	UpdateFrame(deltatime);
	
}




//�Ƿ���ݿɼ��Ժ�enable���������£��ɸ���widget�Լ�����
void CUIWidget::UpdateRecursive( float deltatime )
{
		
	//����update֮ǰ
	UpdateSelf( deltatime );
	

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		CUIWidget* pChild=(*iter);

		//add by lincoln [2006:7:18] [11:54]
		//{{{�����Ч���Ż�
		//if((g_pRootWgt->GetEditMode()==UI_MODE_DEBUG))
		if(pChild->GetUpdateMuteFlag()&UPDATE_MUTE_VISIBLE)
		{
			if(!pChild->IsVisible()) continue;
		}
		//}}}

		pChild->UpdateRecursive(deltatime);
	}

	
	//����update֮��
	PostUpdateSelf( deltatime );

	
}

//��������Լ����������¼�����focus�ˣ���Ҫ�������˳�򿿺󣬴��ֵ���ͻ����
//�˴����޸�ֵ������
void CUIWidget::ActiveSelf(bool bActive)
{

	SetActived(bActive);

	if(m_pFather)
	{
		if(bActive)
		{

			for(int i=0;i<m_pFather->GetNumChild();i++)
			{ 
				CUIWidget* pBrother=m_pFather->GetChild(i);
				if(pBrother!=this)			
					pBrother->SetActived(false);
			}

			//delete by lincoln [2006:5:8] [15:43]
			//{{{init��������
			//ǿ����������
			//m_pFather->SortSelf();
			//m_pFather->UpdateChildPreLayer();
			//}}}
			
		}
			
		//delete by lincoln [2006:5:8] [15:42]
		//{{{init��������
		//m_pFather->SetSorted(bActive);		
		m_pFather->SetSorted(false);		
		//}}}

	}	

}

void CUIWidget::ActiveRecursive(bool bActive)
{
	
	CUIWidget* pWgt=this;
	while(pWgt) 
	{ 
		pWgt->ActiveSelf(bActive);
		pWgt=pWgt->GetFather();
	}
	
	
}


void CUIWidget::SetFrameScheme(int scheme)
{

	if(scheme>=m_vecFrameScheme.size()) 
		scheme=m_vecFrameScheme.size()-1;
	
	if(scheme<0) 
		scheme=0;

	m_CurFrameScheme=scheme;		
	
}

//ע��һ��֡״̬
void CUIWidget::AddFrameScheme(stFrameScheme frmScheme)
{
	if(m_vecFrameScheme.size()<FRAME_SCHEME_MAX)
		m_vecFrameScheme.push_back(frmScheme);
}

//���һ��Ĭ�ϵ�frame state
//��֡����0
void CUIWidget::DefaultFrameScheme()
{
	
	m_vecFrameScheme.clear();
	
	stFrameScheme defFrmScheme;

	m_vecFrameScheme.push_back(defFrmScheme);
	
	m_CurFrameScheme=0;
	
}



//���ݴ���״̬����framestate
int  CUIWidget::GetFrameState()
{
	//��������˹�ע������enable״̬�ķ��
	if(m_Style&WGTSTYLE_FATHERENABLE)
	{
		//���������disable�ˣ���diable״̬�������Լ�
		if(m_pFather&&(!m_pFather->IsEnabled()))
			return FRAME_STATE_DISABLE;
	}

	if(m_State&WGTSTATE_DISABLE)
		return FRAME_STATE_DISABLE;
	
	else if( m_State& WGTSTATE_CLICKDOWN )   
		return FRAME_STATE_CLICK;
	
	else if( m_State& WGTSTATE_MOUSEOVER)    
		return FRAME_STATE_OVER;
	
	else   
		return FRAME_STATE_NORMAL;

}

void CUIWidget::OnSchemeFrame()
{
	
}

void CUIWidget::OnAffirmFrame()
{
	//���֡�ķ�Χ
	if((m_CurFrame<m_vecFrameScheme[m_CurFrameScheme].start[m_CurFrameState])||
		(m_CurFrame>m_vecFrameScheme[m_CurFrameScheme].end[m_CurFrameState]))
		m_CurFrame=m_vecFrameScheme[m_CurFrameScheme].start[m_CurFrameState];

	//�����Ч֡�ķ�Χ
	if(m_pEffectPic)
	{
		if(m_CurEffectFrame>=m_pEffectPic->GetNumFrame())
		   m_CurEffectFrame=0;
	}
}

void CUIWidget::OnPlusFrame()
{

	m_CurFrame++;
	m_CurEffectFrame++;

}

void CUIWidget::UpdateFrame(float deltatime)
{

	//���Ӵ���һ�������Լ�״̬����framescheme�Ļ���
	OnSchemeFrame();

	//��������״̬����framestate
	int frameState=GetFrameState();

	//�л�֡״̬
	if(m_CurFrameState!=frameState)
	{
		m_CurFrameState=frameState;
		m_CurFrame=m_vecFrameScheme[m_CurFrameScheme].start[frameState];
	}

	
	//���µ�ǰ֡
	m_AccumTime+=deltatime;
	if(m_AccumTime>=m_FrameSpeed) 
	{
		m_AccumTime=0.0;
		OnPlusFrame();
	}		
	
	//ȷ��֡�ں���ķ�Χ��
	OnAffirmFrame();
	
}

//�����ֵ�uncheck�������Լ�check
void CUIWidget::SelectSelf() 
{
	
	if(m_GroupId>=0) //������group����,��Ҫ�Զ����������ֵܸı�check����
	{ 
		if(m_pFather) 
			m_pFather->SelectChild(m_GroupId,m_GroupStop);
	}
	else 
	{
		SetChecked(true);	 
	}
	
}

//x,yΪ��������
stDragInfo CUIWidget::ParseDragInfo(int x,int y)
{

	stDragInfo  infoDrag;

	if(!IsInItemDragRect(x,y)) return infoDrag;//���ڵ����϶�����,����Ҫ�����Ϸ�

	if(m_DragType==UI_DRAG_TYPE_NONE) 	return infoDrag;//����Ҫ�����Ϸ�

	
	stValidItem validItem;
	validItem.type=m_DragType;
	validItem.index=m_DragIndex;
	validItem.hosttype=m_HostType;
	validItem.hostindex=m_HostIndex;

	ExecCallBackFunc(UI_Callback_IsValidItem,(void*)&validItem);	
	if(!validItem.candrag)	return infoDrag;

	
	infoDrag.type=m_DragType;
	infoDrag.index=m_DragIndex;
	infoDrag.rect=GetAbsoluteClientRect();
	infoDrag.itemtype=validItem.itemtype;
	infoDrag.maxnum=validItem.nitem;
	infoDrag.bAttach=validItem.canattach;
	infoDrag.bAjust=validItem.canajust;
	infoDrag.pDragSrc=this;
		
	return infoDrag;
	
}

bool CUIWidget::ItemDrop(int x,int y)
{
	
	if(!IsInItemDropRect(x,y)) return false;//���ڵ���Ͷ������,����Ҫ�����Ϸ�
	
	if(m_DragType==UI_DRAG_TYPE_NONE) 	return false;//����Ҫ�����Ϸ�
	

	stDragInfo info=g_pRootWgt->GetDragInfo();
	
	
	stMoveItem data;
	
	data.srcindex=info.index;
	data.srctype=info.type;
	data.srchostindex=info.hostindex;
	data.srchosttype=info.hosttype;

	data.itemnum=info.itemnum;

	data.dstindex=m_DragIndex;
	data.dsttype=m_DragType;
	data.dsthostindex=m_HostIndex;
	data.dsthosttype=m_HostType;


	data.rect=info.rect;
	data.rect.ox=CUICursor::m_Pos.x-data.rect.width/2;
	data.rect.oy=CUICursor::m_Pos.y-data.rect.height/2;
	
	
	ExecCallBackFunc(UI_Callback_MoveItem,(void*)&data);
	
	
	return true;//���ܷ��µ���Ŀ

}


int CUIWidget::DrawDragItem(stDrawItem data)
{

	ExecCallBackFunc(UI_Callback_DrawItem,(void*)&data);
	
	return 0;
}


int CUIWidget::DragEnter(int x,int y)
{

	CUIWidget* pwgt=this;
	while(pwgt)
	{
		if(pwgt->HasScriptFunc(SCRIPT_FUNC_ON_DRAGENTER))
			break;

		pwgt=pwgt->GetFather();

	}

	if(pwgt) 
		pwgt->CallSafeScript(SCRIPT_FUNC_ON_DRAGENTER);

	return 0;
}


int CUIWidget::DragLeave(int x,int y)
{
	
	CUIWidget* pwgt=this;
	while(pwgt)
	{
		if(pwgt->HasScriptFunc(SCRIPT_FUNC_ON_DRAGLEAVE))
			break;

		pwgt=pwgt->GetFather();

	}

	if(pwgt) 
		pwgt->CallSafeScript(SCRIPT_FUNC_ON_DRAGLEAVE);

	return 0;
}


//check������,ָ�����е�ָ������
void CUIWidget::SelectChild(int groupId,int groupstop)
{
	
	if(groupId>=0)
	{
		CUIWidget* pSelWgt=NULL;

		//uncheck all
		for(int i=0;i<m_Children.size();i++)
		{ 			
			CUIWidget* pWgt=m_Children[i];

			if(pWgt->GetGroupId()==groupId) 
			{				
				if(pWgt->GetGroupStop()==groupstop) 
				pSelWgt=pWgt;
				else
				pWgt->SetChecked(false);
			}
				
		}


		//check the one
		if(pSelWgt)
			pSelWgt->SetChecked(true);

		//���⴦��tabwin�ȴ���
		SetSorted(false);

	}
}

//�õ�������,ָ�����еĵ�һ��checkd����groupstop
int CUIWidget::GetSelect(int groupId)
{

	if(groupId>=0) 
	for(int i=0;i<m_Children.size();i++)
	{ 						
		if(m_Children[i]->GetGroupId()==groupId) 
		{				
			if(m_Children[i]->IsChecked()) 
			return m_Children[i]->GetGroupStop();			
		}
	
	}

	return -1;//groupstop=-1; none	

}

CUIWidget* CUIWidget::GetGroupChild(int groupId,int groupstop)
{

	if(groupId>=0) 
	for(int i=0;i<m_Children.size();i++)
	{ 						
		if(m_Children[i]->GetGroupId()==groupId) 
		{				
			if(m_Children[i]->GetGroupStop()==groupstop) 
			return m_Children[i];			
		}
	
	}

	return NULL;//groupstop=-1; none	

}


void CUIWidget::OnLoad()
{
	AlignSelf();//���Լ����뵽���׻�����������

	SortByAlignId();//�����Լ��ĺ��Ӵ���

	CallSafeScript(SCRIPT_FUNC_ON_LOAD);

	SetLoaded(true);

}

void CUIWidget::OnLoadRecursive()
{

	OnLoad();	

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->OnLoadRecursive();
	}
	
}

void CUIWidget::OnInit()
{
	SetUpdateDirty(true);

	CallSafeScript(SCRIPT_FUNC_ON_INIT);
}

void CUIWidget::OnInitRecursive()
{
	SetDiscarded(false);

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->OnInitRecursive();
	}	
	
	OnInit();
}

void CUIWidget::OnDenit()
{	
	CallSafeScript(SCRIPT_FUNC_ON_DENIT);
	if(m_Style&WGTSTYLE_RENDERBUFFER)
		ExecCallBackFunc(UI_Callback_CustomBufferDeinit, &m_CustomBuffer);
}

void CUIWidget::OnDenitRecursive()
{	
	SetDiscarded(true);

	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
	{
		(*iter)->OnDenitRecursive();
	}	
	
	OnDenit();

}




void CUIWidget::OnPaste()
{


}

void CUIWidget::OnCopy()
{


}

void CUIWidget::OnCut()
{


}

void CUIWidget::OnInsert(const char* pText)
{


}

//��ĳ���ƶ������У�ִ�и��²���
void CUIWidget::OnPathMove(stUIPathPoint pt, float factor)
{


	if(pt.command&UI_PATHCMD_ALPHA)
	{	
		SetAlphaFactor(m_AlphaFactor+(pt.alpha-m_AlphaFactor)*factor);		
	}

	if(pt.command&UI_PATHCMD_SCALE)
	{
		SetScaleFactor(m_ScaleFactor+(pt.scale-m_ScaleFactor)*factor);
	}


}

void CUIWidget::OnRecoverPathPoint()
{
	sort(m_PathPoints.begin(), m_PathPoints.end());
}

//�������ԭ������ĳ��ʱִ������
void CUIWidget::OnPathArrive(stUIPathPoint pt)
{

	if(pt.command&UI_PATHCMD_ALPHA)
	{
		m_AlphaFactor=pt.alpha;
	}
	
	if(pt.command&UI_PATHCMD_SCALE)
	{
		m_ScaleFactor=pt.scale;
	}


	if(pt.command&UI_PATHCMD_SIZE)
	{
		m_WinRect.width=pt.width;
		m_WinRect.height=pt.height;
	}	
	
	if(pt.command&UI_PATHCMD_CYCLE)
	{
		if(IsPathMoveFliped())
		{
			
			m_PathPoints.pop_back();
			m_PathPoints.push_front(pt);
		}
		else 
		{
			
			m_PathPoints.pop_front();
			m_PathPoints.push_back(pt);
		}
	}
	else
	{
		if(IsPathMoveFliped())
			m_PathPoints.pop_back();
		else 
			m_PathPoints.pop_front();
	}
	
	//��������
	stUISound sndData;
	sndData.id=m_SoundId[UI_SOUND_ARRIVE];
	sndData.cmd=UI_SOUND_ARRIVE;
	sndData.mode=0;
	ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);

}


void CUIWidget::OnPathCommand(stUIPathPoint pt)
{
	
	if(pt.command&UI_PATHCMD_PAUSE)
	{
		PausePathMove(true);
	}

	if(pt.command&UI_PATHCMD_FLIP)
	{
		FlipPathMove(true);
	}
	
	//����ָ�����ִ�нű�
	CallSafeScriptEx(SCRIPT_FUNC_ON_PATHCOMMAND,"%d,%d,%d",pt.id,pt.x,pt.y);

}


int CUIWidget::OnChar( unsigned int wparam, unsigned int lparam, bool bVirtual)
{

	if(HasScriptFunc(SCRIPT_FUNC_ONCHAR))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONCHAR,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

		
	if( m_pFather ) return m_pFather->OnChar( wparam, lparam, bVirtual );
	else return 0;

}


int CUIWidget::OnKeyDown( unsigned int wparam, unsigned int lparam, bool bVirtual)
{
		
	if(HasScriptFunc(SCRIPT_FUNC_ONKEYDOWN))
	{
		if(CallSafeScriptEx(SCRIPT_FUNC_ONKEYDOWN,"%d,%d,%d",wparam,lparam,int(bVirtual)))
			return 1;
	}

	switch(wparam) 
	{


	//����ж�Ӧenter�Ľű�����,���ýű�����
	//����,����Ĭ�ϵĴ���(���͸�����)
	case VK_RETURN:
		{
			if(HasScriptFunc(SCRIPT_FUNC_ON_ENTER))	
			{
				CallSafeScript(SCRIPT_FUNC_ON_ENTER);
			}
			else if(m_pFather) 
			{			
				return m_pFather->OnKeyDown( wparam, lparam, bVirtual );
			}
			else
			{
				return 0;
			}	
			
		}
		break;
	
	case VK_ESCAPE:
		{
			if(HasScriptFunc(SCRIPT_FUNC_ON_ESCAPE))	
			{
				CallSafeScript(SCRIPT_FUNC_ON_ESCAPE);
			}
			else if(m_pFather) 
			{			
				return m_pFather->OnKeyDown( wparam, lparam, bVirtual );
			}
			else
			{
				return 0;
			}	
		}
		break;

	case VK_TAB:
		{
			if(HasScriptFunc(SCRIPT_FUNC_ON_TAB))	
			{
				CallSafeScript(SCRIPT_FUNC_ON_TAB);
			}
			else if(m_pFather) 
			{			
				return m_pFather->OnKeyDown( wparam, lparam, bVirtual );
			}
			else
			{
				return 0;
			}	

		}
		break;
	

		
	default:
		
		if( m_pFather ) return m_pFather->OnKeyDown( wparam, lparam, bVirtual );
		else return 0;
	}
	
	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}



//ȱʡ��widget��Ϣ�����������ø��׵���Ϣ����
//���û�и����ڣ��򷵻�1
int CUIWidget::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{

	switch(msgtype)
	{
		
	case MSGTYPE_CHAR:
		return	OnChar( wparam, lparam );
		break;
		
	case MSGTYPE_KEYDOWN:
		return OnKeyDown( wparam, lparam );
		break;
		
	case MSGTYPE_MOUSEIDLE:
		{
			//֪ͨ����µĴ���:���ϴ����ֹͣ�ƶ�����,����������ͣ���˶��
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);	
			
			
			//ע�⣺��Щ���ߴ��ڵ�itemdragrect��winrectС
			//���mousein��ʱ�򣬻�parse����tooltip
			//�����������Ҫ������idletime��			
			int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);
			
			if(ParseTooltip(nIndex,GetWinRect(),x,y)==UI_Cmd_SHOW)
			{
				AcceptIdle(false);
			}		

		}
		break;

	case MSGTYPE_MOUSEMOVE:
		{

			int x=LOWORD(lparam);
			int y=HIWORD(lparam);	
			
			//�ƶ���ʱ����ʱparsetooltip
			if(m_IdleTime<0.0f)
			{

				int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);
				ParseTooltip(nIndex,GetWinRect(),x,y);

			}

		}
		break;
		
	case MSGTYPE_MOUSEIN:
		{
			m_State |= WGTSTATE_MOUSEOVER;
			
			//�����Ӵ������������ʱ�򣬼���������Ƿ���
			if(((wparam&MK_LBUTTON)!=0)&&(IsFocused()))
				m_State |= WGTSTATE_CLICKDOWN;

			if(m_Style&WGTSTYLE_AUTOACTIVE)
				ActiveSelf(true);
			
			AcceptIdle(true);

			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			if(g_pRootWgt->GetDragInfo().type!=UI_DRAG_TYPE_NONE)
			{				
				DragEnter(x,y);
			}			

			//��������
			stUISound sndData;
			sndData.id=m_SoundId[UI_SOUND_MOUSEIN];
			sndData.cmd=UI_SOUND_MOUSEIN;
			sndData.mode=0;
			ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);

			CallSafeScript(SCRIPT_FUNC_ON_MOUSEIN);
		}
		break;
		
	case MSGTYPE_MOUSEOUT:
		{
			m_State &= ~WGTSTATE_MOUSEOVER;
			m_State &= ~WGTSTATE_CLICKDOWN;			
			
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			if(g_pRootWgt->GetDragInfo().type!=UI_DRAG_TYPE_NONE)
			{			
				DragLeave(x,y);
			}		

			HideTooltip(true,x,y);		

			CallSafeScript(SCRIPT_FUNC_ON_MOUSEOUT);
		}		
		break;


	case MSGTYPE_RIGHTBTNDOWN:
		{
			
			int x=LOWORD(lparam);
			int y=HIWORD(lparam);
		

			if(IsInItemDragRect(x,y))
			{
				int nIndex=ParseDragIndex(x,y,UI_PARSE_EXACT);

				//��Ҫ�����Ϸ�
				if(m_DragType!=UI_DRAG_TYPE_NONE)
				{
					stClickItem useItem;
					useItem.type=m_DragType;
					useItem.index=nIndex;
					useItem.hosttype=m_HostType;
					useItem.hostindex=m_HostIndex;
					ExecCallBackFunc(UI_Callback_RClickItem,(void*)&useItem);
				}
			
				ParseInfotip(nIndex);

			}


			CallSafeScript(SCRIPT_FUNC_ON_RCLICK);
			
		}
		
		break;

	case WM_RBUTTONUP:			
		break;
		
	//����ؼ�ʱ��,��Ҫ����1(�������Ļ,���⴦��,��CUIScreen��)
	//������Щ��Ϣ��Ҫ����
	case MSGTYPE_LBTNDBLCLK:
		{

			int x=LOWORD(lparam);
			int y=HIWORD(lparam);

			//�ڿͻ�����,��Ҫ�����Ϸ�
			if(IsInItemDragRect(x,y))
			{
				//��Ҫ�����Ϸ�
				if(m_DragType!=UI_DRAG_TYPE_NONE)
				{
					stClickItem useItem;
					useItem.type=m_DragType;
					useItem.index=m_DragIndex;
					useItem.hosttype=m_HostType;
					useItem.hostindex=m_HostIndex;
					ExecCallBackFunc(UI_Callback_DClickItem,(void*)&useItem);
				}

			}

			CallSafeScript(SCRIPT_FUNC_ON_DBCLICK);
			
		}
		break;
			


	case MSGTYPE_LEFTBTNDOWN:	
		{
			
			m_State |= WGTSTATE_CLICKDOWN;


			int x=LOWORD(lparam);
			int y=HIWORD(lparam);			


			//�ڿͻ�����,��Ҫ�����Ϸ�
			if(IsInItemDragRect(x,y))
			{
				//��Ҫ�����Ϸ�
				if(m_DragType!=UI_DRAG_TYPE_NONE)
				{
					stClickItem useItem;
					useItem.type=m_DragType;
					useItem.index=m_DragIndex;
					useItem.hosttype=m_HostType;
					useItem.hostindex=m_HostIndex;
					ExecCallBackFunc(UI_Callback_ClickItem,(void*)&useItem);
				}

			}

			//��������
			stUISound sndData;
			sndData.id=m_SoundId[UI_SOUND_CLICK];
			sndData.cmd=UI_SOUND_CLICK;
			sndData.mode=0;
			ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);


			CallSafeScript(SCRIPT_FUNC_ON_CLICK);
			
		}
		
		break;
		
	case MSGTYPE_LEFTBTNUP:
		if( m_State & WGTSTATE_CLICKDOWN )
		{						
			m_State &= ~WGTSTATE_CLICKDOWN;	
		}	
		
		break;

	
	default:	
		
		if( m_pFather ) return m_pFather->OnMessage( msgtype, wparam, lparam );
		else return 0;
		
	}


	//����ָ����Ϣ,�Լ�����,�����׷���	
	return 1;
	
}

//NOTIFY��Ϣ���������״���
int  CUIWidget::OnNotify( UINotifyType msgtype, unsigned int wparam, unsigned int lparam )
{
	

	switch(msgtype)
	{

	case NTFTYPE_COMMAND_OK:			
	case NTFTYPE_COMMAND_CANCEL:		
		ShowWidget(false);
		break;	
		
	case NTFTYPE_AJUSTTING:
		{

			//�߱��϶�����λ��
			int x=(unsigned int)wparam;
			int y=(unsigned int)lparam;			

			if(m_AjustState&EDGE_FLAG_LEFT)
			{
				this->ExtendTo(x,EDGE_FLAG_LEFT,m_AjustFlag);				
			}
			else if(m_AjustState&EDGE_FLAG_RIGHT)
			{
				this->ExtendTo(x,EDGE_FLAG_RIGHT,m_AjustFlag);
			}			
			
			if(m_AjustState&EDGE_FLAG_UP)
			{
				this->ExtendTo(y,EDGE_FLAG_UP,m_AjustFlag);				
			}
			else if(m_AjustState&EDGE_FLAG_DOWN)
			{
				this->ExtendTo(y,EDGE_FLAG_DOWN,m_AjustFlag);
			}			

		}
		break;

	case NTFTYPE_DRAGGING:
		{
			int x=(unsigned int)wparam;
			int y=(unsigned int)lparam;
			
		
			
			if(m_pFather) 
				m_pFather->ChildQueryMoveTo(x,y,this);							
			
			if(!IsMovableX()) 
				x=GetOriginX();
			
			if(!IsMovableY()) 
				y=GetOriginY();
			
			if(IsMovable())
				MoveTo(x,y);


			if(m_pFather) m_pFather->OnNotify(NTFTYPE_CHILD_DRAGGING,MAKELONG(x,y),(unsigned int)this); 
			
		}
		
		break;
		


	}


	//����ָ��֪ͨ,�Լ�����,�����׷���
	return 1;

}


void CUIWidget::OnSave(FILE* fp)
{
	fprintf(fp,"%s",m_ScriptText.c_str());
}


void CUIWidget::OnSaveRecursive(FILE* fp)
{
	
    if(m_Type==WGTTYPE_SCREEN)
		fprintf(fp,"%s=\n{\n",GetWidgetPathID());
	else 
		fprintf(fp,"%s=\n{\n",m_ID.c_str());
    
    OnSave(fp);		    	
	
    if(m_Children.size()>0) 
    {
		
        fprintf(fp,"\nchildren=\n{\n");        
		
		std::vector<CUIWidget *>::iterator iter;
		for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
		{
			(*iter)->OnSaveRecursive(fp);
		}
		
        fprintf(fp,"};\n\n\n");        
		
    }	
	
    fprintf(fp,"\n};\n\n\n");
	
	
}

int CUIWidget::ParseKeyChar(int x,int y, stKeyChar& keyChar)
{

	return RICHCHAR_CMD_NONE;

}


void CUIWidget::ParseCursor(int x,int y)
{
	
	int flag=this->IsPointInAjustRect(x, y);			

	stRect absCaptionRct=GetAbsoluteCaptionRect();

	stKeyChar keyChar;
			
	if( (flag==EDGE_FLAG_LEFT)||(flag==EDGE_FLAG_RIGHT) )
		g_pRootWgt->SetCurCursor(m_HorizonSizer.c_str());
	else if( (flag==EDGE_FLAG_UP)||(flag==EDGE_FLAG_DOWN) )
		g_pRootWgt->SetCurCursor(m_VerticalSizer.c_str());
	else if(ParseKeyChar(x,y,keyChar)!=RICHCHAR_CMD_NONE)
		g_pRootWgt->SetCurCursor(DEFAULT_LINK_CURSOR);
	else if(IsAcceptInput()&&(absCaptionRct.IsPointIn(x,y)))
		g_pRootWgt->SetCurCursor(DEFAULT_EDIT_CURSOR);	
	else if(!m_CursorName.empty()) 
		g_pRootWgt->SetCurCursor(m_CursorName.c_str());

}

int CUIWidget::ParseDragIndex(int x,int y,int style)
{

	if(!IsInItemDragRect(x,y))
		return -1;

	return m_DragIndex;
}

void CUIWidget::HideInfotip()
{
	CUIWidget* pTooltip=GetSafetBuddyWgt(BUDDY_WIDGET_INFOWGT);	
	
	if(pTooltip) pTooltip->ShowWidget(false);
}

int CUIWidget::ParseInfotip(int nDragIndex)
{
	
	CUIWidget* pInfoWgt=GetSafetBuddyWgt(BUDDY_WIDGET_INFOWGT);
	
	if(!pInfoWgt)  
		return UI_Cmd_NONE;

	if(m_DragType==UI_DRAG_TYPE_NONE) 	
	{
		pInfoWgt->ShowWidget(false);
		return UI_Cmd_NONE;
	}

	
	stValidItem validItem;
	validItem.type=m_DragType;
	validItem.index=nDragIndex;
	validItem.hosttype=m_HostType;
	validItem.hostindex=m_HostIndex;

	ExecCallBackFunc(UI_Callback_IsValidItem,(void*)&validItem);	
	
	if(!validItem.cangetinfo)	
	{
		pInfoWgt->ShowWidget(false);
		return UI_Cmd_NONE;
	}

	pInfoWgt->RegisterBuddyWgt(BUDDY_WIDGET_HOST,BUDDY_STYLE_SELF,"",this);
	pInfoWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_PARSE_INFOTIP,"%d,%d,%d,%d",m_DragType,nDragIndex,m_HostType,m_HostIndex);
	pInfoWgt->ShowWidget(true);

	return UI_Cmd_SHOW;
	
}


void CUIWidget::HideTooltip(bool bMouseOut,int x,int y)
{
	CUIWidget* pTooltip=GetSafetBuddyWgt(BUDDY_WIDGET_TIPWGT);	
	
	if(!pTooltip)  return;

	//����ƶ���tooltip����
	//if(bMouseOut&&pTooltip->IsPointIn(x,y))  return;

	pTooltip->ShowWidget(false);

}


int  CUIWidget::ParseTooltip(int nDragIndex,stRect refRect,int xcursor,int ycursor)
{
	
	CUIWidget* pTipWgt=GetSafetBuddyWgt(BUDDY_WIDGET_TIPWGT);
	if(!pTipWgt) return UI_Cmd_NONE;

		
	stValidItem validItem;
	validItem.type=m_DragType;
	validItem.index=nDragIndex;
	validItem.hosttype=m_HostType;
	validItem.hostindex=m_HostIndex;

	ExecCallBackFunc(UI_Callback_IsValidItem,(void*)&validItem);
	
	stRect absClntRct=GetAbsoluteClientRect();
	int rx=xcursor-absClntRct.ox;
	int ry=ycursor-absClntRct.oy;

	int nCmd=0;

	//�пͻ���tooltip
	if(validItem.cangettip)
	{
		pTipWgt->RegisterBuddyWgt(BUDDY_WIDGET_HOST,BUDDY_STYLE_SELF,"",this);
		nCmd=pTipWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_PARSE_TOOLTIP,"%d,%d,%d,%d,%d,%d",m_DragType,nDragIndex,m_HostType,m_HostIndex,rx,ry);
	}
	else 
	{
		const char* pTipTxt=GetTipText(nDragIndex);
		if(strcmp(pTipTxt,"")!=0)
		{
			pTipWgt->RegisterBuddyWgt(BUDDY_WIDGET_HOST,BUDDY_STYLE_SELF,"",this);
			pTipWgt->SetCaption(pTipTxt);
			nCmd=1;
		}
			 
	}

	if(nCmd==1)
	{

		//������TipPos
		if((m_TipPos.width>=ALIGNTYPE_NONE)||(m_TipPos.height>=ALIGNSTYLE_NONE))
		{
			pTipWgt->AlignToWidget(this,m_TipPos.width,m_TipPos.height,m_TipPos.ox,m_TipPos.oy);
		}
		else 
		{
			Point2D ptResult=GetBestTooltipPos(xcursor,ycursor,pTipWgt->GetWidth(),pTipWgt->GetHeight(),refRect);
			pTipWgt->MoveTo(ptResult.x,ptResult.y);
		}
		
		pTipWgt->ShowWidget(true);

		return UI_Cmd_SHOW;

	}
	else
	{
		pTipWgt->ShowWidget(false);
		return UI_Cmd_NONE;	 
	}

}


//�õ��߼�·��
//root.grand.father.this
const char *CUIWidget::GetWidgetPath()
{

	//s_wins�����˸�widget�Լ��������ϼ�widget
	//s_wins��root,...,grandfather,father,widget
	static std::deque<CUIWidget *>s_wins;
	s_wins.resize(0);
	
	CUIWidget *pWgt = this;
	while( pWgt )
	{
		s_wins.push_front( pWgt );
		pWgt = pWgt->GetFather();
	}
	
	
	
	std::deque<CUIWidget *>::iterator iter;
	static std::string path;
	path.resize(0);
	
	for(iter = s_wins.begin(); iter!=s_wins.end(); iter++ )//����widget��Σ�������������·��
	{
		path += (*iter)->GetWidgetID();
		
		if((*iter)!=this) 
		path += ".";
		
	}
	
	return path.c_str();
	

}

//�õ�����·��(����·��id)
//root.children.grand.children.father.children.this
const char *CUIWidget::GetWidgetPathID()
{

	//s_wins�����˸�widget�Լ��������ϼ�widget
	//s_wins��root,...,grandfather,father,widget
	static std::deque<CUIWidget *>s_wins;
	s_wins.resize(0);
	
	CUIWidget *pWgt = this;
	while( pWgt )
	{
		s_wins.push_front( pWgt );
		pWgt = pWgt->GetFather();
	}
	
	
	
	std::deque<CUIWidget *>::iterator iter;
	static std::string path;
	path.resize(0);
	
	for(iter = s_wins.begin(); iter!=s_wins.end(); iter++ )//����widget��Σ�������������·��
	{

		path += (*iter)->GetWidgetID();
		
		if((*iter)!=this) 
			path += ".children.";
		
	}

	return path.c_str();

}

//�����߼�·��
//�����еĺ�������ָ����id��widget
//��idӦ����ѭ����Ĺ���
//�ӵ�pos���ַ���ʼ,�����ĵ�һ��"."�ַ�����
//���е�һ�����ʱ�ʾһ�����ӵ�id
//û��.children.
//����:�����ڲ���UI.entry.cancel.tip
//��entry����ִ�иú���ʱ,����Ĳ���Ϊ:UI.entry.cancel.tip,9
//��������cancel.tip�Ӵ���,���ҵ�cancel�Ӵ�
//Ȼ�������к���������cancelΪid�ĺ���
//Ȼ��,cancel����ִ�иú���ʱ,����Ĳ���Ϊ:UI.entry.cancel.tip,16
CUIWidget *CUIWidget::FindWidgetByID( const std::string &idpath, int pos )
{
	std::string tmpbuf;
	int endpos = idpath.find_first_of( '.', pos );

	bool isend = false;
	if( endpos == std::string::npos )
	{
		endpos = idpath.size();
		isend = true;
	}
	tmpbuf = idpath.substr( pos, endpos-pos );//tmpbufΪ��pos��ʼ������һ��.���ַ�����������·��

	for( int i=0; i<m_Children.size(); i++ )
	{
		CUIWidget *p = m_Children[i];
		if( p->m_ID == tmpbuf )
		{
			if( isend ) return p;//���·���Ѿ������꣬���ټ�������
			else return p->FindWidgetByID( idpath, endpos+1 );//���·��û�������꣬�����ڸ��Ӷ�����Ӷ���������
		}
	}
	return NULL;
}

//Ѱ��ָ��id�ĺ���
CUIWidget* CUIWidget::FindChildById(const char* pid)
{

	CUIWidget* pChild=NULL;
	std::vector<CUIWidget *>::iterator iter;
	for(iter=m_Children.begin();iter!=m_Children.end();iter++)
	{
		if(strcmp((*iter)->GetWidgetID(),pid)==0)
		{
			pChild=(*iter);
			break;
		}
	}

	return pChild;
}

CUIWidget *CUIWidget::FindChildByAlignId(int id) //�õ�ָ��alignid�ĺ���
{	
	//û������alignid����
	if(id==-1) return 0; 

	for( int i=0; i<m_Children.size(); i++ )
	{
		if( m_Children[i]->GetAlignId()==id) return m_Children[i];
	}
	
	return 0;
}


void CUIWidget::ExecCallBackFunc(UICallbackEvent eventName,CallbackData pData)
{	
	RawExecCallBack(eventName,pData,(IWgtData*)this);	
}

//ȷ���Ƿ����Ԥ����ű�����
void CUIWidget::ConfirmScripts( CVar *pvar )
{

	if(!pvar) return;

	m_ScriptFuncFlags.clear();
	for(int i=0;i<SYSTEM_SCIRPT_NUM;i++)
	{ 
		CVar* pFuncVar=pvar->Element(g_vecScriptFuncs[i].c_str());

		if(!pFuncVar) continue;
			
		if(pFuncVar->GetType()==VAR_FUNC)
			m_ScriptFuncFlags.push_back(stScriptFunc(i,""));
		else if(pFuncVar->GetType()==VAR_STRING)
			m_ScriptFuncFlags.push_back(stScriptFunc(i,pFuncVar->String()));
	}

}

const stScriptFunc* CUIWidget::GetScriptFunc(short funcid)
{

	std::vector<stScriptFunc>::iterator iter;
	for(iter=m_ScriptFuncFlags.begin();iter!=m_ScriptFuncFlags.end();iter++)
	{ 		
		if(iter->funcId==funcid) return &(*iter);
	}
	return NULL;

}

//�������Ԥ�����ָ��id�Ĵ��ڽű��Ӻ����Ƿ����
BOOL CUIWidget::HasScriptFunc(short funcid)
{

	std::vector<stScriptFunc>::iterator iter;
	for(iter=m_ScriptFuncFlags.begin();iter!=m_ScriptFuncFlags.end();iter++)
	{ 		
		if(iter->funcId==funcid) return TRUE;
	}
	return FALSE;
}

//���ý����Ԥ�����ָ��id�Ĵ��ڽű��Ӻ���,�����ھͲ�����
int CUIWidget::CallSafeScriptEx(short funcid,const char *fmt /*=""*/, ...)
{
	
	static char	msg[UI_SCRIPT_BUFSIZE];
	va_list	argptr;
	va_start(argptr,fmt);
	vsprintf(msg,fmt,argptr);
	va_end(argptr);

	return CallSafeScript(funcid,msg);	
	
}

//���ý����Ԥ�����ָ��id�Ĵ��ڽű��Ӻ���,�����ھͲ�����
int CUIWidget::CallSafeScript(short funcid,const char* arg /*=""*/)
{

	const stScriptFunc* pFunc=GetScriptFunc(funcid);
	
	if(pFunc&&IsLoaded())
	{
		//funcPathΪ��function������·��
		std::string funcPath;
		
		if(pFunc->funcPath=="")//�ű���ֱ��Ƕ���˺���
		{
			funcPath=GetWidgetPathID();
			funcPath+=".";
			funcPath+= g_vecScriptFuncs[funcid];
		}
		else//�ű���ָ�����ַ�����ʽ�ĺ���·��
		{
			funcPath=pFunc->funcPath;
		}
		
		return CallScript(funcPath.c_str(),arg);
	}

	return -1;//û���������

}


//����pfunc�ַ���ָ��·�������Ƶ�ȫ��lua����
//����: CallScript("UI.children.entry.OnClick","10,2");
int CUIWidget::CallScript( const char *pfunc,const char* arg /*=""*/ )
{
	static CVarFloat retvalue( "tmpfunc_ret" );
	
	CScriptState *pscript = g_pRootWgt->GetScriptEnv();

	//�˴���Ҫ��gameui�ⲿ����һ����
	static char buffer[4096];	

	if(strlen(arg)>0)
	sprintf( buffer, "selfpath='%s'; tmpfunc = %s; tmpfunc_ret = -1; if tmpfunc then tmpfunc_ret = tmpfunc(%s) end;", GetWidgetPath(), pfunc,arg );//����ú������ڣ����øú���
	else
	sprintf( buffer, "selfpath='%s'; tmpfunc = %s; tmpfunc_ret = -1; if tmpfunc then tmpfunc_ret = tmpfunc() end;", GetWidgetPath(), pfunc );//����ú������ڣ����øú���
	
	
	pscript->DoString( buffer);//ִ��lua���룬����lua����

	retvalue.GetFromScript( pscript );
	return retvalue.Integer();

}

int CUIWidget::GetLayer()
{

	if(IsActived())	
	return (m_InitLayer+m_PlusLayer+m_PreLayer);
	else  
	return (m_InitLayer+m_PreLayer);

}

stCustomBuffer& CUIWidget::GetCustomBuffer()
{
	return m_CustomBuffer;
}


void CUIWidget::RenderCustomBuffer()
{
	
	ExecCallBackFunc(UI_Callback_DrawCustomBuffer,(void*)&m_CustomBuffer);

}


void CUIWidget::RenderCustomItem()
{

	stColorQuad drawColor=GetRenderColor(m_DrawColor);
	
	stDrawItem theItem;
	theItem.type=m_DragType;
	theItem.index=m_DragIndex;
	theItem.hosttype=m_HostType;
	theItem.hostindex=m_HostIndex;
	theItem.state=m_CustomState;
	theItem.rect=GetAbsoluteClientRect();
	theItem.color=drawColor.c;
	ExecCallBackFunc(UI_Callback_DrawItem,(void*)&theItem);


}




//----------------------------------------------------------------------------------------
//--------------------------------class CUIRootWin--------------------------------------
//----------------------------------------------------------------------------------------
CUIRootWin::CUIRootWin()
{

	m_Type = WGTTYPE_ROOT;	

	m_pCurScreen=NULL;

	m_pCurCursor = NULL;

	m_pMouseWgt= this;
	m_pFocusWgt = this;
	m_pPopupWgt=NULL;
	m_pCaptureWgt= NULL;
	m_pDragWgt= NULL;
	m_pAjustWgt=NULL;
	m_pHotWgt=NULL;

	m_pScript = NULL;

	//��ʼ����ɫ������
	g_pDisplay->SetFont(0);

}

CUIRootWin::~CUIRootWin()
{
	m_pCurCursor=NULL;
	std::vector<CUICursor *>::iterator iterCursor;
	for(iterCursor = m_AllCursor.begin(); iterCursor!=m_AllCursor.end(); iterCursor++ ) 
	{
		(*iterCursor)->Release();
	}

	SetCurScreen(NULL);
	std::vector<CUIWidget *>::iterator iterScreen;
	for(iterScreen = m_AllScreen.begin(); iterScreen!=m_AllScreen.end(); iterScreen++ ) 
	{
		(*iterScreen)->Release();
	}

	m_AllScreenDesc.clear();
	m_pFocusWgt = NULL;
	m_pCaptureWgt = NULL;
}

void CUIRootWin::ClearParameter()
{
	CUIWidget::ClearParameter();

	m_Life = 0.0;
	m_MouseIdleTime=0.0;

	m_nEditMode=UI_MODE_DEFAULT;


	//��սű������б�
	g_vecScriptFuncs.clear();

	//����ϵͳ�ű������б�
	for(int i=0;i<SYSTEM_SCIRPT_NUM;i++)
	{ 
		g_vecScriptFuncs.push_back(g_SystemScriptFuncs[i]);
	}

	m_pCurCursor=NULL;
	std::vector<CUICursor *>::iterator iterCursor;
	for(iterCursor = m_AllCursor.begin(); iterCursor!=m_AllCursor.end(); iterCursor++ ) 
	{
		(*iterCursor)->Release();
	}


	SetCurScreen(NULL);
	std::vector<CUIWidget *>::iterator iterScreen;
	for(iterScreen = m_AllScreen.begin(); iterScreen!=m_AllScreen.end(); iterScreen++ ) 
	{
		(*iterScreen)->Release();
	}

	m_AllScreenDesc.clear();
	
}

//�����������Ӻ���
void CUIRootWin::AddRootChild( CUIWidget *pwidget )
{
	if(pwidget)
	{
	
		if(pwidget->GetType()==WGTTYPE_SCREEN)
			AddScreen( pwidget );
		else 
		{	
			AddChild( pwidget );
			
			//ִ�������ʼ��
			pwidget->OnLoadRecursive();
		}

	}
	
}



////////////////////////////Cursor-begin//////////////////////////////////
		

//���һ��cursor
//�����ǰcursorΪ�գ���������Ϊ��ǰcursor
CUICursor *CUIRootWin::AddCursor( const char *pname, const char *path )
{
	CUICursor *p = new CUICursor(pname, path);
	m_AllCursor.push_back(p);
	
	if( m_pCurCursor == NULL ) m_pCurCursor = p;
	return p;
}

CUICursor *CUIRootWin::FindCursor( const char *pname )
{
	std::vector<CUICursor *>::iterator iter;
	for(iter = m_AllCursor.begin(); iter!=m_AllCursor.end(); iter++ )
	{
		if( strcmp((*iter)->GetName(), pname) == 0 ) return *iter;
	}
	return NULL;
}

//����ָ����cursor,����ҵ�,�滻��ǰcursor
//����Ҳ���,���ֵ�ǰcursor����
void CUIRootWin::SetCurCursor( const char *pname )
{
	if(m_DragInfo.type!=UI_DRAG_TYPE_NONE)
	{
		pname=DEFAULT_DRAG_CURSOR;
	}


	CUICursor* pCursor=FindCursor(pname);
	pCursor->SetActive();
	if(pCursor)	m_pCurCursor = pCursor;

}

CUICursor *CUIRootWin::GetCurCursor()
{ 
	return 	m_pCurCursor;
}

////////////////////////////Cursor-end////////////////////////////////////////


////////////////////////////Screen-begin////////////////////////////////////////


//�õ���ǰ������Ļ
CUIWidget* CUIRootWin::GetCurScreen()
{
	return m_pCurScreen;
}

int CUIRootWin::GetNumScreen()
{
	return m_AllScreen.size();
}

CUIWidget* CUIRootWin::GetScreen(int i)
{
	if((i>=0)&&(i<m_AllScreen.size()))
		return m_AllScreen[i];
	else
		return NULL;
}

//��root��ɾ��ԭscreen,֮ǰ������deactive
//�����screen��������active����
void CUIRootWin::SetCurScreen(CUIWidget* pNewScreen)
{

		if( m_pCurScreen )
		{
			m_pCurScreen->OnDenitRecursive();
			DelChild( m_pCurScreen );
		}
		
		//��ǰ��Ļ����Ϊ��
		m_pCurScreen=pNewScreen;
		
		if( m_pCurScreen )
		{

			AddChild( m_pCurScreen );
		
			//û�������
			if(!(m_pCurScreen->IsLoaded()))
			m_pCurScreen->OnLoadRecursive();
			
			m_pCurScreen->OnInitRecursive();

		}



}


void CUIRootWin::DebugScreen()
{
	if(m_pCurScreen)
		m_pCurScreen->CallSafeScriptEx(SCRIPT_FUNC_ON_DEBUG);
}


//�������뵱ǰ��Ļ
void CUIRootWin::ReloadScreen()
{
	
	std::string id;
	
	CancelDrag();

	//����ǵ�ǰ��Ļ
	if(m_pCurScreen)
	{
		id=m_pCurScreen->GetWidgetID();
		SetCurScreen(NULL);
	}


	//��root��screen�����У�ɾ��ָ��id��screen
	DelScreen(id.c_str());

	GotoScreen(id.c_str());

}


//ʼ�ձ���root��ֻ��һ��screen
void CUIRootWin::GotoScreen( const char *id )
{
	
	CUIWidget* pNewScreen=NULL;
	
	//��screen�����У�����ָ��id��screen
	for(int i=0; i<m_AllScreen.size(); i++ )
	{
		CUIWidget *pScreen = m_AllScreen[i];
		if( strcmp(pScreen->GetWidgetID(), id) == 0 )
		{
			pNewScreen=pScreen;
			break;
		}
	}
	
	
	//��root��screen�����У��Ҳ���ָ��id��screen
	//��Ҫ���ļ���ȥ����
	if(!pNewScreen)
	{
		pNewScreen=LoadScreen(id);
	
		if(pNewScreen)
		{
			//��ӵ���Ļ������
			AddScreen(pNewScreen);

			pNewScreen->Release();

		}

	}
	
	
	SetCurScreen(pNewScreen);	

	
}

//���һ��������Ļ(������ӵ�root��,������ӵ���������)
void CUIRootWin::AddScreen( CUIWidget *pscreen )
{
	if(pscreen)
	{
		assert( pscreen->GetType() == WGTTYPE_SCREEN );
		m_AllScreen.push_back( pscreen );
		pscreen->IncRefCount();
	}
}


void CUIRootWin::DelScreen( const char* id )
{
	
	std::vector<CUIWidget *>::iterator iter;
	for( iter=m_AllScreen.begin(); iter!=m_AllScreen.end(); iter++ )
	{
		if( strcmp((*iter)->GetWidgetID(),id)==0)
		{
			(*iter)->Release();
			m_AllScreen.erase(iter);
			return;
		}
	}


}

////////////////////////////Screen-end////////////////////////////////////////





//�������Լ�
void CUIRootWin::RenderSelf()
{

}


void CUIRootWin::PostRenderSelf()
{

	if(m_DragInfo.type!=UI_DRAG_TYPE_NONE&&m_DragInfo.pDragSrc) 
	{
		stDrawItem item;
		item.type=m_DragInfo.type;
		item.index=m_DragInfo.index;
		item.hosttype=m_DragInfo.hosttype;
		item.hostindex=m_DragInfo.hostindex;

		item.rect.width=m_DragInfo.rect.width;
		item.rect.height=m_DragInfo.rect.height;		
		item.rect.ox=CUICursor::m_Pos.x-m_DragInfo.rect.width/2;
		item.rect.oy=CUICursor::m_Pos.y-m_DragInfo.rect.height/2;		


		item.itemnum=m_DragInfo.itemnum;
		item.state=UI_ITEM_STATE_DRAGGING;
		
		m_DragInfo.pDragSrc->DrawDragItem(item);
		
	}

	
	if( m_pCurCursor && CUICursor::m_bShow ) 
	{
		stDrawCursor cursorData;

		//�����ڻ������֮ǰ���ٻ���һЩ�������
		ExecCallBackFunc(UI_Callback_DrawCursor,(void*)&cursorData);	

		if(!cursorData.bClientCursor)
			m_pCurCursor->Render();//cursor�����н����������
		
	}

}

void CUIRootWin::PostUpdateSelf( float deltatime )
{
	//ȷ�����к��Ӱ��ջ���˳������
	if(!IsSorted()) 
	{
		SortSelf();
	}
	if(m_pFocusWgt)
	{
		if(!m_pFocusWgt->IsVisibleEx())
		{
			SetFocusWgt(NULL);
		}
		else if(!m_pFocusWgt->IsEnabledEx())
		{
			SetFocusWgt(NULL);
		}
	}

	if(!m_pFocusWgt)
	{
		if(m_pCurScreen)  
		{
			m_pCurScreen->CallSafeScript(SCRIPT_FUNC_ON_FOCUS_RELAY);
		}
	}


	if(m_pHotWgt)
	{
		if(!m_pHotWgt->IsAcceptInput())
		{
			SetHotWgt(NULL);
		}
		else if(!m_pHotWgt->IsVisibleEx())
		{
			SetHotWgt(NULL);
		}
		else if(!m_pHotWgt->IsEnabledEx())
		{
			SetHotWgt(NULL);
		}
	}

	if(!m_pHotWgt)
	{
		if(m_pCurScreen)  
		{
			m_pCurScreen->CallSafeScript(SCRIPT_FUNC_ON_HOT_RELAY);
		}
	}

	//��ǰcapture��һ������
	if(m_pCaptureWgt&&m_pFocusWgt)
	{
		bool bRelay=false;
		if(!m_pCaptureWgt->IsVisibleEx())
		{
			bRelay=true;
		}
		else if(!m_pCaptureWgt->IsEnabledEx())
		{
			bRelay=true;
		}
		else if((m_pCaptureWgt!=m_pFocusWgt)&&(!m_pCaptureWgt->IsInDescend(m_pFocusWgt)))
		{
			//���focus�Ĵ��ڲ���capture��Ҳ����capture���¼�����
			//�����capture���ڵĽű�(����capture��������captureΪ��)
			bRelay=true;
		}
		
		if(bRelay) 
		{
			m_pCaptureWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_CAPTURE_RELAY);			
		}
	}
	//}}}

}


void CUIRootWin::UpdateSelf( float deltatime )
{

	m_Life += deltatime;
	m_MouseIdleTime+=deltatime;

	if( m_pCurCursor ) 
	{
		m_pCurCursor->Update( deltatime );

		//Edit���ڵ��Զ�����
		if((GetKeyState(VK_LBUTTON) & 0xff00))
		{
			if(m_pFocusWgt&&m_pFocusWgt->IsAcceptInput())
			{
				m_pFocusWgt->OnMessage( MSGTYPE_MOUSEMOVE, MK_LBUTTON, MAKELONG(m_pCurCursor->m_Pos.x,m_pCurCursor->m_Pos.y) );
			}

		}

		if(m_pMouseWgt)
		{

			float idletime=m_pMouseWgt->GetIdleTime();

			if(m_pMouseWgt->IsAcceptIdle()&&(idletime>=0.0f))
			{

				//����idle��Ϣ,���ҵ�ǰͣ��ʱ���㹻
				if(m_MouseIdleTime>=idletime)
				{
					m_pMouseWgt->OnMessage(MSGTYPE_MOUSEIDLE,0,MAKELONG(m_pCurCursor->m_Pos.x,m_pCurCursor->m_Pos.y));
				}
			}

		}
	}

}


int CUIRootWin::OnMessage( UIMsgType msgtype, unsigned int wparam, unsigned int lparam )
{
	if( this->GetCaptureWgt() )//�����ǰ��һ��capture����
	return 1;//����1�������Ϣ�Ĵ�����Ϊֹ

	return 0;//����0���������ݺʹ���

}

void CUIRootWin::SetParameter( CVar *pvar, CUIWidget *pfather )
{

	CUIWidget::SetParameter( pvar, pfather );
	
	CVar* pelement;


	if( (pelement=pvar->Element("AllScript")) )	
	{
		//�����û��ű�
		CVarArray* pScriptNames=(CVarArray*)pelement;
		
		for(int i=0;i<pScriptNames->GetNum();i++)
		{ 
			g_vecScriptFuncs.push_back(pScriptNames->Element(i)->String());
		}
	}

	
	if( (pelement=pvar->Element("AllScreen")) )	
	{

		//������Ļ�ļ���	
		CVarArray* pScreens=(CVarArray*)pelement;
		
		for(int i=0;i<pScreens->GetNum();i++)
		{ 
			CVarTable *pScrn=(CVarTable*)(pScreens->Element(i));

			CVar* pIdVar=pScrn->Element("id");
			CVar* pPathVar=pScrn->Element("path");
			
			if(pIdVar&&pPathVar)
			{
				stScreenDesc desc;				
				
				const char* paths[UI_SCRN_FILE_MAX];
				int pathnum=UI_SCRN_FILE_MAX;
				pathnum=pPathVar->StringArray(paths,pathnum);

				for(int j=0;j<pathnum;j++)
				{ 
					desc.luaPaths.push_back(std::string(paths[j]));
				}
				
				
				AddScreenFile(pIdVar->String(), desc);			
			}
		}


	}

	
	//�������ͼ��
	if( (pelement=pvar->Element("AllCursor")) )	
	{
		CVarArray* Cursors=(CVarArray*)pelement;
			
		for( int i=0; i<Cursors->GetNum(); i++ )
		{
			CVar *pcursorVar = (CVar *)(Cursors->Element(i));

			CVar* pNameVar=pcursorVar->Element("name");
			CVar* pPathVar=pcursorVar->Element("path");
			
			if(pNameVar)
			{

				CUICursor *pcursor = NULL;
				
				if(pPathVar)
					pcursor = this->AddCursor( pNameVar->String(), pPathVar->String() );
				else 
					pcursor = this->AddCursor( pNameVar->String(), "" );

				if( pcursor ) 
				{
					if( pelement=pcursorVar->Element("step") )	
						pcursor->SetAnimStep( pelement->Float() );

					if( pelement=pcursorVar->Element("hotspot") )
					{
						int mypos[2];
						pelement->IntegerArray( mypos, 2 );
						pcursor->SetHotSpot( Point2D(mypos[0], mypos[1]) );
					}
					
					if( pelement=pcursorVar->Element("holdframe") )
						pcursor->SetHoldFrame( pelement->Integer() );

					if( pelement=pcursorVar->Element("rightframe") )
						pcursor->SetRightFrame( pelement->Integer() );


				}

			}
		}
	}

	
	

}



void CUIRootWin::AddScreenFile(const char* id,const stScreenDesc& scrnDesc)
{
	
	m_AllScreenDesc[id]=scrnDesc;
	
}


const char* CUIRootWin::GetCursorInfo(int& x,int& y)
{
	x=CUICursor::m_Pos.x;
	y=CUICursor::m_Pos.y;

	return m_pCurCursor->GetName();

}

void CUIRootWin::OnSaveRecursive(FILE* fp)
{

	fprintf(fp,"%s=\n{\n",m_ID.c_str());
    
	OnSave(fp);	

	//����UI�ķ�screen����
	if(m_Children.size()>0) 
    {
		fprintf(fp,"\nchildren=\n{\n");
        
		std::vector<CUIWidget *>::iterator iter;
		for( iter=m_Children.begin(); iter!=m_Children.end(); iter++ )
		{
			if((*iter)->GetType()!=WGTTYPE_SCREEN)
			(*iter)->OnSaveRecursive(fp);
		}

		fprintf(fp,"};\n\n\n");
    }		
	
    fprintf(fp,"};\n\n\n");

}

CUIWidget *CUIRootWin::FindRootWidgetByID( const std::string &id )
{	

	if(id=="")  return NULL;
	else if(id==m_ID) return this;

	std::string tmpbuf=m_ID;
	tmpbuf+=".";
	
	//����·��һ����root id��ʼ
	if( id.substr(0,tmpbuf.length()) != tmpbuf ) return NULL;

	return FindWidgetByID( id, tmpbuf.length() );//����root id����Ϊ��id�Ǻ��ӵ�·��	
	
}

void CUIRootWin::OnDestroyWidget(CUIWidget *pwidget)
{

	if(m_pFocusWgt==pwidget)
		m_pFocusWgt=NULL;

	if(m_pCaptureWgt==pwidget)
		m_pCaptureWgt=NULL;

	if(m_pMouseWgt==pwidget)
		m_pMouseWgt=NULL;

	if(m_pPopupWgt==pwidget)
		m_pPopupWgt=NULL;

	if(m_pHotWgt==pwidget)
		m_pHotWgt=NULL;

	if(m_pDragWgt==pwidget)
	{
		m_pDragWgt=NULL;
		CancelDrag();
	}


	if(m_pAjustWgt==pwidget)
	{
		m_pAjustWgt=NULL;
	}

}


void CUIRootWin::SetPopupWgt( CUIWidget *pwidget )
{
	m_pPopupWgt = pwidget;
}

//����ָ������Ϊcapture����
//����ָ������Ϊfocus����
void CUIRootWin::SetCaptureWgt( CUIWidget *pwidget )
{
	m_pCaptureWgt= pwidget;

	//ȡ���϶��͵���
	if(m_pCaptureWgt)
	{
		m_pDragWgt=NULL;
		m_pAjustWgt=NULL;
	}
	
}


//����ָ������Ϊfocus����
//������Ϊһ��edit����ʱ: ���ý�����EnableIME,ʧȥ������DisableIME
void CUIRootWin::SetFocusWgt( CUIWidget *pwidget )
{
	if(m_pFocusWgt == pwidget) return;

	//�����SCRIPT_FUNC_ON_LOSTFOCUS�ű�����SetFocus,����Ҫ����
	static bCanFocus;

	bCanFocus=true;
	if(m_pFocusWgt&&m_pFocusWgt->IsFocused())	
	{
		m_pFocusWgt->OnLostFocus();
		if(m_pFocusWgt) m_pFocusWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_LOSTFOCUS);
	}
	if(!bCanFocus) return;

	m_pFocusWgt=pwidget;

	if(m_pFocusWgt&&(!m_pFocusWgt->IsFocused()))
	{
		m_pFocusWgt->OnGainFocus();
		if(m_pFocusWgt) m_pFocusWgt->CallSafeScriptEx(SCRIPT_FUNC_ON_GAINFOCUS);
	}

	bCanFocus=false;

}


void CUIRootWin::SetMouseWgt( CUIWidget *pwidget )
{

	m_pMouseWgt= pwidget;
}

void CUIRootWin::SetDragWgt(  CUIWidget *pwidget )
{
	m_pDragWgt= pwidget;
}

void CUIRootWin::SetAjustWgt( CUIWidget *pwidget )
{
	m_pAjustWgt= pwidget;
}

void CUIRootWin::SetHotWgt( CUIWidget *pwidget )
{
	m_pHotWgt= pwidget;
}


CUIWidget *CUIRootWin::GetMouseWgt() 
{ 
	return m_pMouseWgt; 
}

CUIWidget *CUIRootWin::GetCaptureWgt() 
{ 
	return m_pCaptureWgt; 
}

CUIWidget *CUIRootWin::GetFocusWgt()
{
	return m_pFocusWgt;
}


CUIWidget* CUIRootWin::GetPopupWgt()
{
	return m_pPopupWgt;
}

CUIWidget* CUIRootWin::GetDragWgt()
{
	return m_pDragWgt;
}

CUIWidget* CUIRootWin::GetAjustWgt()
{
	return m_pAjustWgt;
}

CUIWidget* CUIRootWin::GetHotWgt()
{
	return m_pHotWgt;
}


void CUIRootWin::ShowMessageBox(Msgbox_Cmd nCmdShow,const char* pText/*=""*/,const char* pCaption/*="Message"*/,int nMsgId/*=0*/,CUIWidget* pWgt/*=0*/,int nHandle/*=0*/)
{

	if(!m_pCurScreen) return;

	//�����ڵ�ǰ��Ļ�в�����Ϣ��,���û��,�͵��������в���
	CUIWidget* pMsgBox=m_pCurScreen->GetSafetBuddyWgt(BUDDY_WIDGET_MSGBOX);
	if(!pMsgBox) pMsgBox=GetSafetBuddyWgt(BUDDY_WIDGET_MSGBOX);
	
	if(pMsgBox)
	{
		((CUIMessageBox*)pMsgBox)->ShowMsgBox(nCmdShow,pText,pCaption,nMsgId,pWgt,nHandle);
	}

}




//�յ���ݼ�ʱ,�����focus,ֱ�ӷ���focus����,focus�鿴�Լ�,�Լ����ϼ�,..,screen,root�Ƿ���
//���û��focus,ֱ�ӷ���screen,screen�鿴�Լ���root�Ƿ���
//���û��screen,ֱ�ӷ���root
int CUIRootWin::OnAccelerator(UINT msgid, WPARAM wparam, LPARAM lparam)
{

	stAccelItem accel;
	accel.accel_key=(char)wparam;

	accel.modifier_shift=(lparam&MK_SHIFT)!=0;
	accel.modifier_ctrl=(lparam&MK_CONTROL)!=0;
	accel.modifier_alt=(lparam&MK_ALT)!=0;
	
	if(msgid==MSGTYPE_KEYUP) accel.accel_type=UI_KEY_UP;
	else if(msgid==MSGTYPE_KEYDOWN) accel.accel_type=UI_KEY_DOWN;
	else accel.accel_type=UI_KEY_PRESS;



#ifdef _UIEDITMODE
	if(m_nEditMode==UI_MODE_DEBUG)
		OnAccelEditMode(accel);
#endif

	CUIWidget* pWgt=NULL;

	if(m_pFocusWgt) 
	{
		pWgt=m_pFocusWgt->FindWidgetForAccel(accel);
	}
	
	if(!pWgt&&m_pCurScreen)
	{
		pWgt=m_pCurScreen->FindWidgetForAccel(accel);
	}
	
	if(pWgt)
	{
		//���ӽ����ݼ���������
		bool bLocked=false;
		if(m_pCurScreen)
		{
			bLocked=m_pCurScreen->CallSafeScriptEx(SCRIPT_FUNC_ON_QUERYLOCK,"%d,%d,%d,%d",
				accel.accel_key,accel.modifier_shift,accel.modifier_ctrl,accel.modifier_alt,accel.accel_type)>0;
		}

		if(!bLocked)
		{
			return pWgt->ProcessAccelerator(accel, wparam, lparam);
		}
	}
	
	return 0;

}


void CUIRootWin::HidePopup(int x,int y)
{
	
	if(!m_pPopupWgt)
		return;
	
	//IsPopup()���ж���Ϊ��֧��ͼ������
	if(!m_pPopupWgt->IsPopup())
		return;

	//������������ⲿ��֪ͨpopup����
	if(!m_pPopupWgt->IsPointInDescendant(x,y))
	{
		CUIWidget* pBuddy=m_pPopupWgt->GetSafetBuddyWgt(BUDDY_WIDGET_BUDDY);

		//���popup�����л�鴰�ڣ���������������鴰����
		//��popup���ڲ���Ҫ�Զ�����
		if((pBuddy)&&(pBuddy->IsPointInDescendant(x,y)))
			return;

		m_pPopupWgt->ShowWidget(false);		
	}

}

//�����ǰ���϶��������϶���Ʒ����Ŀ
bool CUIRootWin::AjustDragNumber(bool bAdd)
{

	if(m_DragInfo.type==UI_DRAG_TYPE_NONE)
		return false;
	
	if(!m_DragInfo.pDragSrc)
		return false;

	//���������
	if(!m_DragInfo.bAjust)
		return false;

	if(bAdd)
		m_DragInfo.itemnum++;
	else 
		m_DragInfo.itemnum--;

	if(m_DragInfo.itemnum<0)
		m_DragInfo.itemnum=0;
	
	if(m_DragInfo.itemnum>m_DragInfo.maxnum)
		m_DragInfo.itemnum=m_DragInfo.maxnum;

	return true;
	
}


//�����ǰ���϶���ȡ���϶�
bool CUIRootWin::CancelDrag()
{

	if(m_DragInfo.type==UI_DRAG_TYPE_NONE)
		return false;

	//֪ͨԴ����,�ϷŽ���,�������ñ��Ϸŵ��ߵ�״̬��־
	if(m_DragInfo.pDragSrc)
	{
		//֪ͨ�ͻ��ˣ������϶���Ŀ
		stClickItem useItem;
		useItem.type=m_DragInfo.type;
		useItem.index=m_DragInfo.index;
		useItem.hosttype=m_DragInfo.hosttype;
		useItem.hostindex=m_DragInfo.hostindex;
		m_DragInfo.pDragSrc->ExecCallBackFunc(UI_Callback_EndDragItem,(void*)&useItem);	

	}
	
	//�����Ϸ�
	m_DragInfo.Reset();

	return true;

}

//�����ǰ���϶���������һ�����ڽ����ͷţ����ͷ���Ʒ��ȡ���϶�
bool CUIRootWin::AcceptDrop(CUIWidget* pTarget,int x,int y)
{

	if(!pTarget) 
		return false;
		
	if(pTarget->ItemDrop(x,y))
	{

		CancelDrag();

		//��������
		stUISound sndData;
		sndData.id=pTarget->GetSoundId(UI_SOUND_DROP);
		sndData.cmd=UI_SOUND_DROP;
		sndData.mode=0;
		pTarget->ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);

	}

	return true;
				
				
}

//�����һ�������ṩ�϶�����ʼ�϶�
bool CUIRootWin::AcceptDrag(CUIWidget* pSource,int x,int y,int style)
{
	if(!pSource) 
		return false;

	m_DragInfo=pSource->ParseDragInfo(x,y);

	if(m_DragInfo.type==UI_DRAG_TYPE_NONE)
		return false;
	
	if(!m_DragInfo.pDragSrc)
		return false;


	//ȷ���϶���Ŀ
	if(style==UI_DRAG_STYLE_SINGLE)
		m_DragInfo.itemnum=1;
	else if(style==UI_DRAG_STYLE_MULTI)
		m_DragInfo.itemnum=m_DragInfo.maxnum;
	else
		m_DragInfo.itemnum=-1;

	//֪ͨ�ͻ��ˣ���ǰ��ʼ�϶���Ŀ
	stClickItem useItem;
	useItem.type=m_DragInfo.type;
	useItem.index=m_DragInfo.index;
	useItem.hosttype=m_DragInfo.hosttype;
	useItem.hostindex=m_DragInfo.hostindex;
	pSource->ExecCallBackFunc(UI_Callback_BeginDragItem,(void*)&useItem);	


	//��������
	stUISound sndData;
	sndData.id=pSource->GetSoundId(UI_SOUND_DRAG);
	sndData.cmd=UI_SOUND_DRAG;
	sndData.mode=0;
	pSource->ExecCallBackFunc(UI_Callback_PlaySound,(void*)&sndData);	


	return true;


}

CUIWidget* CUIRootWin::FindTargetOnPoint(int x,int y)
{
	CUIWidget* pTarget=NULL;

	if( m_pCaptureWgt )
	{
		pTarget = m_pCaptureWgt->FindWidgetOnPoint( x, y );
		
		if(!pTarget) 
			pTarget= m_pCaptureWgt;
	}
	else 
	{
		pTarget = this->FindWidgetOnPoint( x, y );
	}

	return pTarget;
}

int CUIRootWin::OnWindowMessage( UINT msgid, WPARAM wparam, LPARAM lparam )
{
			
	int mousex = LOWORD(lparam) / g_fXScale;
	int mousey = HIWORD(lparam) / g_fYScale;

	if (WM_LBUTTONDOWN == msgid || WM_LBUTTONDBLCLK == msgid || WM_LBUTTONUP == msgid ||
		WM_RBUTTONDOWN == msgid || WM_RBUTTONDBLCLK == msgid || WM_RBUTTONUP == msgid ||
		WM_MOUSEMOVE   == msgid )
	{
		lparam = MAKELONG(mousex, mousey);
	}

	switch(msgid) 
	{
		
	case WM_CHAR:		
		if( m_pFocusWgt ) return m_pFocusWgt->OnMessage(MSGTYPE_CHAR, wparam, lparam);
		if( m_pCaptureWgt ) return m_pCaptureWgt->OnMessage( MSGTYPE_CHAR, wparam, lparam );		
		break;

	case WM_KEYLAST:
		
		//�˴���Ҫ�����ݼ�
		if(OnAccelerator(MSGTYPE_KEYPRESS, wparam, lparam)) 
			return 1;

		//�˴���Ҫ�����ݼ�
		if(OnAccelerator(MSGTYPE_KEYDOWN, wparam, lparam)) 
			return 1;

		if( m_pFocusWgt ) return m_pFocusWgt->OnMessage( MSGTYPE_KEYDOWN, wparam, lparam );
		if( m_pCaptureWgt ) return m_pCaptureWgt->OnMessage( MSGTYPE_KEYDOWN, wparam, lparam );

		break;

		
		
	case WM_KEYDOWN:		

		//�˴���Ҫ�����ݼ�
		if(OnAccelerator(MSGTYPE_KEYDOWN, wparam, lparam)) 
			return 1;

		if( m_pFocusWgt ) return m_pFocusWgt->OnMessage( MSGTYPE_KEYDOWN, wparam, lparam );
		if( m_pCaptureWgt ) return m_pCaptureWgt->OnMessage( MSGTYPE_KEYDOWN, wparam, lparam );

		break;

	case WM_KEYUP:

		//�˴���Ҫ�����ݼ�
		if(OnAccelerator(MSGTYPE_KEYUP, wparam, lparam)) 
			return 1;

		if( m_pFocusWgt ) return m_pFocusWgt->OnMessage( MSGTYPE_KEYUP, wparam, lparam );
		if( m_pCaptureWgt ) return m_pCaptureWgt->OnMessage( MSGTYPE_KEYUP, wparam, lparam );
		break;

	case WM_LBUTTONDBLCLK:
		{		
			
			HidePopup(mousex,mousey);

			CancelDrag();
					
			CUIWidget* pwin = FindTargetOnPoint(mousex,mousey);						
				
			this->SetFocusWgt( pwin );
				
			if( pwin )
			{
				pwin->HideTooltip(false,mousex,mousey);				
				
				int nMsgResult=pwin->OnMessage( MSGTYPE_LBTNDBLCLK, wparam, lparam );
				
				//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
				if((pwin->GetStyle()&WGTSTYLE_CLEANWIN)!=0)	return 0;
				else  return nMsgResult;		
			}
				
		}
		
		break;

	case WM_LBUTTONDOWN:
		{
			

			HidePopup(mousex,mousey);		
					
				
			CUIWidget* pwin=FindTargetOnPoint(mousex,mousey);			
			this->SetFocusWgt( pwin );
			if(!pwin) return 0;

			pwin->HideTooltip(false,mousex,mousey);
			
			//�Ƿ���Ҫ�ͷŵ���
			if((m_DragInfo.type!=UI_DRAG_TYPE_NONE)&&(m_DragInfo.pDragSrc))
			{
				if(m_DragInfo.bAttach)
				{
					AcceptDrop(pwin,mousex,mousey);

					return 1;
				}
			}
			
			//�ϷŲ������
			int dragStyle=0;

			if(wparam&MK_CONTROL)	 
				dragStyle=UI_DRAG_STYLE_MULTI;
			else if(wparam&MK_SHIFT) 
				dragStyle=UI_DRAG_STYLE_SINGLE;
			else 
				dragStyle=UI_DRAG_STYLE_CUSTOM;
			
			if(AcceptDrag(pwin,mousex,mousey,dragStyle))
			{
				pwin->OnMessage( MSGTYPE_LEFTBTNDOWN, wparam, lparam );
				return 1;
			}


			//���û����Ŀ�϶�,���Ƿ��е��������
			stKeyChar keyChar;
			if((pwin->ParseKeyChar(mousex,mousey,keyChar)&RICHCHAR_CMD_CLICK)!=0)
			{
				keyChar.cmdid=RICHCHAR_CMD_CLICK;
				pwin->ExecCallBackFunc(UI_Callback_HitKeyChar,(void*)&keyChar);
				
				return 1;
				
			}

			int flag=pwin->IsPointInAjustRect(mousex, mousey);
			
			//�Ƿ��ܵ������ڴ�С
			if( flag!=EDGE_FLAG_NONE )
			{

				m_DragOffset=Point2D(0,0);

				m_pAjustWgt = pwin;
				m_pAjustWgt->SetAjustState(flag);							
				
				//���ڱ������ı߾�����������ƫ��λ��
				if(flag&EDGE_FLAG_LEFT)
				{
					m_DragOffset.x = pwin->GetOriginX() - mousex;
				}
				else if(flag&EDGE_FLAG_RIGHT)
				{
					m_DragOffset.x = pwin->GetEndX() - mousex;
				}

				if(flag&EDGE_FLAG_UP)
				{
					
					m_DragOffset.y = pwin->GetOriginY() - mousey;
				}
				else if(flag&EDGE_FLAG_DOWN)
				{
					m_DragOffset.y = pwin->GetEndY() - mousey;
				}

				pwin->OnMessage( MSGTYPE_LEFTBTNDOWN, wparam, lparam );
				
				return 1;
					
			}
			
			//�Ƿ����϶�����
			if( pwin->IsMovable() && pwin->IsPointInDragRect(mousex, mousey) )
			{
				m_pDragWgt = pwin;

				//����ԭ�������������ƫ��λ��
				m_DragOffset.x = pwin->GetOriginX() - mousex;
				m_DragOffset.y = pwin->GetOriginY() - mousey;
				
				m_pDragWgt->EnableDragging(true);

				pwin->OnMessage( MSGTYPE_LEFTBTNDOWN, wparam, lparam );

				return 1;
				
			}				
			

			int nMsgResult=pwin->OnMessage( MSGTYPE_LEFTBTNDOWN, wparam, lparam );
			
			//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
			if((pwin->GetStyle()&WGTSTYLE_CLEANWIN)!=0)	return 0;
			else  return nMsgResult;		
		
		}
		
		break;


	//�����ǰ��ģʽ���ڣ�capture�������Ƿ����˸�ģʽ�����ϵĿؼ���
	//�������ˣ�����Ϣ���͵����ӿؼ������򣬽���Ϣ���͵�ģʽ���ڡ�
	//�����ǰ��ģʽ���ڣ������Ƿ�����ĳ���ؼ�������ǣ�����Ϣ���͵��ÿؼ���
	case WM_LBUTTONUP:
		{
			CUIWidget* pCursorTarget=NULL;
		
			//�ϷŲ������			
			if( m_pDragWgt)//��ǰ���ϷŴ���
			{	
				pCursorTarget=m_pDragWgt;
				m_pDragWgt->EnableDragging(false);
				m_pDragWgt->OnMessage( MSGTYPE_LEFTBTNUP, wparam, lparam );
				m_pDragWgt = NULL;			
			}
			else if(m_pAjustWgt)
			{
				pCursorTarget=m_pAjustWgt;
				m_pAjustWgt->OnMessage( MSGTYPE_LEFTBTNUP, wparam, lparam );
				m_pAjustWgt->SetAjustState(EDGE_FLAG_NONE);
				m_pAjustWgt=NULL;
			}
			else
			{
				
				CUIWidget* pwin = FindTargetOnPoint(mousex,mousey);
				pCursorTarget=pwin;
				
				//�Ƿ���Ҫ�ͷŵ���
				if((m_DragInfo.type!=UI_DRAG_TYPE_NONE)&&(m_DragInfo.pDragSrc))
				{
					if(!m_DragInfo.bAttach)
					{
						AcceptDrop(pwin,mousex,mousey);

						return 1;
					}
				}
				
				if(pwin)
				{
					BOOL bCleanWin = ((pwin->GetStyle()&WGTSTYLE_CLEANWIN)!=0);
					int nMsgResult=pwin->OnMessage( MSGTYPE_LEFTBTNUP, wparam, lparam );
				
					//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
					if(bCleanWin)	return 0;
					else  return nMsgResult;		
				}
			
			}	
			
			//�����������ʱ�򣬼�������״
			if(pCursorTarget) pCursorTarget->ParseCursor(mousex,mousey);

		}
		break;

	case WM_RBUTTONDOWN:		
		{

			
			HidePopup(mousex,mousey);
			
			//�ϷŲ������
			if(CancelDrag()) return 1;
			
			CUIWidget* pwin = FindTargetOnPoint(mousex,mousey);			
			if(pwin)
			{
				pwin->HideTooltip(false,mousex,mousey);

				//������
				stKeyChar keyChar;
				if((pwin->ParseKeyChar(mousex,mousey,keyChar)&RICHCHAR_CMD_RCLICK)!=0)
				{
					keyChar.cmdid=RICHCHAR_CMD_RCLICK;
					pwin->ExecCallBackFunc(UI_Callback_HitKeyChar,(void*)&keyChar);						

					//���������൱��һ������,�ڱ�����������
					return 1;

				}

				int nMsgResult=pwin->OnMessage( MSGTYPE_RIGHTBTNDOWN, wparam, lparam );
			
				//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
				if((pwin->GetStyle()&WGTSTYLE_CLEANWIN)!=0)	return 0;
				else  return nMsgResult;		
			
			}
			
		}
		break;

	case WM_RBUTTONUP:
		{	

			CUIWidget* pwin = FindTargetOnPoint(mousex,mousey);
		

			if(pwin)
			{
				int nMsgResult=pwin->OnMessage( MSGTYPE_RIGHTBTNUP, wparam, lparam );
				
				//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
				if((pwin->GetStyle()&WGTSTYLE_CLEANWIN)!=0)	return 0;
				else  return nMsgResult;		
			}

		}
		break;

	case WM_MOUSEMOVE:
		{
			CUICursor::m_Pos = Point2D( mousex,mousey );
			m_MouseIdleTime=0.0;

			//�˴��Ƿ�Ҫ����if(wparam&MK_LBUTTON)���ж��أ�

			if( m_pDragWgt )
			{
				int x=m_DragOffset.x+mousex;
				int y=m_DragOffset.y+mousey;
						
				return m_pDragWgt->OnNotify(NTFTYPE_DRAGGING,(unsigned int)x,(unsigned int)y);		
			}
			else if( m_pAjustWgt )
			{
				int x=m_DragOffset.x+mousex;
				int y=m_DragOffset.y+mousey;
				
				return m_pAjustWgt->OnNotify(NTFTYPE_AJUSTTING,(unsigned int)x,(unsigned int)y);		
			}
			else
			{
				//Edit���ڵ��Զ�����
				if(wparam&MK_LBUTTON)
				{
					if(m_pFocusWgt&&m_pFocusWgt->IsAcceptInput())
					{
						return m_pFocusWgt->OnMessage( MSGTYPE_MOUSEMOVE, wparam, lparam );
					}

				}

				CUIWidget* pwin = FindTargetOnPoint(mousex,mousey);

				if( pwin != m_pMouseWgt )//����һ�������ƶ�������һ������
				{
					if( m_pMouseWgt ) m_pMouseWgt->OnMessage( MSGTYPE_MOUSEOUT, wparam, lparam  );
					if( pwin ) pwin->OnMessage( MSGTYPE_MOUSEIN, wparam, lparam  );
					m_pMouseWgt = pwin;
				}
				
				if( m_pMouseWgt ) 
				{
					//����ƶ���ʱ�򣬼�������״
					m_pMouseWgt->ParseCursor(mousex,mousey);

					int nMsgResult=m_pMouseWgt->OnMessage( MSGTYPE_MOUSEMOVE, wparam, lparam );
					
					//��������Ҳ����Ĵ���,���,��Ȼ���ý�ɫ�߶�
					if((m_pMouseWgt->GetStyle()&WGTSTYLE_CLEANWIN)!=0)	return 0;
					else  return nMsgResult;		
					
				}

			}

		}
		break;
		
	case WM_MOUSEWHEEL:
		{

			if(!AjustDragNumber(HIWORD(wparam)>0))
			{
				if( m_pFocusWgt ) return m_pFocusWgt->OnMessage( MSGTYPE_MOUSEWHEEL, wparam, lparam );
				if( m_pCaptureWgt ) return m_pCaptureWgt->OnMessage( MSGTYPE_MOUSEWHEEL,wparam,lparam);
				if( m_pMouseWgt ) return m_pMouseWgt->OnMessage( MSGTYPE_MOUSEWHEEL, wparam, lparam );
			}

		}
		break;
	}
	
	//�˴��Ƿ���screen���߸����ڲ�����Ϣ��?
	//��걻capture���ڲ�����Ϣ�����������ݴ���
	if( this->GetCaptureWgt() )	 return 1;
	
	return 0;


}

void CUIRootWin::ToggleConsole()
{
	CUIConsole* pConsole=NULL;
	
	if(m_pCurScreen) 
		pConsole=(CUIConsole*)(m_pCurScreen->GetSafetBuddyWgt(BUDDY_WIDGET_CONSOLE)); 

	if(!pConsole)
		pConsole=(CUIConsole*)(GetSafetBuddyWgt(BUDDY_WIDGET_CONSOLE)); 

	if(pConsole)
	{
		if(pConsole->IsPathMovePaused()) 
			pConsole->PausePathMove(false);
		else 
			pConsole->FlipPathMove(!(pConsole->IsPathMoveFliped()));
		
	}
}

void CUIRootWin::AddConsoleInfo( const char *pchar )
{
	if (!pchar) 
	{
		return;
	}
	CUIConsole* pConsole=NULL;
	
	if(m_pCurScreen) 
		pConsole=(CUIConsole*)(m_pCurScreen->GetSafetBuddyWgt(BUDDY_WIDGET_CONSOLE)); 

	if(!pConsole)
		pConsole=(CUIConsole*)(GetSafetBuddyWgt(BUDDY_WIDGET_CONSOLE)); 

	if(pConsole)
		((CUIConsole*)pConsole)->PushString( pchar );

}


CScriptState *CUIRootWin::GetScriptEnv()
{ 
	return m_pScript;
}

void CUIRootWin::SetScriptEnv(CScriptState *pScript)
{  
	m_pScript=pScript; 
	
	AddUIScriptFunc(); 
}

#ifdef _UIEDITMODE
//���ӿ���̨�л����༭״̬�л�����Ļ�������룬ִ����Ļ���Խű���4������
int CUIRootWin::OnAccelEditMode(stAccelItem accel)
{

	//�Ǳ༭ģʽ�£������ϵͳ��ݼ����ô���
	CUIWidget* pMouseWgt=GetMouseWgt();
	if(!pMouseWgt) return 0;

	int offset=1;
	if(accel.modifier_shift) offset=10;
	if(accel.modifier_ctrl)  offset=50;

	
	//ctrl+s:����
	if(accel==stAccelItem(83,false,true))
	{
		std::string strFile=pMouseWgt->GetWidgetPath();
		strFile+=".lua";

		FILE* fp;
		fp=fopen(strFile.c_str(),"w");
		pMouseWgt->OnSaveRecursive(fp);
		fclose(fp);

		return 0;

	}
	
	//�ƶ�winrectλ��//////////////////////////////////////////////////////////////////////////
	
	//left
	else if((accel.accel_key==VK_LEFT)&&(accel.accel_type==UI_KEY_DOWN))
	{
		
		pMouseWgt->Move(-offset,0);		
		
		stRect rct=pMouseWgt->GetRelativeWinRect();
		GameUIConsoleInfo("%s winrect(%d,%d,%d,%d)",pMouseWgt->GetWidgetPath(),rct.ox,rct.oy,rct.width,rct.height);
		
		return 0;

	}
	//up
	else if((accel.accel_key==VK_UP)&&(accel.accel_type==UI_KEY_DOWN))
	{

		pMouseWgt->Move(0,-offset);

		stRect rct=pMouseWgt->GetRelativeWinRect();
		GameUIConsoleInfo("%s winrect(%d,%d,%d,%d)",pMouseWgt->GetWidgetPath(),rct.ox,rct.oy,rct.width,rct.height);
		
		return 0;

	}
	//right
	else if((accel.accel_key==VK_RIGHT)&&(accel.accel_type==UI_KEY_DOWN))
	{

		pMouseWgt->Move(offset,0);

		stRect rct=pMouseWgt->GetRelativeWinRect();
		GameUIConsoleInfo("%s winrect(%d,%d,%d,%d)",pMouseWgt->GetWidgetPath(),rct.ox,rct.oy,rct.width,rct.height);
		
		return 0;

	}
	//down
	else if((accel.accel_key==VK_DOWN)&&(accel.accel_type==UI_KEY_DOWN))
	{

		pMouseWgt->Move(0,offset);

		stRect rct=pMouseWgt->GetRelativeWinRect();
		GameUIConsoleInfo("%s winrect(%d,%d,%d,%d)",pMouseWgt->GetWidgetPath(),rct.ox,rct.oy,rct.width,rct.height);
		
		return 0;

	}


	return 0;

}
#endif


CUIRootWin* g_pRootWgt=NULL;