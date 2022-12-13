GarageTab_UIID_=
{
	type = "TABWIN",
	rect = {11, _UIID_ * 62, 40, 62};
	tabrect = {0, 0, 40, 62},
	--tabcover = shoptabsel[_UIID_+1].pic,
	tabcover = shoptabsel[_UIID_+2].pic,--跳过第一个推荐类
	groupid = 141,
	groupstop = _UIID_,
	style = wgtstyle_template;
	
	
	OnCheck = function(bCheck)
		if (bCheck==1) then
			UI.children.shop.children.ItemShow_dlg._cat = _UIID_+1;--跳过第一个推荐类
			--Win_ConsoleInfo("Choosed ShopCat=".._UIID_);
			local realcatid = shoptabsel[_UIID_+2].id;--跳过第一个推荐类
			Win_ConsoleInfo("Garage BaseType="..realcatid);
			UI.children.shop.children.ItemShow_dlg._page = 0;
			UI.children.shop.children.ItemShow_dlg.children.ItemViewerWgt.OnSwitch(realcatid);
		end;
	end;
	
	children=
	{
	};
};