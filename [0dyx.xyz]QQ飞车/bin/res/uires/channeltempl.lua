channel_UIID_=
{
	type = "TABWIN",
	rect={_UIID_ * 77,0,77,35},
	tabrect = {0,0,77,35},
	--tabcover = myproperchannellist[_UIID_].tabcover;
	style=wgtstyle_template,
	groupid = 30,
	groupstop = _UIID_,
	
	OnCheck = function(bCheck)
		Win_ConsoleInfo("check".._UIID_);
		if bCheck==1 then
			Win_SetValue("UI.roomsel.MainFrame.RoomListPanel.channel.channel".._UIID_,500,value_channel_timerspace);
			channellist.curselectedchannel = _UIID_;
		end;
	end;
	
	OnClick=function()
	end;
	
	OnTimer=function()
		--Win_ClearTemplates("UI.roomsel.MainFrame.RoomListPanel");
		LoginGame(logintype_multi,myproperchannellist[_UIID_].id);
	end;
	OnInit = function()
		Win_SetTabCover("UI.roomsel.MainFrame.RoomListPanel.channel.channel".._UIID_, myproperchannellist[_UIID_].tabcover);
	end;
	OnDenit=function()
		LogText("channeldenit");
	end;
};