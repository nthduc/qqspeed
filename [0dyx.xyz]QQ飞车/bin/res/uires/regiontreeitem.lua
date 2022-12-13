regiontreeitem_UIID_=
{
	type="WIDGET",
	rect={0,_UIID_*30+40,200,25},
	style=wgtstyle_template,
	
	children=
	{
		label=
		{
			type="CHECK",
			rect={0,0,150,25},
			captionrect={0,5,150,20},
			drawcolor={0,0,255,255},
			textsize=12,
			bkimage = "res/uires/PDA/BTN_ratio.gsa",
			textstyle=dt_right,
			
			OnClick=function()
				UI.children.SysPDA.children.childpanel.children.region_panel.children.list.ResetCheck();
				if not Win_IsChecked(selfpath) then
					Win_SetCheck(selfpath, true);
				end;
			end;
		};
		pingbar=
		{
			type="WIDGET",
			rect={210,2,36,17},
			style=wgtstyle_clipself,
			bkimage="res/uires/PDA/serverspeed_fixing.gsa"
		};
	};
};