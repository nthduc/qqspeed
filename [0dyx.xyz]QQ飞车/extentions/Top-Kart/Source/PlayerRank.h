#ifndef PlayerRank_H
#define PlayerRank_H

#include "singlet.h"
#include "RenderBase.h"
#include "singlet.h"
#include <vector>
#include <algorithm>
using namespace std;

#define MAX_PLAYER_NUM			8	
#define MAX_PLAYERID_LEN		32
#define GRID_NUM				5			//��������
#define NETSPEED_PER_GRID		24			//ÿ������ʱ�䳤��
#define NORMAL_NETWORK_SPEED    200			//��ͨ������
#define IDEAL_NETWORK_SPEED		80			//p2p����Ĵ���ʱ�䣨�����Ժ���Ϊ��λ��
#define HARD_NETWOEK_SPEED		320			//���õ�����
#define NETWORK_UNREACH			1500		//���粻ͨ

#define MinRestmiles			-100000.0	//��С·��

typedef map<unsigned int, NiSourceTexturePtr> UIntKeyToTextureMap;

enum enPlayerColer
{
	enRed,
	enBlue,
	enGreen,
	enColorNum,
};

enum enNetSitu
{
	enGood,
	enBad,
	enUnReach,
	enNetSituNum,

};


struct CPlayerRankInfo
{
	enPlayerColer m_enColor;
	unsigned int m_unUin;
	char m_cNickName[MAXNICKNAMELEN];
	float m_fRestMiles;
	unsigned int m_uiItemHeight;
	bool m_bisHost;
	int m_iIndex;
	unsigned int m_unTeamID;
	bool m_bIsFinish;
	int	m_nNetSpeed;
	int	m_nColor;
	DWORD	m_dwGPs[2];

	bool operator <(const CPlayerRankInfo & other) const
	{
		return m_fRestMiles < other.m_fRestMiles;

	}
};

typedef struct tagPLAY_RANKINFO
{
	UINT	Uin;
	int		nTeamID;
	DWORD	nColor;
	CHAR	szNickName[MAXNICKNAMELEN + 1];
	int		nNetSpeed;
	bool	bFinish;
	DWORD	dwGPs[2];
}PLAY_RANKINFO;

typedef struct tagGAME_RANKINFO
{
	int		nPlayNum;
	PLAY_RANKINFO	ri[MAX_PLAYER_NUM];
}GAME_RANKINFO;

//����������Ļ���
class CPlayerRankItem : public NiMemObject
{

public:
	CPlayerRankItem(){}
	virtual ~CPlayerRankItem(){}
	virtual void ChangePropTex(unsigned int unEletID, NiSourceTexturePtr spTex){}
	virtual void SetPropNum(unsigned int unProp){}
	virtual void MoveTo(unsigned int x, unsigned int y){}
	virtual void DrawItem(NiRendererPtr pkRenderer){}
	virtual void DrawUI(){}
	virtual void UpdateNetSitu(unsigned int uiTime){}
	virtual bool IsHost() = 0;

};

//������ҵ�������
class CHostRankItem : public CPlayerRankItem
{
public:
	CHostRankItem();
	virtual ~CHostRankItem();

	virtual void ChangePropTex(unsigned int unEletID, NiSourceTexturePtr spTex){}
	virtual void SetPropNum(unsigned int unProp){}
	virtual void MoveTo(unsigned int x, unsigned int y);
	virtual void DrawItem(NiRendererPtr pkRenderer);
	virtual void DrawUI();
	virtual void UpdateNetSitu(unsigned int uiTime){}
	virtual bool IsHost(){ return true; }

private:
	int	m_ox;
	int m_oy;
};


//Զ����ҵ�������
class CRemovePlayerRankItem : public CPlayerRankItem 
{
public:
	CRemovePlayerRankItem(char * playNickName, NiSourceTexturePtr tIcon);
	virtual ~CRemovePlayerRankItem();
	void SetTitleIcon(NiSourceTexturePtr pTex);
	void SetNetSituIcon(NiSourceTexturePtr pTex);
	virtual void ChangePropTex(unsigned int unEletID, NiSourceTexturePtr spTex);
	virtual void SetPropNum(unsigned int unProp);
	virtual void UpdateNetSitu(unsigned int uiTime);
	virtual void MoveTo(unsigned int x, unsigned int y);
	virtual void DrawItem(NiRendererPtr pkRenderer );
	virtual void DrawUI();
	virtual bool IsHost() { return false; }

private:	
	string m_strNickName;
	NiScreenElementsPtr m_spTitleIcon;
	NiScreenElementsPtr m_spNetIcon;
	NiScreenElementsPtr m_aspPropElet[2];
	NiSourceTexturePtr m_aspNetSituTexs[enNetSituNum];
	float m_fTitleWidth;
	float m_fTitleHeight;
	float m_fNetIconX;
	float m_fNetIconY;
	float m_fNetIconWidth;
	float m_fNetIconHeight;

	unsigned int m_unNickNameX;
	unsigned int m_unNickNameY;
	unsigned int m_uiIDLen;

	float m_fGridWidthX;
	float m_fGridWidthU;

	unsigned int m_unPropEletNum;
};



class CPlayerRankMgr : public CRenderBase2D, public CSingleT<CPlayerRankMgr>
{
	enum enTurnBackEvent
	{
		enNone = 0,
		enLeft = 1,
		enRight = 2,
	};
public:
	CPlayerRankMgr();
	~CPlayerRankMgr();
	virtual void Update(float fTime);
	virtual void Draw(NiRendererPtr spRenderer);
	
	unsigned short	GetHostCurRank();					//�õ�������ҵ�ǰ����
	unsigned short	GetPlayerRank(unsigned int unUin);	//�õ���ҵ�����
	int	 GetPreviousInterval();				//�õ���ǰһ��ҵľ���
	void SetRaceType(bool bIsTeamRace); 
	void Init(unsigned int* unPlayerUin, int curNumPlayer);
	void SetPlayerFinish(DWORD dwUin);			//������ɱ����ˣ��̶�����
	float GetCompensateValue() { return m_fCompensateValue;}

	GAME_RANKINFO* GetGameRankInfo() { return &m_ri; }

protected:
	enTurnBackEvent GetTriggerTurnBackEvent(float fDist, unsigned int unRemote);
	void TriggerTurnBackEvent(bool bLeft, unsigned int unRemote);
	void StopTurnBackEvent();
	
	// �ٶȲ��� hoverzhoa
	void   UpdateCompensate();
	// �ٶȲ��� end

private:
	CPlayerRankItem* m_apPRankItems[MAX_PLAYER_NUM];
	std::vector<CPlayerRankInfo> m_vPlayerRank; 
	NiSourceTexturePtr m_aspColorTexs[enColorNum];	
	int m_iCurNumOfPlayers;
	unsigned int m_uiCurHeight;

	float  m_fCompensateDis;
	float  m_fCompensateValue;


	unsigned short m_ushHostCurRank;		//������ҵ�ǰ����
	int m_nPreviousInterval;			//���������ǰһ��ҵľ���
	bool m_bIsTeamRace;
	UIntKeyToTextureMap	m_mapAllGPTex;						//���ߵ�ͼƬ
	int m_nFinishPlayerNum;			//�����յ����ҵĸ���
		
	static bool m_bTurnBack;

	GAME_RANKINFO	m_ri;
};

#endif