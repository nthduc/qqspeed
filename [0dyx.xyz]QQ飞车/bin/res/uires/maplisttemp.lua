MapList_UIID_=
{
	type="BUTTON",
	rect={0, 0, 0, 0},
	groupstop=_UIID_,
	groupid=1,
	style=wgtstyle_template,
	tabrect={0,0,200,65},
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.room.MapSelDlg.MapListPanel.list");
		local x0 = 0;
		local y0= 0;
		local w = 180;
		local h = 65;
		local x = x0 + rcParent.x;
		local y = y0 + rcParent.y + h  * _UIID_;
		local myid="UI.room.MapSelDlg.MapListPanel.list.MapList".._UIID_;
		Win_SetRect(myid, x, y, w, h);
		
		local mapcat=UI.children.room.children.MapSelDlg._mapcat;
		local mapindex=_UIID_;
		local t=FindMapByCatIndex(mapcat, mapindex);
		Win_SetBackPic(myid, t.tabcover);
		Win_SetBackPic(myid..".icon", t.icon);
		
	end;
	
	children=
	{
		icon=
		{
			type="LABEL",
			rect={-1,-2,60,60},
			enable=0,
		};
		above=
		{
			type="LABEL",
			rect={-12,0,180,65},
			
			OnClick=function()
				UI.children.room.children.MapSelDlg._mapindex=_UIID_;
				UI.children.room.children.MapSelDlg.Update(1);
			end;
			OnMouseIn=function()
				local myid="UI.room.MapSelDlg.MapListPanel.list.MapList".._UIID_;
				Win_SetBackPic(myid..".above", "res/uires/mapsel/mapselected.gsa");
			end;
			OnMouseOut=function()
				local myid="UI.room.MapSelDlg.MapListPanel.list.MapList".._UIID_;
				Win_SetBackPic(myid..".above", "");
			end;
		};
	};
};