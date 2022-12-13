
//extern CUIRootWin* g_pRootWgt;

inline CUIWidget* CUIWidget::GetFather()
{
	return m_pFather;
}

inline CUIWidget* CUIWidget::GetGrandpa()
{
	if(m_pFather) 
		return m_pFather->GetFather();
	else 
		return 0;
}

inline CUIWidget* CUIWidget::GetUncle(int alignId)
{
	
	if(GetGrandpa()) 
		return GetGrandpa()->FindChildByAlignId(alignId);
	else 
		return 0;
		
}

inline CUIWidget* CUIWidget::GetBrother(int alignId)
{
	
	if(m_pFather) 
		return m_pFather->FindChildByAlignId(alignId);
	else 
		return 0;
	
}

inline int CUIWidget::GetOriginX()
{
	return m_WinRect.ox;
}

inline int CUIWidget::GetOriginY()
{
	return m_WinRect.oy;
}

inline int CUIWidget::GetEndX()
{
	return m_WinRect.ox+m_WinRect.width;
}

inline int CUIWidget::GetEndY()
{
	return m_WinRect.oy+m_WinRect.height;
}

inline void CUIWidget::SetWidth(int width)
{
	m_WinRect.width=width;
}

inline void CUIWidget::SetHeight(int height)
{
	m_WinRect.height=height;
}

inline void CUIWidget::SetSize(int width,int height)
{
	m_WinRect.width=width;
	m_WinRect.height=height;
}

inline int CUIWidget::GetWidth()
{
	return m_WinRect.width;
}



inline int CUIWidget::GetHeight()
{
	return m_WinRect.height;
}


inline void CUIWidget::SetPreLayer( int i )
{
	m_PreLayer= i;	
}

inline int  CUIWidget::GetPreLayer()
{
	return m_PreLayer;
}

inline void CUIWidget::SetPlusLayer( int i )
{
	m_PlusLayer = i;	
}

inline int  CUIWidget::GetPlusLayer()
{
	return m_PlusLayer;
}

inline void CUIWidget::SetInitLayer(int i)
{
	m_InitLayer=i;	
}

inline int CUIWidget::GetInitLayer()
{
	return m_InitLayer;
}

inline void CUIWidget::SetTipFlag(int flag) 
{ 
	m_TipFlag = flag; 
}

inline int  CUIWidget::GetTipFlag() 
{ 
	return m_TipFlag; 
}


inline void CUIWidget::SetDrawFlag(int flag) 
{ 
	m_DrawFlag = flag; 
}

inline int  CUIWidget::GetDrawFlag() 
{ 
	return m_DrawFlag; 
}

inline void CUIWidget::SetCurFrame(int frame) 
{ 
	m_CurFrame= frame; 
}

inline int  CUIWidget::GetCurFrame() 
{ 
	return m_CurFrame; 
}

inline void CUIWidget::SetGroupId(int id) 
{ 
	m_GroupId = id; 
}

inline int  CUIWidget::GetGroupId() 
{ 
	return m_GroupId; 
}

inline void CUIWidget::SetRectId(int id) 
{ 
	m_RectId = id; 
}

inline int  CUIWidget::GetRectId() 
{ 
	return m_RectId; 
}


inline void CUIWidget::SetAlignId(int id) 
{ 
	m_AlignId = id; 
}

inline int  CUIWidget::GetAlignId() 
{ 
	return m_AlignId; 
}

inline int CUIWidget::GetTextFont()
{
	return m_TextFont;
}

inline void CUIWidget::SetTextFont(int font)
{
	m_TextFont=font;
}

inline int CUIWidget::GetTextSize()
{
	return m_TextSize;
}

inline void CUIWidget::SetTextSize(int size)
{
	m_TextSize=size;
}


inline void CUIWidget::SetRowSpace(int space) 
{ 
	m_RowSpace = space; 
}

inline int  CUIWidget::GetRowSpace() 
{ 
	return m_RowSpace; 
}

inline void CUIWidget::SetRichMode(int mode) 
{ 
	m_RichMode = mode; 
}

inline int  CUIWidget::GetRichMode() 
{ 
	return m_RichMode; 
}

inline void CUIWidget::SetRichFlag(int flag) 
{ 
	m_RichFlag = flag; 
}

inline int  CUIWidget::GetRichFlag() 
{ 
	return m_RichFlag; 
}

inline int  CUIWidget::GetMarginH()
{
	return m_MarginH;
}

inline void CUIWidget::SetMarginH(int margin)
{
	
	m_MarginH=margin;
	
}

