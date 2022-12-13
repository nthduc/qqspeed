function cmd_responsedefault()
	if Param.resultid ~= 0 then
		Win_ShowMsgBox(Param.reason);
	end;
end;

UI.children.AppEventHandle=
	{
		type="WIDGET",
		rect={0,0,0,0},
		visible=false,
		
		--事件通知
		--network
		OnSocketClose=function(entity, code)
			--if entity == eGameServer then
				UI.children.SysLoginMsgbox.OnSocketClose(entity,code);
			--end;
		end;
		
		OnSocketConnect=function(entity, code)
			--if entity == eGameServer then
				UI.children.SysLoginMsgbox.OnSocketConnect(entity, code);
			--end;
		end;
		
		OnSocketCreate=function(entity, code)
			--if entity == eGameServer then
				UI.children.SysLoginMsgbox.OnSocketCreate();
			--end;
		end;
		
		--Login
		OnStartLogin=function(entity, code)
			UI.children.SysLoginMsgbox.OnStartLogin(entity, code);
		end;
		
		OnResponseLogin=function(entity, result)
			UI.children.SysLoginMsgbox.OnResponseLogin(entity, result);
		end;
		
		OnResponseLogin2=function()
			UI.children.SysLoginMsgbox.OnResponseLogin2();
			if Param.logintype==logintype_multi then
				GotoUIScreen("roomsel");
				UI.children.roomsel.OnResponseLogin2();
			end;
		end;
		
		OnResponseRegist=function()
			if Param.resultid==0 then
				cmd_gohome();
				--UI.children.startmenu.SetTutorial(true);
			end;
			cmd_responsedefault();
		end;
		
		OnResponseGetRoomList=function()
			local cur_screen=Win_GetCurScreen();
			if cur_screen=="roomsel" then
				UI.children.roomsel.OnResponseGetRoomList();
			end;
		end;
		
		OnResponseCreateRoom=function()
			if Param.resultid == 0 then
				GotoUIScreen("room");
			end;
			cmd_responsedefault();
		end;
		
		OnResponseEnterRoom=function()
			if Param.resultid == 0 then
				GotoUIScreen("room");
			end;
			cmd_responsedefault();
		end;
		
		OnResponseLeaveRoom=function()
			if Param.resultid == 0 then
				if (UI.children.navigation._IsInRoomPressBtn ~= true) then
					GotoUIScreen("roomsel");
				else
					local UIScreen_NavitationBtn = 
					{
						prefix = "UI.navigation.",
						[0] = 
						{
							btnname = "multiplay_btn",
							uiscreen  = "roomsel",
						},
						[1] = 
						{
							btnname = "shop_btn",
							uiscreen  = "shop",
						},
						[2] =
						{
							btnname = "carbarn_btn",
							uiscreen  = "startmenu",
						},
						[3] =
						{
							btnname = "task_btn",
							uiscreen  ="",
						},
						[4] =
						{
							btnname = "practice_btn",
							uiscreen  = "",
						},
						[5] =
						{
							btnname = "citycenter_btn",
							uiscreen  = "",
						},
						[6] =
						{
							btnname = "challenge_btn",
							uiscreen  = "",
						},
						[7] =
						{
							btnname = "honor_btn",
							uiscreen  = "",
						},
					};
					for i = 0, 7 do
						if (UI.children.navigation._curSelectBtnPath == UIScreen_NavitationBtn.prefix..UIScreen_NavitationBtn[i].btnname) then
							GotoUIScreen(UIScreen_NavitationBtn[i].uiscreen);
							UI.children.navigation._IsInRoomPressBtn = false;
						end;
					end;
					
				end;
			end;
			cmd_responsedefault();
		end;
		
		OnResponseQuickEnterRoom=function()
			if Param.resultid == 0 then
				GotoUIScreen("room");
			end;
			cmd_responsedefault();
		end;
		
		OnResponseChangeSeatState=function()
			if Param.resultid == 0 and Win_GetCurScreen() == "room" then
				UI.children.room.OnResponseChangeSeatState();
			end;
			cmd_responsedefault();
		end;
		
		OnResponseKickFromRoom=function()
			if Param.resultid == 0 and Win_GetCurScreen() == "room" then
				UI.children.room.OnResponseKickFromRoom();
			end;
			cmd_responsedefault();
		end;
		
		OnResponseChangeReadyState=function()
			if Param.resultid == 0 and Win_GetCurScreen() == "room" then
				UI.children.room.OnResponseChangeReadyState();
			end;
			cmd_responsedefault();
		end;
		
		OnResponseStartGame=function()
			Win_EnableWidget("UI.shortcut_dlg.roomAssociated_wgt.ReadyBtn",true);
			cmd_responsedefault();
		end;
		
		OnNotifyEnterRoom=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyEnterRoom();
			end;
		end;
		
		OnNotifyLeaveRoom=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyLeaveRoom();
			end;
		end;
		
		OnNotifyChangeSeatState=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyChangeSeatState();
			end;
		end;
		
		OnNotifyKickFromRoom=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyKickFromRoom();
			end;
		end;
		
		OnNotifyChangeReadyState=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyChangeReadyState();
			end;
		end;
		
		OnResponseTalk=function()
			cmd_responsedefault();
		end;
		
		OnNotifyTalk=function()
			if Win_GetCurScreen() == "room" then
				local txt=Param.nickname.."说:"..Param.content;
				Win_SetText("UI.room.MainFrame.ChatList", txt, value_channel_itemtext, -1);
				TalkFilter(Param.content, Param.uin);
			end;
		end;
		
		OnRequestTalk_P2P=function()
			if Win_GetCurScreen() == "room" then
				local txt=Param.uin..":"..Param.content;
				Win_SetText("UI.room.MainFrame.ChatList", txt, value_channel_itemtext, -1);
				
				for i=0,6,1 do
					local id="UI.room.MainFrame.PlayerListPanel.PlayerList"..i;
					local value=Win_GetValue(id,value_channel_customindex);
					if value==Param.uin then
						local rc = Win_GetRect(id);
						AddBubble(Param.uin,Param.content,rc.x+10,rc.y+5,10000,12,0,4278190080,130);
								
					end;
				end;
				
				TalkFilter(Param.content);
			end;
		end;
		
		OnNotifyKickFromServer=function()
			if Param.uin==GetMyInfo().uin then
				--GotoUIScreen("entry");
				--Win_ShowMsgBox(Param.reason);
				Win_SetText("UI.SysLoginMsgbox.sentence", Param.reason);
				Win_ShowWidget("UI.SysLoginMsgbox", true);	
			end;
		end;
		
		OnNotifyGameBegin = function()
			if Param.pnum > 0 then
				UI.children.room.OnNotifyGameBegin();
				GotoUIScreen("gameui");
			end;
		end;
		
		OnNotifySingleGameBegin = function()
			RemoveSelfKartFromUIScene("ROOT");
			RemoveSelfRoleFromUIScene("playermode");
			ReBindSelfPlayer();
			SelfPlayAnimation(1102);
			GotoUIScreen("gameui");
		end;
		
		OnNotifyGameOver = function()
			GotoUIScreen("room");
		end;
		
		OnNotifyRaceOver=function()
			GotoUIScreen("score");
		end;
		
		OnNotifyChangeMap=function()
			
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyChangeMap();
			end;
		end;
		
		OnResponseChangeMap=function()
			if Param.resultid== 0 and Win_GetCurScreen() == "room" then
				UI.children.room.OnResponseChangeMap();
			end;
			cmd_responsedefault();
		end;
		
		OnNotifyPlayerChangeTeam=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyPlayerChangeTeam();
			end;
		end;
		
		OnResponseChangeTeam=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnResponseChangeTeam();
			end;
			cmd_responsedefault();
		end;
		
		OnNotifyChangeRoomOwner=function()
			if Win_GetCurScreen() == "room" then
				UI.children.room.OnNotifyChangeRoomOwner();
			end;
		end;
		
		OnResponseChangeItemStatus=function()
			local lScn=Win_GetCurScreen();
			--Win_ConsoleInfo("OnResponseChangeItemStatus Scene="..lScn);
			if lScn == "startmenu"  or lScn == "room" then
				UI.children.garage.children.AvatarListPanel.Update();
			elseif lScn == "shop" then
				local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch(realcatid);
			end;
		end;		
		
		OnResponseFindPlayer=function()
			if Param.resultid==0 then
				cmd_lookatbyfind(Param);
			end;
			cmd_responsedefault();
		end;
		
		OnResponseCheckName=function()
			--只管弹出服务器派发的消息
			Param.resultid=1;
			cmd_responsedefault();
		end;
		
		OnResponseAssignAttribute=function()
			ResetPoints(GetMyInfo().uin);
			cmd_responsedefault();
		end;
		
		OnResponseClearAttribute=function()
			ResetPoints(GetMyInfo().uin);
			cmd_responsedefault();
		end;
		
		OnNotifyPingResult=function(loc, pingval)
			UI.children.SysRegionDlg.UpdateData();
		end;
		
		OnResponseBuyCommodity=function()
			cmd_responsedefault();
		end;
		
		OnNotifyUpdateMoney=function()
			local myinfo = GetMyInfo();
			local path = "UI.navigation.miscellaneous_dlg";
			Win_SetText(path..".kubi_label", " "..myinfo.playerdbinfo.money);
		end;
		
		OnBeforeBind=function()
			RemoveSelfKartFromUIScene("ROOT");
			RemoveSelfRoleFromUIScene("playermode");
			ReBindSelfPlayer();
		end;
		
		OnAfterBind=function()
			BindSelfKartToScene("ROOT");
			BindSelfRoleToScene("playermode");
			--SelfPlayAnimation(1104);
			cmd_setselfact();
			SelfShowFixedTurn(15);	--把车子摆到制定角度
			--RecoverSelfTurn(-110);
			Win_ConsoleInfo("SelfShowFixedTurn -110");
		end;
		
		--keyboard event
		OnKeyDown=function(key, otherkey)
			if otherkey == 0 then
				if key >= 65 and key <= 90 or key >= 48 and key <= 57 then
					UI.OnAccel_OnEnter(0,0);
				end;
			end;
		end;
		
	};
