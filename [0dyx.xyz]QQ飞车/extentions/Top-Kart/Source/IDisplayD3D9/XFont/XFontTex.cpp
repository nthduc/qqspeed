#include "Top-KartPCH.h"
#include "XFontTex.h"
#include <stdlib.h>
#include <cassert>
#include "display.h"
XFontTexSet fontTexSet;

//��ʼ����������
//HRESULT XFontTex::Init( void* pTexture, DWORD texSize, int HzSize)
HRESULT XFontTex::Init( int texSize, int HzSize){
	if (pTexture_) return 0;	//already init!
	
	pTexture_ = CreateFontTex( texSize);

	//DWORD i,j;
	XFontNode*	tmp;

	blockSize_ = HzSize;
	texSize_ = texSize;

	int w = (texSize_/blockSize_);

	//new false head node
	m_List = (XFontNode*) malloc( sizeof(XFontNode));
	if(m_List==NULL)
		return E_FAIL;
	//ѭ��
	m_List->next = m_List;
	m_List->prev = m_List;

	for( int i=0;i<w;i++){
		for(int j=0;j<w;j++){
			tmp = (XFontNode*) malloc( sizeof(XFontNode));	//get a node
			if(tmp==NULL)
				return E_FAIL;
			//set node
			tmp->left = j*blockSize_;
			tmp->top = i*blockSize_;
			tmp->c = 0;
			//insert to list head
			tmp->next = m_List->next;
			tmp->prev = m_List;
			m_List->next = tmp;
			tmp->next->prev = tmp;
		}
	}

//	ClearCache();		//czy+

	return S_OK;
}

#include "../texture9.h"
extern CTexMgr9 texMgr9;
//�ͷŻ�������
void XFontTex::Release(){
	if ( pTexture_ )
	{
		texMgr9.ReleaseHandle((HTEX)pTexture_);
	}
	if(!m_List)
		return;

	XFontNode* cur, *next;	//from head

	for(cur=m_List->next; cur!=m_List; cur=next){
		next = cur->next;		//next node
		free( cur);
	}
	free(m_List);		//�ͷ�αͷ�ڵ�
	m_List = NULL;
}

//reset cache
void XFontTex::ClearCache()
{
	XFontNode *cur = m_List->next;
	while(cur!=m_List){		//not finish?
		cur->c = 0;
		cur = cur->next;
	}
}

//�ѵ�ǰ�����Ƶ���ͷ.
void XFontTex::Move2Head( XFontNode* cur){
	//delete cur from list
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
	
	//insert tmp to list head;
	cur->next = m_List->next;
	cur->prev = m_List;
	m_List->next = cur;
	cur->next->prev = cur;
}

//�õ��ַ�c����Ϣ������CFontTexNode *
XFontNode* XFontTex::GetChar( XFontLib* pFontLib,  unsigned int c, int fontType){
	//�ɻ���ͷ����ʼ����
	c = c | (fontType<<16);
	XFontNode* cur = m_List->next;	//cur -> first node
	m_List->c = c;		//����!	:) clever
	while( cur->c!= c)	{//not find ?
		cur = cur ->next;
	}
	
	if( cur==m_List){	//find ����/not find/���ַ����ڻ����ڲ�?	
		//c���ڻ�����,�ڶ�β�ڵ�д���º���
		cur = cur->prev;		//cur=list tail

		CFontHz* zdata = pFontLib->GetChar(c & 0xFFFF);	//�õ��ַ�c�ĵ�����Ϣ
		//	DEBUG_TXT("%d %d %d %d",tmp->left,tmp->top,zdata->w,zdata->h);
		//	ClearDS(m_lptexture,tmp->left,tmp->top,m_blockHeight,m_blockHeight,0);

		//д����
		DrawChar2Tex( pTexture_, cur->left, cur->top, 0xffff, zdata);
	
		cur ->w = zdata->w;
		cur ->h = zdata->h;
		cur ->c = c;
	}

	Move2Head(cur);
	return cur;
}

//==================== class XFontTexSet =======================
XFontTex* XFontTexSet::getFontTex( int hzSize, int& texWid, int& texHei){
	texWid = texHei = 512;
	
	if (10== hzSize){
		fontTex10_.Init( 512, 10);
		return &fontTex10_;
	}
	if ( 12== hzSize) {
		fontTex12_.Init( 512, 12);
		return &fontTex12_;
	}
	if ( 16== hzSize) {
		fontTex16_.Init( 512, 16);
		return &fontTex16_;
	}
	if ( 24== hzSize) {
		fontTex24_.Init( 512, 24);
		return &fontTex24_;
	}
	assert( 0);		//err size of hz
	return 0;
}