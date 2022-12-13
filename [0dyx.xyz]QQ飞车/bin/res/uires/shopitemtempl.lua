ShopItem_UIID_=
{
	type = "BUTTON",
	style=wgtstyle_template,
	bkimage = "res/uires/shop/shopcell.gsa",
	framescheme={{0,0,1,1}},	--解决只有两帧图片的gsa图，最后一帧作为MouseOver显示用
	rect = {math.mod(_UIID_, 2)*241 + 13, math.floor(math.mod(_UIID_, 8)/2) * 127 + 15, 247, 134},
	shopitemid=0;	--商品真实id
	turnangle=0;	--商品展示窗口当前商品模型旋转角度,绕正Z轴顺时针
	turnstate=0;	--旋转状态，0=不旋转，1=旋转
	style = wgtstyle_template;

	OnInit=function()
		--Win_ConsoleInfo("ShopItemTempl.lua Create ShopItem=".._UIID_);
		local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
		local item=GetShopCatItemId(realcatid,_UIID_);
		if item ~= nil then --获取item具体参数并填充
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid=item.itemid;
			Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.ItemShow",item.itemid,value_channel_hostindex);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.ItemName",item.itemname);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.QBiPrice","Q币价格:"..item.price.qbi);--item.itemname);
			Win_SetText("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.KBiPrice","酷币价格:"..item.price.kbi);--item.desc.generaldesc);
		end;
	end;

	OnTimer = function()
		local l_turnstate=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnstate;
		if (l_turnstate == 1) then --转动模型
			local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid;
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnangle=
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnangle+3;
			l_turnangle=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnangle;
			Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem".._UIID_,30,value_channel_timerspace);
			
			
			
			
			ShopSelfShowTurn(l_turnangle,idTmp);
		end;					
	end;
	
	
	children = 
	{
		ItemShow = 
		{
			type = "BUTTON",
			rect = {10, 14, 127, 107},
			style = wgtstyle_renderitem,
			hosttype = 5;
			hostindex=0;
			_mousein=0;
			
			OnTimer=function()
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.children.ItemShow._mousein==1) then
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.children.ItemShow.SetTipPos();
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.ItemShow",50,value_channel_timerspace);
				end;
			end;
			
			OnMouseIn = function()
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.children.ItemShow._mousein=1;
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid;
				if UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnstate==0  and idTmp>0 then
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem".._UIID_,30,value_channel_timerspace);
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnstate=1;
				end;
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid>0) then
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.children.ItemShow.SetTipPos();
					Win_SetValue("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem_UIID_.ItemShow",50,value_channel_timerspace);
				end;
			end;
			
			OnMouseOut = function()
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.children.ItemShow._mousein=0;
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnstate=0;
				UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.turnangle=0;
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid;
				ShopSelfShowTurn(0,idTmp);
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid>0) then
					Win_ShowWidget("UI.shop.ItemInfoTip", false);
				end;
			end;
			
			SetTipPos=function()
				--显示tip
				local rc=Win_GetRect("UI.shop.ItemShow_dlg.ItemViewerWgt.ShopItem".._UIID_);
				local moinfo=Win_GetCursorInfo();				
				local rcMain=Win_GetRect("UI.shop.ItemInfoTip");
				local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				local item=GetShopCatItemId(realcatid,_UIID_);
				local attr=GetDisplayAttr();
				rcMain.x = moinfo.x+ rc.w/3;
				if ((moinfo.y+rcMain.h-30)>attr.height) then
					rcMain.y = attr.height-rcMain.h;
				else
					rcMain.y = moinfo.y-30;
				end;
				
				Win_SetRect("UI.shop.ItemInfoTip",rcMain.x,rcMain.y,rcMain.w,rcMain.h);
				Win_SetValue("UI.shop.ItemInfoTip.ItemShow",
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid,value_channel_hostindex);
				Win_SetText("UI.shop.ItemInfoTip.ItemName",item.itemname);
				Win_SetText("UI.shop.ItemInfoTip.ItemDesc",item.desc.generaldesc);
				Win_SetText("UI.shop.ItemInfoTip.ItemAuxDesc",item.desc.attributedesc);
				Win_ShowWidget("UI.shop.ItemInfoTip", true);
			end;
			
			OnClick = function()
				local idTmp=UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid;
				EquipShopItem(idTmp);
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
		
		QBiPrice = 
		{
			type = "LABEL",
			rect  = {145, 39, 90, 20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,			
			
			caption = "",
		},
		
		KBiPrice = 
		{
			type = "LABEL",
			rect = {145, 66, 90, 20},
			textsize=12,
			textfont=1,
			textstyle=dt_center,			
			
			caption = "",
		},
		
		Buy_btn = 
		{
			type = "BUTTON",
			rect  = {144, 89, 52, 39},
			--caption = "购买",
			bkimage = "res/uires/shop/buy.gsa",
			OnClick = function()
				if (UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid>0) then
					UI.children.shop.children.BuyItemDlg.shopitemid=
						UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid;			
					Win_ShowWidget("UI.shop.BuyItemDlg", true);
				end;						
				local realcatid = shoptabsel[UI.children.shop.children.ItemShow_dlg._cat+1].id;
				local item=GetShopCatItemId(realcatid,_UIID_);
				if item ~= nil then --获取item具体参数并填充
					local myinfo = GetMyInfo();
					UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.children.ShopItem_UIID_.shopitemid=item.itemid;
					Win_SetValue("UI.shop.BuyItemDlg.ItemShow",item.itemid,value_channel_hostindex);
					Win_SetText("UI.shop.BuyItemDlg.ItemName",item.itemname);
					Win_SetText("UI.shop.BuyItemDlg.QBiPrice",item.price.qbi);
					Win_SetText("UI.shop.BuyItemDlg.QDianPrice",item.price.qdian);
					Win_SetText("UI.shop.BuyItemDlg.KBiPrice",item.price.kbi);
					Win_SetText("UI.shop.BuyItemDlg.PropKBi",myinfo.playerdbinfo.money);
			
				end;
			end;
		},
		
		Present_btn = 
		{
			type = "BUTTON",
			rect = {191, 89, 52, 39},
			--caption = "赠送",
			bkimage = "res/uires/shop/present.gsa",
			OnClick = function()
			end;
		},
	},
};