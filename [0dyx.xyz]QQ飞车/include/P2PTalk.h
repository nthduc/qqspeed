
#if !defined (_P2PTALK_H_)
#define _P2PTALK_H_

#include <windows.h>
#include "typedef.h"


namespace VOICETALK
{
    //组队聊天下队员状态：在组聊下，获取其他人的状态
    enum
    {
        //team talk 
        TALKER_ST_INIT,             //初始化
        TALKER_ST_UNLINK,           //未连接
        TALKER_ST_NOMIC,            //在聊天状态下, 无MIC
        TALKER_ST_HAVEMIC,          //在聊天状态下, 有MIC

        //whisper talk
        TALKER_ST_CALLER,           //是呼叫者
        TALKER_ST_RECEIVER         //被呼叫者
    };

    //麦分配模式：创建组聊时用
    enum
    {
        MIC_MODE_AUTO_MANAGER,      //麦由电脑自动管理
        MIC_MODE_MANUAL_MANAGER     //麦由队长手动管理 
    };

    //Uin Error 
    enum
    {
        UIN_ERROR = 0
    };

    //聊天类型
    enum
    {
        VT_NONE,          //没有Talk
        VT_INTEAM = 1 ,   //TeamTalk
        VT_INWHISPER = 2  //Whisper
    };

    enum { NO_ANSWER, ANSWER_ACCEPT, ANSWER_FAIL, ANSWER_REFUSE };

    //
    //初始化聊天环境
    //
    int  Init ( HWND hWnd, u32 uiMyUin, const u8* pSigBuf, u32 uiSigLen, 
                const u8* pKeyBuf, u32 uiKeyLen, DISPFUNC funcTeamTalkDisp , DISPFUNC funcWhisperDisp, UIN2NAMEFUNC funcUin2Name
         );

    //
    //释放聊天环境 :: 注意，不能依赖dtor自动调用这个UnInit，因为P2P网络进程会回调该类中的接口，如果在dtor里面，该类接口已经无效，会产生调用问题。
    //
    void UnInit();
	
	//
	//是否阻塞文字消息
	//
	void BlockMessage ( bool bBlock );

    //
    //获得自己的Uin
    //
    u32 GetMyUin();

    //
    //测试自己的声音
    //
    void StartHearMySpeech();

    //
    //关闭自己的声音
    //
    void EndHearMySpeech();

    //
    //返回初始化状态
    //
    bool IsInitOk();

	//
	//设置回声消除功能
	//
	void SetAECStatus( bool bUseAec );

    //
    //-=-=-=-=-= 组队聊天 =-=-=-=-=-=-
    //

    //
    //开启组聊
    //
    int StartTeamTalk ( u32* pUIN, u32 uiMaxMicNum, u32 uiMicMode, u32 uiGatherMicTime );

    //
    //结束组聊
    //
    int EndTeamTalk ( );

    //
    //加入队员到组聊
    //
    int JoinToTeamTalk ( u32 uiUin );

    //
    //将队员踢出组聊
    //
    int KickFromTeamTalk ( u32 uiUin );

    //
    //获得队员（包括队长）状态
    //
    u32 GetTeamMemberState ( u32 uiUin );

    //
    //拿麦（非队长调用）
    //
    void GetMic ( );

    //
    //放麦（非队长调用）
    //
    void PutMic ( );

    //
    //从队员处拿回麦
    //
    bool GetMicFromUin ( u32 uiUin );

    //
    //给麦给队员
    //
    bool GiveMicToUin ( u32 uiUin );

    //
    //获得剩余的麦数
    //
    u32  GetRestMicNum ( ) ;

    //
    //改变组聊参数
    //
    u32  ChangeTeamParam ( u32 uiMicNum, u32 uiMicMode, u32 uiGatherTime );

    //
    //获取组聊参数
    //
    void  GetTeamParam ( u32& uiMicNum, u32& uiMicMode, u32& uiGatherTime );