inline int  CUIWidget::GetMarginV()
{
	return m_MarginV;
}

inline void CUIWidget::SetMarginV(int margin)
{
	
	m_MarginV=margin;
	
}


inline void CUIWidget::SetSnapH(int snap)
{
	
	m_SnapH=snap;
	
}

inline int  CUIWidget::GetSnapH()
{
	return m_SnapH;
}



inline int  CUIWidget::GetSnapV()
{
	return m_SnapV;
}

inline void CUIWidget::SetSnapV(int snap)
{
	
	m_SnapV=snap;
	
}

inline int  CUIWidget::GetAjustEdge()	
{

	return m_AjustEdge;
}

inline void CUIWidget::SetAjustEdge(int edge)
{
	m_AjustEdge=edge;
}


inline int  CUIWidget::GetAjustState()
{
	return m_AjustState;
}

inline void CUIWidget::SetAjustState(int state)
{
	m_AjustState=state;
}

inline int  CUIWidget::GetAjustFlag()
{
	return m_AjustFlag;
}

inline void CUIWidget::SetAjustFlag(int flag)
{
	m_AjustFlag=flag;
}


inline int  CUIWidget::GetTemplateId()
{
	return m_TemplateId;
}	

inline void CUIWidget::SetTemplateId(int index)
{
	m_TemplateId=index;
}	

//add by lincoln [2006:7:18] [11:54]
//{{{界面库效率优化
inline int  CUIWidget::GetUpdateMuteFlag()
{
	return m_UpdateMuteFlag;
}

inline void CUIWidget::SetUpdateMuteFlag(int flag)
{
	m_UpdateMuteFlag=flag;
}
//}}}

inline int  CUIWidget::GetEdgeCheck()
{
	return m_EdgeCheck;
}

inline void CUIWidget::SetEdgeCheck(int check)
{

	m_EdgeCheck=check;
}

inline int  CUIWidget::GetEdgeThick()
{
	return m_EdgeThick;
}

inline void CUIWidget::SetEdgeThick(int thick)
{

	m_EdgeThick=thick;
}



inline int  CUIWidget::GetMouseDelta()
{
	return m_MouseDelta;
}

inline void CUIWidget::SetMouseDelta(int delta)
{
	m_MouseDelta=delta;
}


inline int  CUIWidget::GetMinHeight()
{
	return m_MinHeight;
}

inline void CUIWidget::SetMinHeight(int height)
{
	m_MinHeight=height;
}

inline int  CUIWidget::GetMaxHeight()
{
	return m_MaxHeight;
}

inline void CUIWidget::SetMaxHeight(int height)
{
	m_MaxHeight=height;
}

inline int  CUIWidget::GetMinWidth()
{
	return m_MinWidth;
}

inline void CUIWidget::SetMinWidth(int width)
{
	m_MinWidth=width;
}

inline int  CUIWidget::GetMaxWidth()
{
	return m_MaxWidth;
}

inline void CUIWidget::SetMaxWidth(int width)
{
	m_MaxWidth=width;
}

inline void CUIWidget::SetAlignToId(int id) 
{ 
	m_AlignToId = id; 
}

inline int  CUIWidget::GetAlignToId() 
{ 
	return m_AlignToId; 
}

inline void CUIWidget::SetTextStyle(int style) 
{ 
	m_TextStyle= style; 
}

inline int  CUIWidget::GetTextStyle() 
{ 
	return m_TextStyle; 
}

inline void CUIWidget::SetGroupStop(int id) 
{ 
	m_GroupStop = id; 
}

inline int  CUIWidget::GetGroupStop() 
{ 
	return m_GroupStop; 
}

inline void CUIWidget::SetTabStop(int id) 
{ 
	m_TabStop = id; 
}

inline int  CUIWidget::GetTabStop() 
{ 
	return m_TabStop; 
}



inline void CUIWidget::SetChildStop(int id) 
{ 
	m_ChildStop = id; 
}

inline int  CUIWidget::GetChildStop() 
{ 
	return m_ChildStop; 
}


inline int  CUIWidget::GetState()
{
	return m_State;
}


inline void CUIWidget::SetStyle(int style)
{
	m_Style=style;	
}

inline int  CUIWidget::GetStyle()
{
	return m_Style;
}


inline void CUIWidget::SetPattern(int pattern)
{
	m_Pattern=pattern;	
}


inline int  CUIWidget::GetPattern()
{
	return m_Pattern;
}

inline void CUIWidget::SetExtendStyle(int style)
{	
	m_ExtendStyle=style;
}	

