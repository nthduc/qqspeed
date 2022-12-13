local tutorialintroduce=
{
	[1] = "想知道怎么操作卡丁车吗，那就来这里吧，我们会教你如何开",
	[2] = "想来一圈试下手吗?",
	[3] = "这项任务你还不能进行",
	[4] = "这项任务你已经完成了",
};

local tutorialsteps=
{
	[1] = {bkimag = "res/uires/tutorial/welcome_wgt.gsa",rect = {298, 43, 629, 552},btnimg = "res/uires/tutorial/next_btn.gsa",canskip = false,speechtime = 16000,},
	[2] = {bkimag = "res/uires/tutorial/operation_wgt.gsa",rect = {298, 83, 547, 508},btnimg = "res/uires/tutorial/starttrain_btn.gsa",canskip = false,speechtime = 2800,},
	[3] = {bkimag = "res/uires/tutorial/drift_wgt.gsa",rect = {298, 40, 672, 547},btnimg = "res/uires/tutorial/starttrain_btn.gsa",canskip = false,speechtime = 3920,},
	[4] = {bkimag = "res/uires/tutorial/showtime.gsa",rect = {298, 40, 672, 547},btnimg = "res/uires/tutorial/next_btn.gsa",canskip = false,speechtime = 6000,},
	[5] = {bkimag = "res/uires/tutorial/congratulation_wgt.gsa",rect = {298, 74, 629, 552},btnimg = "res/uires/tutorial/next_btn.gsa",canskip = false,speechtime = 13500,},	
	[6] = {bkimag = "res/uires/tutorial/bye_newbie.gsa",rect = {298, 40, 672, 547},btnimg = "res/uires/tutorial/next_btn.gsa",canskip = false,speechtime = 5200,},
};

local welcomechoice = 1;
local operationchoice = 2;
local driftchoice = 3;
local showtime = 4;
local congratulationchoice = 5;
local bye_newbie = 6;

