scoreitem_UIID_=
{
	type="BUTTON",
	rect={0,0,0,0},
	groupstop=_UIID_,
	style=wgtstyle_template,
	
	OnInit=function()
		local rcParent=Win_GetRect("UI.score.MainFrame.scorelist");
		local myid="UI.score.MainFrame.scorelist.scoreitem".._UIID_;
		--Win_SetRect(myid,rcParent.x + 0,rcParent.y + 65*_UIID_,772,65);
		Win_SetRect(myid,0-_UIID_*100-400,rcParent.y + 65*_UIID_,772,65);
		Win_SetValue(myid,100,value_channel_timerspace);
	end;
	
	OnTimer=function()
		local rcParent=Win_GetRect("UI.score.MainFrame.scorelist");
		local myid="UI.score.MainFrame.scorelist.scoreitem".._UIID_;
		local rcMe=Win_GetRect(myid);
		if rcMe.x < rcParent.x then
			rcMe.x = rcMe.x + 100;
			Win_SetRect(myid,rcMe.x,rcMe.y,rcMe.w,rcMe.h);
			Win_SetValue(myid,100,value_channel_timerspace);
		end;
	end;
	
	SetColMask=function(uiid,mask)
		local colwidth=140;
		local colheight=65;
		local xoffset=50;

		local myid="UI.score.MainFrame.scorelist.scoreitem"..uiid;
		local rcParent=Win_GetRect(myid);
		
		if BitAnd(mask,scorecol_nickname) ~= 0 then
			Win_ShowWidget(myid..".nickname", true);
			Win_SetRectAll(myid..".nickname", rcParent.x+xoffset,rcParent.y,colwidth,colheight);
			xoffset=xoffset+colwidth;
		end;
		if BitAnd(mask,scorecol_ftime) ~= 0 then
			Win_ShowWidget(myid..".ftime", true);
			Win_SetRectAll(myid..".ftime", rcParent.x+xoffset,rcParent.y,colwidth,colheight);
			xoffset=xoffset+colwidth;
		end;
		if BitAnd(mask,scorecol_expn) ~= 0 then
			Win_ShowWidget(myid..".expn", true);
			Win_SetRectAll(myid..".expn", rcParent.x+xoffset,rcParent.y,colwidth,colheight);
			xoffset=xoffset+colwidth;
		end;
		if BitAnd(mask,scorecol_money) ~= 0 then
			Win_ShowWidget(myid..".money", true);
			Win_SetRectAll(myid..".money", rcParent.x+xoffset,rcParent.y,colwidth,colheight);
			xoffset=xoffset+colwidth;
		end;
		if BitAnd(mask,scorecol_tp) ~= 0 then
			Win_ShowWidget(myid..".tp", true);
			Win_SetRectAll(myid..".tp", rcParent.x+xoffset,rcParent.y,colwidth,colheight);
			xoffset=xoffset+colwidth;
		end;
		
		local idframe="UI.score.MainFrame.scorelist";
		local rcFrame=Win_GetRect(idframe);
		Win_SetRectAll(idframe,0,0,xoffset,rcFrame.h);
		Win_AlignWidget(idframe,"UI.score.MainFrame",aligntype_father+aligntype_winrect,alignstyle_hcenter+alignstyle_vcenter);
	end;
	
	SetData=function(uiid,teamid,billing,nickname,ftime,expn,expntotal,money,tp)
		if uiid==0 then	--第一行为域名称字段
			return;
		end;
		local itemid="UI.score.MainFrame.scorelist.scoreitem"..uiid;
		if billing >= 0 then
			if ftime < 4294967295 then
				UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.children.billing.SetBilling(uiid,teamid,billing);
				local ms=math.floor(math.mod(ftime,1000)/10);
				ftime=math.floor(ftime/1000);
				local s=math.mod(ftime,60);
				local m=math.floor(ftime/60);
				local sztime=tostring(m)..":"..tostring(s)..":"..tostring(ms);
				Win_SetText(itemid..".ftime",sztime);
			else
				Win_SetText(itemid..".ftime","未完成");
				UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.children.billing.SetBilling(uiid,teamid,0);
			end;
		end;
		if billing<0 then
			UI.children.score.children.MainFrame.children.scorelist.children.scoreitem0.children.billing.SetBilling(uiid,teamid,billing);
		end;
		Win_SetBackPic(itemid..".ftime","");
		Win_SetText(itemid..".nickname",nickname);
		Win_SetBackPic(itemid..".nickname","");
		Win_SetText(itemid..".expn",expntotal.."(+"..expn..")");
		Win_SetBackPic(itemid..".expn","");
		Win_SetText(itemid..".money",money);
		Win_SetBackPic(itemid..".money","");
		Win_SetText(itemid..".tp",tp);
		Win_SetBackPic(itemid..".tp","");
	end;
	
	SetWin=function(uiid,bWin)
		local itemid="UI.score.MainFrame.scorelist.scoreitem"..uiid..".winorloss";
		Win_ShowWidget(itemid,true);
		if bWin~=0 then
			Win_SetBackPic(itemid,"res/uires/score/win.gsa");
		end;
		if bWin== 0 then
			Win_SetBackPic(itemid,"res/uires/score/loss.gsa");
		end;
	end;
	
	children=
	{
		winorloss=
		{
			type="LABEL",
			rect={-80,10,20,20},
			visible=0,
		};
		
		billing=
		{
			type="LABEL",
			rect={0,20,20,20},
			
			SetBilling=function(uiid,teamid,billing)
				local itemid="UI.score.MainFrame.scorelist.scoreitem"..uiid;
				Win_ShowWidget(itemid..".billing.billing",false);
				Win_ShowWidget(itemid..".billing.billing_r",false);
				Win_ShowWidget(itemid..".billing.billing_b",false);
				Win_ShowWidget(itemid..".billing.billing_t_b",false);
				Win_ShowWidget(itemid..".billing.billing_t_r",false);
				if billing>=0 then
					if teamid<0 then
						Win_SetText(itemid..".billing.billing",tostring(billing));
						Win_ShowWidget(itemid..".billing.billing",true);
					end;
					if teamid==1 then	--蓝
						Win_SetText(itemid..".billing.billing_b",tostring(billing));
						Win_ShowWidget(itemid..".billing.billing_b",true);
					end;
					if teamid==0 then		--红
						Win_SetText(itemid..".billing.billing_r",tostring(billing));
						Win_ShowWidget(itemid..".billing.billing_r",true);
					end;
				end;
				if billing<0 then
					if teamid==1 then
						Win_ShowWidget(itemid..".billing.billing_t_b",true);
					end;
					if teamid==0 then
						Win_ShowWidget(itemid..".billing.billing_t_r",true);
					end;
				end;
			end;
			children=
			{
				billing=	--个人赛的排名
				{
					type="PICLABEL",
					rect={0,0,20,20},
					fontpic="res/uires/score/billing.gsa",
					visible=0,
				};
				billing_r=	--组队赛第一页的排名（红色）
				{
					type="PICLABEL",
					rect={0,0,20,20},
					fontpic="res/uires/score/billing_r.gsa",
					visible=0,
				};
				billing_b=	--组队赛第一页的排名（篮色）
				{
					type="PICLABEL",
					rect={0,0,20,20},
					fontpic="res/uires/score/billing_b.gsa",
					visible=0,
				};
				billing_t_b=	--组队赛第二页的排名（篮色）
				{
					type="LABEL",
					rect={0,0,20,20},
					bkimage="res/uires/score/b.gsa",
					visible=0,
				};
				billing_t_r=	--组队赛第二页的排名（红色）
				{
					type="LABEL",
					rect={0,0,20,20},
					bkimage="res/uires/score/r.gsa",
					visible=0,
				};
			};
		};
		
		nickname=
		{
			type="LABEL",
			bkimage="res/uires/score/nickname.gsa";
			bkimagepos={40,15},
			visible=0,
			drawcolor={255,255,0,255},
		};
		
		ftime=
		{
			type="LABEL",
			bkimage="res/uires/score/fintime.gsa";
			bkimagepos={40,15},
			visible=0,
			drawcolor={255,255,0,255},
		};
		
		expn=
		{
			type="LABEL",
			bkimage="res/uires/score/expn.gsa";
			bkimagepos={40,15},
			visible=0,
			drawcolor={255,255,0,255},
		};
		
		money=
		{
			type="LABEL",
			bkimage="res/uires/score/money.gsa";
			bkimagepos={40,15},
			visible=0,
			drawcolor={255,255,0,255},
		};
		tp=
		{
			type="LABEL",
			bkimage="res/uires/score/tp.gsa";
			bkimagepos={40,15},
			visible=0,
			drawcolor={255,255,0,255},
		};
	};
};