propassignchoice_UIID_=
{
	type = "CHECK",
	rect = {12 + math.mod(_UIID_, 2) * 86 , 6 + math.floor(_UIID_/2) * 34, 21, 22},
	bkimage = "res/uires/playerinfo/autoasign_check.gsa",
	extendstyle = ui_btn_style_radio,
	groupid = 40,
	groupstop = _UIID_+1,
	
	OnCheck = function(bCheck)
		if (bCheck) then
			SetPointAssignScheme(_UIID_);
		end;
	end;
	
	OnClick = function()
		local schemepath = "UI.playerinfo_dlg.playerinfo_content.prop_dlg.autoasign_dlg.checkgroup.propassignchoice_UIID_";
		Win_SelectSelf(schemepath);
		SetPointAssignScheme(_UIID_);		
	end;
};