inline int  CUIWidget::GetExtendStyle()
{	
	return m_ExtendStyle;	
}


inline void CUIWidget::SetCustomState(int state)
{ 
	m_CustomState= state;
}

inline int  CUIWidget::GetCustomState()
{ 
	return m_CustomState;
}

inline void CUIWidget::SetCustomStyle(int style)
{ 
	m_CustomStyle= style;
}

inline int  CUIWidget::GetCustomStyle()
{ 
	return m_CustomStyle;
}



inline void CUIWidget::SetCustomId(int id)
{ 
	m_CustomId= id;
}

inline int  CUIWidget::GetCustomId()
{ 
	return m_CustomId;
}


inline void CUIWidget::SetCustomIndex(int index)
{ 
	m_CustomIndex= index;
}

inline int  CUIWidget::GetCustomIndex()
{ 
	return m_CustomIndex;
}


inline void CUIWidget::SetCustomType(int type)
{ 
	m_CustomType= type;
}

inline int  CUIWidget::GetCustomType()
{ 
	return m_CustomType;
}



inline void CUIWidget::SetCustomName(const char* txt)
{
	m_CustomName=txt;
}

inline void CUIWidget::SetCustomText(const char* txt)
{
	m_CustomText=txt;
}

inline void CUIWidget::SetCustomString(const char* txt)
{
	m_CustomString=txt;
}

inline const char* CUIWidget::GetCustomName()
{
	return m_CustomName.c_str();	
}

inline const char* CUIWidget::GetCustomText()
{
	return m_CustomText.c_str();		
}

inline const char* CUIWidget::GetCustomString()
{
	return m_CustomString.c_str();		
}


inline void CUIWidget::SetAlignType(int type)
{
	m_AlignType=type;
}
inline int  CUIWidget::GetAlignType()
{
	return m_AlignType;
}

inline void CUIWidget::SetAlignStyle(int style)
{
	m_AlignStyle=style;
}
inline int  CUIWidget::GetAlignStyle()
{
	return m_AlignStyle;
}


inline void CUIWidget::SetIdleTime(float time)
{
	m_IdleTime=time;
}
inline float  CUIWidget::GetIdleTime()
{
	return m_IdleTime;
}

inline void CUIWidget::SetPathSpeed(double speed)
{
	m_PathSpeed=speed;
}

inline double CUIWidget::GetPathSpeed()
{
	return m_PathSpeed;
}


inline void CUIWidget::SetFrameSpeed(double speed)
{
	m_FrameSpeed=speed;
}

inline double CUIWidget::GetFrameSpeed()
{
	return m_FrameSpeed;
}


inline double CUIWidget::GetScaleFactor()
{
	return m_ScaleFactor;
}


inline double  CUIWidget::GetAlphaSpeed()
{
	return m_AlphaSpeed;
}


inline void CUIWidget::SetAlphaState(int state)
{
	m_AlphaState=state;
}

inline int CUIWidget::GetAlphaState()
{
	return m_AlphaState;
}


inline void CUIWidget::SetBackColor( stColorQuad color )
{
	m_BackColor = color;
}

inline void CUIWidget::SetDrawColor( stColorQuad color )
{
	m_DrawColor = color;
}

inline void CUIWidget::SetFocusColor( stColorQuad color )
{
	m_FocusColor = color;
}

inline void CUIWidget::SetEdgeColor( stColorQuad color )
{
	m_EdgeColor = color;
}

inline void CUIWidget::SetMagicColor( stColorQuad color )
{
	m_MagicColor = color;
}


inline void CUIWidget::SetLinkColor( stColorQuad color )
{
	m_LinkColor = color;
}


inline stColorQuad CUIWidget::GetBackColor( )
{
    return m_BackColor;
}

inline stColorQuad CUIWidget::GetDrawColor( )
{
    return m_DrawColor;
}

inline stColorQuad CUIWidget::GetFocusColor( )
{
    return m_FocusColor;
}

inline stColorQuad CUIWidget::GetEdgeColor( )
{
    return m_EdgeColor;
}
	

inline stColorQuad CUIWidget::GetMagicColor( )
{
    return m_MagicColor;
}


inline stColorQuad CUIWidget::GetLinkColor( )
{
    return m_LinkColor;
}


inline void CUIWidget::SetWidgetID( const char *pstr )
{
	m_ID = pstr;
}

inline const char *CUIWidget::GetWidgetID()
{
	return m_ID.c_str();
}

inline int  CUIWidget::GetNumChild()
{
	return m_Children.size();
}

