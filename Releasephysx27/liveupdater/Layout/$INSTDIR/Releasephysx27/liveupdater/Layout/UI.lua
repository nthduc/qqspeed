function UpdateUI(pLiveUpdateUI, szCurrentFileName, nTotalProcess, dwCurrentFileSize, enmCurrentStep, dDownloadSpeed, fSingleProcess)
		---[=[
        SingleProcess = fSingleProcess
        CurrentFileSize = dwCurrentFileSize
        CurrentFileName = szCurrentFileName
	DetaileInfo = ""
	
		--f = io.open("testout.txt", "a+")
		--f:write("123")
		--f:close();
        if enmCurrentStep<=3 then
                if dwCurrentFileSize>20*1024*1024 then  --����20M
                        if fSingleProcess<=2 then
                                CurrentFileName = "info.exe"
                                SingleProcess = SingleProcess*100/2
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "���ڼ����»����Ͱ汾��Ϣ�����Եȡ���"
                        elseif fSingleProcess>2 and fSingleProcess<=32 then
                                CurrentFileName = "kart.exe"
                                SingleProcess = (SingleProcess-2)*100/30
                                CurrentFileSize = CurrentFileSize*(30/100)
                                DetaileInfo = "�����������������Ϣ�����Եȡ���"
                        elseif fSingleProcess>32 and fSingleProcess<=42 then
                                CurrentFileName = "music.exe"
                                SingleProcess = (SingleProcess-32)*100/10
                                CurrentFileSize = CurrentFileSize*(10/100)
                                DetaileInfo = "�����������������Ϣ�����Եȡ���"
                        elseif fSingleProcess>42 and fSingleProcess<=72 then
                                CurrentFileName = "resource.exe"
                                SingleProcess = (SingleProcess-42)*100/30
                                CurrentFileSize = CurrentFileSize*(30/100)
                                DetaileInfo = "����������Դ�����Ϣ�����Եȡ���"
                        elseif fSingleProcess>72 and fSingleProcess<=77 then
                                CurrentFileName = "pet.exe"
                                SingleProcess = (SingleProcess-72)*100/5
                                CurrentFileSize = CurrentFileSize*(5/100)
                                DetaileInfo = "�������س��������Ϣ�����Եȡ���"
                        elseif fSingleProcess>77 and fSingleProcess<=97 then
                                CurrentFileName = "avatar.exe"
                                SingleProcess = (SingleProcess-77)*100/20
                                CurrentFileSize = CurrentFileSize*(20/100)
                                DetaileInfo = "�������ط�װ�����Ϣ�����Եȡ���"
                        elseif fSingleProcess>97 and fSingleProcess<=100 then
                                CurrentFileName = "config.exe"
                                SingleProcess = (SingleProcess-97)*100/3
                                CurrentFileSize = CurrentFileSize*(3/100)
                                DetaileInfo = "��Դ������ɣ��������ã����Եȡ���"
                        end
                        CurrentFileName = string.gsub(szCurrentFileName, "%.exe$", "_" .. CurrentFileName)
                elseif dwCurrentFileSize>0 then  --С��20M
                        if fSingleProcess<=5 then
                                CurrentFileName = "info.exe"
                                SingleProcess = SingleProcess*100/5
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "���ڼ����»����Ͱ汾��Ϣ�����Եȡ���"
                        elseif fSingleProcess>5 and fSingleProcess<=90 then
                                CurrentFileName = "resource.exe"
                                SingleProcess = (SingleProcess-5)*100/85
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "����������Դ�����Ϣ�����Եȡ���"
                        elseif fSingleProcess>90 and fSingleProcess<=100 then
                                CurrentFileName = "config.exe"
                                SingleProcess = (SingleProcess-90)*100/10
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "��Դ������ɣ��������ã����Եȡ���"
                        end
                        CurrentFileName = string.gsub(szCurrentFileName, "%.exe$", "_" .. CurrentFileName)
                end
        else
                if enmCurrentStep==4 then
                        DetaileInfo = "����У�������������Եȡ���"
                elseif enmCurrentStep==5 then
                        DetaileInfo = "���ڳ����޸������������Եȡ���"
                end
        end

        TotalProcess = nTotalProcess
        CurrentStep = enmCurrentStep
        DownloadSpeed = dDownloadSpeed
        LiveUpdateUI = tolua.cast(pLiveUpdateUI, 'Tenio::ILiveUpdateUI')
	
        LiveUpdateUI:ShowDetaileInfo(DetaileInfo)

        LiveUpdateUI:ShowProcess(SingleProcess, TotalProcess)

        LiveUpdateUI:ShowCurrentFileSize(CurrentFileSize)

        LiveUpdateUI:ShowLiveUpdateState(CurrentStep, CurrentFileName, DownloadSpeed)

		--]=]
end