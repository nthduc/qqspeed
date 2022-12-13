MapSelCat_UIID_=
{
	type="TABWIN",
	rect={0, 0, 0, 0},
	groupstop=_UIID_,
	groupid=0,
	style=wgtstyle_template,
	tabrect={0,0,83,47},
	tabcover=mapscat[_UIID_+1].tabcover,
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.room.MapSelDlg.TabBar");
		local x0 = 0;
		local y0= 4;
		local w = 83;
		local h = 47;
		local x = x0 + rcParent.x + w * _UIID_;
		local y = y0 + rcParent.y + 2;
		local myid="UI.room.MapSelDlg.TabBar.MapSelCat".._UIID_;
		Win_SetRect(myid, x, y, w, h);		
		
		local cat=UI.children.room.children.MapSelDlg._mapcat;
		if _UIID_ ==  cat then
			Win_SetCheck("UI.room.MapSelDlg.TabBar.MapSelCat".._UIID_,true);
		end;
	end;
	
	OnCheck=function(bCheck)
		if bCheck == 1 then
			UI.children.room.children.MapSelDlg._mapcat = _UIID_;
			UI.children.room.children.MapSelDlg._mapindex = 0;
			UI.children.room.children.MapSelDlg.Update(1);
			UI.children.room.children.MapSelDlg.children.MapListPanel.Update();
		end;
	end;
};