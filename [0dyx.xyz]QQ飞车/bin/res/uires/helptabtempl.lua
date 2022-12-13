helptab_UIID_=
{
	type = "TABWIN",
	rect={140*_UIID_, 0, 140, 44},
	groupstop=_UIID_,
	groupid=12,
	style=wgtstyle_template,
	tabrect={0,0,140,44},
	--tabcover=mapscat[_UIID_+1].tabcover,
	
	OnInit = function()
		Win_SetTabCover("UI.help.helptab.helptab_UIID_", "res/uires/help/"..(_UIID_+1)..".gsa");
	end;
};