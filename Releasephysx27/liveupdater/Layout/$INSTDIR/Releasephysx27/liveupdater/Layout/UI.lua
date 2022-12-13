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
                if dwCurrentFileSize>20*1024*1024 then  --大于20M
                        if fSingleProcess<=2 then
                                CurrentFileName = "info.exe"
                                SingleProcess = SingleProcess*100/2
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "正在检测更新环境和版本信息，请稍等……"
                        elseif fSingleProcess>2 and fSingleProcess<=32 then
                                CurrentFileName = "kart.exe"
                                SingleProcess = (SingleProcess-2)*100/30
                                CurrentFileSize = CurrentFileSize*(30/100)
                                DetaileInfo = "正在下载赛车相关信息，请稍等……"
                        elseif fSingleProcess>32 and fSingleProcess<=42 then
                                CurrentFileName = "music.exe"
                                SingleProcess = (SingleProcess-32)*100/10
                                CurrentFileSize = CurrentFileSize*(10/100)
                                DetaileInfo = "正在下载音乐相关信息，请稍等……"
                        elseif fSingleProcess>42 and fSingleProcess<=72 then
                                CurrentFileName = "resource.exe"
                                SingleProcess = (SingleProcess-42)*100/30
                                CurrentFileSize = CurrentFileSize*(30/100)
                                DetaileInfo = "正在下载资源相关信息，请稍等……"
                        elseif fSingleProcess>72 and fSingleProcess<=77 then
                                CurrentFileName = "pet.exe"
                                SingleProcess = (SingleProcess-72)*100/5
                                CurrentFileSize = CurrentFileSize*(5/100)
                                DetaileInfo = "正在下载宠物相关信息，请稍等……"
                        elseif fSingleProcess>77 and fSingleProcess<=97 then
                                CurrentFileName = "avatar.exe"
                                SingleProcess = (SingleProcess-77)*100/20
                                CurrentFileSize = CurrentFileSize*(20/100)
                                DetaileInfo = "正在下载服装相关信息，请稍等……"
                        elseif fSingleProcess>97 and fSingleProcess<=100 then
                                CurrentFileName = "config.exe"
                                SingleProcess = (SingleProcess-97)*100/3
                                CurrentFileSize = CurrentFileSize*(3/100)
                                DetaileInfo = "资源下载完成，更新配置，请稍等……"
                        end
                        CurrentFileName = string.gsub(szCurrentFileName, "%.exe$", "_" .. CurrentFileName)
                elseif dwCurrentFileSize>0 then  --小于20M
                        if fSingleProcess<=5 then
                                CurrentFileName = "info.exe"
                                SingleProcess = SingleProcess*100/5
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "正在检测更新环境和版本信息，请稍等……"
                        elseif fSingleProcess>5 and fSingleProcess<=90 then
                                CurrentFileName = "resource.exe"
                                SingleProcess = (SingleProcess-5)*100/85
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "正在下载资源相关信息，请稍等……"
                        elseif fSingleProcess>90 and fSingleProcess<=100 then
                                CurrentFileName = "config.exe"
                                SingleProcess = (SingleProcess-90)*100/10
                                CurrentFileSize = CurrentFileSize*(2/100)
                                DetaileInfo = "资源下载完成，更新配置，请稍等……"
                        end
                        CurrentFileName = string.gsub(szCurrentFileName, "%.exe$", "_" .. CurrentFileName)
                end
        else
                if enmCurrentStep==4 then
                        DetaileInfo = "正在校验升级包，请稍等……"
                elseif enmCurrentStep==5 then
                        DetaileInfo = "正在尝试修复升级包，请稍等……"
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