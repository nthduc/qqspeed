--�̵����б�
shoptabsel=
{
	{
		id = ECBT_BASE_HOTRECOMMAND,
		desc = "�����Ƽ�",
		pic = "res/uires/shop/recommend.gsa",		
	},
	{
		id = ECBT_BASE_KART,
		desc = "����",
		pic = "res/uires/shop/kart.gsa",
	},
	{
		id = ECBT_BASE_KARTEQP,
		desc = "����װ��",
		pic = "res/uires/shop/kartavatar.gsa",
	},
	{
		id = ECBT_BASE_CHARACTOREQP,
		desc = "��ɫװ��",
		pic = "res/uires/shop/roleavatar.gsa",
	},
	{
		id = ECBT_BASE_PET,
		desc = "����",
		pic = "res/uires/shop/pet.gsa",
	},
	{
		id = ECBT_BASE_OTHER,
		desc = "����",
		pic = "res/uires/shop/other.gsa",
	},
	{
		id = ECBT_BASE_EFFECT,
		desc = "��Ч",
		pic = "res/uires/shop/effect.gsa",
	},
};



function GetTabLength(tab)
	local num = 0;
	for element in tab do
		num = num + 1;
	end;
	return num;
end;


function PrintTab(tab)
	
	for element in tab do
		print("id = "..tab[element].id);
		print("pic = "..tab[element].pic);
	end;
	
end;

print(GetTabLength(shoptabsel));
PrintTab(shoptabsel);