inline IWgtData* CUIWidget::GetWgtFather()
{
	return m_pFather;
}

inline IWgtData* CUIWidget::GetWgtChild( int i )
{
	return m_Children[i];
}


inline CUIWidget* CUIWidget::GetChild( int i )
{
	return m_Children[i];
}


//////////////////////////////////////////////////////////////////////////


inline BOOL CUIWidget::IsEditable()
{
	return (m_Style&WGTSTYLE_EDITABLE);
}

inline BOOL CUIWidget::IsVertical()
{
	return (m_Style&WGTSTYLE_VERTICAL);
}

inline BOOL CUIWidget::IsPrimary()
{
	return (m_Style&WGTSTYLE_PRIMARY);
}

inline BOOL CUIWidget::IsPopup()
{
	return (m_Style&WGTSTYLE_POPUP);
}

inline BOOL CUIWidget::IsDragging()
{
	return (m_State&WGTSTATE_DRAGGING);
}

inline void CUIWidget::EnableDragging( bool bDrag )
{ 
	if( bDrag ) m_State |= WGTSTATE_DRAGGING;
	else m_State &= ~WGTSTATE_DRAGGING;
}

inline BOOL CUIWidget::IsDiscarded()
{
	return (m_State&WGTSTATE_DISCARDED);
}

inline void CUIWidget::SetDiscarded( bool bEdit )
{ 
	if( bEdit ) m_State |= WGTSTATE_DISCARDED;
	else m_State &= ~WGTSTATE_DISCARDED;
}

inline BOOL CUIWidget::IsAcceptIdle()
{
	return (m_State&WGTSTATE_ACCEPT_IDLE);
}

inline void CUIWidget::AcceptIdle( bool bIdle )
{ 
	if( bIdle ) m_State |= WGTSTATE_ACCEPT_IDLE;
	else m_State &= ~WGTSTATE_ACCEPT_IDLE;
}

inline BOOL CUIWidget::IsPathMovePaused()
{
	return (m_State&WGTSTATE_PAUSE_PATHMOVE);
}

inline void CUIWidget::PausePathMove( bool bPause )
{ 
	if( bPause ) m_State |= WGTSTATE_PAUSE_PATHMOVE;
	else m_State &= ~WGTSTATE_PAUSE_PATHMOVE;
}

inline BOOL CUIWidget::IsPathMoveFliped()
{
	return (m_State&WGTSTATE_FLIP_PATHMOVE);
}

inline void CUIWidget::FlipPathMove( bool bFlip )
{ 
	if( bFlip ) m_State |= WGTSTATE_FLIP_PATHMOVE;
	else m_State &= ~WGTSTATE_FLIP_PATHMOVE;
}

inline BOOL CUIWidget::IsVisible()
{
	return ((m_State&WGTSTATE_VISIBLE)!=0);
}


inline bool CUIWidget::IsEnabled()
{
	
	return ((m_State&WGTSTATE_DISABLE)==0);
	
}


inline BOOL CUIWidget::IsMovable()
{
	return (m_Style&(WGTSTYLE_SELF_XMOVE|WGTSTYLE_SELF_YMOVE));
}

inline BOOL CUIWidget::IsMovableX()
{
	return (m_Style&WGTSTYLE_SELF_XMOVE);
}

inline BOOL CUIWidget::IsMovableY()
{
	return (m_Style&WGTSTYLE_SELF_YMOVE);
}

inline BOOL CUIWidget::IsSizable()
{
	return (m_Style&(WGTSTYLE_SELF_XSIZE|WGTSTYLE_SELF_YSIZE));
}

inline BOOL CUIWidget::IsSizableX()
{
	return (m_Style&WGTSTYLE_SELF_XSIZE);
}

inline BOOL CUIWidget::IsSizableY()
{
	return (m_Style&WGTSTYLE_SELF_YSIZE);
}


inline bool CUIWidget::IsImeEnabled()
{ 
	return (m_State&WGTSTATE_IME_ENABLE)!=0 ;
}

inline void CUIWidget::EnableIme( bool bFlag)
{ 
	if(bFlag) m_State |= WGTSTATE_IME_ENABLE;
	else m_State &= ~WGTSTATE_IME_ENABLE;
}

inline bool CUIWidget::IsAlignDirty()
{ 
	return (m_State&WGTSTATE_ALIGN_DIRTY)!=0 ;
}

inline void CUIWidget::SetAlignDirty( bool bDirty)
{ 
	if( bDirty) m_State |= WGTSTATE_ALIGN_DIRTY;
	else m_State &= ~WGTSTATE_ALIGN_DIRTY;
}

