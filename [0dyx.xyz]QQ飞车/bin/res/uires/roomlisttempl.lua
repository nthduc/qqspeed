RoomList_UIID_=
{
	type="BUTTON",
	rect={0,0,0,0},
	groupstop=_UIID_,
	style=wgtstyle_template + wgtstyle_clipchild_window,
	framescheme={{0,0,0,1}},
	bkimage="res/uires/roomsel/race_btn.gsa",--"res/uires/roomsel/emptybk.gsa",
	framescheme={{0,0,1,2}},
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.roomsel.MainFrame.RoomListPanel");
		--local row=math.floor(_UIID_/2);
		--local col=math.floor(_UIID_-row*2);
		local row = _UIID_;
		local col = 0;
		local w = 321;
		local h = 88;
		local xd = 0;
		local yd = -2;
		local x=rcParent.x+col*(w+xd)+10;
		local y=rcParent.y+row*(h+yd)+10;
		local myid="UI.roomsel.MainFrame.RoomListPanel.RoomList".._UIID_;
		Win_SetRect(myid,x,y,w,h);
		
		local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
		local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
		local indexfrom=page*npp;

		local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
		local tmap={	[0] = gmodebase_rand, 
					[1] = gmodebase_item,
					[2] = gmodebase_race,
					[3] = gmodebase_race,
				};
		
		local from,num,rooms=GetRoomList(tmap[playcatlog],gmodesub_rand,_UIID_+indexfrom,1);
		--Win_ConsoleInfo("from="..from);
		--Win_ConsoleInfo("num="..num);
		local roomtype = 0;
		if num == 1 then
			--Win_ConsoleInfo("now get info");
			local thisroom=rooms[1];
			if (thisroom.gamemodebase == gmodebase_race) then
				Win_SetBackPic(myid, "res/uires/roomsel/race_btn.gsa");
				if (thisroom.gamemodesub == gmodesub_team) then
					roomtype = 3;
				else
					roomtype = 2;
				end;
			else
				Win_SetBackPic(myid, "res/uires/roomsel/item_btn.gsa");
				if (thisroom.gamemodesub == gmodesub_team) then
					roomtype = 1;
				else
					roomtype = 0;
				end;
			end;
			Win_ShowWidget(myid..".roomtypelabel", true);
			Win_SetText(myid..".roomtypelabel", tostring(roomtype));
			Win_SetText(myid..".namelabel",thisroom.roomname);
			Win_SetText(myid..".roomidlabel",thisroom.roomid);
			--Win_SetText(myid..".mapnamelabel.pnumlabel",thisroom.pnum.."/"..thisroom.seatnum);
			local width = 9 * thisroom.pnum;
			local rc=Win_GetRect(myid..".mapnamelabel.pnumlabel");
			Win_SetRectAll(myid..".mapnamelabel.pnumlabel.pnum", rc.x - width+rc.w, rc.y, width, rc.h);
			
			local tmap={	[0] = "scheme/allrooms/",
						[1] = "scheme/itemplay/",
						[2] = "scheme/raceplay/",
						[3] = "scheme/combatplay/",
					};
			local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
			local schemeid=tmap[playcatlog];
			--Win_SetBackPic(myid,"res/uires/roomsel/"..schemeid.."roombk.gsa");
			
			local t=FindMap(thisroom.mapid);
			Win_SetText(myid..".mapnamelabel",t.id);
			Win_SetBackPic(myid..".mapiconlabel",t.icon);
			
			if thisroom.flag==1 then
				Win_ShowWidget(myid..".mapnamelabel.locklabel",true);
			end;
			if thisroom.flag==0 then
				Win_ShowWidget(myid..".mapnamelabel.locklabel",false);
			end;
			
			--在游戏的房间，变灰～
			--1-等待,2-游戏中,3-颁奖时
			if thisroom.status == 2 then
				Win_EnableWidget(myid,false);
			end;
		else
			Win_ShowWidget(myid..".roomtypelabel", false);
		end;
		
	end;
	
	OnClick=function()
		local page = UI.children.roomsel.children.MainFrame.children.RoomListPanel._pageno;
		local npp=UI.children.roomsel.children.MainFrame.children.RoomListPanel._npp;
		local indexfrom=page*npp;

		local playcatlog=UI.children.roomsel.children.MainFrame.children.RoomListPanel._playcatlog;
		local tmap={	[0] = gmodebase_rand, 
					[1] = gmodebase_item,
					[2] = gmodebase_race,
					[3] = gmodebase_race,
				};
		
		local from,num,rooms=GetRoomList(tmap[playcatlog],gmodesub_rand,_UIID_+indexfrom,1);
		if num == 1 then
			if rooms[1].flag==0 then
				EnterRoom(rooms[1].roomid);
			end;
			if rooms[1].flag==1 then
				UI.children.roomsel.children.MainFrame.children.InputPwdDlg.Show(rooms[1].roomid);
			end;
		end;
	end;
	
	children=
	{
		mapiconlabel=
		{
			type="LABEL",
			rect={15,15,90,62},
			style=wgtstyle_static;
			bkimagepos={10,10},
		};
		roomidlabel=
		{
			type="LABEL",
			rect={108,12,62,18},
			style=wgtstyle_static;			
		};
		roomtypelabel=
		{
			type="PICLABEL",
			rect = {250, 10, 79, 20},
			fontpic="res/uires/roomsel/roomtype.gsa";
			visible = 0;
		};
		namelabel=
		{
			type="LABEL",
			rect={108,30,202,24},
			textsize=12,
			textstyle=dt_left+dt_vcenter,
			textcolor={0,255,100,255},
			fontcoor={0,255,100,255},
			style=wgtstyle_static;
			captionrect={10,0,200,24},			
		};
		
		mapnamelabel=
		{
			type="LABEL",
			rect={108,57,202,24},
			textsize=12,
			textstyle=dt_left+dt_vcenter,
			style=wgtstyle_static;
			captionrect={30,0,100,25},	
			--caption = "地图名称",
			
			children=
			{
				locklabel=
				{
					type="LABEL",
					rect={0,0,20,24},
					bkimage="res/uires/roomsel/lock.gsa",
					visible=0,
				};	

				pnumlabel=
				{
					type="LABEL",
					rect={130,5,72,14},
					textsize=12,					
					--alignstyle=alignstyle_right_in,
					aligntype=aligntype_father+aligntype_winrect,
					bkimage = "res/uires/roomsel/pnumlabel_bk.gsa",
					--caption = "???",
					
					children = 
					{
						pnum=
						{
							type = "LABEL",
							rect  = {0, 0, 0,0},
							bkimage = "res/uires/roomsel/pnumlabel_mask.gsa",
							align_style=alignstyle_right_in,
							style = wgtstyle_clipself,
						},
					},
				};
			};
		};
				
	};
};