UI.children.startmenu = 
{
	type = "SCREEN",
	rect = { 0, 0, 1024, 768},	
	bkimage="res/uires/startmenu/carbarn_bk.gsa",
	autoscale=1,
	visible=0,	
	_curStage = 0,
	_bIsNewbie = false,
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.startmenu",true);	
		Win_ShowWidget("UI.startmenu.PlayerShowWgt.tasklistdlg", false);	
		Win_ShowWidget("UI.startmenu.tutorialWgt.flashview_wgt", false);
		Win_ShowWidget("UI.startmenu.tutorialWgt.showtime_flash_wgt",false);
		Win_ShowWidget("UI.startmenu.mutiplaytips", false);
		--Win_ShowWidget("UI.taskbar", true);
		--cmd_shownavigation(0);
		UI.children.startmenu._curStage = 0;
		return 1;			   
	end;	

	OnDenit = function()	
		Win_ShowWidget("UI.startmenu",false);
		Win_ShowWidget("UI.startmenu.PlayerShowWgt.tasklistdlg", false);	
		Win_ShowWidget("UI.startmenu.mutiplaytips", false);
		--Win_ShowWidget("UI.taskbar",false);
		--cmd_shownavigation(1);
		return 1;			   
	end;

	OnSelfShow = function()					
		--Win_ConsoleInfo("SetCurStage = 0");
		Win_SetValue("UI.startmenu",100,value_channel_timerspace);
		Win_ShowWidget("UI.startmenu.PlayerShowWgt.tasklistdlg", false);		
		--Win_ShowWidget("UI.startmenu.PlayerShowWgt", false);
						
		if (IsFinishNewHandTask(1) and IsFinishNewHandTask(2)) then			
			if (UI.children.startmenu.children.tutorialWgt._taskstage ~= 5) then				
				Win_ConsoleInfo("关闭教学面板");
				UI.children.startmenu.SetTutorial(false);
				cmd_shownavigation();
			else
				if( IsFinishNewHandTask(2) ) then																			
					UI.children.startmenu.SetTaskStatus(2, true);	
					UI.children.startmenu.children.tutorialWgt._taskstage = 5; -- 6为退出menu的状态
				end;
			end;
		else
			Win_ConsoleInfo("打开教学面板");
			UI.children.startmenu.SetTutorial(true);			
			if(IsFinishNewHandTask(1)) then
				UI.children.startmenu.SetTaskStatus(1, true);	
			end;					
		end;
		
		if( IsAward()) then
			Win_ConsoleInfo("award");			
			--Win_ShowWidget("UI.startmenu.PlayerShowWgt", false);
			cmd_hidenavigation();
			Win_ShowWidget("UI.startmenu.awardWgt", true);
			Win_ConsoleInfo("award1");			
		end;
		--播放背景音乐
		PlayMusic("Data/Win32/SOUNDS/ui_circle.mp3",1,1);
		
		
	end;
	
	OnSelfHide = function()
		--ReBindSelfPlayer();
		UI.children.startmenu._curStage = 0;
		
		--cmd_shownavigation();			
	end;
	
	OnTimer = function()		
		--Win_ShowWidget("UI.startmenu.PlayerShowWgt", true);
		if (UI.children.startmenu._curStage == 0) then
			--Win_ConsoleInfo("BindPlayer SetCamera");
			--SetSceneCamera("CameraCB");				
			cmd_setcamera();			
			UI.children.startmenu._curStage = 1;
			Win_SetValue("UI.startmenu",1000,value_channel_timerspace);
		else
			Win_ShowWidget("UI.startmenu.PlayerShowWgt.tasklistdlg", true);
			UI.children.startmenu._curStage = 0;
		end;			
	end;
	
	SetTutorial = function(bNewbie)
			_bIsNewbie = bNewbie;
		if (bNewbie) then			
			Win_ShowWidget("UI.startmenu.tutorialWgt", true);
			Win_ShowWidget("UI.startmenu.PlayerShowWgt", false);
			--Win_EnableWidget("UI.navigation", false);
			--Win_EnableWidget("UI.shortcut_dlg", false);
			cmd_hidenavigation();
			Win_SetValue("UI.navigation", 1, value_channel_pathmove_pause);	
			Win_SetValue("UI.shortcut_dlg", 1, value_channel_pathmove_pause);			
			RemoveSelfKartFromUIScene("ROOT");
			RemoveSelfRoleFromUIScene("playermode");
			ReBindSelfPlayer();
			UI.children.startmenu.children.tutorialWgt.children.npc.StartAnimation(1);
			
		else
			Win_ShowWidget("UI.startmenu.tutorialWgt", false);
			Win_ShowWidget("UI.startmenu.PlayerShowWgt", true);
			UI.children.startmenu.children.tutorialWgt.children.npc.StopAnimation();
			cmd_shownavigation();
			BindSelfKartToScene("ROOT");		
			BindSelfRoleToScene("playermode");	
			for i = 913, 918 do
				StopSound(i);		
			end;
			--Win_EnableWidget("UI.navigation", true);
			--Win_EnableWidget("UI.shortcut_dlg", true);
		end;		
	end;

	SetTaskStatus = function(taskindex, bFinishStatus)		
		Win_ConsoleInfo("task "..taskindex.." finished");
				
		if (bFinishStatus) then
			if (1 == taskindex) then
				tutorialsteps[operationchoice].canskip = true;	
				UI.children.startmenu.children.tutorialWgt._taskstage = 3;
			elseif(2 == taskindex) then
				tutorialsteps[driftchoice].canskip = true; 
				UI.children.startmenu.children.tutorialWgt._taskstage = 4;
			end;				
		end;
		
		UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();
	end;
	
	children = 
	{
		tutorialWgt=
		{
			type = "WIDGET",
			rect  = {0, 0, 1024, 768},
			widget_layer = 1,
			bkimage = "res/uires/tutorial/tutorial_bk.gsa",
			--_btask1finish = false,
			--_btask2finish = false,
			_taskstage = 1,			
			visible = 0,
			
			OnInit=function()
				UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();
			end;
			
			OnSelfShow = function()
				PlaySound(913);
			end;			
			
			UpdateTutorialUI = function()				
				local curstage = UI.children.startmenu.children.tutorialWgt._taskstage;
				local path1 = "UI.startmenu.tutorialWgt.welcome_wgt";
				local path2 = "UI.startmenu.tutorialWgt.next_btn";
				--local npcdlgpath = "UI.startmenu.tutorialWgt.npc.npc_dlg";
				Win_ShowWidget("UI.startmenu.tutorialWgt.flashview_wgt", false);
				Win_ShowWidget("UI.startmenu.tutorialWgt.showtime_flash_wgt",false);
				Win_ShowWidget("UI.startmenu.tutorialWgt.Replay_btn", false);
				Win_ShowWidget("UI.startmenu.tutorialWgt.startvadio2_btn", false);
				if (Win_IsVisible("UI.startmenu.tutorialWgt")) then
					if (curstage >=2 and curstage <= 6) then						
						PlaySound(912+curstage);
						for i = 913, 912+curstage-1 do
							StopSound(i);		
						end;					
						if (3 == curstage) then
							--Win_ShowWidget("UI.startmenu.tutorialWgt.drifthint", true);
							--Win_SetValue("UI.startmenu.tutorialWgt", 7500, value_channel_timerspace);
						end;
					end;				
				end;
				if (curstage == driftchoice) then
					Win_ShowWidget("UI.startmenu.tutorialWgt.Replay_btn", true);					
					--Win_ShowWidget("UI.startmenu.tutorialWgt.startvadio2_btn", true);
				end;
				if (curstage == showtime) then
					Win_ShowWidget("UI.startmenu.tutorialWgt.Replay_btn", true);					
					--Win_ShowWidget("UI.startmenu.tutorialWgt.startvadio2_btn", true);
				end;
				if (IsFinishNewHandTask(1) and UI.children.startmenu.children.tutorialWgt._taskstage == 2) then
					tutorialsteps[operationchoice].canskip = true;	
					UI.children.startmenu.children.tutorialWgt._taskstage = 3;
				end;
				Win_ConsoleInfo("CurTaskStage = "..curstage);
				if (curstage > 0 and curstage <= 6) then						
					Win_SetBackPic(path1, tutorialsteps[curstage].bkimag);
					Win_SetBackPic(path2, tutorialsteps[curstage].btnimg);
					local rect = tutorialsteps[curstage].rect;
					Win_SetRectAll(path1, rect[1], rect[2], rect[3], rect[4]);				
				end;
				--[[if (curstage == 5) then
					Win_SetBackPic(path1, tutorialsteps[curstage-1].bkimag);
					Win_SetBackPic(path2, tutorialsteps[curstage-1].btnimg);
					local rect = tutorialsteps[curstage-1].rect;
					Win_SetRectAll(path1, rect[1], rect[2], rect[3], rect[4]);				
				end;]]
				curstage = curstage + 1;
				UI.children.startmenu.children.tutorialWgt._taskstage = curstage;
				local num = table.getn(tutorialsteps);
				--Win_ConsoleInfo("NowTaskStage = "..curstage);
				if(_bIsNewbie) then
					UI.children.startmenu.children.tutorialWgt.children.npc.StartAnimation(1);
				end;
				--[[for i = curstage, num do
					if (tutorialsteps[i].canskip ~= true) then
						UI.children.startmenu.children.tutorialWgt._taskstage = i;
						Win_ConsoleInfo("AfterActionTaskStage = "..i);
						return;							
					end;
				end;	]]
			end;
			
			OnTimer=function()
				--[[local driftintroducestep = UI.children.startmenu.children.tutorialWgt.children.drifthint._driftintroducestep;
				local path = "UI.startmenu.tutorialWgt.drifthint";
				Win_SetText(path, tostring(driftintroducestep-1));
				Win_ConsoleInfo("DriftStep"..(driftintroducestep));
				if (driftintroducestep == 1) then
					Win_SetValue("UI.startmenu.tutorialWgt", 3700, value_channel_timerspace);
				elseif (driftintroducestep == 2) then
					Win_SetValue("UI.startmenu.tutorialWgt", 3700, value_channel_timerspace);
				elseif (driftintroducestep == 3) then
					Win_SetValue("UI.startmenu.tutorialWgt", 2000, value_channel_timerspace);
				else
					--Win_ShowWidget(path, false);
				end;
				
				UI.children.startmenu.children.tutorialWgt.children.drifthint._driftintroducestep = driftintroducestep + 1;]]
			end;
			
			children = 
			{
				welcome_wgt = 
				{
					type = "WIDGET",
					bkimage = "res/uires/tutorial/welcome_wgt.gsa",
					rect = {298, 27, 629, 552},
					enable = 0,
					visible = 1,
					initlayer = 0,	
					updatemute = 1,
				},
				
				Replay_btn = 
				{
					type = "BUTTON",
					rect = {298+553, 27+183, 93, 125},
					--caption = "Replay",
					bkimage = "res/uires/tutorial/play_btn.gsa",
					visible = 0,
					--textsize = 14;
					initlayer = msgbox_layer-1;
					updatemute = 1,
					_bPlay = false;
					
					SetBtnPlayPic = function(bPlay)
						if(bPlay) then
							Win_SetBackPic("UI.startmenu.tutorialWgt.Replay_btn", "res/uires/tutorial/play_btn.gsa");
							UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay = false;
						else 
							Win_SetBackPic("UI.startmenu.tutorialWgt.Replay_btn", "res/uires/tutorial/pause_btn.gsa");
							UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay = true;
						end;
					end;
					
					IsFlashPlay = function()
						return  _bplay;
					end;
					
					OnClick = function()
						local bPlay = UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay;
						if (bPlay == false) then
							Win_SetBackPic("UI.startmenu.tutorialWgt.Replay_btn", "res/uires/tutorial/pause_btn.gsa");
							Win_ConsoleInfo("Convert To pause");
							bPlay = true;
						else
							Win_ConsoleInfo("Convert To start");
							Win_SetBackPic("UI.startmenu.tutorialWgt.Replay_btn", "res/uires/tutorial/play_btn.gsa");
							bPlay = false;
						end;
						local path = "UI.startmenu.tutorialWgt.flashview_wgt";
						local curstage = UI.children.startmenu.children.tutorialWgt._taskstage - 1;
						Win_ConsoleInfo("flash state:"..curstage);
						if (curstage == showtime) then
							path = "UI.startmenu.tutorialWgt.showtime_flash_wgt";
						end;					
						if (Win_IsVisible(path)) then
							local resid = Win_GetCustomBuffer(path);
							Win_SetValue("UI.startmenu.tutorialWgt", -1, value_channel_timerspace);
							--Win_ShowWidget("UI.startmenu.tutorialWgt.drifthint", false);
							if (resid ~= nil) then	
								--if (bPlay) then
								--	RewindFlash(resid);
								--	PlayFlash(resid);
								--	SetFlashLoop(resid, false);
								--else
									PauseFlash(resid);
								--end;
							--Win_ShowWidget(path..".Replay_btn", false);
							--Win_ShowWidget(path..".Pause_btn", true);
							end;
						else
							Win_ShowWidget(path, true);
							RewindFlash(resid);
							PlayFlash(resid);
							SetFlashLoop(resid, false);
						end;
						
						UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay = bPlay;
					end;
				};
				drifthint=
				{
					type = "PICLABEL",
					rect = {631, 304, 672, 547},
					fontpic = "res/uires/tutorial/drifthint.gsa",
					_driftintroducestep = 1,
					visible = 0,
					updatemute = 1,
				};
				flashview_wgt=
				{
					type = "WIDGET",
					rect = {360, 184, 476, 357},
					style=wgtstyle_renderbuffer;
					visible = 0,
					updatemute = 1,
					custombuffer = 2,
					buffervalue =
					{
						banimation = 1,
						cycletype = 2, 
						restype = 2, 
						resname="scene/tutorial.swf",			
					},
					
					OnSelfShow = function()
						local resid = Win_GetCustomBuffer("UI.startmenu.tutorialWgt.flashview_wgt");
						if (resid ~= nil) then
							StopFlash(resid);							
						end;
					end;
					
					children = 
					{												
						--[[Pause_btn = 
						{
							type = "BUTTON",
							rect = {491, 212, 93, 125},
							bkimage = "res/uires/tutorial/pause_btn.gsa",
							--visible = 0,
							
							OnClick = function()
								local path = "UI.startmenu.tutorialWgt.flashview_wgt";
								local resid = Win_GetCustomBuffer(path);
								if (resid == nil) then
									return;
								end;								
								PauseFlash(resid);	
								--Win_ShowWidget(path..".Replay_btn", true);
								--Win_ShowWidget(path..".Pause_btn", false);								
							end;
						};]]
						
						--[[Forward_btn = 
						{
							type = "BUTTON",
							rect = {550, 400, 40, 20},
							caption = "forward",
							textsize = 14;
							
							OnClick = function()
								local resid = Win_GetCustomBuffer("UI.startmenu.tutorialWgt.flashview_wgt");
								if (resid ~= nil) then
									ForwardFlash(resid);
								end;
							end;
						};]]
					};
				},
				
				showtime_flash_wgt=
				{
					type = "WIDGET",
					rect = {360, 184, 476, 357},
					style=wgtstyle_renderbuffer;
					visible = 0,
					updatemute = 1,
					custombuffer = 2,
					buffervalue =
					{
						banimation = 1,
						cycletype = 2, 
						restype = 2, 
						resname="scene/drift_xiaopen.swf",			
					},
					
					OnSelfShow = function()
						local resid = Win_GetCustomBuffer("UI.startmenu.tutorialWgt.showtime_flash_wgt");
						if (resid ~= nil) then
							StopFlash(resid);							
						end;
					end;
					
					children = 
					{												
					};
				},
				
				npc = 
				{
					type = "DIALOG",
					rect = {0, 0, 239, 678},
					aligntype=aligntype_father+aligntype_winrect,
					alignstyle=alignstyle_bottom_in+alignstyle_left_in,	
					bkimage = "res/uires/tutorial/npc.gsa",
					initlayer = 100, 
					
					OnInit = function()
						--Win_ShowWidget("UI.startmenu.tutorialWgt.npc.npc_dlg", false);
					end;
					
					StartAnimation = function(startIndex)
						Win_ConsoleInfo("StartAnimation");
						local facepath = "UI.startmenu.tutorialWgt.npc.face_label";
						UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num = startIndex;
						UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._step = 0;
						local num = UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num;
						Win_SetText(facepath , GetCharSign(num));
						Win_SetValue(facepath, 180, value_channel_timerspace);
					end;
					
					StopAnimation = function()
						UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._step = 1;
						UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num = 1;
						local facepath = "UI.startmenu.tutorialWgt.npc.face_label";
						Win_SetValue(facepath, -1, value_channel_timerspace);
						Win_SetText(facepath , GetCharSign(0));
					end;
					
					--[[OnClick = function()
								local num = UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num;								
															
								Win_SetText("UI.startmenu.tutorialWgt.npc.face_label", GetCharSign(num));
								Win_ConsoleInfo(GetCharSign(num));
								num = math.mod(num + 1, 18);
								UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num = num;
							end;
					]]
					children = 
					{
						face_label = 
						{
							type="PICLABEL";
							--type = "LABEL",
							rect = {166, 138, 93, 84},							
							fontpic = "res/uires/tutorial/face.gsa",
							_step = 1;
							_speechtime = 0;
							_num = 1;
							--bkimage = "res/uires/tutorial/x.gsa",
							
							OnInit = function()
								local facepath = "UI.startmenu.tutorialWgt.npc.face_label";
								local num = UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num;
								Win_SetText(facepath , GetCharSign(num));
								Win_SetValue(facepath, 180, value_channel_timerspace);
								_speechtime = 0;
							end;
							
							OnTimer = function()							
								if (UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._step ~= 0) then
									return;
								end;	
								
								local facepath = "UI.startmenu.tutorialWgt.npc.face_label";
								local num = UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num;								
								Win_SetText(facepath , GetCharSign(num+1));								
								num = math.mod(num + 1, 18);	
								
								UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num = num;
								local curstage = UI.children.startmenu.children.tutorialWgt._taskstage - 1;
								local curspeechtime = UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._speechtime;
								if(curspeechtime > tutorialsteps[curstage].speechtime) then
									UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._step = 1;
									UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._speechtime = 0;
									UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._num = 1;
									Win_SetText(facepath , GetCharSign(1));
									
									if(curstage == driftchoice)then
										local bPlay = UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay;
										if( bPlay == false) then
											local path = "UI.startmenu.tutorialWgt.flashview_wgt";
											local resid = Win_GetCustomBuffer(path);
											Win_ShowWidget(path, true);
											RewindFlash(resid);
											PlayFlash(resid);
											SetFlashLoop(resid, false);
											UI.children.startmenu.children.tutorialWgt.children.Replay_btn.SetBtnPlayPic(false);
										end;
									end;
									
									if(curstage == showtime)then
										local bPlay = UI.children.startmenu.children.tutorialWgt.children.Replay_btn._bPlay;
										if( bPlay == false) then
											local path = "UI.startmenu.tutorialWgt.showtime_flash_wgt";
											local resid = Win_GetCustomBuffer(path);
											Win_ShowWidget(path, true);
											RewindFlash(resid);
											PlayFlash(resid);
											SetFlashLoop(resid, false);
											UI.children.startmenu.children.tutorialWgt.children.Replay_btn.SetBtnPlayPic(false);
										end;
									end;
								else
									curspeechtime = curspeechtime + 180;
									UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._speechtime = curspeechtime;
								end;
								
								Win_SetValue(facepath, 180, value_channel_timerspace);
							end;
						},
						
						--[[
						npc_dlg=
						{
							type = "DIALOG",
							rect = {300, 18, 246, 245},
							captionrect = {18, 25, 210, 200},
							caption = "I Hit This!!!",
							bkimage = "res/uires/tutorial/npc_dlg.gsa",
							visible = 0,
							drawcolor = {0, 0, 0, 255},
							textstyle = dt_magicback + dt_magicedge + dt_vcenter,
						},]]
					},
				},		
					
				startvadio2_btn=
				{
					type = "BUTTON",
					rect = {450, 586, 250, 170},
					bkimage = "res/uires/tutorial/playvadio_btn.gsa",
					visible = 0,
					updatemute = 1,
					
					OnClick = function()
					end;
				},
				
				next_btn=
				{
					type = "BUTTON",
					rect = {817, 636, 149, 69},
					bkimage = "res/uires/tutorial/next_btn.gsa",
					updatemute = 1,
					
					OnClick = function()
						for i = 913, 918 do
							StopSound(i);		
						end;
						UI.children.startmenu.children.tutorialWgt.children.npc.children.face_label._speechtime = 0;
						local curstage = UI.children.startmenu.children.tutorialWgt._taskstage;
						if (curstage == (operationchoice+1)) then
							local uin = GetMyInfo().uin;
							RemoveSelfKartFromUIScene("ROOT");
							RemoveSelfRoleFromUIScene("playermode");
							ReBindSelfPlayer();							
							RequestStartTaskSingleTask(uin, 1);	
						elseif (curstage == (driftchoice+1)) then							
							local uin = GetMyInfo().uin;
							RemoveSelfKartFromUIScene("ROOT");
							RemoveSelfRoleFromUIScene("playermode");
							ReBindSelfPlayer();
							Win_ShowWidget("UI.startmenu.tutorialWgt.flashview_wgt", false);							
							local resid = Win_GetCustomBuffer("UI.startmenu.tutorialWgt.flashview_wgt");
							if (resid ~= 0) then	
								--Win_ConsoleInfo("stop flash"..resid);
								RemoveFlash(resid);							
							end;
							UI.children.startmenu.children.tutorialWgt.children.Replay_btn.SetBtnPlayPic(true);
							RequestStartTaskSingleTask(uin, 2);
						elseif(curstage == 5) then
							Win_ShowWidget("UI.startmenu.tutorialWgt.showtime_flash_wgt",false);
							local resid = Win_GetCustomBuffer("UI.startmenu.tutorialWgt.showtime_flash_wgt");
							if (resid ~= 0) then								
								RemoveFlash(resid);							
							end;
							UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();
						elseif( curstage == 7) then
							UI.children.startmenu.SetTutorial(false);
							Win_ShowWidget("UI.startmenu.mutiplaytips", true);
						else							
							UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();
						end;						
					end;
				},
				
				quit_btn=
				{
					type = "BUTTON";
					rect = {983, 3, 20, 20};
					bkimage = "res/uires/room/closebtn.gsa",
					OnClick = function()
						ExitApp();
					end;
				},
				
				--[[task1_btn=
				{
					type = "BUTTON",
					rect = {558, 143, 341, 116},
					bkimage = "res/uires/tutorial/task1_btn.gsa",
					
					OnClick = function()
						--[[if (UI.children.startmenu.children.tutorialWgt._btask1finish == true) then
							Win_SetText("UI.startmenu.tutorialWgt.npc.npc_dlg", tutorialintroduce[4]);
							return;
						end;]]
						
						local uin = GetMyInfo().uin;
						RequestStartTaskSingleTask(uin, 1, 109);
					end;
					
					
					OnMouseIn = function()
						Win_SetText("UI.startmenu.tutorialWgt.npc.npc_dlg", tutorialintroduce[1]);
						Win_ShowWidget("UI.startmenu.tutorialWgt.npc.npc_dlg", true);		
					end;
					
					OnMouseOut = function()
						Win_ShowWidget("UI.startmenu.tutorialWgt.npc.npc_dlg", false);
					end;
					
					children = 
					{
						taskfinish_label=
						{	
							type = "LABEL",
							rect  = {-61, -51, 123, 112},
							bkimage = "res/uires/tutorial/taskfinish.gsa",
							enable = 0,	
							visible = 0,
						},
					},
				},
				
				task2_btn=
				{
					type = "BUTTON",
					rect = {558, 143+97, 341, 116},
					bkimage = "res/uires/tutorial/task2_btn.gsa",
					--enable = 0,
					
					OnClick = function()
						if (UI.children.startmenu.children.tutorialWgt._btask1finish == false) then
							Win_SetText("UI.startmenu.tutorialWgt.npc.npc_dlg", tutorialintroduce[3]);
							return;
						elseif(UI.children.startmenu.children.tutorialWgt._btask2finish == true) then
							Win_SetText("UI.startmenu.tutorialWgt.npc.npc_dlg", tutorialintroduce[4]);
							return;
						end;
						
						local uin = GetMyInfo().uin;
						RequestStartTaskSingleTask(uin, 2, 110);						
						
					end;
					
					OnMouseIn = function()
						Win_SetText("UI.startmenu.tutorialWgt.npc.npc_dlg", tutorialintroduce[2]);
						Win_ShowWidget("UI.startmenu.tutorialWgt.npc.npc_dlg", true);		
					end;
					
					OnMouseOut = function()
						Win_ShowWidget("UI.startmenu.tutorialWgt.npc.npc_dlg", false);
					end;
					
					children = 
					{
						taskfinish_label=
						{	
							type = "LABEL",
							rect  = {-61, -51, 123, 112},
							bkimage = "res/uires/tutorial/taskfinish.gsa",
							enable = 0,
							visible = 0,
						},
					},
				},]]
				--[[nexttime_btn=
				{
					type = "BUTTON",
					rect = {558, 143+97*2, 341, 116},
					bkimage = "res/uires/tutorial/nexttime_btn.gsa",
					
					OnClick = function()
						ExitApp();
						--cmd_exit();
					end;
				},]]
				
			},
		},
		PlayerShowWgt = 
		{
			type = "WIDGET",
			rect = {0, 0, 1024, 768},
			style = wgtstyle_renderitem,
			widget_layer = 2,
			hosttype=4,
			hostindex=111,
			visible = 0;
			updatemute = 1,
			
			OnSelfHide = function()
				
			end;
			
			children = 
			{
				tipinfodlg =
				{
					type = "LABEL",
					rect = {316, 116, 400, 16},
					textsize=12,
					textfont = 1,
					drawcolor = {255, 0, 0, 255},
					
					OnInit = function()
						local num = table.getn(tiptext);
						math.randomseed(GetTickCount());
						local pos = math.random(1, num);
						Win_SetText("UI.startmenu.PlayerShowWgt.tipinfodlg", "提示: "..tiptext[pos]);
					end;
				},
				tasklistdlg = 
				{
					type = "DIALOG",
					rect = {700, 150, 174, 185},
					drawcolor = {255, 0, 0, 255},
					captionrect = {0, 3, 140, 16},
					widget_layer = focus_layer_plus+1,
					--caption = "今日任务",			
					moveable = 1,
					visible = 0,
					bkimage = "res/uires/task/taskdlg_bk.gsa",
					
					OnInit = function()
						UI.children.startmenu.children.PlayerShowWgt.children.tasklistdlg.FillTaskList(5);
					end;
					
					
					
					FillTaskList = function(taskcount)
						local path = "UI.startmenu.PlayerShowWgt.tasklistdlg.listbox";
						Win_SetValue(path, taskcount, value_channel_item_num);
						
						for i = 1, taskcount do
							Win_SetText(path, "任务"..i,value_channel_itemtext, i - 1);
						end;
						
					end;
					
					children = 
					{		
						
						listbox = 
						{
							type="LISTBOX",
							rect={12,40,150,130},				
							--style=wgtstyle_clipself,
							extendstyle=listbox_style_itemheight_fix+listbox_style_wheel+listbox_style_richtext + scrollbar_auto_hide,
							richmode=ui_richmode_color,
							textsize=12,
							textfont = 1;
							ysizeable=1,														
							itemheight=26,
							itemmarginv=0,
							itemmarginh=0,
							itemdrawcolor = {255, 255, 255, 255},
							--itemfocuscolor = {255, 255, 255, 255},	
							itemselcolor    = {0, 0, 0, 255},
							cursor="Click",																	
							--itemselback="res/uires/practice/itemunsel.gsa",
							itemfocuspos={0,0,0,-1},
							itemtextstyle = dt_left + dt_vcenter + dt_center;		
							bkcolor= { 26, 27, 22, 196 },	
		
							OnClick = function()								
								SlectedSubQsType = Win_GetValue("UI.startmenu.PlayerShowWgt.tasklistdlg.listbox",value_channel_hot);
								--local status = GetChoiceStatus(SlectedMainQsType, SlectedSubQsType);
								
								if (status ~= nil and status.State == 2) then
									--
								end;
								--GetPracticeChoice(SlectedMainQsType);
							end;		
							
		
							children = 
							{
								vscroll = 
								{
									type="SCROLLBAR",
									rect={130,10,12,170},
									style=wgtstyle_vertical,					
									bkcolor={0,0,0,255},
									edgecolor={128,128,128,255},						
									pagesize=8,
									marginh=3,
									marginv=0,
									alignstyle=alignstyle_right_out+alignstyle_vcenter,	
									--alignstyle=alignstyle_vcenter,	
									extendstyle=scrollbar_auto_hide,
									drawflag=drawflag_win_fill,
									
									children=
									{
										blockbtn=
										{
											type="BUTTON",
											rect={0,0,22,50},
											bkimage="res/uires/practice/scroll_middle.gsa",
											bkimagepos={0,0},
											moveable=1,
											aligntype=aligntype_father+aligntype_clientrect,
											alignstyle=alignstyle_top_in+alignstyle_hcenter,	
											
										};--blockbtn
										
										
										spinup=
										{
											type="SPIN",
											rect={0,0,22,21},
											bkimage="res/uires/practice/scroll_up.gsa",
											style=wgtstyle_primary,
											textstyle=1,		
											aligntype=aligntype_father+aligntype_winrect,
											alignstyle=alignstyle_top_out+alignstyle_hcenter,	
											marginh=0,
											marginv=0,							
											
										};--spinup
										
										spindown=
										{
											type="SPIN",
											rect={0,0,22,21},
											bkimage="res/uires/practice/scroll_down.gsa",	
											aligntype=aligntype_father+aligntype_winrect,
											alignstyle=alignstyle_bottom_out+alignstyle_hcenter,	
											marginh=0,
											marginv=0,
																	
										};--spindown
										
									};--children
								
								};--scroll
							},
						},									
					},
				},
			},
		},
		
		awardWgt = 
		{			
			type = "WIDGET",
			rect  = {0, 0, 1024, 768},
			widget_layer = 1,
			bkimage = "res/uires/tutorial/tutorial_bk.gsa",			
			visible = 0,
			children = 
			{
				npc = 
				{
					type = "DIALOG",
					rect = {0, 0, 239, 678},
					aligntype=aligntype_father+aligntype_winrect,
					alignstyle=alignstyle_bottom_in+alignstyle_left_in,	
					bkimage = "res/uires/tutorial/npc.gsa",
					initlayer = 100, 					
				},
				dialog_wgt = 
				{
					type = "WIDGET",
					bkimage = "res/uires/tutorial/award_l5.gsa",
					rect = {298, 27, 629, 552},
					enable = 0,
					visible = 1,
					initlayer = 0,	
					updatemute = 1,
				},
				next_btn=
				{
					type = "BUTTON",
					rect = {817, 636, 149, 69},
					bkimage = "res/uires/tutorial/next_btn.gsa",
					updatemute = 1,					
					OnClick = function()
						Win_ShowWidget("UI.startmenu.awardWgt", false);
						cmd_shownavigation();
						FinishAward();
					end;
				},
			},			
		},
		
		mutiplaytips =
		{
			type = "LABEL",
			rect  = {600, 370, 348, 225},
			bkimage = "res/uires/tutorial/entermulplay_hint.gsa",
			--initlayer = msgbox_layer - 1;
			enable = 0,
			visible = 0,
		},
		--[[splasyWgt=
		{
			type = "WIDGET",
			rect = {0, 0, 1024, 768},
			initlayer = msgbox_layer;
			alphaspeed=0.25,
			alphafactor=0.0,
			visible = 1,			
			
			bkimage="res/uires/entry/bk.gsa",
			
			accel=
			{
				{"UI.startmenu.splasyWgt.OnEscape",27,0,1},--ctrl+v粘贴
			};
			
			OnEscape = function()
				local path = "UI.startmenu.splasyWgt";
				Win_SetValue(path, -1, value_channel_timerspace);
				Win_ShowWidget(path, false);
			end;
			
			OnSelfShow = function()
				local path = "UI.startmenu.splasyWgt";
				Win_SetValue(path, 3000, value_channel_timerspace);
			end;
			
			OnClick = function()
				local path = "UI.startmenu.splasyWgt";
				Win_SetValue(path, -1, value_channel_timerspace);
				Win_ShowWidget(path, false);
			end;
			
			OnTimer = function()
				local path = "UI.startmenu.splasyWgt";
				Win_ShowWidget(path, false);
			end;
		},]]
	},
	
	--[[children=
	{
		MainPanel=
		{
			type="MESSAGEBOX",
			rect={0,0,920,710},
			visible=true,
			moveable=0,
			
			children=
			{
				Toolbar=
				{
					type="DIALOG",
					rect={10,0,900,90},
					aligntype=aligntype_father+aligntype_winrect,
					alignstyle=alignstyle_bottom_in,
					
					children=
					{
						mutiplaybtn=
						{
							type="BUTTON",
							rect={0,0,90,90},
							bkimage="res/uires/startmenu/mutiplaybtn.gsa",
							bkimagepos={-5,-10},
					
							OnClick=function()
								cmd_mutiplay();
							end;
						};
						returnbtn=
						{
							type="BUTTON",
							rect={805,0,90,90},
							bkimage="res/uires/startmenu/returnbtn.gsa",
							bkimagepos={-5,-10},
					
							OnClick=function()
								cmd_goback();
							end;
						};
						
						licencetestbtn=
						{
							type="BUTTON",
							rect={149,45,50,70},
							bkimage="res/uires/startmenu/licencetestbtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							caption="驾照考试",
							
							OnClick=function()
								cmd_licencetest();
							end;
						};
						timeracebtn=
						{
							type="BUTTON",
							rect={222,45,50,70},
							bkimage="res/uires/startmenu/timeracebtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							caption="计时赛",
							
							OnClick=function()
								cmd_timerace();
							end;
						};
						playrecordbtn=
						{
							type="BUTTON",
							rect={296,45,50,70},
							bkimage="res/uires/startmenu/playrecordbtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							caption="回放",
							
							OnClick=function()
								cmd_playrecord();
							end;
						};
						gpbtn=
						{
							type="BUTTON",
							rect={368,45,50,70},
							bkimage="res/uires/startmenu/gpbtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							enable=0,
							
							OnClick=function()
								cmd_gp();
							end;
						};
						garagebtn=
						{
							type="BUTTON",
							rect={441,45,50,70},
							bkimage="res/uires/startmenu/garagebtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							caption="车库",
							
							OnClick=function()
								cmd_gotogarage();
							end;
							
						};
						shopbtn=
						{
							type="BUTTON",
							rect={514,45,50,70},
							bkimage="res/uires/startmenu/shopbtn.gsa",
							captionrect={0,50,50,20},
							textsize=12,
							drawcolor={0,0,0,255},
							caption="商店",
							
							OnClick=function()
								cmd_gotoshop();
							end;
						};
					};
				};
				--[[Boardbar=
				{
					type="DIALOG",
					rect={0,139,650,387},
					
					children=
					{
						ad=
						{
							type="WIDGET",
							rect={5,5,640,377},
							--bkimage="res/uires/ad/ad.gsa",
							autoscale=1,
							style=wgtstyle_clipself,
						};
					};
					
				};
				]]
			};
		};
	};]]
};
	
	