inline bool CUIWidget::IsUpdateDirty()
{ 
	return (m_State&WGTSTATE_UPDATE_DIRTY)!=0 ;
}

inline void CUIWidget::SetUpdateDirty( bool bDirty)
{ 
	if( bDirty) m_State |= WGTSTATE_UPDATE_DIRTY;
	else m_State &= ~WGTSTATE_UPDATE_DIRTY;
}

inline bool CUIWidget::IsActived()
{ 
	return (m_State&WGTSTATE_ACTIVED)!=0 ;
}


inline bool CUIWidget::IsChecked()
{ 
	return (m_State&WGTSTATE_CHECKED)!=0 ;
}


inline bool CUIWidget::IsFocused()
{ 
	return (m_State&WGTSTATE_FOCUSED)!=0 ;
}

inline bool CUIWidget::IsClickDown()
{ 
	return (m_State&WGTSTATE_CLICKDOWN)!=0 ;
}

inline bool CUIWidget::IsMouseOver()
{ 
	return (m_State&WGTSTATE_MOUSEOVER)!=0 ;
}

inline bool CUIWidget::IsSorted()
{
	return (m_State&WGTSTATE_UPDATE_SORTED)!=0;
}

inline void CUIWidget::SetSorted(bool bSort)
{

	if(bSort) m_State|=WGTSTATE_UPDATE_SORTED;
	else m_State&=~WGTSTATE_UPDATE_SORTED;
}


inline bool CUIWidget::IsAligned()
{
	return (m_State&WGTSTATE_UPDATE_ALIGNED)!=0;
}

inline void CUIWidget::SetAligned(bool bAlign)
{
	
	if(bAlign) m_State|=WGTSTATE_UPDATE_ALIGNED;
	else m_State&=~WGTSTATE_UPDATE_ALIGNED;
}

inline bool CUIWidget::IsLoaded()
{
	return (m_State&WGTSTATE_LOADED)!=0;
}

inline void CUIWidget::SetLoaded(bool bLoad)
{
	if(bLoad) m_State|=WGTSTATE_LOADED;
	else m_State&=~WGTSTATE_LOADED;
}


inline bool CUIWidget::IsAjusted()
{
	return (m_State&WGTSTATE_UPDATE_AJUSTED)!=0;
}

inline void CUIWidget::SetAjusted(bool bAjust)
{
	
	if(bAjust) m_State|=WGTSTATE_UPDATE_AJUSTED;
	else m_State&=~WGTSTATE_UPDATE_AJUSTED;
}

inline BOOL CUIWidget::IsWakeWidget()
{
	return (m_Style&WGTSTYLE_AREAWAKE)!=0;
}

inline BOOL CUIWidget::IsStaticWidget()
{
	return (m_Style&WGTSTYLE_STATIC)!=0;
}

inline BOOL CUIWidget::IsTopMost()
{
	return (m_Style&WGTSTYLE_TOPMOST)!=0;
}

inline BOOL CUIWidget::IsTemplateWgt()
{
	return (m_Style&WGTSTYLE_TEMPLATE)!=0;
}



//全局函数//////////////////////////////////////////////////////////////////////////

inline bool SortWidgetByLayer( CUIWidget *p1, CUIWidget *p2 )
{
	return p1->GetLayer() < p2->GetLayer();
}

inline bool SortWidgetByTabStop( CUIWidget *p1, CUIWidget *p2 )
{
	return p1->GetTabStop() < p2->GetTabStop();
}

inline bool SortWidgetByGroupStop( CUIWidget *p1, CUIWidget *p2 )
{
	return p1->GetGroupStop() < p2->GetGroupStop();
}

inline bool SortWidgetByAlignId( CUIWidget *p1, CUIWidget *p2 )
{
	return p1->GetAlignId() < p2->GetAlignId();
}

inline void RawExecCallBack(UICallbackEvent eventName,CallbackData pData,IWgtData* pWgtData=NULL)
{

	CallBackFunc pFunc=g_vecCallbackFuncs[eventName];
	if(pFunc) pFunc(pData,pWgtData);	

}

inline void InsertSecurityPointer(IWgtData* pWgt)
{

	g_vecSecuritySet.insert(pWgt);

}

inline void EraseSecurityPointer(IWgtData* pWgt)
{

	g_vecSecuritySet.erase(pWgt);

}

inline bool IsSecurityPointer(IWgtData* pWgt)
{

	return(g_vecSecuritySet.count(pWgt)>0);

}