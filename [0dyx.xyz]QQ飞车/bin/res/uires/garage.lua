garagetype=
{
	{
		typeid=ECBT_BASE_KART;--车辆
		tabcover="res/uires/avatarsel/car.gsa";
	};
	{
		typeid=ECBT_BASE_PET;--角色
		tabcover="res/uires/avatarsel/role.gsa";
	};
	{
		typeid=ECBT_BASE_KARTEQP;--装备用道具
		tabcover="res/uires/avatarsel/adorn.gsa";
	};
	{
		typeid=ECBT_BASE_CHARACTOREQP;--装饰用道具
		tabcover = "res/uires/avatarsel/item.gsa",
	};
};

UI.children.garage = 
{
	type="MESSAGEBOX",
	rect={268,140,700,531},
	style=wgtstyle_renderbuffer;
	bkimage="res/uires/avatarsel/bk.gsa",
	initlayer=msgbox_layer-1,
	visible=0,
	alignstyle=alignstyle_hcenter+alignstyle_vcenter,	
	
	_avatarcat=ECBT_BASE_KART;
	_avatarindex=0;
	--产生背景黑影动画
	drawflag = drawflag_backprior,
	custombuffer = 1,
	buffervalue =
	{
		banimation = 1,
		cycletype = 2, 
		restype = 1, 
		resname="scene/splash.nif",			
	},
	
	OnSelfShow=function()
		Win_SetCapture("UI.garage");
		Win_SetFocus("UI.garage");
		cmd_shownavigation();
		cmd_setcamera();
		cmd_setselfact();		
		UI.children.garage.Update();
		UI.children.garage.children.TabBar.Update();
		UI.children.garage.children.AvatarListPanel.Update();
	end;
	
	OnSelfHide=function()
		Win_SetCapture();
		Win_SetFocus();
	end;
	
	Update=function()
	--[[	local avatarcat=UI.children.garage._avatarcat-1;
		local catid="UI.garage.TabBar.AvatarSelCat"..avatarcat;
		Win_ConsoleInfo("sel:"..catid);
		local bCheck = Win_IsChecked(catid);
		if bCheck ~= true then
			Win_SelectSelf(catid);
		end;]]
	end;
	
	children = 
	{
		miniclosebtn=
		{
			type="BUTTON",
			rect={660,-3,40,44};
			bkimage="res/uires/common/miniclose.gsa",
			defaultcancel=1,
			soundid={1007},				
		},	
		TabBar=
		{
			type="WIDGETLIST",
			rect={56,40,140 * 4+20,50},
			style=wgtstyle_clipchild_window,
			extendstyle=wgtlist_style_enum_item;
			template={"res/uires/avatarselcattempl.lua"},
			
			Update=function()
				local count=table.getn(garagetype);
				Win_ConsoleInfo("garagetype num="..count);
				Win_ClearTemplates("UI.garage.TabBar");
				Win_AddTemplate("UI.garage.TabBar", 0, 0, count-1);
			end;
			
			OnInit=function()
				--local count=table.getn(garagetype);
				--Win_ClearTemplates("UI.garage.TabBar");
				--Win_AddTemplate("UI.garage.TabBar", 0, 0, count-1);
			end;
			children=
			{
			};
		};
		PrevBtn=
		{
			type="BUTTON",
			rect={35,48,50,30},
			bkimage="res/uires/roomsel/prevbtn.gsa",
			visible = 0,
			
			OnClick=function()
				local myid="UI.garage.TabBar";
				local sel=Win_GetValue(myid, value_channel_sel);
				Win_SetValue(myid, sel-1,value_channel_sel);
			end;
		};
		NextBtn=
		{
			type="BUTTON",
			rect={615,48,50,30},
			bkimage="res/uires/roomsel/nextbtn.gsa",
			visible = 0,
			
			OnClick=function()
				local myid="UI.garage.TabBar";
				local sel=Win_GetValue(myid, value_channel_sel);
				Win_SetValue(myid, sel+1,value_channel_sel);
			end;
		};
		CloseBtn=
		{
			type="BUTTON",
			rect={294,518,100,40},
			bkimage="res/uires/common/okbtn.gsa",
			
			OnClick=function()
				Win_EndWidget("UI.garage", false);
			end;
		};
		AvatarListPanel=
		{
			type="WIDGET",
			rect={33,89,612,400},
			_page=0,
			_npp=8;
			
			
			OnSelfShow=function()
				UI.children.garage.children.AvatarListPanel.Update();
			end;
			
			OnInit=function()
				--UI.children.garage.children.AvatarListPanel.Update();
			end;
			
			Update=function()
				local avatarcat=UI.children.garage._avatarcat;
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
			
			children=
			{
				list=
				{
					type="WIDGETLIST",							
					rect={0,30,612,400},
					style=wgtstyle_clipchild_window,
					template={"res/uires/avatarlisttemp.lua"},
					
					children=
					{
					};
				};
				PrevBtn=
				{
					type="BUTTON",
					rect={-69,193,50, 40},
					bkimage = "res/uires/common/leftarrow.gsa",
					visible = 1,
					
					OnClick=function()
						local page=UI.children.garage.children.AvatarListPanel._page;
						if page>0 then
							UI.children.garage.children.AvatarListPanel._page=page-1;
							UI.children.garage.children.AvatarListPanel.Update();
						end;
					end;
				};
				NextBtn=
				{
					type="BUTTON",
					rect={641,193,50, 40},
					bkimage = "res/uires/common/rightarrow.gsa",
					visible = 1,
					
					OnClick=function()
						local page=UI.children.garage.children.AvatarListPanel._page;
						local npp=UI.children.garage.children.AvatarListPanel._npp;
						local avatarcat=UI.children.garage._avatarcat;
						local avatarcount=GetAvatarTypeCount(avatarcat);
						if (page+1)*npp < avatarcount then
							UI.children.garage.children.AvatarListPanel._page=page+1;
							UI.children.garage.children.AvatarListPanel.Update();
						end;
					end;
				};
			};
		};
	};
};