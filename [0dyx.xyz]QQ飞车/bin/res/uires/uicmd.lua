function cmd_gohome()
	GotoUIScreen("startmenu");
end;

function cmd_regist()
	GotoUIScreen("Regist");
end;

function cmd_findplayer()
	Win_ShowWidget("UI.roomsel.MainFrame.FindPlayerDlg",true);
end;

function cmd_showmenu()
	if Win_IsVisible("UI.MainMenu") == false then
		Win_AlignWidget("UI.MainMenu","UI.taskbar",aligntype_winrect,alignstyle_top_out+alignstyle_left_in);
		Win_ShowWidget("UI.MainMenu",true);
	else
		Win_ShowWidget("UI.MainMenu",false);
	end;
end;

function cmd_mutiplay()
	GotoUIScreen("roomsel");
end;

function cmd_gp()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_licencetest()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_timerace()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_playrecord()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_gotogarage()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_gotoshop()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_opentin()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_showfriends()
	Win_ShowMsgBox("对不起，改功能尚未开放");
end;

function cmd_showmyinfo()
	Win_ShowWidget("UI.SysMyInfoDlg",true);
end;

function cmd_set()
	Win_ShowWidget("UI.SysSetDlg", true);
end;

function cmd_goback()
	local cur_screen=Win_GetCurScreen();
	if cur_screen=="startmenu" then
		--GotoUIScreen("entry");
		ExitApp();
	elseif cur_screen=="room" then
		LeaveRoom();
		--GotoUIScreen("roomsel");
	else
		GotoUIScreen("startmenu");
	end;
end;

function cmd_createroom()
	Win_ShowWidget("UI.roomsel.CreateRoomDlg", true);
end;

function cmd_lookat(uin)
	local seatid=UinToSeat(uin);
	local pi=GetRoomPlayerInfo(seatid);
	if pi== nil then
		return;
	end;
	
	UI.children.SysPInfoDlg._uin=pi.uin;
	UI.children.SysPInfoDlg._nickname=pi.playerdbinfo.nickname;
	UI.children.SysPInfoDlg._expn=pi.playerdbinfo.exp;
	UI.children.SysPInfoDlg._money=pi.playerdbinfo.money;
	UI.children.SysPInfoDlg._level=GetPlayerLevelByExp(pi.expn);--pi.level;
	UI.children.SysPInfoDlg._roleid=FindRoleItem(pi.itemnum,pi.items);
	UI.children.SysPInfoDlg._roomview=1;
	Win_ShowWidget("UI.SysPInfoDlg", true);
end;

function cmd_lookatbyfind(msg)
	local pi=msg.pinfo;
	UI.children.SysPInfoDlg._uin=pi.uin;
	UI.children.SysPInfoDlg._nickname=pi.playerdbinfo.nickname;
	UI.children.SysPInfoDlg._expn=pi.playerdbinfo.exp;
	UI.children.SysPInfoDlg._money=pi.playerdbinfo.money;
	UI.children.SysPInfoDlg._level=GetPlayerLevelByExp(pi.expn);--pi.level;
	UI.children.SysPInfoDlg._roomflag=msg.positioninfo.roominfo.flag;
	UI.children.SysPInfoDlg._status=msg.positioninfo.status;
	UI.children.SysPInfoDlg._online=msg.isonline;
	UI.children.SysPInfoDlg._roomid=msg.positioninfo.roominfo.roomid;
	UI.children.SysPInfoDlg._svrid=msg.positioninfo.svrid;
	UI.children.SysPInfoDlg._roleid=FindRoleItem(msg.pinfo.itemnum,msg.pinfo.items);
	UI.children.SysPInfoDlg._roomview=0;
	Win_ShowWidget("UI.SysPInfoDlg", true);	
end;

function cmd_exit()
	Win_ShowWidget("UI.SysExitDlg", true);
end;

function cmd_dostring(str)
	DoString(str);
end;

function cmd_getmapname(mapid)
	return "森林奶酪";
end;

function cmd_mapsel()
	Win_ShowWidget("UI.room.MapSelDlg", true);
end;

function cmd_avatarsel(garagetype)
	UI.children.garage._avatarcat=garagetype;
	Win_ShowWidget("UI.garage", true);
	--UI.children.garage._avatarcat=garagetype;
	--local cat=garagetype-1;
	--Win_SetCheck("UI.garage.TabBar.AvatarSelCat".._UIID_,true);
end;

function Win_SetRectAll(id,x,y,w,h)
	Win_SetRect(id,x,y,w,h);
	Win_SetRect(id,0,0,w,h,value_channel_captionrect);
	Win_SetRect(id,0,0,w,h,value_channel_dragrect);
	Win_SetRect(id,0,0,w,h,value_channel_clientrect);
end;

function FindRoleItem(itemnum,items)
	for i=0,itemnum-1,1 do
		local itemid=items[itemnum-i].itemid;
		if items[itemnum-i].status== 1 then
			if itemid==10000 or itemid==10001 or itemid==10012 then
				return itemid;
			end;
		end;
	end;
	return 10000;
end;
function cmd_shownavigation()
	Win_SetValue("UI.navigation", 0, value_channel_pathmove_pause);	
	Win_SetValue("UI.shortcut_dlg", 0, value_channel_pathmove_pause);	
	--Win_ShowWidget("UI.navigation", true);
	--Win_ConsoleInfo("Call shownavigation");
end;