    //
    //获得麦克风总数
    //
    u32  GetMicNum ( ) ;

    //
    //得到麦克风管理模式
    //
    u32  GetMicMode ( ) ;

    //
    //得到麦克风回收时间
    //
    u32  GetMicGatherTime ( ) ;    

    //
    //是否在组聊中
    //
    bool IsInTeamTalk ( ) ;

    //
    //是否为组聊的组长
    //    
    bool IsTeamLeader ( );

    //
    //friend list
    //
    int AddToTeamFriendList ( u32 uiUin );
    int RemoveFromTeamFriendList ( u32 uiUin );
    void ClearTeamFriendList ( );
    bool IsInTeamFriendList ( u32 uiUin );

    //
    // -=-=-=-=-=-=-= 密聊 =-=-=-=-=-=-=-
    //
	
    //
    //开启和某某的密聊
    //
    int StartWhisperTalkWith ( u32 uiUin );

    //
    //结束和某某的密聊
    //
    int EndWhisperTalkWith ( u32 uiUin );

    //
    //结束所有密聊
    //
    int EndAllWhisperTalk ( );

    //
    //获得密聊的状态（对方发起还是自己发起）
    //
    u32 GetWhisperTalkState( u32 uiUin );
    
    //
    //记忆密聊对象的信息
    //
    void AddWhisperMemo(const char* szName,u32 uiUin);

    //
    //删除记忆的密聊对象信息
    //
    void DelWhisperMemo(const char* szName);

    //
    //从记忆中寻找密聊对象的号码
    //
    u32 GetWhisperUinByName(const char* szName);

    //
    //检查是否在记忆中
    //
    bool IsInWhisperMemo(const char* szName);

    //
    //是否在密聊中
    //
    bool IsInWhisperTalk ( );

    //
    // friend list
    // 
    int AddToWhisperFriendList ( u32 uiUin );
    int RemoveFromWhisperFriendList ( u32 uiUin );
    void ClearWhisperFriendList ( );
    bool IsInWhisperFriendList ( u32 uiUin );


    //
    // -=-=-=-=-=-=-= 通用 =-=-=-=-=-=-=-
    //

    //
    //结束所有的聊天状态
    //
    void EndAllTalk();

    //
    //是否和某人正在聊天
    //
    bool IsTalkWith ( u32 uiUin, int talk_type=VT_INTEAM+VT_INWHISPER );
    bool IsTalkWith ( const char* szName, int talk_type=VT_INTEAM+VT_INWHISPER );

    //
    //聊天允许/不允许，系统设置
    //
    bool IsTeamTalkEnable();
    bool IsWhisperTalkEnable();
    void SetEnableTalk( bool bEnableTeamTalk, bool bEnableWhisperTalk );

    //
    //设置变调
    //
    void SetPitchShift ( u32 uiTalkType, int uiSemitones );
    
    //
    //主循环
    //
    void Update ( );
    
    //
    //注册显示函数
    //
    void RegisterDisplayFunc( u32 uiTalkType, DISPFUNC funcDisp );

    //
    //注册组聊的号码到名字的转换函数（注意仅供组聊使用）
    //
    void RegisterUin2NameFunc( UIN2NAMEFUNC funcUin2Name );
    
    //
    //获得应用类型
    //
    u8   GetAppType ( );
}


namespace VOICETALK
{
    bool SetMicBoost( bool bBoost );
    bool GetMicBoost( bool& bBoost );
    bool SetMicMute( bool bMute );
    bool GetMicMute( bool& bMute );
    bool GetMicPlayVolumeRange( u32& uiMin, u32& uiMax );
    bool GetMicRecVolumeRange( u32& uiMin, u32& uiMax );
    bool SetMicRecVolume( u32 uiVol );
    bool GetMicRecVolume( u32& uiVol );
    bool SetMicPlayVolume( u32 uiVol );
    bool GetMicPlayVolume( u32& uiVol );
}


#endif