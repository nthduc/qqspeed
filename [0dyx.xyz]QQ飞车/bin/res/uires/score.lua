UI.children.score=
{
	type = "SCREEN",
	rect = {0,0,1024,768},	
	visible=0,	
	_scores=nil;
	_roominfo=nil;
	_playersinfo=nil;
	_showstep=0;
	--updatemute = 1,
	
	OnDenit = function()	
		Win_ShowWidget("UI.score",false);
		return 1;			   
	end;

	OnInit=function()
		UI.children.score._scores=Param;
		UI.children.score._roominfo=GetMyRoomInfo();
		UI.children.score._playersinfo=
		{
			[1]=GetRoomPlayerInfo(0);
			[2]=GetRoomPlayerInfo(1);
			[3]=GetRoomPlayerInfo(2);
			[4]=GetRoomPlayerInfo(3);
			[5]=GetRoomPlayerInfo(4);
			[6]=GetRoomPlayerInfo(5);
			[7]=GetRoomPlayerInfo(6);
			[8]=GetRoomPlayerInfo(7);
		};
		UI.children.score._showstep=0;
		Win_SetValue("UI.score",3000,value_channel_timerspace);
	end;
	
	OnTimer=function()
		if UI.children.score._showstep==1 then
			UI.children.score.FillTeamData();
			UI.children.score._showstep=2;
		end;
		if UI.children.score._showstep==0 then
			UI.children.score.FillData();
			UI.children.score._showstep=1;
			Win_ShowWidget("UI.score",true);	
			
			local ri=UI.children.score._roominfo;
			if ri.gamemodesub==gmodesub_team then
				Win_SetValue("UI.score",6000,value_channel_timerspace);
			end;
		end;
	end;
	
	FillData=function()
		Win_ClearTemplates("UI.score.MainFrame.scorelist");
		
		local scores=UI.children.score._scores;
		if scores==nil then
			return 0;
		end;
		
		if scores.pnum==nil then
			return 0;
		end;
		Win_AddTemplate("UI.score.MainFrame.scorelist",0,0,scores.pnum);

		local colmask=0;
		local ri=UI.children.score._roominfo;
		if ri.gamemodebase==gmodebase_race then
			if ri.gamemodesub==gmodesub_single then
				colmask=scorecol_nickname+scorecol_ftime+scorecol_expn+scorecol_money;
			end;
			if ri.gamemodesub==gmodesub_team then
				colmask=scorecol_nickname+scorecol_ftime+scorecol_tp;
			end;
		end;
		if ri.gamemodebase==gmodebase_item then
			if ri.gamemodesub==gmodesub_single then
				colmask=scorecol_nickname+scorecol_ftime+scorecol_expn+scorecol_money;
			end;
			if ri.gamemodesub==gmodesub_team then
				colmask=scorecol_nickname+scorecol_ftime;
			end;
		end;
		local pnum=scores.pnum;
		
		local psi=UI.children.score._playersinfo;
		--标题行
		UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetColMask(0,colmask);
		local tp_r=0;
		local tp_b=0;
		for i=1,pnum,1 do
			local billing=i;
			local pi;
			local uin=scores.scores[i].uin;
			local myuin=GetMyInfo().uin;
			
			if uin == myuin then
				pi = GetMyInfo();
			end;
			if uin ~= myuin then
				local seatid=UinToSeat(uin);
				if seatid ~= nil then
					--pi = GetRoomPlayerInfo(seatid);
					pi=psi[seatid+1];
				end;
			end;
			
			if pi == nil then
				LogText("cannot find player information");
			end;
			if pi ~= nil then
				local teamid=pi.teamid;
				local ftime=scores.scores[i].fintime;
				local expn=scores.scores[i].expadd;
				local money=scores.scores[i].moneyadd;
				local expntotal=scores.scores[i].expn;
				local nickname=pi.playerdbinfo.nickname;
				local tp=scores.scores[i].tp;
				
				if ri.gamemodesub==gmodesub_single then
					teamid=-1;
				end;
				if ri.gamemodesub==gmodesub_team then
					if teamid==1 then
						tp_b=tp_b+tp;
					end;
					if teamid==0 then
						tp_r=tp_r+tp;
					end;
				end;
				UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetColMask(i,colmask);
				UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetData(i,teamid,billing,nickname,ftime,expn,expntotal,money,tp);
			end;
		end;
		--LogText("run here?");
		--LogText("modebase:"..ri.gamemodebase);
		--LogText("modesub:"..ri.gamemodesub);
		if ri.gamemodesub==gmodesub_team and ri.gamemodebase==gmodebase_race then
			--LogText("race and team");
			UI.children.score.children.MainFrame.children.tp_r.SetTP(tp_r);
			UI.children.score.children.MainFrame.children.tp_b.SetTP(tp_b);
			Win_ShowWidget("UI.score.MainFrame.tp_r",true);
			Win_ShowWidget("UI.score.MainFrame.tp_b",true);
		else
			--LogText("not race and team");
			UI.children.score.children.MainFrame.children.tp_r.SetTP(0);
			UI.children.score.children.MainFrame.children.tp_b.SetTP(0);
			Win_ShowWidget("UI.score.MainFrame.tp_r",false);
			Win_ShowWidget("UI.score.MainFrame.tp_b",false);
		end;
	end;
	
	FillTeamData=function()
		Win_ClearTemplates("UI.score.MainFrame.scorelist");
		Win_ShowWidget("UI.score.MainFrame.tp_r",false);
		Win_ShowWidget("UI.score.MainFrame.tp_b",false);

		local scores=UI.children.score._scores;
		if scores==nil then
			return 0;
		end;
		
		if scores.pnum==nil then
			return 0;
		end;
		Win_AddTemplate("UI.score.MainFrame.scorelist",0,0,scores.pnum+1);

		local colmask=0;
		local ri=UI.children.score._roominfo;
		if ri.gamemodebase==gmodebase_race then
			if ri.gamemodesub==gmodesub_single then
				colmask=scorecol_nickname+scorecol_ftime+scorecol_expn+scorecol_money;
			end;
			if ri.gamemodesub==gmodesub_team then
				colmask=scorecol_nickname+scorecol_expn+scorecol_money;
			end;
		end;
		if ri.gamemodebase==gmodebase_item then
			if ri.gamemodesub==gmodesub_single then
				colmask=scorecol_nickname+scorecol_ftime+scorecol_expn+scorecol_money;
			end;
			if ri.gamemodesub==gmodesub_team then
				colmask=scorecol_nickname+scorecol_expn+scorecol_money;
			end;
		end;
		
		local psi=UI.children.score._playersinfo;
		--标题行
		UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetColMask(0,colmask);
				
		local pnum=scores.pnum;
		local winnerid=scores.winnerid;
		local index=1;
		--local loopfirst=1;
		for loop=1,2,1 do
			local loopfirst=1;
			Win_ConsoleInfo("startloop,loopfirst="..loopfirst);
			for i=1,pnum,1 do
				local billing=i;
				local pi;
				local uin=scores.scores[i].uin;
				local myuin=GetMyInfo().uin;
				
				if uin == myuin then
					pi = GetMyInfo();
				end;
				if uin ~= myuin then
					local seatid=UinToSeat(uin);
					if seatid ~= nil then
						--pi = GetRoomPlayerInfo(seatid);
						pi=psi[seatid+1];
					end;
				end;
				
				if pi == nil then
					Win_ConsoleInfo("cannot find player information");
				end;
				
				if pi~=nil then
					if loop==1 and pi.teamid~=winnerid then
						pi = nil;
					end;
					if loop==2 and pi.teamid==winnerid then
						pi = nil;
					end;
				end;
				
				if pi ~= nil then
					local teamid=pi.teamid;
					local ftime=scores.scores[i].fintime;
					local expn=scores.scores[i].expadd;
					local money=scores.scores[i].moneyadd;
					local expntotal=scores.scores[i].expn;
					local nickname=pi.playerdbinfo.nickname;
					local tp=scores.scores[i].tp;
				
					local win=0;
					if teamid==winnerid then
						win=1;
					end;
					if loopfirst==1 then
						--Win_ConsoleInfo("loopfirst,setwin:"..nickname);
						UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetWin(index,win);
						loopfirst=0;
					end;
					UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetColMask(index,colmask);
					UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.SetData(index,teamid,-1,nickname,ftime,expn,expntotal,money,tp);
					index=index+1;
				end;
			end;
		end;
	end;

	children=
	{
		MainFrame=
		{
			type="MESSAGEBOX",
			rect={0,0,772,619},
			bkimage="res/uires/score/bk.gsa",
			
			
			OnInit=function()
			end;
			
			children=
			{
				scorelist=
				{
					type="WIDGET",
					rect={0,18,772,584},
					template={"res/uires/scoreitem.lua"},
					
					children={},
				};
				tp_r=
				{
					type="WIDGET",
					rect={35,625,280,40},
					bkimage="res/uires/score/tp_r.gsa",
					--style=wgtstyle_customalpha,
					_tp=0;
					_splash=0;
					_splashstep=0;
					
					--[[
					OnInit=function()
						UI.children.score.children.MainFrame.children.tp_r.SetTP(857);
						--UI.children.score.children.MainFrame.children.tp_r.SetWin(1);
					end;]]
					SetTP=function(tp)
						UI.children.score.children.MainFrame.children.tp_r._tp=tp;
						local num1=math.floor(tp/100);
						local num2=math.floor(math.mod(tp,100)/10);
						local num3=math.floor(math.mod(tp,10));
						UI.children.score.children.MainFrame.children.tp_r.children.num1.SetNum(num1);
						UI.children.score.children.MainFrame.children.tp_r.children.num2.SetNum(num2);
						UI.children.score.children.MainFrame.children.tp_r.children.num3.SetNum(num3);
					end;
					SetWin=function(win)
						if win~=0 then
							UI.children.score.children.MainFrame.children.tp_r._splash=1;
							UI.children.score.children.MainFrame.children.tp_r._splashstep=0;
							Win_SetValue("UI.score.MainFrame.tp_r",4000,value_channel_timerspace);
						end;
					end;
					
					OnTimer=function()
						local splashstep=UI.children.score.children.MainFrame.children.tp_r._splashstep;
						local alpha = math.mod(splashstep,10) * 0.1;
						if alpha<0.2 then
							alpha=0.2;
						end;
						Win_SetValue("UI.score.MainFrame.tp_r",alpha,value_channel_alphafactor);
						if splashstep<20 then
							UI.children.score.children.MainFrame.children.tp_r._splashstep=splashstep+1;
							Win_SetValue("UI.score.MainFrame.tp_r",200,value_channel_timerspace);
						end;
						if splashstep==20 then
							Win_SetValue("UI.score.MainFrame.tp_r",1.0,value_channel_alphafactor);
						end;
					end;
					
					children=
					{
						num1=
						{
							type="LABEL",
							rect={152,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num1",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_r.children.num1._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_r.num1",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_r.children.num1._num=num;
								UI.children.score.children.MainFrame.children.tp_r.children.num1._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_r.children.num1._num;
								local step=UI.children.score.children.MainFrame.children.tp_r.children.num1._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_r.children.num1._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num1",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_r.children.num1._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_r.num1",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						num2=
						{
							type="LABEL",
							rect={182,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num2",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_r.children.num2._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_r.num2",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_r.children.num2._num=num;
								UI.children.score.children.MainFrame.children.tp_r.children.num2._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_r.children.num2._num;
								local step=UI.children.score.children.MainFrame.children.tp_r.children.num2._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_r.children.num2._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num2",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_r.children.num2._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_r.num2",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						num3=
						{
							type="LABEL",
							rect={212,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num3",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_r.children.num3._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_r.num3",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_r.children.num3._num=num;
								UI.children.score.children.MainFrame.children.tp_r.children.num3._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_r.children.num3._num;
								local step=UI.children.score.children.MainFrame.children.tp_r.children.num3._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_r.children.num3._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_r.num3",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_r.children.num3._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_r.num3",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						
					};
				};
				tp_b=
				{
					type="WIDGET",
					rect={454,625,280,40},
					bkimage="res/uires/score/tp_b.gsa",
					--style=wgtstyle_customalpha,
					_tp=0;
					_splash=0;
					_splashstep=0;
					
					--[[
					OnInit=function()
						UI.children.score.children.MainFrame.children.tp_b.SetTP(857);
						UI.children.score.children.MainFrame.children.tp_b.SetWin(1);
					end;]]
					SetTP=function(tp)
						UI.children.score.children.MainFrame.children.tp_b._tp=tp;
						local num1=math.floor(tp/100);
						local num2=math.floor(math.mod(tp,100)/10);
						local num3=math.floor(math.mod(tp,10));
						UI.children.score.children.MainFrame.children.tp_b.children.num1.SetNum(num1);
						UI.children.score.children.MainFrame.children.tp_b.children.num2.SetNum(num2);
						UI.children.score.children.MainFrame.children.tp_b.children.num3.SetNum(num3);
					end;
					SetWin=function(win)
						if win~=0 then
							UI.children.score.children.MainFrame.children.tp_b._splash=1;
							UI.children.score.children.MainFrame.children.tp_b._splashstep=0;
							Win_SetValue("UI.score.MainFrame.tp_b",4000,value_channel_timerspace);
						end;
					end;
					
					OnTimer=function()
						local splashstep=UI.children.score.children.MainFrame.children.tp_b._splashstep;
						local alpha = math.mod(splashstep,10) * 0.1;
						if alpha<0.2 then
							alpha=0.2;
						end;
						Win_SetValue("UI.score.MainFrame.tp_b",alpha,value_channel_alphafactor);
						if splashstep<20 then
							UI.children.score.children.MainFrame.children.tp_b._splashstep=splashstep+1;
							Win_SetValue("UI.score.MainFrame.tp_b",200,value_channel_timerspace);
						end;
						if splashstep==20 then
							Win_SetValue("UI.score.MainFrame.tp_b",1.0,value_channel_alphafactor);
						end;
					end;
					
					children=
					{
						num1=
						{
							type="LABEL",
							rect={152,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num1",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_b.children.num1._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_b.num1",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_b.children.num1._num=num;
								UI.children.score.children.MainFrame.children.tp_b.children.num1._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_b.children.num1._num;
								local step=UI.children.score.children.MainFrame.children.tp_b.children.num1._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_b.children.num1._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num1",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_b.children.num1._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_b.num1",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						num2=
						{
							type="LABEL",
							rect={182,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num2",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_b.children.num2._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_b.num2",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_b.children.num2._num=num;
								UI.children.score.children.MainFrame.children.tp_b.children.num2._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_b.children.num2._num;
								local step=UI.children.score.children.MainFrame.children.tp_b.children.num2._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_b.children.num2._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num2",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_b.children.num2._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_b.num2",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						num3=
						{
							type="LABEL",
							rect={212,0,25,40},
							bkimage="res/uires/score/tpnum_anim.gsa",
							textsize=40;
							style=wgtstyle_clipself,
							_num=0,
							_step=0,
							_stepmax=0,
							
							SetNum=function(num)
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=0;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num3",imageinfo);
								if num~= 0 then
									local stepmax=(20+num)*4;
									UI.children.score.children.MainFrame.children.tp_b.children.num3._stepmax=stepmax;
									Win_SetValue("UI.score.MainFrame.tp_b.num3",3000/stepmax,value_channel_timerspace);
								end;
								UI.children.score.children.MainFrame.children.tp_b.children.num3._num=num;
								UI.children.score.children.MainFrame.children.tp_b.children.num3._step=0;
							end;
							OnTimer=function()
								local num=UI.children.score.children.MainFrame.children.tp_b.children.num3._num;
								local step=UI.children.score.children.MainFrame.children.tp_b.children.num3._step;
								local stepmax=UI.children.score.children.MainFrame.children.tp_b.children.num3._stepmax;
								local y=-step*41/4;
								y=math.floor(math.mod(y,410));
								local imageinfo=
								{
									bkimagex=0;
									bkimagey=y;
								};
								Win_SetImageInfo("UI.score.MainFrame.tp_b.num3",imageinfo);
								if step<stepmax then
									UI.children.score.children.MainFrame.children.tp_b.children.num3._step=step+1;
									Win_SetValue("UI.score.MainFrame.tp_b.num3",3000/stepmax,value_channel_timerspace);
								end;
							end;
						};
						
					};
				};
			};
		};
	};
};