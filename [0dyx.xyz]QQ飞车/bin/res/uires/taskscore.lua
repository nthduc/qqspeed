tutorialfinish=
{
	[1] = "恭喜完成操作练习，下面进行漂移练习",
	[2] = "恭喜你完成了漂移练习",
	[3] = "失败乃成功之母，再来！！"
};


UI.children.taskscore=
{
 	type = "SCREEN",
	rect = {0,0,1024,768},
	visible = 0,
	updatemute = 1,
	
	OnInit = function()
	     Win_ShowWidget("UI.taskscore", true);
	end;
	
	OnSelfShow = function()
		if( IsCurTaskFinish() ) then
			if( not IsFinishNewHandTask(1) ) then					
				Win_SetText("UI.taskscore.score",  tutorialfinish[1]);
			elseif ( IsFinishNewHandTask(1) ) then
				Win_SetText("UI.taskscore.score",  tutorialfinish[2]);
			end;
		else
			Win_SetText("UI.taskscore.score",  tutorialfinish[3]);
		end;
	end;
	
	children =
	{
		score =
		{
			type="MESSAGEBOX",
			rect = { 0, 0, 339, 230},
			bkimage="res/uires/common/msgbox.gsa",			
			visible = 1,
			--moveable = 1,
			--widget_layer = msgbox_layer-1;
			drawflag = drawflag_backprior,
			custombuffer = 1,
			buffervalue =
			{
				banimation = 1,
				cycletype = 2, 
				restype = 1, 
				resname="scene/splash.nif",			
			},
			
			OnSelfShow = function()							
			end;

			children =
			{
			    okbtn=
				{
					type="BUTTON",
					rect={121,220,90,40},
					bkimage="res/uires/common/okbtn.gsa",
					--defaultok=1,
					--soundid={1005},

					OnClick = function()
						--Win_ShowWidget("UI.taskscore", false);
						LeaveTask();						
						
						if (IsFinishNewHandTask(1)) then
							GotoUIScreen("startmenu");
						--[[else
							if( IsFinishNewHandTask(2) ) then		
								UI.children.startmenu.children.tutorialWgt._taskstage = 4;
								UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();					
							else
								UI.children.startmenu.children.tutorialWgt._taskstage = 3;
								UI.children.startmenu.children.tutorialWgt.UpdateTutorialUI();
							end;
							RemoveSelfKartFromUIScene("ROOT");
							RemoveSelfRoleFromUIScene("playermode");
							ReBindSelfPlayer();]]
						end;
						
						
		     				--[[GotoUIScreen("startmenu");
						UI.children.startmenu.SetTaskStatus(1, true);						
						UI.children.startmenu.SetTutorial(true);
						Win_ShowMsgBox("恭喜完成新手特训（大家鼓掌）, 你可以进行多人游戏了！！");]]
					end;
				},
			},
			},
	}
};