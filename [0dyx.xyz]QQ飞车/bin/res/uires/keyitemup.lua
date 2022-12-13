keyitemup_UIID_=
{
	type="PICLABEL",
	rect={math.mod(_UIID_,12)*23+17,math.floor(_UIID_/12)*24+16,23,24},	
	captionrect={7,3,16,16},
	caption=keyboard_chars_up[_UIID_+1];
	fontpic = "res/uires/common/fontpic.gsa",
	bkimage = "res/uires/titleui/keychar.gsa",
	
	drawcolor={0,0,0,255},
	groupid=0,
	groupstop=_UIID_,
	style=wgtstyle_template,
	
	framescheme={{1,0,0,0}},
	captionoffset={{1,1}},
	extendstyle=ui_btn_style_popupmsg,
	
	OnClick=function()
		Win_OnInsert("UI.entry.btnpanel.pwdedit",keyboard_chars_up[_UIID_+1]);
		Win_SetFocus("UI.entry.btnpanel.pwdedit");
		end;
};