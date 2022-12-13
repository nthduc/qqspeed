////////////////////////////////////////////////
//     FileName:    BarItem.h
//     Author:      Rayhuang
//     Date:        2007-3-20
//     Description: 能量条表现（可用做进度条）
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

	//渲染对象
	void Draw(NiRendererPtr spRenderer);

	//进度条归零
	void ReSetBar();	

	//设置进度条条位置
	void MoveTo(float fLeft, float fTop);

	//设置进度条宽
	void SetWidth(float fWidth);

	//设置进度条高
	void SetHeight(float fHeight);

	//每单位数值进度条的长度
	void SetBarLenPerUnit(float fLen);

	//设置进度条满的数值
	void SetFullUnits(unsigned int unUnits);

	//设置进度条当前的数值
	void SetCurUnits(unsigned int unUnits);

	//设置进度条的贴图 
	void SetBarTex(unsigned int unType, NiSourceTexturePtr spTex);

	void SetBarStartLen(float fLen);

	//设置动画时间间隔
private:
	NiScreenElementsPtr m_aspBarElet[enBarNum];	

	float m_fElementX;				//进度条位置x		
	float m_fElementY;				//进度条位置y
	float m_fElementWidth;			//进度条的长度
	float m_fElementHeight;			//进度条的宽
	float m_fBarStartLen;			//power bar开始的位置，相对于整张贴图
	float m_fBarLenPerUnit;			//每单位数值进度条的长度


	float m_fBarCurWidth;			//进度条的当前长度（表现出的长度）
	unsigned int m_unCurUnits;		//当前的进度值（如当前的能量值）（实际的数值）
	unsigned int m_unMaxUnits;		//最大进度值（实际的数值）

	DWORD m_dwAnimLastTime;
	DWORD m_dwAnimTime;
};