TPFUI=
{
	OK = function(uilib, dwID_Click)
		---在编辑器里设置Stc_Info的CustomID为10001,
		---在编辑器里设置Edt_ID的CustomID为10002,
		---在编辑器里设置Edt_ID的CustomID为10003,
		---通过CustomID获得ControlID
		local dwStaticInfo = uilib:GetControlIDbyCustomID(10001);
		local pControl = uilib:GetControlPtr(dwStaticInfo);
		local pStaticInfo = tolua.cast(pControl, "Tenio::ITStatic");
		
		local dwEdt_ID = uilib:GetControlIDbyCustomID(10002);
		pControl = uilib:GetControlPtr(dwEdt_ID);
		local pEdt_ID = tolua.cast(pControl, "Tenio::ITEdit");
		local strID = pEdt_ID:GetText();
		
		local dwEdt_PSW = uilib:GetControlIDbyCustomID(10003);
		pControl = uilib:GetControlPtr(dwEdt_PSW);
		local pEdt_PSW = tolua.cast(pControl, "Tenio::ITEdit");
		local nPSWLen = pEdt_PSW:GetPasswordLen();
		
		local strWarning = "";
		if strID=="" then
			strWarning = TPFGetTranslation(ID_IDWARNING);
		elseif nPSWLen==0 then
			strWarning = TPFGetTranslation(ID_PSWWARNING);
		end;
		
		pStaticInfo:SetText(strWarning);
		pStaticInfo:Show(1);
		pStaticInfo:Show(0);
	end;
	
	Cancel = function(uilib, dwID_Click)
		---在编辑器里设置Root Dialog的CustomID为10000,通过CustomID获得ControlID
		local dwRootDlg = uilib:GetControlIDbyCustomID(10000);
		local pControl = uilib:GetControlPtr(dwRootDlg);
		local pRootDlg = tolua.cast(pControl, "Tenio::ITDialog");
		
		pRootDlg:EndModal(0);
	end;
};