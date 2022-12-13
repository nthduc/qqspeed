
#if !defined (_P2PTALK_H_)
#define _P2PTALK_H_

#include <windows.h>
#include "typedef.h"


namespace VOICETALK
{
    //��������¶�Ա״̬���������£���ȡ�����˵�״̬
    enum
    {
        //team talk 
        TALKER_ST_INIT,             //��ʼ��
        TALKER_ST_UNLINK,           //δ����
        TALKER_ST_NOMIC,            //������״̬��, ��MIC
        TALKER_ST_HAVEMIC,          //������״̬��, ��MIC

        //whisper talk
        TALKER_ST_CALLER,           //�Ǻ�����
        TALKER_ST_RECEIVER         //��������
    };

    //�����ģʽ����������ʱ��
    enum
    {
        MIC_MODE_AUTO_MANAGER,      //���ɵ����Զ�����
        MIC_MODE_MANUAL_MANAGER     //���ɶӳ��ֶ����� 
    };

    //Uin Error 
    enum
    {
        UIN_ERROR = 0
    };

    //��������
    enum
    {
        VT_NONE,          //û��Talk
        VT_INTEAM = 1 ,   //TeamTalk
        VT_INWHISPER = 2  //Whisper
    };

    enum { NO_ANSWER, ANSWER_ACCEPT, ANSWER_FAIL, ANSWER_REFUSE };

    //
    //��ʼ�����컷��
    //
    int  Init ( HWND hWnd, u32 uiMyUin, const u8* pSigBuf, u32 uiSigLen, 
                const u8* pKeyBuf, u32 uiKeyLen, DISPFUNC funcTeamTalkDisp , DISPFUNC funcWhisperDisp, UIN2NAMEFUNC funcUin2Name
         );

    //
    //�ͷ����컷�� :: ע�⣬��������dtor�Զ��������UnInit����ΪP2P������̻�ص������еĽӿڣ������dtor���棬����ӿ��Ѿ���Ч��������������⡣
    //
    void UnInit();
	
	//
	//�Ƿ�����������Ϣ
	//
	void BlockMessage ( bool bBlock );

    //
    //����Լ���Uin
    //
    u32 GetMyUin();

    //
    //�����Լ�������
    //
    void StartHearMySpeech();

    //
    //�ر��Լ�������
    //
    void EndHearMySpeech();

    //
    //���س�ʼ��״̬
    //
    bool IsInitOk();

	//
	//���û�����������
	//
	void SetAECStatus( bool bUseAec );

    //
    //-=-=-=-=-= ������� =-=-=-=-=-=-
    //

    //
    //��������
    //
    int StartTeamTalk ( u32* pUIN, u32 uiMaxMicNum, u32 uiMicMode, u32 uiGatherMicTime );

    //
    //��������
    //
    int EndTeamTalk ( );

    //
    //�����Ա������
    //
    int JoinToTeamTalk ( u32 uiUin );

    //
    //����Ա�߳�����
    //
    int KickFromTeamTalk ( u32 uiUin );

    //
    //��ö�Ա�������ӳ���״̬
    //
    u32 GetTeamMemberState ( u32 uiUin );

    //
    //���󣨷Ƕӳ����ã�
    //
    void GetMic ( );

    //
    //���󣨷Ƕӳ����ã�
    //
    void PutMic ( );

    //
    //�Ӷ�Ա���û���
    //
    bool GetMicFromUin ( u32 uiUin );

    //
    //�������Ա
    //
    bool GiveMicToUin ( u32 uiUin );

    //
    //���ʣ�������
    //
    u32  GetRestMicNum ( ) ;

    //
    //�ı����Ĳ���
    //
    u32  ChangeTeamParam ( u32 uiMicNum, u32 uiMicMode, u32 uiGatherTime );

    //
    //��ȡ���Ĳ���
    //
    void  GetTeamParam ( u32& uiMicNum, u32& uiMicMode, u32& uiGatherTime );

    //
    //�����˷�����
    //
    u32  GetMicNum ( ) ;

    //
    //�õ���˷����ģʽ
    //
    u32  GetMicMode ( ) ;

    //
    //�õ���˷����ʱ��
    //
    u32  GetMicGatherTime ( ) ;    

    //
    //�Ƿ���������
    //
    bool IsInTeamTalk ( ) ;

    //
    //�Ƿ�Ϊ���ĵ��鳤
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
    // -=-=-=-=-=-=-= ���� =-=-=-=-=-=-=-
    //
	
    //
    //������ĳĳ������
    //
    int StartWhisperTalkWith ( u32 uiUin );

    //
    //������ĳĳ������
    //
    int EndWhisperTalkWith ( u32 uiUin );

    //
    //������������
    //
    int EndAllWhisperTalk ( );

    //
    //������ĵ�״̬���Է��������Լ�����
    //
    u32 GetWhisperTalkState( u32 uiUin );
    
    //
    //�������Ķ������Ϣ
    //
    void AddWhisperMemo(const char* szName,u32 uiUin);

    //
    //ɾ����������Ķ�����Ϣ
    //
    void DelWhisperMemo(const char* szName);

    //
    //�Ӽ�����Ѱ�����Ķ���ĺ���
    //
    u32 GetWhisperUinByName(const char* szName);

    //
    //����Ƿ��ڼ�����
    //
    bool IsInWhisperMemo(const char* szName);

    //
    //�Ƿ���������
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
    // -=-=-=-=-=-=-= ͨ�� =-=-=-=-=-=-=-
    //

    //
    //�������е�����״̬
    //
    void EndAllTalk();

    //
    //�Ƿ��ĳ����������
    //
    bool IsTalkWith ( u32 uiUin, int talk_type=VT_INTEAM+VT_INWHISPER );
    bool IsTalkWith ( const char* szName, int talk_type=VT_INTEAM+VT_INWHISPER );

    //
    //��������/������ϵͳ����
    //
    bool IsTeamTalkEnable();
    bool IsWhisperTalkEnable();
    void SetEnableTalk( bool bEnableTeamTalk, bool bEnableWhisperTalk );

    //
    //���ñ��
    //
    void SetPitchShift ( u32 uiTalkType, int uiSemitones );
    
    //
    //��ѭ��
    //
    void Update ( );
    
    //
    //ע����ʾ����
    //
    void RegisterDisplayFunc( u32 uiTalkType, DISPFUNC funcDisp );

    //
    //ע�����ĵĺ��뵽���ֵ�ת��������ע���������ʹ�ã�
    //
    void RegisterUin2NameFunc( UIN2NAMEFUNC funcUin2Name );
    
    //
    //���Ӧ������
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