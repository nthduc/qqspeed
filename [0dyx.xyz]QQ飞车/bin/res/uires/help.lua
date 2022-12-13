UI.children.help=
{
	type="MESSAGEBOX",
	rect = {0, 0, 693, 526},
	bkimage="res/uires/help/help_bk.gsa",
	initlayer=msgbox_layer-1,			
	visible = 0,
	style=wgtstyle_topmost+wgtstyle_renderbuffer;
	--drawflag = drawflag_clientprior,
	drawflag = drawflag_backprior,
	visible=0,
	defaultendok=0,
	alphaspeed=0.25,
	alphafactor=0.0,
	
	custombuffer = 1,
	buffervalue =
	{
		banimation = 1,
		cycletype = 2, 
		restype = 1, 
		resname="scene/splash.nif",			
	},
	
	OnSelfShow = function()
		Win_SetCapture("UI.help");
		Win_SetFocus("UI.help");
	end;
	
	children = 
	{
		helptab=
		{
			type = "CONTAINER";
			rect = {67, 50, 560, 44},
			
			template = {"res/uires/helptabtempl.lua",},
			
			OnInit = function()
				Win_ClearTemplates("UI.help.helptab");
				Win_AddTemplate("UI.help.helptab", 0, 0, 3);
				Win_SetCheck("UI.help.helptab.helptab0", true);
			end;
			
			children = 
			{
				
			};
		};
		miniclosebtn=
		{
			type="BUTTON",
			rect={661,-10,40,44};
			bkimage="res/uires/common/miniclose.gsa",
			defaultcancel=1,
			soundid={1007},				
		},	
		CancelBtn=
		{
			type="BUTTON",
			rect={326,512,100,40},
			bkimage="res/uires/common/affirmbtn.gsa",		
			defaultcancel=1,
		};
	};
};