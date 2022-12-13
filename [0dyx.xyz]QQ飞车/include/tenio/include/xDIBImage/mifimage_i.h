#ifndef __IMIFIMAGE_I_H__
#define __IMIFIMAGE_I_H__

#include "../teniointerface.h"
#include "dibimage_i.h"

namespace Tenio
{
	typedef struct _mimage_header
	{
		UINT	unVersion;
		UINT	unWidth;
		UINT	unHeight;
		UINT	unAlphaInfo;
		UINT	unFrames;
	}MIF_FILE_HEADER;

	typedef struct _mimage_header_ext
	{
		UINT unLoopCount;			// ����������������ѭ����ѭ������ ��0������ѭ����> 0 ����ѭ����
		UINT unLoopDelay;			// ��������ѭ�����
		//
		UINT unHaveSubLoop;			// �Ƿ�����ѭ��		(0 ����ѭ��  1 ����ѭ��)	//��unAlphaInfo�б�ʾ//���ظ�
		UINT unSubLoopStartFrame;	// ��ѭ����ʼ֡
		UINT unSubLoopEndFrame;		// ��ѭ������֡
		UINT unSubLoopCount;		// ��ѭ������ѭ������ ��0������ѭ����> 0 ����ѭ����
		UINT unSubLoopDelay;		// ��ѭ������ѭ�����
		UINT unTriggerFrame;		// ����֡	---------  ֻ��Ӧ����ѭ������ѭ�����ᴥ��
		char fileName01[50];		// ��Ӧ����ѭ���������ļ���
		char fileName02[50];		// ��Ӧ����ѭ���������ļ���	//����Ժ����µ�֧�ִ�����㲥�ŵ�����ģ�� ���ֶοɲ���
	}MIF_FILE_HEADER_EXT;

	class IMifImage;

	TENIO_MAKE_TYPEID_MAP(IMifImage, COMPONENTID_IMIFIMAGE);

	class IMifImage : public IDIBImage
	{
	public:
		virtual void GetExtInfo(MIF_FILE_HEADER_EXT* pExtInfo) = 0;
	};
};

#endif

// InterfaceName:IMifImage
// SubFolderName:MGClock
