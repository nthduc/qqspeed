regiontree_UIID_=
{
	type="WIDGET",
	rect={30,_UIID_*25,250,25},
	style=wgtstyle_clipchild_window+wgtstyle_template,
	groupstop=_UIID_,
	
	OnInit=function()
		local myid="UI.SysPDA.childpanel.region_panel.list.regiontree".._UIID_;
		local mychildpanel=myid..".childpanel";
		local locnum,locs=GetLocationInfo();
		local locid=locs[_UIID_+1].ID;
		local svrnum,svrs=GetLocationServers(locid);
		
		if svrnum > 0 then
			Win_AddTemplate(mychildpanel,0,0,svrnum-1);
		end;
		
		local mylabel=myid..".label";
		Win_SetText(mylabel, locs[_UIID_+1].Name);
			
		--local locid,svrid=GetLastRegionInfo();
		local cur_svrid = GetGameVar("ServerID");
		for i=1, svrnum do
			local myitem=mychildpanel..".regiontreeitem"..(i-1);
			Win_SetText(myitem..".label",svrs[i].Name);
			
			if locs[_UIID_+1].Ping < 0 then
				Win_SetBackPic(myitem..".pingbar", "res/uires/PDA/serverspeed_fixing.gsa");
			end;
			if locs[_UIID_+1].Ping <= 200 then
				Win_SetBackPic(myitem..".pingbar", "res/uires/PDA/serverspeed_fast.gsa");
			end;
			if locs[_UIID_+1].Ping > 200 and locs[i].Ping <= 500 then
				Win_SetBackPic(myitem..".pingbar", "res/uires/PDA/serverspeed_mid.gsa");
			end;
			if locs[_UIID_+1].Ping > 500 then
				Win_SetBackPic(myitem..".pingbar", "res/uires/PDA/serverspeed_slow.gsa");
			end;
			
			if svrs[i].ID==cur_svrid then
				Win_SetCheck(myitem..".label",true);
			end;
		end;
		
	end;
	
	children=
	{
		expandbtn=
		{
			type="BUTTON",
			rect={0,0,29,24},
			bkimage="res/uires/PDA/BTN_region_+.gsa",
			OnClick=function()
				local parentid = "UI.SysPDA.childpanel.region_panel.list.regiontree".._UIID_;
				myid=parentid..".expandbtn";
				if Win_IsChecked(myid) then
					Win_SetCheck(myid, false);
					local rc = Win_GetRect(parentid);
					UI.children.SysPDA.children.childpanel.children.region_panel.children.list.ResetTrees();
					Win_SetRect(parentid, rc.x, rc.y, rc.w, 25);			
					UI.children.SysPDA.children.childpanel.children.region_panel.children.list.AlignTrees();
				else
					local locnum,locs=GetLocationInfo();
					local locid=locs[_UIID_+1].ID;
					local svrnum,svrs=GetLocationServers(locid);
					local expandheight = svrnum*25+25;
					
					Win_SetCheck(myid, true);
					local rc = Win_GetRect(parentid);
					UI.children.SysPDA.children.childpanel.children.region_panel.children.list.ResetTrees();
					Win_SetRect(parentid, rc.x, rc.y, rc.w, expandheight);			
					UI.children.SysPDA.children.childpanel.children.region_panel.children.list.AlignTrees();
				end;
			end;		
			OnCheck=function(bcheck)
				local parentid = "UI.SysPDA.childpanel.region_panel.list.regiontree".._UIID_;
				myid=parentid..".expandbtn";
				if bcheck == 0 then
					Win_SetBackPic(myid,"res/uires/PDA/BTN_region_+.gsa");
				else
					Win_SetBackPic(myid,"res/uires/PDA/BTN_region_-.gsa");
				end;
			end;
		};
		label=
		{
			type="BUTTON",
			rect={30,0,200,24},
			caption="±±¾©",
			drawcolor={0,0,255,255},
		};
		childpanel=
		{
			type="WIDGETLIST",
			rect={0,25,200,24},
			template={"res/uires/regiontreeitem.lua"},
			children={};			
		};
	},
};