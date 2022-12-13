#ifndef __MIFIMAGELAYER_I_H__
#define __MIFIMAGELAYER_I_H__

#include "../teniointerface.h"
#include "layer_i.h"

namespace Tenio
{
	class IMifImageLayer;
	class IAnimateImageEvent;

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

	TENIO_MAKE_TYPEID_MAP(IMifImageLayer, COMPONENTID_MIFIMAGELAYER);
	TENIO_MAKE_EVENT_MAP(IMifImageLayer, IAnimateImageEvent);

	/**
	 * @ingroup tenuilib
	 * @brief mif �������ŵĲ����
	 *
	 * <p>
	 *     IMifImageLayer �ǲ��� mif �����Ĳ���� ����mif��ÿһ֡�� ʹ�� IImageBox ����ʵ�֡�
	 *     �����˶��ڵײ��������Ĵ����������˶������߼����ơ�ͬʱ������Ӧ�ļ����¼���
	 *	   ֧��mif�����ļ��⣬Ҳ֧����ʾ IImageBox ֧�ֵ����е���������ͼ��
	 * </p>
	 *
	 * @see IJuneLayer, IComponent
	 */
	
	class IMifImageLayer : 
		public IJuneLayer,
		public IEventProducer,
		public IComponent
	{
	public:
		virtual BOOL DrawFrameAt(IJuneCanvas* pCanvas, int x, int y, int nFrameIndex) = 0;	
		/**
		 * @brief װ��ͼ���ļ�
		 * @param lpszFileName [in] �ļ���
		 * @return װ�سɹ�������TRUE�����򷵻�FALSE
		 */
		virtual BOOL LoadImage(LPCTSTR lpszFileName) = 0;

		virtual void UnloadImage() = 0;

		/**
		 * @brief ��ʼ����
		 * @param bReply [in] ѭ������
		 */
		virtual void StartPlay(BOOL bReply = FALSE) = 0;


		/**
		 * @brief ͣס����
		 */
		virtual void StopPlay() = 0;

		/**
		 * @brief ��ȡmif��֡����
		 */
		virtual int GetFrameCount() = 0;

		/**
		 * @brief ��ȡmif�Ķ����ĵ�ǰ֡
		 */
		virtual int GetCurrentFrame() = 0;


		/**
		 * @brief ��ȡ��֡���ӳٵȴ�ʱ��
		 * @param nIndex [in] ֡��
		 */
		virtual int GetFrameDelay(int nIndex) = 0;

		virtual void GetExtInfo(MIF_FILE_HEADER_EXT* pExtInfo) = 0;
		
		virtual UINT* GetFrameDelay() = 0;
	};

	/**
	 * @ingroup tenuilib
	 * @brief IMifImageLayer ���¼��ص��ӿ�
	 *
	 * <p>
	 *     IMifImageLayer ���¼��ص��ӿڡ�
	 * </p>
	 *
	 * @see IMifImageLayer, IEvent, IComponent
	 */
	class IAnimateImageEvent : public IEvent
	{
	public:
		/**
		 * @brief ֡�¼�
		 */
		virtual void OnFrameChange(LONG nFrame, LPVOID lpFrom) = 0;
		
		/**
		 * @brief �ض�֡�¼���ÿһ������Ŀǰ֧��һ���ض�֡�� �û�������Ӧ����¼�����������ض�֡����mif�ļ����趨��
		 */
		virtual void OnFrameTrigger(LONG nFrame, LPVOID lpFrom) = 0;

		/**
		 * @brief ��������
		 */
		virtual void OnAnimateStart() = 0;

		/**
		 * @brief ��������
		 */
		virtual void OnAnimateEnd() = 0;

	};
};

#endif

// InterfaceName:MifImageLayer
// SubFolderName:Layer
