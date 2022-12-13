AvatarList_UIID_=
{
	type="BUTTON",
	--rect = {math.mod(_UIID_, 2)*241 + 13, math.floor(math.mod(_UIID_, 8)/2) * 127 + 15, 247, 134},
	rect = {math.mod(_UIID_, 4)*149 + 18, math.floor(math.mod(_UIID_, 8)/4) * 159 + 51, 140, 145},
	
	groupstop=_UIID_,
	groupid=1,
	style=wgtstyle_template,
	--tabrect={0,0,200,65},
	OnInit=function()
		local myid="UI.garage.AvatarListPanel.list.AvatarList".._UIID_;
		local page=UI.children.garage.children.AvatarListPanel._page;
		local npp=UI.children.garage.children.AvatarListPanel._npp;
								
		local avatarcat=UI.children.garage._avatarcat;
		local avatarindex=_UIID_+page*npp;--+1;
		local itemone,itemname=GetAvatarInfoByType(UI.children.garage._avatarcat,avatarindex);
		if (itemone~=nil) then
			Win_SetText(myid..".name",itemname);
			Win_SetValue("UI.garage.AvatarListPanel.list.AvatarList_UIID_.icon",itemone.itemid,value_channel_hostindex);
			if itemone.status==0 then
				Win_ShowWidget(myid..".checklabel",false);
			else
				Win_ShowWidget(myid..".checklabel",true);
			end;		
		end;
		
	end;
	
	OnClick=function()
		local page=UI.children.garage.children.AvatarListPanel._page;
		local npp=UI.children.garage.children.AvatarListPanel._npp;	
		--local itemtwo=GetAvatarAllList(GetMyInfo().uin).items[_UIID_+page*npp+1];
		local itemid=Win_GetValue("UI.garage.AvatarListPanel.list.AvatarList_UIID_.icon",value_channel_hostindex);
		--RemoveSelfKartFromUIScene("ROOT");
		--RemoveSelfRoleFromUIScene("playermode");
		--ReBindSelfPlayer();
		ChangeAvatarStatus(GetMyInfo().uin,itemid);
		--BindSelfKartToScene("ROOT");
		--BindSelfRoleToScene("playermode");
		--SelfPlayAnimation(1104);
	end;
	
	SetCheck=function(bCheck)
		local myid="UI.garage.AvatarListPanel.list.AvatarList".._UIID_;
		if bCheck==0 then
			Win_ShowWidget(myid..".checklabel",false);
		end;
		if bCheck~=0 then
			Win_ShowWidget(myid..".checklabel",true);
		end;
	end;
	
	children=
	{
		name=
		{
			type="LABEL",
			rect={0,5,142,20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,
			enable=0,
		};
		icon=
		{
			type="LABEL",
			rect={0,20,142,115},
			--bkimagepos={-10,-50},
			--autoscale = 1,
			style = wgtstyle_renderitem,
			hosttype = 2;
			hostindex=0;
			enable=0,
		};
		checklabel=
		{
			type="LABEL",
			rect={103,115,0,60},
			bkimage="res/uires/avatarsel/checkicon.gsa",
			visible=1,
		};
	};
};