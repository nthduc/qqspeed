AvatarSelCat_UIID_=
{
	type="TABWIN",
	rect={0, 0, 0, 0},
	groupstop=_UIID_,
	groupid=0,
	style=wgtstyle_template,
	tabrect={0,0,140,44},
	tabcover=garagetype[_UIID_+1].tabcover,
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.garage.TabBar");
		local x0 = 0;
		local y0= 0;
		local w = 140;
		local h = 44;
		local x = x0 + rcParent.x + w * _UIID_;
		local y = y0 + rcParent.y + 2;
		local myid="UI.garage.TabBar.AvatarSelCat".._UIID_;
		Win_SetRect(myid, x, y, w, h);		
		
		local cat=UI.children.garage._avatarcat-1;
		if _UIID_ ==  cat then
			Win_SetCheck("UI.garage.TabBar.AvatarSelCat".._UIID_,true);
		end;
	end;
	
	OnCheck=function(bCheck)
		if bCheck == 1 then
			UI.children.garage._avatarcat =garagetype[_UIID_+1].typeid; --_UIID_+1
			UI.children.garage._avatarindex = 0;
			UI.children.garage.children.AvatarListPanel.Update();
		end;
	end;
};