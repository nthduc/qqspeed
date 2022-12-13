ShopItemTab_UIID_=
{
	type = "TABWIN",
	rect = {11, _UIID_ * 62, 40, 62};
	tabrect = {0, 0, 40, 62},
	tabcover = shoptabsel[_UIID_+1].pic,
	groupid = 40,
	groupstop = _UIID_,
	style = wgtstyle_template;
	
	
	OnCheck = function(bCheck)
		if (bCheck == 1) then
			UI.children.shop.children.ItemShow_dlg._cat = _UIID_;
			--Win_ConsoleInfo("Choosed ShopCat=".._UIID_);
			local realcatid = shoptabsel[_UIID_+1].id;
			Win_ConsoleInfo("Shot BaseType="..realcatid);
			UI.children.shop.children.ItemShow_dlg._page = 0;
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch(realcatid);
		end;
	end;
	
	children=
	{
	};
};