UI.children.taskescbox=
{
 	type="MESSAGEBOX",
	rect = { 0, 0, 339, 230},
	bkimage="res/uires/common/msgbox.gsa",			
	visible = 0,
	caption = "�����������ѵ����?",
	initlayer=msgbox_layer,
			
	OnSelfShow = function()	
		
	end;

	children =
	{
		okbtn=
		{
			type="BUTTON",
			rect={50,215,90,40},
			bkimage="res/uires/common/okbtn.gsa",
			
			OnClick = function()	
				local myinfo = GetMyInfo();
				RequestReportCurrentOpt(myinfo.uin, 30001);
				Win_ShowWidget("UI.taskescbox", false);
				ShowPointer(false);
			end;
		},
		cancelbtn=
		{
			type="BUTTON",
			rect={207,215,90,40},
			bkimage="res/uires/common/cancelbtn.gsa",
			
			OnClick = function()	
				Win_ShowWidget("UI.taskescbox", false);
				ShowPointer(false);
			end;
		},
	},	
};

UI.children.taskf4box=
{

	--Win_ShowMsgBox("�������ڿ����С�����");
 	type="MESSAGEBOX",
	rect = { 0, 0, 339, 230},
	bkimage="res/uires/common/msgbox.gsa",			
	visible = 0,
	caption = "�������ڿ����С�����",
	initlayer=msgbox_layer,
			
	OnSelfShow = function()	
		
	end;
	
		
	children =
	{
		okbtn=
		{
			type="BUTTON",
			rect={123,215,90,40},
			bkimage="res/uires/common/okbtn.gsa",
			
			OnClick = function()	
				Win_ShowWidget("UI.taskf4box", false);
				ShowPointer(false);				
			end;
		},		
	},
};