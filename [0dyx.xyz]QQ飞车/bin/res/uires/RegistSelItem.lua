RegistSelItem_UIID_=
{
	type="BUTTON",
	rect={0,0,0,0},
	groupstop=_UIID_,
	style=wgtstyle_template,
	
	_sex=0;
	_index=0;
	_selindex=0;
	
	OnInit = function()	
		local myid="UI.Regist.RegistWgt.RegistSelPanel.RegistSelItem_UIID_.bar";
		Win_SetText(myid,  RegistSelInfo[1][_UIID_+1].tiptext, value_channel_tiptext);
	end;
	
	UpdateData=function(sex,index, bfirstrun)		
		--Win_ConsoleInfo("UpdateData");
		local rcParent=Win_GetRect("UI.Regist.RegistWgt");
		local myid="UI.Regist.RegistWgt.RegistSelPanel.RegistSelItem".._UIID_;
		Win_SetRectAll(myid,rcParent.x+0,60+rcParent.y+_UIID_*40,330,40);
		local selindex = UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex;
		if (UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._sex ~= sex) then
			local selindex3 = UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2._selindex;
			RemoveSelfKartFromUIScene("ROOT");
			RemoveSelfRoleFromUIScene("playermode");
			ReBindSelfPlayer();
			SelfChangeSelf(RegistSelInfo[sex+1][2][1].id);
			--SelfChangeSelf(RegistSelInfo[sex+1][3][1].id);
			SelfPlayAnimation(1104);
			BindSelfKartToScene("ROOT");		
			BindSelfRoleToScene("playermode");	
		end;
		
		UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._sex=sex;
		UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._index=index;
		--local selindex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex;
		Win_SetBackPic(myid..".label",RegistSelInfo[sex+1][index+1].icon);		
		Win_SetText(myid..".bar",RegistSelInfo[sex+1][index+1][selindex+1].desc);			
	end;
	
	GetSelID=function()
		local selindex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex;
		local sex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._sex;
		local index=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._index;
		return RegistSelInfo[sex+1][index+1][selindex+1].id;
	end;
	
	
	children=
	{
		label=
		{
			type="LABEL";
			rect={0,0,60,40};
			bkimagepos={10,10},
		};
		prevbtn=
		{
			type="BUTTON";
			rect={60,0,60,40};
			bkimage="res/uires/common/leftarrow.gsa";
			
			OnClick=function()								
				local sex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._sex;
				local index=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._index;
				local selindex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex;
				
				local num = table.getn(RegistSelInfo[sex+1][index+1]);
				selindex = math.mod(selindex-1+num, num);
				if (num == 1) then
					selindex = 0;
				end;
				--Win_ConsoleInfo("selindex = "..selindex);
				
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex = selindex;				
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_.UpdateData(sex,index);

				if _UIID_==0 then --如果此项是性别，需要更新其他选项	
					UI.children.Regist.children.RegistWgt.children.RegistSelPanel.UpdateData(selindex);						
				elseif _UIID_ == 1 then
					local selindex3 = UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2._selindex;
					RemoveSelfKartFromUIScene("ROOT");
					RemoveSelfRoleFromUIScene("playermode");
					ReBindSelfPlayer();
					SelfChangeSelf(RegistSelInfo[sex+1][2][1].id);
					--SelfChangeSelf(RegistSelInfo[sex+1][3][1].id);
					SelfPlayAnimation(1104);					
					BindSelfKartToScene("ROOT");		
					BindSelfRoleToScene("playermode");	
				elseif _UIID_ == 2 then
					RemoveSelfKartFromUIScene("ROOT");
					RemoveSelfRoleFromUIScene("playermode");
					ReBindSelfPlayer();
					SelfChangeSelf(RegistSelInfo[sex+1][3][selindex+1].id);
					SelfPlayAnimation(1104);
					BindSelfKartToScene("ROOT");		
					BindSelfRoleToScene("playermode");
				elseif _UIID_ == 4 then
					EquipItemImmediate(GetMyInfo().uin, RegistSelInfo[sex+1][_UIID_+1][selindex+1].id);
				end;		
				
			end;
		};
		nextbtn=
		{
			type="BUTTON";
			rect={270,0,60,40};
			bkimage="res/uires/common/rightarrow.gsa";

			OnClick=function()				
				local selindex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex;
				local sex=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._sex;
				local index=UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._index;

				local num = table.getn(RegistSelInfo[sex+1][index+1]);
				
				selindex = math.mod(selindex+1, num);
				if (num == 1) then
					selindex = 0;
				end;
				Win_ConsoleInfo("..........");
				Win_ConsoleInfo("num = "..num.." selindex="..selindex.." _U IID_=".._UIID_);
				
				--Win_ConsoleInfo("selindex = "..selindex);
				
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex = selindex;
				
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_._selindex=selindex;
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_.UpdateData(sex,index);
				if (_UIID_==0) then --如果此项是性别，需要更新其他选项						
					UI.children.Regist.children.RegistWgt.children.RegistSelPanel.UpdateData(selindex);	
				elseif _UIID_ == 1 then
					local selindex3 = UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem2._selindex;
					RemoveSelfKartFromUIScene("ROOT");
					RemoveSelfRoleFromUIScene("playermode");
					ReBindSelfPlayer();
					SelfChangeSelf(RegistSelInfo[sex+1][2][1].id);
					--SelfChangeSelf(RegistSelInfo[sex+1][3][1].id);
					SelfPlayAnimation(1104);
					BindSelfKartToScene("ROOT");		
					BindSelfRoleToScene("playermode");	
				elseif _UIID_ == 2 then
					RemoveSelfKartFromUIScene("ROOT");
					RemoveSelfRoleFromUIScene("playermode");
					ReBindSelfPlayer();
					SelfChangeSelf(RegistSelInfo[sex+1][3][selindex+1].id);
					SelfPlayAnimation(1104);
					BindSelfKartToScene("ROOT");		
					BindSelfRoleToScene("playermode");
				elseif _UIID_ == 4 then
					EquipItemImmediate(GetMyInfo().uin, RegistSelInfo[sex+1][_UIID_+1][selindex+1].id);
				end;				
					
				
			end;
		};
		bar=
		{
			type="BUTTON";
			bkimage="res/uires/regist/bar.gsa";
			rect={120,0,150,40};
			bkimagepos={-5,-5};
			
			OnClick = function()
				Win_ConsoleInfo("OnClick");
				UI.children.Regist.children.RegistWgt.children.RegistSelPanel.children.RegistSelItem_UIID_.children.nextbtn.OnClick();
			end;
		};
	};
};

