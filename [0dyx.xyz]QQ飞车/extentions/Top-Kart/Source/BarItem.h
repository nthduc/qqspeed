////////////////////////////////////////////////
//     FileName:    BarItem.h
//     Author:      Rayhuang
//     Date:        2007-3-20
//     Description: ���������֣���������������
////////////////////////////////////////////////


#pragma once 


class CBarItem : public NiMemObject
{
	enum enBarKinds
	{
		enBaseBar,
		enPowerBar,
		enAnimBar1,
		enAnimBar2,
		enBarNum,
	};
public:
	CBarItem();
	~CBarItem();

	//��Ⱦ����
	void Draw(NiRendererPtr spRenderer);

	//����������
	void ReSetBar();	

	//���ý�������λ��
	void MoveTo(float fLeft, float fTop);

	//���ý�������
	void SetWidth(float fWidth);

	//���ý�������
	void SetHeight(float fHeight);

	//ÿ��λ��ֵ�������ĳ���
	void SetBarLenPerUnit(float fLen);

	//���ý�����������ֵ
	void SetFullUnits(unsigned int unUnits);

	//���ý�������ǰ����ֵ
	void SetCurUnits(unsigned int unUnits);

	//���ý���������ͼ 
	void SetBarTex(unsigned int unType, NiSourceTexturePtr spTex);

	void SetBarStartLen(float fLen);

	//���ö���ʱ����
private:
	NiScreenElementsPtr m_aspBarElet[enBarNum];	

	float m_fElementX;				//������λ��x		
	float m_fElementY;				//������λ��y
	float m_fElementWidth;			//�������ĳ���
	float m_fElementHeight;			//�������Ŀ�
	float m_fBarStartLen;			//power bar��ʼ��λ�ã������������ͼ
	float m_fBarLenPerUnit;			//ÿ��λ��ֵ�������ĳ���


	float m_fBarCurWidth;			//�������ĵ�ǰ���ȣ����ֳ��ĳ��ȣ�
	unsigned int m_unCurUnits;		//��ǰ�Ľ���ֵ���統ǰ������ֵ����ʵ�ʵ���ֵ��
	unsigned int m_unMaxUnits;		//������ֵ��ʵ�ʵ���ֵ��

	DWORD m_dwAnimLastTime;
	DWORD m_dwAnimTime;
};