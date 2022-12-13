GarageItem_UIID_=
{
	type = "BUTTON",
	style=wgtstyle_template,
	bkimage = "res/uires/shop/shopcell.gsa",
	framescheme={{0,0,1,1}},	--解决只有两帧图片的gsa图，最后一帧作为MouseOver显示用
	rect = {math.mod(_UIID_, 2)*241 + 13, math.floor(math.mod(_UIID_, 8)/2) * 127 + 15, 247, 134},
	avataritemid=0;	--avatar真实id
	turnangle=0;	--商品展示窗口当前商品模型旋转角度,绕正Z轴顺时针
	turnstate=0;	--旋转状态，0=不旋转，1=旋转
	style = wgtstyle_template;

	OnInit=function()
		--Win_ConsoleInfo("GarageItemtempl.lua CreateGarageItem=".._UIID_);
		local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
		local item,itemname=GetAvatarInfoByType(realcatid,_UIID_);
		if item ~= nil then --获取item具体参数并填充
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid=item.itemid;
			Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.ItemShow",item.itemid,value_channel_hostindex);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.ItemName",itemname);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.ItemNum","道具总数:"..item.itemnum);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.AvaPeriod","有效期:"..item.availperiod);
			if item.status==0 then
				Win_ConsoleInfo("Set garage check");
				Win_ShowWidget("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.checklabel",false);
			else
				Win_ShowWidget("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.checklabel",true);
			end;		
		end;
	end;

	OnTimer = function()
		local l_turnstate=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnstate;
		if (l_turnstate == 1) then --转动模型
			local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid;
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnangle=
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnangle+3;
			l_turnangle=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnangle;
			Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem".._UIID_,30,value_channel_timerspace);
			
			
			
			
			ShopSelfShowTurn(l_turnangle,idTmp);
		end;					
	end;
	
	SetCheck=function(bCheck)
		local myid="UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem".._UIID_;
		if bCheck==0 then
			Win_ShowWidget(myid..".checklabel",false);
		end;
		if bCheck~=0 then
			Win_ShowWidget(myid..".checklabel",true);
		end;
	end;
	
	children = 
	{
		ItemShow = 
		{
			type = "BUTTON",
			rect = {10, 14, 127, 107},
			style = wgtstyle_renderitem,
			hosttype = 2;
			hostindex=0;
			_mousein=0;
			
			OnTimer=function()
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.children.ItemShow._mousein==1) then
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.children.ItemShow.SetTipPos();
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.ItemShow",50,value_channel_timerspace);
				end;
			end;
			
			OnMouseIn = function()
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.children.ItemShow._mousein=1;
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid;
				if UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnstate==0  and idTmp>0 then
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem".._UIID_,30,value_channel_timerspace);
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnstate=1;
				end;
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid>0) then
					--UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.children.ItemShow.SetTipPos();
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem_UIID_.ItemShow",50,value_channel_timerspace);
				end;
			end;
			
			OnMouseOut = function()
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.children.ItemShow._mousein=0;
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnstate=0;
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.turnangle=0;
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid;
				ShopSelfShowTurn(0,idTmp);
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid>0) then
					Win_ShowWidget("UI.shop.GarageItemInfoTip", false);
				end;
			end;
			
			SetTipPos=function()
				--显示tip
				local rc=Win_GetRect("UI.shop.ItemShow_dlg.ItemViewerWgt.GarageItem".._UIID_);
				local moinfo=Win_GetCursorInfo();				
				local rcMain=Win_GetRect("UI.shop.GarageItemInfoTip");
				local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				local item,itemname=GetAvatarInfoByType(realcatid,_UIID_);
				local attr=GetDisplayAttr();
				rcMain.x = moinfo.x+ rc.w/3;
				if ((moinfo.y+rcMain.h-30)>attr.height) then
					rcMain.y = attr.height-rcMain.h;
				else
					rcMain.y = moinfo.y-30;
				end;
				
				Win_SetRect("UI.shop.GarageItemInfoTip",rcMain.x,rcMain.y,rcMain.w,rcMain.h);
				Win_SetValue("UI.shop.GarageItemInfoTip.ItemShow",
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid,value_channel_hostindex);
				Win_SetText("UI.shop.GarageItemInfoTip.ItemName",itemname);
				--[[
				Win_SetText("UI.shop.GarageItemInfoTip.ItemDesc",item.desc.generaldesc);
				Win_SetText("UI.shop.GarageItemInfoTip.ItemAuxDesc",item.desc.attributedesc);
				]]
				Win_ShowWidget("UI.shop.GarageItemInfoTip", true);
			end;
			
			OnClick = function()
				--装备
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid;
				if (idTmp>0) then
					ChangeAvatarStatus(GetMyInfo().uin,idTmp);
				end;
			end;

		},
		
		ItemName = 
		{
			type = "LABEL",
			rect  = {145, 13, 90, 20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,			
			caption = "",
		},
		
		ItemNum = 
		{
			type = "LABEL",
			rect  = {145, 39, 90, 20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,			
			
			caption = "",
		},
		
		AvaPeriod = 
		{
			type = "LABEL",
			rect = {145, 66, 90, 20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,			
			
			caption = "",
		},
		
		checklabel=
		{
			type="LABEL",
			rect={103,12,0,60},
			bkimage="res/uires/avatarsel/checkicon.gsa",
			visible=0,
		};
		
		EquipBtn = 
		{
			type = "BUTTON",
			rect = {148, 90, 90, 39},
			bkimage = "res/uires/shop/equip.gsa",
			OnClick = function()
				--装备
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.GarageItem_UIID_.avataritemid;
				if (idTmp>0) then
					ChangeAvatarStatus(GetMyInfo().uin,idTmp);
				end;
			end;
		},
	},
};