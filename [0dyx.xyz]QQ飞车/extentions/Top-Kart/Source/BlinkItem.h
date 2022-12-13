////////////////////////////////////////////////
//     FileName:    BlinkItem.h
//     Author:      Rayhuang
//     Date:        2007-3-20
//     Description: ��������������Ч��
////////////////////////////////////////////////


#pragma once 

class CBlinkItem : public NiMemObject
{
	enum enmEletType
	{
		ELTT_TEX1 = 1,
		ELTT_TEX2 = 2,
		enTexNum = 2,
	};
public:
	CBlinkItem();
	~CBlinkItem();
	
	//��ʼ��˸����
	void StartAnim();

	//ֹͣ��˸����
	void StopAnim();

	//���ö���ʱ�䳤��
	void SetAnimTimeLen(unsigned int unTime);

	//���ض���ʱ�䳤��
	unsigned int GetAnimTimeLen();

	//������˸��λ��
	void MoveTo(float fLeft, float fTop);

	//���ö���������
	void SetTex(unsigned int unType, NiSourceTexturePtr spTex);

	//���ÿ��
	void SetWidth(float fWidth);

	//���ø߶�
	void SetHeight(float fHeight);

	//��Ⱦ
	void Draw(NiRendererPtr spRenderer);

private:
	NiScreenElementsPtr m_aspItemElet[enTexNum];

	float m_fElementX;			//��Ļλ��X
	float m_fElementY;			//��Ļλ��Y
	float m_fElementWidth;		//��
	float m_fElementHeight;		//��

	unsigned int m_unAnimTimeLen;	//����ʱ�䳤��
	unsigned int m_unAnimStartTime;	//������ʼʱ��

	bool m_bIsAnim;					//�Ƿ񲥷Ŷ���
	unsigned int m_unShowTexId;		//��Ⱦ��Ԫ���±�
};