RegistSelInfo=
{
	{--女
		{--性别
			icon="res/uires/regist/性别.gsa";
			tiptext="角色性别",
			{
				id=0;
				desc="女";
			};
			{
				id=1;
				desc="男";
			};
		};
		{--脸形
			icon="res/uires/regist/脸形.gsa";
			tiptext="暂不开放",
			--[[{
				id=10001;
				desc="小甜妞";
			};]]
			{
				id = 10018;
				desc = "脸型1";
			};
		};
		{--发型
			icon="res/uires/regist/发型.gsa";
			tiptext="角色发型",
			{
				id=10021;
				desc="发型1";
			};
			{
				id=10026;
				desc="发型2";
			};			
		};
		{--套装
			icon="res/uires/regist/套装.gsa";
			tiptext="暂不开放",
			{
				id=0;
				desc="套装1";
			};
			--[[{
				id=1;
				desc="套装2";
			};
			{
				id=2;
				desc="套装3";
			};]]
		};
		{--车辆喷漆
			icon="res/uires/regist/车辆喷漆.gsa";
			tiptext="选择喷漆",
			{
				id=10014;
				desc="红色喷漆";
			};
			{
				id=10015;
				desc = "黄色喷漆";
			};
			{
				id=10016;
				desc = "蓝色喷漆";
			};
			{
				id=10029;
				desc = "黑色喷漆";
			};
		};
		{--出生地
			icon="res/uires/regist/出生地.gsa";
			tiptext="暂不开放",
			{
				id=1;
				desc="疾风王国";
			};
			{
				id=2;
				desc="飞雪王国";
			};
			--[[{
				id=3;
				desc="流火王国";
			};]]
		};
	};
	{--男
		{--性别
			icon="res/uires/regist/性别.gsa";
			{
				id=0;
				desc="女";
			};
			{
				id=1;
				desc="男";
			};
		};
		{--脸形
			icon="res/uires/regist/脸形.gsa";
			--[[{
				id=10000;
				desc="金刚";
			};]]
			{
				id=10022;
				desc="脸型1",
			};
		};
		{--发型
			icon="res/uires/regist/发型.gsa";
			{
				id=10025;
				desc="发型1";
			};
			{
				id=10027;
				desc="发型2";
			};
		};
		{--套装
			icon="res/uires/regist/套装.gsa";
			{
				id=0;
				desc="套装1";
			};
			--[[{
				id=1;
				desc="套装2";
			};
			{
				id=2;
				desc="套装3";
			};]]
		};
		{--车辆喷漆
			icon="res/uires/regist/车辆喷漆.gsa";
			{
				id=10014;
				desc="红色喷漆";
			};
			{
				id=10015;
				desc = "黄色喷漆";
			};
			{
				id=10016;
				desc = "蓝色喷漆";
			};
			{
				id=10029;
				desc = "黑色喷漆";
			};
		};
		{--出生地
			icon="res/uires/regist/出生地.gsa";
			{
				id=1;
				desc="疾风王国";
			};
			{
				id=2;
				desc="飞雪王国";
			};
			--[[{
				id=3;
				desc="流火王国";
			};]]
		};
	};
};

print( table.getn(RegistSelInfo[1+1][1+1]));