function cmd_hidenavigation()
	Win_ShowWidget("UI.navigation", false);
	Win_ShowWidget("UI.shortcut_dlg", false);
	Win_SetValue("UI.navigation", 1, value_channel_pathmove_pause);	
	Win_SetValue("UI.shortcut_dlg", 1, value_channel_pathmove_pause);	
end;

function cmd_isnavigationvisible()
	return Win_IsVisible("UI.navigation");
end;

function cmd_showplayerinfo(playeruin, bShow)
	--Win_ConsoleInfo("info: uin = "..playeruin);
	if (bShow==true) then
		--Win_ConsoleInfo("ShowPlayerInfo");
		UI.children.playerinfo_dlg.OnFillPlayerInfo(playeruin);
	end;
	Win_ShowWidget("UI.playerinfo_dlg", bShow);
end;

function cmd_isselfinfovisible()
	return Win_IsVisible("UI.playerinfo_dlg");
end;
function cmd_hideselfInfo()
	Win_ShowWidget("UI.playerinfo_dlg", false);
end;

--createCharacter: C
--carbarn: B (startmenu),   AC, AB
--shop: C		           CA, CB
--hall: A (roomsel)	          BC, BA
--room: C		         CA, CB


camera = 
{
	["Regist"] = "C",
	["entry"] = "A",	
	["startmenu"] = "B",
	["roomsel"] = "A",
	["room"] = "C",
	["score"] = "A",
	["taskscore"] = "A",
	["shop"] = "C",
};

function SetMyCamera(lastscreen, curscreen)
	SetSceneCamera("Camera"..camera[lastscreen]..camera[curscreen]);
end;

function cmd_setselfact()
	local curscreen = GetCurUIScreen();
	local lastscreen= GetLastUIScreen();
	
	--[[
	if (curscreen == "room" or curscreen == "shop") then
		--RotateUISceneDummy("playermode", 25);
		SelfPlayAnimation(1104);
	elseif((lastscreen == "room" or lastscreen == "shop"))then
		--RotateUISceneDummy("playermode", -25);
		SelfPlayAnimation(1103);
	end;]]
	
	local camerapos = camera[curscreen];
	if ( camerapos~= nil) then
		if (camerapos == "B" or camerapos == "A") then
			SelfPlayAnimation(1103);
		elseif(camerapos == "C") then
			SelfPlayAnimation(1104);
		end;
	end;
end;

function cmd_setcamera()
	local curscreen = GetCurUIScreen();
	local lastscreen= GetLastUIScreen();
	
	--[[
	if (curscreen == "room" or curscreen == "shop") then
		--RotateUISceneDummy("playermode", 25);
		SelfPlayAnimation(1104);
	elseif((lastscreen == "room" or lastscreen == "shop"))then
		--RotateUISceneDummy("playermode", -25);
		SelfPlayAnimation(1103);
	end;]]
	
	local camerapos = camera[curscreen];
	if ( camerapos~= nil) then
		if (camerapos == "B" or camerapos == "A") then
			SelfPlayAnimation(1103);
		elseif(camerapos == "C") then
			SelfPlayAnimation(1104);
		end;
	end;
	--Win_ConsoleInfo("lastscreen = "..lastscreen.."  curscreen = "..curscreen);	
	SetMyCamera(lastscreen, curscreen);	
	if (curscreen == "Regist") then
		BindSelfKartToScene("ROOT");
	elseif ((lastscreen == "entry" or lastscreen == "Regist") and curscreen == "startmenu") then		
	--	SetSceneCamera("CameraCA");--curscreen must be "startmenu"
	--elseif(lastscreen == "Regist" and curscreen == "startmenu") then		
		--SetSceneCamera("CameraBA");
		--SetMyCamera(lastscreen, curscreen);
		BindSelfKartToScene("ROOT");		
		BindSelfRoleToScene("playermode");	
	elseif(lastscreen == "score" and curscreen == "room") then				
		Win_SetValue("UI.room", 50, value_channel_timerspace);
		--SetSceneCamera("CameraAC");
		--SetMyCamera(lastscreen, curscreen);		
		--BindSelfKartToScene("ROOT");	
		--BindSelfRoleToScene("playermode");		
	--[[elseif(lastscreen == "startmenu") then	--carbarn A
		--if (curscreen == "shop") then	--shop C
		--	SetSceneCamera("CameraAC");
		--elseif(curscreen == "roomsel") then--
		--	SetSceneCamera("CameraAB");			
		--end;
		SetMyCamera(lastscreen, curscreen);
	elseif(lastscreen == "shop") then -- shop C
		--if (curscreen == "startmenu") then
		--	SetSceneCamera("CameraCA");
		--elseif(curscreen == "roomsel") then
		--	SetSceneCamera("CameraCB");
		--end;
		SetMyCamera(lastscreen, curscreen);
	elseif(lastscreen == "roomsel") then --hall B
		--if (curscreen == "room" or curscreen == "shop") then --C
		--	SetSceneCamera("CameraBC");
		--elseif(curscreen == "startmenu") then	--A
		--	SetSceneCamera("CameraBA");
		--end;
		SetMyCamera(lastscreen, curscreen);
	elseif(lastscreen == "room") then    --C
		if (curscreen == "shop") then  --C
			return;
		--elseif(curscreen == "roomsel") then --B
		--	SetSceneCamera("CameraCB");
		--elseif(curscreen == "startmenu") then --A
		--	SetSceneCamera("CameraCA");
		end;
		SetMyCamera(lastscreen, curscreen);]]	
	end;			
end;