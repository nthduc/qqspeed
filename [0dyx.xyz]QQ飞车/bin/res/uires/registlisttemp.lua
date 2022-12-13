RegistList_UIID_=
{
	type="BUTTON",
	rect={0, 0, 0, 0},
	groupstop=_UIID_,
	groupid=0,
	style=wgtstyle_template,
	bkimage="res/uires/regist/cel.gsa",
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.Regist.AvatarSelDlg.AvatarListPanel.list");
		local x0 = 20;
		local y0= 20;
		local w = 135;
		local h = 180;
		local wpad=20;
		local x = x0 + rcParent.x + (w + wpad) * _UIID_;
		local y = y0 + rcParent.y;
		local myid="UI.Regist.AvatarSelDlg.AvatarListPanel.list.RegistList".._UIID_;
		Win_SetRectAll(myid, x, y, w, h);
		
		local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
		local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
												
		local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
		local avatarindex=_UIID_+page*npp;
		local t=FindRegistInfo(avatarcat, avatarindex);
		
		Win_SetText(myid..".name",t.id);
		Win_SetValue(myid..".icon",t.itemid,value_channel_hostindex);
		
		UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.children.list.children.RegistList0.Update(_UIID_);
	end;
	
	OnClick=function()
		local myid="UI.Regist.AvatarSelDlg.AvatarListPanel.list.RegistList".._UIID_;
		local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
		local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
		local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
		local avatarindex=_UIID_+page*npp;
		local t=FindRegistInfo(avatarcat, avatarindex);
		
		if avatarcat==1 then
			UI.children.Regist.children.RegistDlg._roleid=t.itemid;
		end;
		if avatarcat==2 then
			UI.children.Regist.children.RegistDlg._colorid=t.itemid;
		end;
		
		UI.children.Regist.children.RegistDlg.children.ShowLabel.Update();
		UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel.children.list.children.RegistList0.Update(_UIID_);
	end;
	
	Update=function(uiid)
		local myid="UI.Regist.AvatarSelDlg.AvatarListPanel.list.RegistList"..uiid;
		local page=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._page;
		local npp=UI.children.Regist.children.AvatarSelDlg.children.AvatarListPanel._npp;
		local avatarcat=UI.children.Regist.children.AvatarSelDlg._avatarcat;
		local avatarindex=uiid+page*npp;
		local t=FindRegistInfo(avatarcat, avatarindex);

		if avatarcat==1 then
			if UI.children.Regist.children.RegistDlg._roleid==t.itemid then
				Win_SelectSelf(myid);
			end;
		end;
		if avatarcat==2 then
			if UI.children.Regist.children.RegistDlg._colorid==t.itemid then
				Win_SelectSelf(myid);
			end;
		end;
	end;
	
	OnCheck=function(bCheck)
		local myid="UI.Regist.AvatarSelDlg.AvatarListPanel.list.RegistList".._UIID_;
		if bCheck==1 then
			Win_ShowWidget(myid..".inuse",true);
		end;
		if bCheck==0 then
			Win_ShowWidget(myid..".inuse",false);
		end;
	end;
	
	children=
	{
		name=
		{
			type="LABEL",
			rect={0,20,135,20},
			enable=0,
			style=wgtstyle_static;
			textsize=12;
		};
		icon=
		{
			type="LABEL",
			rect={0,40,135,160},
			bkimagepos={20,30},
			enable=0,
			style=wgtstyle_static+wgtstyle_renderitem,
			
			hosttype=2,
			--hostindex=10000,
			drawflag=drawflag_clientprior,
			
		};
		inuse=
		{
			type="LABEL",
			rect={0,0,50,30},
			bkimage="res/uires/regist/inuse.gsa",	
			visible=0,
		};
	};
};