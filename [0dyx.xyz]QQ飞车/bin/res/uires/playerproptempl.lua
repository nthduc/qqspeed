playerprop_UIID_=
{
	type="LABEL",
	rect={2, 40 + _UIID_ * (20 + 4), 210, 20},
	style=wgtstyle_template,
	
	children=
	{
		prop_disc=
		{
			type="LABEL",
			rect={0, 3, 40, 16},
			--caption="xx",
			textstyle = dt_magicback + dt_center+dt_vcenter;
			--drawcolor = {0, 0, 0, 255},
		};		
		prop_progressbar=
		{
			type="PROGRESSBAR",
			rect={43, 2, 148, 16},
			--captionrect = {10, 0, 60, 16},
			--bkimage="res/uires/playerinfo/prop_progress_bk.gsa",
			progresslayer="res/uires/playerinfo/expprogress.gsa",
			progressaimer="res/uires/gameui/status/aimer.gsa",
			delaytime=0.1,
			slowtime=0.0,
			step=30,
			textsize=16,
			textfont = 1,
			style=wgtstyle_clipself;					
			--tiptext = "energy",		
		};
		cur_proppoint=
		{
			type="LABEL",
			rect={186,0,40,20},
			caption="255",	
			textstyle = dt_magicback + dt_center;
			--drawcolor = {0, 0, 0, 255},			
		};
		addpoint_btn=
		{
			type="BUTTON",
			rect={222, -1, 17, 21},
			--caption="+",
			--framescheme = {{1, 0, 1, 2}},
			bkimage = "res/uires/playerinfo/add_btn.gsa",
			--drawcolor = {0, 0, 0, 255},
			extendstyle = ui_btn_style_spin,
			threshold = 500;
		
			
			OnClick=function()				
				AddPoints(_UIID_);				
			end;		

			OnMouseOut = function()
				SendPointsToSever();
			end;
		};		
	};
};