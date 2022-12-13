
UI.children.gameui = 
{
	type = "SCREEN",
	rect = {0,0,1024,768},	
	visible=0,	
	updatemute = 1,
	
	OnInit = function()	
		cmd_hideselfInfo();
		cmd_hidenavigation();
		Win_ShowWidget("UI.gameui",true);
			TeamColorsTexMap=
			{
				[0]="Colors/红.tga";
				[1]="Colors/蓝.tga";
			};
			SingleColorsTexMap=
			{
				[0]="Colors/红.tga";
				[40]="Colors/黄.tga";
				[200]="Colors/蓝.tga";
			};
			GPIconTexMap=
			{
				--[0]="";
				[10]="textures/shield1.tga";
				[2]="textures/kubi1.tga";
				[9]="textures/fog1.tga";
				[3]="textures/missile1.tga";
				[1]="textures/n2o1.tga";
				[7]="textures/waterbomb1.tga";
				[4]="textures/tortoise1.tga";
				[6]="textures/devil1.tga";
				[5]="textures/magnet1.tga";
				[8]="textures/banana1.tga";
				[2001]="textures/n2o1.tga";
				[2002]="textures/n2o1.tga";
				[11]="textures/waterbomb1.tga";
				[12]="textures/shield1.tga";
				[13]="textures/shield1.tga";
			};
			RankBkMap=
			{
				[0]="textures/playrankitem.tga";
				[1]="textures/hostrankitem.tga";
			};
		table.foreach(TeamColorsTexMap,function(k,v) TeamColorsTexMap[k]=TexAddRef(v) end);
		table.foreach(SingleColorsTexMap,function(k,v) SingleColorsTexMap[k]=TexAddRef(v) end);
		table.foreach(GPIconTexMap,function(k,v) GPIconTexMap[k]=TexAddRef(v) end);
		table.foreach(RankBkMap,function(k,v) RankBkMap[k]=TexAddRef(v) end);
		return 1;			   
	end;	
	
	OnDenit = function()	
		Win_ShowWidget("UI.gameui",false);
		table.foreach(TeamColorsTexMap,function(k,v) TexRelease(v) end);
		table.foreach(SingleColorsTexMap,function(k,v) TexRelease(v) end);
		table.foreach(GPIconTexMap,function(k,v) TexRelease(v) end);
		table.foreach(RankBkMap,function(k,v) TexRelease(v) end);
		return 1;			   
	end;
	
	OnSelfShow = function()
		cmd_hidenavigation();
		ShowPointer(false);
	end;
	
	OnSelfHide = function()
		ShowPointer(true);
	end;
	
	children=
	{
		TimePanel=
		{
			type="WIDGET";
			rect={792,10,300,200};
			bkimage="res/uires/gameui/计时区.gsa";
			updatemute = 1,
			
			OnInit=function()
				Win_SetValue("UI.gameui.TimePanel",100,value_channel_timerspace);
				
				if not IsFinishNewHandTask(1) or not IsFinishNewHandTask(2) then
					Win_ShowWidget("UI.gameui.TimePanel",false);
				else 
					Win_ShowWidget("UI.gameui.TimePanel",true);
				end;
			end;
			
			OnTimer=function()
				--Win_ConsoleInfo("update gamerank");
				local hosttime,besttime=GetRaceTime();
				if (hosttime == nil or besttime == nil ) then
					Win_SetValue("UI.gameui.TimePanel",100,value_channel_timerspace);
					return;
				end;
				--自己的时间
				local hour=math.floor(hosttime/6000);
				local minute=math.floor((hosttime-hour*6000)/100);
				local second=math.floor(hosttime-hour*6000-minute*100);
				local h1=math.floor(hour/10);
				local h2=hour-h1*10;
				local m1=math.floor(minute/10);
				local m2=minute-m1*10;
				local s1=math.floor(second/10);
				local s2=second-s1*10;
				
				--Win_ConsoleInfo(tostring(h1)..":"..tostring(h2)..":"..tostring(m1)..":"..tostring(m2)..":"..tostring(s1)..":"..tostring(s2));
				Win_SetText("UI.gameui.TimePanel.HostTime.hour1",tostring(h1));
				Win_SetText("UI.gameui.TimePanel.HostTime.hour2",tostring(h2));
				Win_SetText("UI.gameui.TimePanel.HostTime.minute1",tostring(m1));
				Win_SetText("UI.gameui.TimePanel.HostTime.minute2",tostring(m2));
				Win_SetText("UI.gameui.TimePanel.HostTime.second1",tostring(s1));
				Win_SetText("UI.gameui.TimePanel.HostTime.second2",tostring(s2));
				
				--最佳时间
				local hour=math.floor(besttime/6000);
				local minute=math.floor((besttime-hour*6000)/100);
				local second=math.floor(besttime-hour*6000-minute*100);
				local h1=math.floor(hour/10);
				local h2=hour-h1*10;
				local m1=math.floor(minute/10);
				local m2=minute-m1*10;
				local s1=math.floor(second/10);
				local s2=second-s1*10;
				
				--Win_ConsoleInfo(tostring(h1)..":"..tostring(h2)..":"..tostring(m1)..":"..tostring(m2)..":"..tostring(s1)..":"..tostring(s2));
				Win_SetText("UI.gameui.TimePanel.BestTime.hour1",tostring(h1));
				Win_SetText("UI.gameui.TimePanel.BestTime.hour2",tostring(h2));
				Win_SetText("UI.gameui.TimePanel.BestTime.minute1",tostring(m1));
				Win_SetText("UI.gameui.TimePanel.BestTime.minute2",tostring(m2));
				Win_SetText("UI.gameui.TimePanel.BestTime.second1",tostring(s1));
				Win_SetText("UI.gameui.TimePanel.BestTime.second2",tostring(s2));

				--圈数
				local laps,totallaps=GetLaps();
				Win_SetText("UI.gameui.TimePanel.Laps.HostLaps",tostring(laps));
				Win_SetText("UI.gameui.TimePanel.Laps.TotalLaps",tostring(totallaps));
				
				Win_SetValue("UI.gameui.TimePanel",100,value_channel_timerspace);
			end;
			
			children=
			{
				Laps=
				{
					type="WIDGET";
					rect={93,92,100,40};
					updatemute = 1,
					
					children=
					{
						HostLaps=
						{
							type="PICLABEL";
							rect={-11,0,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
						Slash=
						{
							type="LABEL";
							rect={2,-13,32,31};
							bkimage="res/uires/gameui/杠.gsa";
							updatemute = 1,
						};
						TotalLaps=
						{
							type="PICLABEL";
							rect={64,0,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
					};
				};
				HostTime=
				{
					type="WIDGET";
					rect={57,3,200,40};
					updatemute = 1,
					
					children=
					{
						hour1=
						{
							type="PICLABEL";
							rect={-1,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						hour2=
						{
							type="PICLABEL";
							rect={24,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						dot1=
						{
							type="LABEL";
							rect={33,-3,20,31};
							bkimage="res/uires/gameui/timedot.gsa";
							updatemute = 1,
						};
						minute1=
						{
							type="PICLABEL";
							rect={64,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						minute2=
						{
							type="PICLABEL";
							rect={89,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						dot2=
						{
							type="LABEL";
							rect={98,-3,20,31};
							bkimage="res/uires/gameui/timedot.gsa";
							updatemute = 1,
						};
						second1=
						{
							type="PICLABEL";
							rect={129,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						second2=
						{
							type="PICLABEL";
							rect={154,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
					};
				};
				BestTime=
				{
					type="WIDGET";
					rect={57,39,200,40};
					updatemute = 1,
					
					children=
					{
						hour1=
						{
							type="PICLABEL";
							rect={-1,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						hour2=
						{
							type="PICLABEL";
							rect={24,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						dot1=
						{
							type="LABEL";
							rect={33,-3,20,31};
							bkimage="res/uires/gameui/timedot.gsa";
							updatemute = 1,
						};
						minute1=
						{
							type="PICLABEL";
							rect={64,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						minute2=
						{
							type="PICLABEL";
							rect={89,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						dot2=
						{
							type="LABEL";
							rect={98,-3,20,31};
							bkimage="res/uires/gameui/timedot.gsa";
							updatemute = 1,
						};
						second1=
						{
							type="PICLABEL";
							rect={129,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};
						second2=
						{
							type="PICLABEL";
							rect={154,0,32,31};
							fontpic="res/uires/gameui/timenum.gsa";
							updatemute = 1,
						};

					};
				};
			};
		};
		
		GameRankPanel=
		{
			type="WIDGET";
			rect={0,170,260,400};
			template={"res/uires/playrank.lua"};
			updatemute = 1,
			
			OnInit=function()
				local path="UI.gameui.GameRankPanel";
				Win_ClearTemplates(path);
				Win_AddTemplate(path,0,0,7);
				Win_SetValue(path,500,value_channel_timerspace);
			end;
			
			OnDenit=function()
				local path="UI.gameui.GameRankPanel";
				Win_ClearTemplates(path);
				Win_SetValue(path,0,value_channel_timerspace);
			end;
			
			OnTimer=function()
				local path="UI.gameui.GameRankPanel";
				local gr=GetGameRankInfo();
				if gr==nil then
					return;
				end;
				local rcParent=Win_GetRect("UI.gameui.GameRankPanel");
				local myuin=GetMyInfo().uin;
				local x=0;
				local y=0;
				local h=25;
				local lh=87;
				local ymargin=2;
				local w=175;
				local lw=175;
				local roominfo=GetMyRoomInfo();
				for i=1,gr.pnum,1 do
					local path="UI.gameui.GameRankPanel.PlayRank"..(i-1);
					Win_ShowWidget(path,true);
					local bHost=false;
					if gr.ri[i].uin==myuin then
						bHost=true;
						Win_SetRectAll(path,rcParent.x+x,rcParent.y+y,lw,lh);
						y=y+lh+ymargin;
					else
						Win_SetRectAll(path,rcParent.x+x,rcParent.y+y,w,h);
						y=y+h+ymargin;
					end;
					UI.children.gameui.children.GameRankPanel.UpdateItem(i-1,gr.ri[i],bHost,roominfo);
				end;
				Win_SetValue(path,1000,value_channel_timerspace);
			end;
			
			UpdateItem=function(id,ri,bHost,roominfo)
				local path="UI.gameui.GameRankPanel.PlayRank"..id;		
				local myteamid=roominfo.teamid;
				
				if roominfo.gamemodesub==gmodesub_team then
					Win_ShowWidget(path..".GP1",true);
					Win_ShowWidget(path..".GP2",true);
				else
					Win_ShowWidget(path..".GP1",false);
					Win_ShowWidget(path..".GP2",false);
				end;
				
				if bHost==true then
					if ri.finish==true then
						--LogText("-------gameover");
						Win_ShowWidget("UI.gameui",false);
					end;
					
					Win_ShowWidget(path..".Team",false);
					Win_ShowWidget(path..".Name",false);
					Win_ShowWidget(path..".NetSpeed",false);
					Win_ShowWidget(path..".Portrait",true);
					Win_ShowWidget(path..".GP1",false);
					Win_ShowWidget(path..".GP2",false);
					Win_SetValue(path,RankBkMap[1],value_channel_hostindex);
					Win_SetText("UI.gameui.GameRankPanel.HostRank",tostring(id));
				else
					Win_ShowWidget(path..".Team",true);
					Win_ShowWidget(path..".Name",true);
					Win_ShowWidget(path..".NetSpeed",true);
					Win_ShowWidget(path..".Portrait",false);
					Win_ShowWidget(path..".GP1",true);
					Win_ShowWidget(path..".GP2",true);
					Win_SetValue(path,RankBkMap[0],value_channel_hostindex);
					--颜色
					if roominfo.gamemodesub==gmodesub_team then
						Win_SetValue(path..".Team",TeamColorsTexMap[ri.teamid],value_channel_hostindex);
					else
						Win_SetValue(path..".Team",SingleColorsTexMap[ri.color],value_channel_hostindex);
					end;
					--名字
					Win_SetText(path..".Name"," "..ri.nickname);
					if ri.finish==true then
						Win_SetValue(path..".Name",GetColorIndex(255,0,0,255),value_channel_color_magic);
					else
						Win_SetValue(path..".Name",GetColorIndex(0,0,0,255),value_channel_color_magic);
					end;
					--网络状况
					local step=0;
					--Win_ConsoleInfo(tostring(ri.netspeed));
					if ri.netspeed<200 then
						Win_SetBackPic(path..".NetSpeed","res/uires/gameui/goodnet.gsa");
						if ri.netspeed<80 then
							step=5;
						else
							step=(200-ri.netspeed)/24+1;
						end;
					elseif ri.netspeed<1500 then
						Win_SetBackPic(path..".NetSpeed","res/uires/gameui/midnet.gsa");
						if ri.netspeed<320 then
							step=(ri.netspeed-200)/24+1;
						else
							step=5;
						end;
					else
						Win_SetBackPic(path..".NetSpeed","res/uires/gameui/badnet.gsa");
						step=3;
					end;
					step=step+0.0001;
					local rc=Win_GetRect(path..".NetSpeed");
					--Win_ConsoleInfo(tostring(math.floor(step)*6));
					Win_SetRectAll(path..".NetSpeed",rc.x,rc.y,math.floor(step)*6,rc.h);
					--道具
					Win_SetValue(path..".GP1",0,value_channel_hostindex);
					Win_SetValue(path..".GP2",0,value_channel_hostindex);
					if roominfo.gamemodesub==gmodesub_team and ri.teamid==myteamid then
						if ri.gp1~=0 then
							Win_SetValue(path..".GP1",GPIconTexMap[ri.gp1],value_channel_hostindex);
						end;
						if ri.gp2~=0 then
							Win_SetValue(path..".GP2",GPIconTexMap[ri.gp2],value_channel_hostindex);
						end;
					end;
				end;
			end;
			
			children=
			{
				HostRank=
				{
					type="PICLABEL";
					rect={40,-30,40,40};
					fontpic="res/uires/gameui/playranknum.gsa";
					updatemute = 1,
				};
			};
		};
		SpeedPanel=
		{
			type="WIDGET";
			rect={882,689,100,40};
			updatemute = 1,
			
			OnInit=function()
				Win_SetValue("UI.gameui.SpeedPanel",100,value_channel_timerspace);
			end;
			
			OnTimer=function()
				local speed=GetSpeed();
				local a=math.floor(speed/100);
				local b=math.floor((speed-a*100)/10);
				local c=math.floor(speed-a*100-b*10);
				local path="UI.gameui.SpeedPanel";
				--Win_ConsoleInfo(tostring(a).."-"..tostring(b).."-"..tostring(c));
				Win_SetText(path..".a",tostring(a));
				Win_SetText(path..".b",tostring(b));
				Win_SetText(path..".c",tostring(c));
				Win_SetValue("UI.gameui.SpeedPanel",100,value_channel_timerspace);
			end;
			
			children=
			{
				a=
				{
					type="PICLABEL";
					rect={10,10,20,31};
					fontpic="res/uires/gameui/表盘数字.gsa";
					updatemute = 1,
				};
				b=
				{
					type="PICLABEL";
					rect={36,10,20,31};
					fontpic="res/uires/gameui/表盘数字.gsa";
					updatemute = 1,
				};				
				c=
				{
					type="PICLABEL";
					rect={62,10,20,31};
					fontpic="res/uires/gameui/表盘数字.gsa";
					updatemute = 1,
				};
			};
		};
		
		CountDownTimePanel=
		{
			type="WIDGET";
			rect={329,21,300,200};
			bkimage="res/uires/gameui/倒计时背景.gsa";
			updatemute = 1,
			
			OnInit=function()
				Win_SetValue("UI.gameui.CountDownTimePanel",100,value_channel_timerspace);
				
				if GetCurTaskID() ~= 3 then
					Win_ShowWidget("UI.gameui.CountDownTimePanel",false);
				else
					Win_ShowWidget("UI.gameui.CountDownTimePanel",true);
				end;
			end;
			
			OnTimer=function()
				local cdtime=GetCountDownTime();
				if cdtime < 0 then
					cdtime = 0;
				end;
				--自己的时间
				local hour=math.floor(cdtime/60000);
				local minute=math.floor((cdtime-hour*60000)/1000);
				local second=math.floor(cdtime-hour*60000-minute*1000);
				local h1=math.floor(hour/10);
				local h2=hour-h1*10;
				local m1=math.floor(minute/10);
				local m2=minute-m1*10;
				local s1=math.floor(second/100);
				local s2=math.floor((second-s1*100)/10);
				
				--Win_ConsoleInfo(tostring(h1)..":"..tostring(h2)..":"..tostring(m1)..":"..tostring(m2)..":"..tostring(s1)..":"..tostring(s2));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.hour1",tostring(h1));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.hour2",tostring(h2));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.minute1",tostring(m1));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.minute2",tostring(m2));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.second1",tostring(s1));
				Win_SetText("UI.gameui.CountDownTimePanel.CDownTime.second2",tostring(s2));				
				
				Win_SetValue("UI.gameui.CountDownTimePanel",100,value_channel_timerspace);
			end;
			
			children=
			{
				CDownTime=
				{
					type="WIDGET";
					rect={68,10,381,93};
					updatemute = 1,
					
					children=
					{
						hour1=
						{
							type="PICLABEL";
							rect={-24,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
						};
						hour2=
						{
							type="PICLABEL";
							rect={27,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
						dot1=
						{
							type="LABEL";
							rect={42,0,54,45};
							bkimage="res/uires/gameui/cdowntimedot.gsa";
							updatemute = 1,
						};
						minute1=
						{
							type="PICLABEL";
							rect={91,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
						minute2=
						{
							type="PICLABEL";
							rect={143,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
						dot2=
						{
							type="LABEL";
							rect={157,0,54,45};
							bkimage="res/uires/gameui/cdowntimedot.gsa";
							updatemute = 1,
						};
						second1=
						{
							type="PICLABEL";
							rect={207,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
						second2=
						{
							type="PICLABEL";
							rect={259,27,32,31};
							fontpic="res/uires/gameui/LapsNum.gsa";
							updatemute = 1,
						};
					};
				};
				
			};
		};
		
	};
};