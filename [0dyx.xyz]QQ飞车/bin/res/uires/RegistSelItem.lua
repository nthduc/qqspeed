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

				if _UIID_==0 then --����������Ա���Ҫ��������ѡ��	
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
				if (_UIID_==0) then --����������Ա���Ҫ��������ѡ��						
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
	{--Ů
		{--�Ա�
			icon="res/uires/regist/�Ա�.gsa";
			tiptext="��ɫ�Ա�",
			{
				id=0;
				desc="Ů";
			};
			{
				id=1;
				desc="��";
			};
		};
		{--����
			icon="res/uires/regist/����.gsa";
			tiptext="�ݲ�����",
			--[[{
				id=10001;
				desc="С���";
			};]]
			{
				id = 10018;
				desc = "����1";
			};
		};
		{--����
			icon="res/uires/regist/����.gsa";
			tiptext="��ɫ����",
			{
				id=10021;
				desc="����1";
			};
			{
				id=10026;
				desc="����2";
			};			
		};
		{--��װ
			icon="res/uires/regist/��װ.gsa";
			tiptext="�ݲ�����",
			{
				id=0;
				desc="��װ1";
			};
			--[[{
				id=1;
				desc="��װ2";
			};
			{
				id=2;
				desc="��װ3";
			};]]
		};
		{--��������
			icon="res/uires/regist/��������.gsa";
			tiptext="ѡ������",
			{
				id=10014;
				desc="��ɫ����";
			};
			{
				id=10015;
				desc = "��ɫ����";
			};
			{
				id=10016;
				desc = "��ɫ����";
			};
			{
				id=10029;
				desc = "��ɫ����";
			};
		};
		{--������
			icon="res/uires/regist/������.gsa";
			tiptext="�ݲ�����",
			{
				id=1;
				desc="��������";
			};
			{
				id=2;
				desc="��ѩ����";
			};
			--[[{
				id=3;
				desc="��������";
			};]]
		};
	};
	{--��
		{--�Ա�
			icon="res/uires/regist/�Ա�.gsa";
			{
				id=0;
				desc="Ů";
			};
			{
				id=1;
				desc="��";
			};
		};
		{--����
			icon="res/uires/regist/����.gsa";
			--[[{
				id=10000;
				desc="���";
			};]]
			{
				id=10022;
				desc="����1",
			};
		};
		{--����
			icon="res/uires/regist/����.gsa";
			{
				id=10025;
				desc="����1";
			};
			{
				id=10027;
				desc="����2";
			};
		};
		{--��װ
			icon="res/uires/regist/��װ.gsa";
			{
				id=0;
				desc="��װ1";
			};
			--[[{
				id=1;
				desc="��װ2";
			};
			{
				id=2;
				desc="��װ3";
			};]]
		};
		{--��������
			icon="res/uires/regist/��������.gsa";
			{
				id=10014;
				desc="��ɫ����";
			};
			{
				id=10015;
				desc = "��ɫ����";
			};
			{
				id=10016;
				desc = "��ɫ����";
			};
			{
				id=10029;
				desc = "��ɫ����";
			};
		};
		{--������
			icon="res/uires/regist/������.gsa";
			{
				id=1;
				desc="��������";
			};
			{
				id=2;
				desc="��ѩ����";
			};
			--[[{
				id=3;
				desc="��������";
			};]]
		};
	};
};

print( table.getn(RegistSelInfo[1+1][1+1]));