UI.children.shop = 
{
	type = "SCREEN",
	rect = {0,0,1024,768},	
	bkimage="res/uires/startmenu/carbarn_bk.gsa",
	visible=0,	
	autoscale = 1,

	maincat=1,	--1=�̵꣬2=����

	_onepagenum=8, --ÿҳ��ʾ��icon��
	updatemute = 1,
	
	OnInit = function()	
		Win_ShowWidget("UI.shop",true);	
		--����ԭ��װ��״̬
		--BackupShopEquip();
		return 1;			   
	end;
	 
	OnDenit = function()	
		Win_ShowWidget("UI.shop",false);
		
		return 1;			   
	end;
	
	OnSelfShow = function()
		cmd_shownavigation();
		cmd_setcamera();
		cmd_setselfact();
		--Win_ShowWidget("UI.shop.PlayerViewDlg", true);
	end;
	
	OnSelfHide = function()
		--��ԭ����װ��״̬
		RemoveSelfKartFromUIScene("ROOT");
		RemoveSelfRoleFromUIScene("playermode");
		ReBindSelfPlayer();
		RestoreShopEquip();
		BindSelfKartToScene("ROOT");
		BindSelfRoleToScene("playermode");
		--���������Ϣ
		CleanShopEquipInfo();
		--SelfPlayAnimation(1104);
		cmd_setselfact();
		--Win_ShowWidget("UI.shop.PlayerViewDlg", false);
	end;
	
	children = 
	{
		PlayerViewDlg=
		{--������ת��ʾ���ƴ���
			type = "DIALOG",
			rect = {1024/1024 * 660, 768/768 * 145, 1024/1024 * 320, 768/768 * 500},
			captionrect = {1024/1024 * 250, 3, 1024/1024 * 100, 16 },
			--caption = "��������չʾ",
			--enable = 0,
			visible = 1,
			_turnstate = 0,
			initlayer = 0,
			
			OnInit = function()
				Win_SetValue("UI.shop.PlayerViewDlg",30,value_channel_timerspace);
			end;
			
			OnTimer = function()
				local turnstate = UI.children.shop.children.PlayerViewDlg._turnstate;
				
				if (turnstate ~= 0) then
					if (UI.children.shop.children.PlayerViewDlg.IsTurnStop()) then
						local ret = RecoverSelfTurn(turnstate);
						RecoverRoleTurn(turnstate);
						if (ret == 1) then
							UI.children.shop.children.PlayerViewDlg._turnstate = 0;
						end;
					else
						SelfShowTurn(turnstate);
						SelfRoleShowTurn(turnstate);
					end;
					Win_SetValue("UI.shop.PlayerViewDlg",30,value_channel_timerspace);
				end;					
			end;
			
			IsTurnStop = function()
				if (UI.children.shop.children.PlayerViewDlg.children.left_btn._leftturn == 0 and UI.children.shop.children.PlayerViewDlg.children.right_btn._rightturn == 0) then
					return true;
				end;
				return false;
			end;
			
			OnClick = function()
				--��ֹ�ѱ����ڼ��������ʾ�������
				Win_SetFocus("UI.shop.ItemShow_dlg");
			end;
			
			children = 
			{
				selfshow_wgt = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 0, 20, 1024/1024 * 320, 768/768 * 460},
					style=wgtstyle_static+wgtstyle_renderitem,
					hosttype=6,
					hostindex=-1,
					enable = 0,
				},
				left_btn = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 0, 400, 1024/1024 * 100, 768/768 * 100},
					--caption = "turn left",
					bkimage="res/uires/regist/leftrot.gsa";
					initlayer=msgbox_layer,
					_leftturn = 0,
					
					OnClick = function()
						--��ֹ�ѱ����ڼ��������ʾ�������
						Win_SetFocus("UI.shop.ItemShow_dlg");
					end;
										
					OnMouseIn = function()
						UI.children.shop.children.PlayerViewDlg._turnstate = 1;
						UI.children.shop.children.PlayerViewDlg.children.left_btn._leftturn = 1;							
						Win_SetValue("UI.shop.PlayerViewDlg",30,value_channel_timerspace);
					end;
					
					OnMouseOut = function()
						UI.children.shop.children.PlayerViewDlg.children.left_btn._leftturn = 0;	
					end;
					
					
				},
				right_btn = 
				{
					type = "WIDGET",
					rect = {1024/1024 * 220, 400,1024/1024 * 100, 768/768 * 100},
					--caption = "turn right",
					bkimage="res/uires/regist/rightrot.gsa";
					initlayer=msgbox_layer,
					_rightturn = 0,
					
					OnClick = function()
						--��ֹ�ѱ����ڼ��������ʾ�������
						Win_SetFocus("UI.shop.ItemShow_dlg");
					end;
					
					OnMouseIn = function()
						UI.children.shop.children.PlayerViewDlg._turnstate = -1;
						UI.children.shop.children.PlayerViewDlg.children.right_btn._rightturn = 1;	
						Win_SetValue("UI.shop.PlayerViewDlg",30,value_channel_timerspace);						
					end;
					
					OnMouseOut = function()
						UI.children.shop.children.PlayerViewDlg.children.right_btn._rightturn = 0;
					end;
					
				},
			},
		};
		--[[
		PlayerShowWgt = 
		{
			type = "WIDGET",
			rect = {700,200,600,450},
			moveable = 1,
			visible = true;
			enable = 0,
			initlayer = 0,
			style = wgtstyle_renderitem,
			--autoscale = 0,
			enable = 0,
			hosttype = 6,
			hostindex =  -1;
		};
		]]

		ItemShow_dlg = 
		{--��Ʒ�б���ʾ����
			type = "DIALOG",
			rect = {100, 150, 499, 500},	
			moveable = 1,
			_cat = 0,
			_page = 0,
			initlayer = 2,
			
			OnInit = function()
				Win_SetValue("UI.shop.ItemShow_dlg", 100, value_channel_timerspace);
			end;
			
			OnTimer = function()
				--Win_SetCheck("UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup.ShopItemTab0", 1);
				--local realcatid=shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				--UI.children.shop.children.ItemShow_dlg.children.ShopTabBtn.OnRun(true);
				if (UI.children.shop.maincat==1) then
					Win_ConsoleInfo("fffffffffffffffffffffffff");
					local realcatid=shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
					local lpath="UI.shop.ItemShow_dlg.ShopTabBtn";
					Win_SetCheck(lpath, 1);
					UI.children.shop.children.ItemShow_dlg.children.ShopTabBtn.OnRun(true);
					Win_SetCheck("UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup.ShopItemTab"..UI.children.shop.children.ItemShow_dlg._cat, 1);
				elseif (UI.children.shop.maincat==2) then
				end;
			end;
			
			--[[
			Update=function()
				local avatarcat=shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				local avatarcount=GetAvatarTypeCount(avatarcat);
				local page=UI.children.garage.children.AvatarListPanel._page;
				local npp=UI.children.garage.children.AvatarListPanel._npp;
				local showcount=avatarcount-page*npp;
				if showcount<0 then
					showcount=0;
				end;
				if showcount>npp then
					showcount=npp;
				end;
				Win_ConsoleInfo("avatarcount="..avatarcount.."showcount="..showcount.."page="..page.."npp="..npp);
				Win_ClearTemplates("UI.garage.AvatarListPanel.list");
				Win_AddTemplate("UI.garage.AvatarListPanel.list",0,0,showcount-1);
			end;
			]]
			
			children = 
			{
				ItemViewerWgt = 
				{
					type = "WIDGET",
					rect = {0, 0, 499, 525},
					bkimage = "res/uires/shop/listarea.gsa",
					
					
					
					
					--template = {"res/uires/shopitemtempl.lua"},
					--template = {"res/uires/garageitemtempl.lua"},
					template={"res/uires/shopitemtempl.lua","res/uires/garageitemtempl.lua"},
					
					initlayer = 0,
					
					
					OnInit = function()
						--UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch();						
					end;
					
					OnSwitch = function(realcatid)
						local path = "UI.shop.ItemShow_dlg.ItemViewerWgt";
						local num = UI.children.shop._onepagenum;
						local page= UI.children.shop.children.ItemShow_dlg._page;
						
						
						
						--�����̵�ͳ����ò�ͬ��ģ��
						if (UI.children.shop.maincat==1) then
							Win_ClearTemplates(path,0);--���̵�
							Win_ClearTemplates(path,1);--�峵��
							Win_AddTemplate(path, 0,num * page, num * (page+1) - 1);
						elseif (UI.children.shop.maincat==2) then
							Win_ConsoleInfo("refresh garage");
							Win_ClearTemplates(path,0);--���̵�
							Win_ClearTemplates(path,1);--�峵��
							Win_AddTemplate(path, 1,num * page, num * (page+1) - 1);
						end;
						--����_cat����ȡ������ݲ����л���
						--[[
						for i = (1 + num * page), (num * (page+1)) do
							local elempath = path..".ShopItem"..(i-1)..".ItemName";
						end;
						]]
					end;
					
					children = 
					{
						pre_btn = 
						{
							type = "BUTTON",
							rect= {-60, 240, 50, 40},
							--caption = "Prev",
							bkimage = "res/uires/common/leftarrow.gsa",
							OnClick = function()
								local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
								if (UI.children.shop.children.ItemShow_dlg._page>0) then
									UI.children.shop.children.ItemShow_dlg._page = UI.children.shop.children.ItemShow_dlg._page - 1;
									UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch();
								end;
							end;     
						},
						next_btn = 
						{
							type = "BUTTON",
							rect= {538, 240, 50, 40},
							--caption = "Next",
							bkimage = "res/uires/common/rightarrow.gsa",
							
							OnClick = function()
								local num = UI.children.shop._onepagenum;
								local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
								local getres = 0;
								--�����̵�ͳ���õ���ͬ��item��
								if (UI.children.shop.maincat==1) then
									getres = GetShopCatItemNum(realcatid);
								elseif (UI.children.shop.maincat==2) then
									getres = GetAvatarTypeCount(realcatid);
								end;
								if ((UI.children.shop.children.ItemShow_dlg._page*num+num) < getres) then
									UI.children.shop.children.ItemShow_dlg._page = UI.children.shop.children.ItemShow_dlg._page + 1;
									UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch();
								end;
							end;
						},
						
						ItemTagGroup = 
						{
							type = "LABEL",
							rect  = {498, 23, 20, 50 * GetTabLength(shoptabsel)},
							template = {"res/uires/shopitemtagtempl.lua","res/uires/garageitemtagtempl.lua"},
							initlayer = 100,
							
							
							
							OnInit = function()						
								UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ItemTagGroup.OnFillGroup();								
							end;							
							
							
							OnFillGroup = function()
																
								--local tabnum = GetTabLength(shoptabsel);
								local path = "UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup";
								Win_ConsoleInfo("FillGroup");
								if (UI.children.shop.maincat==1) then
									--�̵�
									Win_ConsoleInfo("FillGroup1");
									local tabnum  = GetTabLength(shoptabsel);
									--Win_ClearTemplates(path,0);--���̵�
									Win_ClearTemplates(path,1);--�峵��
									Win_AddTemplate(path,0,0,tabnum-1);
								elseif (UI.children.shop.maincat==2) then
									--����
									Win_ConsoleInfo("FillGroup2");
									local tabnum = GetTabLength(shoptabsel)-1;
									--Win_ClearTemplates(path,1);--�峵��
									Win_ClearTemplates(path,0);--���̵�
									Win_AddTemplate(path,1,0,tabnum-1);
								end;
								--[[
								local i = 0;
								for element in shoptabsel do
									tabpath=nil;
									if (UI.children.shop.maincat==2 and shoptabsel[i+1].id==ECBT_BASE_HOTRECOMMAND) then
										--����ģʽ�����Ƽ�����������
									else
										tabpath = path..".ShopItemTab"..i;
									end;
									i = i + 1;
								end;
								]]
							end;
							
							children = 
							{
							};
						},	
					},
					
				},
				
				ShopTabBtn=
				{
					type = "TABWIN",
					rect={33,-39,190,45},
					tabrect = {0,0,190,45},					
					--caption="ȷ��",
					tabcover="res/uires/shop/shop_tab.gsa",
					textsize=12,
					groupstop = 1,
					tabstop=1,
					groupid = 200,
					
					OnCheck=function()
						UI.children.shop.children.ItemShow_dlg.children.ShopTabBtn.OnRun(false);
					end;
					--bForceRun=true,ǿ������
					OnRun=function(bForceRun)
						if (bForceRun==true or UI.children.shop.maincat~=1) then
							--����ԭ��װ��״̬
							BackupShopEquip();
							UI.children.shop.children.ItemShow_dlg._cat=0;
							UI.children.shop.maincat=1;
							local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
							UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ItemTagGroup.OnFillGroup();
							UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch(realcatid);
							local realcatid=UI.children.shop.children.ItemShow_dlg._cat;
							local lpath="UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup.ShopItemTab"..realcatid;
							Win_SetCheck(lpath, 1);
							Win_ShowWidget("UI.shop.ResetBtn",true);
						end;
					end;
				};
				
				GarageTabBtn=
				{
					type = "TABWIN",
					rect={213,-39,190,45},
					tabrect = {0,0,190,45},
					
					--caption="ȷ��",
					tabcover="res/uires/shop/garage_tab.gsa",
					textsize=12,
					tabstop=2,	
					groupid = 200,
					groupstop = 2,
					
					OnCheck=function()
						UI.children.shop.children.ItemShow_dlg.children.GarageTabBtn.OnRun(false);
					end;
					
					OnRun=function(bForceRun)
						if (bForceRun==true or UI.children.shop.maincat~=2) then
							--��ԭ����װ��״̬
							RemoveSelfKartFromUIScene("ROOT");
							RemoveSelfRoleFromUIScene("playermode");
							ReBindSelfPlayer();
							RestoreShopEquip();
							BindSelfKartToScene("ROOT");
							BindSelfRoleToScene("playermode");
							--���������Ϣ
							CleanShopEquipInfo();
							UI.children.shop.children.ItemShow_dlg._cat=1;
							UI.children.shop.maincat=2;
							local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
							UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ItemTagGroup.OnFillGroup();
							UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch(realcatid);
							local realcatid=UI.children.shop.children.ItemShow_dlg._cat-1;
							local lpath="UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup.GarageTab"..realcatid;
							Win_SetCheck(lpath, 1);
							Win_ShowWidget("UI.shop.ResetBtn",false);
						end;
					end;
				};				
				
			};
		},
		BuyItemDlg=
		{
			type="MESSAGEBOX",
			rect={290,185,397,413},
			initlayer=msgbox_layer-1,
			style=wgtstyle_renderbuffer;
			bkimage="res/uires/shop/buy_panel.gsa",
			visible=0,
			shopitemid=0,
			operastate=0,	--��ǰ����״̬,0=û�в���,1=�����Ѿ�ȷ�����ȴ����,2=������ɣ���ʾ�û�,3=�رչ��򴰿�
			
			--����������Ӱ����
			drawflag = drawflag_backprior,
			custombuffer = 1,
			buffervalue =
			{
				banimation = 1,
				cycletype = 2, 
				restype = 1, 
				resname="scene/splash.nif",			
			},
			
			OnInit = function()
			
			end;
			
			OnTimer = function()
				--local path = "UI.shop.ItemShow_dlg.ItemViewerWgt.ItemTagGroup.ShopItemTab0";
				if (UI.children.shop.children.BuyItemDlg.operastate==1) then
					local lState,lDoc=GetShopBuyState(UI.children.shop.children.BuyItemDlg.shopitemid);
					if (lState==0) then
						--��������ˣ���ʾ
						Win_ShowMsgBox(lDoc);
						UI.children.shop.children.BuyItemDlg.operastate=2;
						--Win_SetValue("UI.shop.BuyItemDlg", 3000, value_channel_timerspace);
					elseif (lState==1) then
						Win_SetValue("UI.shop.BuyItemDlg", 200, value_channel_timerspace);
					elseif (lState==2) then
						Win_ShowMsgBox(lDoc);
						UI.children.shop.children.BuyItemDlg.operastate=2;
						Win_SetValue("UI.shop.BuyItemDlg", 200, value_channel_timerspace);
					end;
				elseif (UI.children.shop.children.BuyItemDlg.operastate==2) then
					--�رմ���
					--Win_ShowWidget("UI.shop.BuyItemDlg", false);
					--Win_EndWidget("UI.SysMsgbox");
					UI.children.shop.children.BuyItemDlg.operastate=3;	--������ɣ���������ʧ��
				end;
			end;			
			
			OnSelfShow=function()
				Win_SetCapture("UI.shop.BuyItemDlg");
			end;
			
			OnSelfHide=function()
				Win_SetCapture();
			end;
			
			children=
			{
			--[[
				SplashDlg=
				{
					type="MESSAGEBOX",
					initlayer=msgbox_layer-2,
					visible=1,
					style=wgtstyle_renderbuffer;
					custombuffer = 1,
					buffervalue =
					{
						banimation = 1,
						cycletype = 2, 
						restype = 1, 
						resname="scene/splash.nif",			
					},
				},
				]]
				ItemShow = 
				{
					type = "LABEL",
					rect = {52, 93, 127, 107},
					style = wgtstyle_renderitem,
					hosttype = 5;
					hostindex=0;
				},
		
				ItemName = 
				{--��Ʒ����
					type = "LABEL",
					rect = {125, 47, 150, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				---------------�۸���Ϣ-------------
				QBiPrice = 
				{--Q�Ҽ۸�
					type = "LABEL",
					rect = {214 ,111, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				QDianPrice = 
				{--Q��۸�
					type = "LABEL",
					rect = {214, 147, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				KBiPrice = 
				{--��Ҽ۸�
					type = "LABEL",
					rect = {214, 184, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "��Ҽ۸�",
				},
				------------�Ʋ���Ϣ-------------
				PropQBi = 
				{--Q�ҲƲ�
					type = "LABEL",
					rect = {148, 271, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "0",
				},
				PropQDian = 
				{--Q��Ʋ�
					type = "LABEL",
					rect = {148, 307, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "0",
				},
				PropKBi = 
				{--��ҲƲ�
					type = "LABEL",
					rect = {148, 343, 90, 20},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "��ҲƲ�",
				},
				------------------------------------
				--����
				BuyBtn=
				{
					type="BUTTON",
					rect={70,400,97,45},
					--caption="ȷ��",
					bkimage="res/uires/shop/kbi_buy_btn.gsa",
					textsize=12,
					tabstop=1,	
					
					OnClick=function()
						--Win_EndWidget("UI.roomsel.CreateRoomDlg",true);
						if (UI.children.shop.children.BuyItemDlg.shopitemid>0) then
							ShopBuyItem(UI.children.shop.children.BuyItemDlg.shopitemid,1,1);
							Win_ShowWidget("UI.shop.BuyItemDlg", false);
							Win_SetValue("UI.shop.BuyItemDlg", 500, value_channel_timerspace);
							Win_ShowMsgBox("��������У����Ժ�...");
							UI.children.shop.children.BuyItemDlg.operastate=1;
						end;
					end;
				};
				--ȡ��
				CancelBtn=
				{
					type="BUTTON",
					rect={250,400,97,45},
					--caption="ȡ��",
					bkimage="res/uires/common/cancelbtn.gsa",
					textsize=12,
					tabstop=1,	
					OnClick=function()
						Win_EndWidget("UI.shop.BuyItemDlg",false);
					end;
				};
			};
		};
		
		ResetBtn=
		{--�ָ�ԭʼװ��
			type="BUTTON",
			rect={910,120,97,90},
			--caption="ȡ��",
			bkimage="res/uires/shop/initstate.gsa",
			textsize=12,
			tabstop=1,	
			OnClick=function()
				RemoveSelfKartFromUIScene("ROOT");
				RemoveSelfRoleFromUIScene("playermode");
				ReBindSelfPlayer();
				RestoreShopEquip();
				BindSelfKartToScene("ROOT");
				BindSelfRoleToScene("playermode");
				--SelfPlayAnimation(1104);
				cmd_setselfact();
			end;
		};
		ItemInfoTip=
		{--��Ʒ��Ϣtip��ʾ����
			type = "LABEL",
			rect={0,0,285,418},
			bkimage="res/uires/shop/iteminfotip.gsa",			
			textfont=1,
			textsize=12,
			visible=0,

			initlayer=systooltip_layer,
			alphaspeed=0.1,		
			
			children=
			{
				ItemShow = 
				{
					type = "LABEL",
					rect = {20, 50, 135, 120},
					style = wgtstyle_renderitem,
					hosttype = 5;
					hostindex=0;
				},
	
				ItemNameCaption = 
				{--��Ʒ����
					type = "LABEL",
					rect = {170, 40, 110, 60},
					drawcolor={255,255,0,255},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "��Ʒ����",
				},
				
				ItemName = 
				{--��Ʒ����
					type = "LABEL",
					rect = {170, 60, 110, 60},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},

				ItemDescCaption = 
				{--��Ʒ��Ҫ����
					type = "LABEL",
					rect = {50, 195, 180, 60},
					drawcolor={255,255,0,255},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "��Ʒ����",
				},
				
				ItemDesc = 
				{--��Ʒ��Ҫ����
					type = "LABEL",
					rect = {50, 215, 180, 60},
					textsize=12,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				
				ItemAuxDesc = 
				{--��Ʒ�o������
					type = "LABEL",
					rect = {50,300, 180, 80},
					textsize=12,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
			};
		};

		GarageItemInfoTip=
		{--������Ϣtip��ʾ����
			type = "LABEL",
			rect={0,0,285,418},
			bkimage="res/uires/shop/iteminfotip.gsa",			
			textfont=1,
			textsize=12,
			visible=0,

			initlayer=systooltip_layer,
			alphaspeed=0.1,		
			
			children=
			{
				ItemShow = 
				{
					type = "LABEL",
					rect = {20, 50, 135, 120},
					style = wgtstyle_renderitem,
					hosttype = 2;
					hostindex=0;
				},
	
				ItemNameCaption = 
				{--��Ʒ����
					type = "LABEL",
					rect = {170, 40, 110, 60},
					drawcolor={255,255,0,255},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "װ������",
				},
				
				ItemName = 
				{--��Ʒ����
					type = "LABEL",
					rect = {170, 60, 110, 60},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				--[[
				ItemDescCaption = 
				{--��Ʒ��Ҫ����
					type = "LABEL",
					rect = {50, 195, 180, 60},
					drawcolor={255,255,0,255},
					textsize=16,
					textfont=1,
					textstyle=dt_center,			
					caption = "װ������",
				},
				]]
				
				ItemDesc = 
				{--��Ʒ��Ҫ����
					type = "LABEL",
					rect = {50, 215, 180, 60},
					textsize=12,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
				
				ItemAuxDesc = 
				{--��Ʒ�o������
					type = "LABEL",
					rect = {50,300, 180, 80},
					textsize=12,
					textfont=1,
					textstyle=dt_center,			
					caption = "",
				},
			};
		};				
	},
};