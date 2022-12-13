////////////////////////////////////////////////
//     FileName:    BlinkItem.h
//     Author:      Rayhuang
//     Date:        2007-3-20
//     Description: 进度条闪动动画效果
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
	
	//开始闪烁动画
	void StartAnim();

	//停止闪烁动画
	void StopAnim();

	//设置动画时间长度
	void SetAnimTimeLen(unsigned int unTime);

	//返回动画时间长度
	unsigned int GetAnimTimeLen();

	//设置闪烁条位置
	void MoveTo(float fLeft, float fTop);

	//设置动画条纹理
	void SetTex(unsigned int unType, NiSourceTexturePtr spTex);

	//设置宽度
	void SetWidth(float fWidth);

	//设置高度
	void SetHeight(float fHeight);

	//渲染
	void Draw(NiRendererPtr spRenderer);

private:
	NiScreenElementsPtr m_aspItemElet[enTexNum];

	float m_fElementX;			//屏幕位置X
	float m_fElementY;			//屏幕位置Y
	float m_fElementWidth;		//宽
	float m_fElementHeight;		//高

	unsigned int m_unAnimTimeLen;	//动画时间长度
	unsigned int m_unAnimStartTime;	//动画开始时间

	bool m_bIsAnim;					//是否播放动画
	unsigned int m_unShowTexId;		//渲染的元素下标
};