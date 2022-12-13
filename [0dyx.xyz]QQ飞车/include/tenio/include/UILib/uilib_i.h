/********************************************************************
��created         : 2006/6/1
��filename        : uilib_i.h
��author          : season
��version         : 1.0
��description     : UI�����ӿڣ���UI�⿪����Աʵ��
��history         :
��1 day	          : 2006/6/1
����author        : season
����modification: : created	
*********************************************************************/

#ifndef __UILIB_I_H__
#define __UILIB_I_H__

#include "../teniointerface.h"
#include "../june/canvas_i.h"
#include "../june/canvasupdate_i.h"
#include "variableparam_i.h"
#include "uiconfig_i.h"
#include "uiscriptmanager_i.h"
#include "uiclock_i.h"
#include "../layer/scene_i.h"
#include "../layer/sceneupdate_i.h"

namespace Tenio
{
	class IUILib;
	class IUILibEvent;

	TENIO_MAKE_TYPEID_MAP(IUILib, COMPONENTID_UILIB);

	//////////////////////////////////////////////////////////////////////////
	/**
	 * @ingroup tenuilib
	 * @brief UI�����ӿ�
	 *
	 * <p>
	 *���� ������TenioUILib��������ӿڡ���������:
	 * <ol>
	 *   <li>��ȡXML�ļ��ָ�UI����</li>
	 *   <li>���������ٿؼ������ÿؼ����ԣ����ƿؼ�</li>
	 *   <li>��ÿؼ���ָ���ID</li>
	 * </ol>
	 * </p>
	 * <p>
	 *	ÿһ��UILib�����Ӧһ�����㴰��(topwindow)��ϵ��ÿ�����㴰����ϵ�пؼ������ƺ�ID��Ψһ�ġ�<br>
	 *	�û�ʹ��UI��ʱ���������ȴ���һ��IUILib����<br>
	 *  �û�����ʹ��UI�༭���༭UI�����ļ���Ҳ����ʹ�� IUILib ���󴴽������ؼ���<br>	
	 *  ÿ���ؼ��Ĵ��������ٱ���ͨ������UILib�Ľӿ�ִ�С�<br>
	 * </p>
	 *
	 * @sa 
	 */
	class IUILib : public IComponent
	{
	public:

		/**
		 *  @brief �ָ�һ��������TopWindow����
		 *  @param lpszXMLFile	����������Ϣ��xml�ļ���������������·�����������ǣ���Ĭ��������ڿ�ִ��Ŀ¼��config/layoutĿ¼
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
		 *  @sa ��RestoreFromXMLNode() 
		 */		
		virtual BOOL RestoreFromXMLFile(LPCTSTR lpszXMLFile) = 0;

		/**
		 *  @brief ��xml�ļ��е�ĳ�����ָ��ӿؼ�����
		 *  @param dwParentID �ý��ؼ��ĸ��ؼ�ID�����ؼ������Ѿ���ͬһ��uilib���󴴽�
		 *  @param lpszXMLPath ���ָ��Ľ����xml�ļ��е�·����·����ʽ:xxx.xml/node1/node2/../nodename
		 *  @return �ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
		 *  @sa RestoreFromXMLFile() 
		 */
		virtual BOOL RestoreFromXMLNode(DWORD dwParentID, LPCTSTR lpszXMLPath) = 0;

		/**
		 *  @brief ʹ��xml�ļ��������control�� �ڲ����control����������϶��� parentname_childname
		 *  @param dwParentID ���ؼ�ID
		 *  @param lpszXMLFileName xml�ļ�
		 *  @return �������ɹ�������TRUE����֮ΪFALSE
		 *  @sa CreateControl()
		 */
		virtual BOOL CreateCompositeControlFromXML(DWORD dwParentID, LPCTSTR lpszXMLFileName) = 0;

		/**
		 *  @brief ʹ��xml�ļ��е�properties��㴴���ؼ�
		 *  @param lpszXMLPath �ؼ������xml�ļ��е�·����·����ʽ:xxx.xml/node1/node2/../nodename
		 *  @param pParam �������õĿؼ����Բ���
		 *  @return �����سɹ�������TRUE����֮ΪFALSE
		 *  @sa CreateControl()
		 */
		virtual BOOL LoadControlProperties(
			LPCTSTR lpszXMLPath, 
			IVariableParam* pParam) = 0;

		/**
		 *  @brief ���ô�UILib��Ӧtopwindow�ĸ����ھ��
		 *  @param ��pParent �����ھ��
		 */	
		virtual void SetRootParentHandle(HWND hWnd) = 0;

		/**
		 *  @brief ��ո�uilib���������пؼ�
		 *  @return 
		 *  @sa CreateControl() , DeleteControl() , 
		 */		
		virtual void ClearAll() = 0;

		/**
		 *  @brief ����һ���ؼ�
		 *  @param pParam �����ÿؼ����õ��Ĳ��������Լ�
		 *  @param lpszName ���û�ָ���Ŀؼ����ƣ�ͬ�����ӿؼ��������ظ���\n
		 *					���ΪNULL���򳤶�Ϊ�㣬����ϵͳ�Զ�����һ���ؼ�����
		 *	@param dwParentID ָ���ÿؼ��ĸ��ؼ�ID�������� IControl::GetControlID() , GetControlID() �Ⱥ������
		 *  @return �������ɹ������ؿؼ���ID��ʧ���򷵻ء�INVALID_CONTROLID
		 *  @sa DeleteControl() 
		 */
		virtual DWORD CreateControl(
			IVariableParam* pParam, 
			LPCTSTR lpszName, 
			DWORD dwParentID) = 0;

		/**
		 *  @brief ɾ��һ���ؼ����������������ӿؼ�
		 *  @param dwID ��Ҫɾ���ؼ���ID
		 *  @return ɾ���ɹ��򷵻�TRUE��ʧ�ܷ���FALSE
		 *  @sa CreateControl() 
		 */	
		virtual BOOL  DeleteControl(DWORD dwID) = 0;

		/**
		 *  @brief ���ÿؼ�����
		 *  @param dwID �ؼ�ID
		 *  @param pParam �ؼ����Լ�������μ����ؼ��������б�
		 *  @return ���óɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL SetProperty(DWORD dwID, IVariableParam* pParam) = 0;

		/**
		 *  @brief ���ؼ�������ָ����Canvas��
		 *  @param dwID �ؼ�ID
		 *  @param pCanvas Canvasָ��
		 *  @param x ��Canvas�ϵ�x����
		 *  @param y ��Canvas�ϵ�y����
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */
		virtual BOOL DrawControl(DWORD dwID, IJuneCanvas* pCanvas, int x, int y) = 0;
		
		/**
		 *  @brief ��ÿؼ���
		 *  @param dwControl �ؼ�ID
		 *  @param lpszName ��ſؼ������ַ���
		 *  @param nMaxLen �ַ�����󳤶�
		 *  @return �ɹ����ȡ�򷵻�TRUE��ʧ���򷵻�FALSE
		 */	
		virtual BOOL GetControlName(DWORD dwControl, LPTSTR lpszName, UINT nMaxLen) = 0;

		/**
		 *  @brief ��ÿؼ�ȫ·����
		 *  @param dwControl �ؼ�ID
		 *  @param lpszPath ��ſؼ�·�������ַ���
		 *  @param nMaxLen �ַ�����󳤶�
		 *  @return �ɹ����ȡ�򷵻�TRUE��ʧ���򷵻�FALSE
		 */	
		virtual BOOL GetControlFullPath(DWORD dwControl, LPTSTR lpszPath, UINT nMaxLen) = 0;
		
		/**
		 *  @brief ���ؼ���������
		 *  @param dwControl �ؼ�ID
		 *  @param lpszNewName �ؼ��µ�����
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 */
		virtual BOOL RenameControl(DWORD dwControl, LPCTSTR lpszNewName) = 0;

		/**
		 *  @brief �ɿؼ�����ÿؼ�ID
		 *  @param lpszName �ؼ�����
		 *  @return ������ΪlpszName�ĵ�һ���ؼ�ID������ؼ������ڣ��򷵻� INVALID_CONTROLID
		 *  @sa 
		 */
		virtual DWORD GetControlID(LPCTSTR lpszName) = 0;

		/**
		 *  @brief ���ݿؼ�·�������ҿؼ�ID
		 *  @param lpszPath �ؼ�·�����������IDΪdwParent�Ŀؼ���\n
		 *			���磺		root
		 *						/  \			\n
							   a	f			\n
							  /\				\n
							 b	c				\n
								/\				\n
							   d  e				\n
					�ؼ�d����ȫ·���ǣ�"root.a.c.d";		\n
					�ؼ�d������ڡ��ؼ�a����·���ǣ�"c.d";	\n
					
		 *  @param dwRelativeControl ·������ԵĿؼ�ID��\n
					����ֵΪINVALID_CONTROLIDʱ�����ݿؼ�ȫ·�������ҡ�
		 *  @return ���ز��ҵ��Ŀؼ�ID��ʧ���򷵻�INVALID_CONTROLID
		 */		
		 virtual DWORD GetControlIDbyPath(LPCTSTR lpszPath, DWORD dwRelativeControl) = 0;		

		/**
		 *  @brief �ɿؼ�ID��ÿؼ������ָ��
		 *  @param dwID �ؼ�ID
		 *  @return ���ؿؼ�����ָ�룬���ؼ��������򷵻�NULL
		 *  @sa GetControlPtr() , IControl 
		 */
		virtual LPVOID GetControlPtr(DWORD dwID) = 0;

		/**
		 *  @brief ��ö���ؼ�����ָ��
		 *  @return ���ض���ؼ�����ָ�룬���������򷵻�FALSE
		 *  @sa 
		 */
		virtual LPVOID GetRootControlPtr() = 0;

		/**
		 *  @brief ���ý���ؼ�
		 *  @param dwID ָ����ý���Ŀؼ������ֵΪ INVALID_CONTROLID �����ʹ��ǰ����ؼ�ʧȥ����
		 *  @return 
		 *  @sa 
		 */
		virtual void SetFocusControl(DWORD dwID) = 0;

		/**
		 *  @brief ��ý���ؼ�
		 *  @return ��û�ý���Ŀؼ�ID�����ֵΪ INVALID_CONTROLID ������
		 *  @sa 
		 */
		virtual DWORD GetFocusControl() = 0;

		/**
		 *  @brief ע��ؼ��¼����
		 *  @param dwID ָ�������¼��Ŀؼ�ID
		 *  @param pEvent �¼������߶���
		 *  @return 
		 *  @sa ʹ��������ο� IUILibEvent 
		 */		
		virtual void RegisterEventHandle(DWORD dwID, IUILibEvent* pEvent) = 0;

		/**
		 *  @brief ����ȫ��tip���󣬸ö�������Ӧ����¼�ʱ��ʾ�ؼ�tip���Ա�ʶ���ַ���
		 *  @param dwTipControlID tip�ؼ�ID���˿ؼ���������Զ���ؼ�Ϊ���ؼ���
		 */
		virtual void SetGlobalTipControl(DWORD dwTipControlID) = 0;

		/**
		 *  @brief ���ĳЩ�������
		 *  @param pParam ���ڴ�Ż�ñ����Ķ���ָ��
		 *  @param dwParamID ϣ����ñ�����ID��\n
							 Ŀǰ֧�� PARAMID_TOPWINDOWHANDLE ,�����㴰�ھ��
		 */		
		virtual void GetParam(IVariableParam* pParam, DWORD dwParamID) = 0;

		/**
		 *  @brief ���UILib��configָ�룬singleton
		 *  @return ����Configָ��
		 *  @sa 
		 */
		virtual IUIConfig* GetConfig() = 0;

		/**
		 *  @brief ���ýű�������ָ�룬���ڽű�֧��
		 *  @return  ����ɹ��򷵻�IUIScriptManagerָ�룬���򷵻�NULL
		 *  @sa 
		 */
		virtual void SetScriptManager(IUIScriptManager* pUIScriptManager) = 0;

		/**
		 *  @brief ��õ�ǰʹ�õĽű�������
		 *  @return ���ؽű�������ָ�룬������NULL
		 */
		virtual IUIScriptManager* GetScriptManager() = 0;
		
		/**
		 *  @brief ��õ�uilib��ͳһʱ��ָ��
		 *  @return ����IUIClockָ�롣
		 */	
		virtual IUIClock* GetUIClock() = 0;

		/**
		 *  @brief ����ؼ��޴���ʱ����Ϣ���
		 *  @param pParam ������Ϣ����
		 *  @return ��ֹ��Ϣ������������TRUE,���򷵻�FALSE
		 */		
		virtual BOOL OnOutWindowMessage(IVariableParam* pParam) = 0;

		/**
		 *  @brief ����ʹ���ⲿ����������ʱ�Ļ��滭�����»ص�����
		 *	@param pScene
		 *  @param pSceneUpdate ����ΪNULL
		 *  @param pCanvasUpdate �������»ص�
		 */		
		virtual void SetRootScene(
			IJuneScene* pScene,
			IJuneSceneUpdate* pSceneUpdate) = 0;

		/**
		 *  @brief ��ö���Scene,������ʹ���ⲿ����������ʱʹ��
		 *  @param ppScene ����Scene��ָ���ַ��(out)
		 *  @param ppSceneUpdate ����SceneUpdateָ��ĵ�ַ
		 */
		virtual void GetRootScene(IJuneScene** ppScene, IJuneSceneUpdate** ppSceneUpdate) = 0;
		
		/**
		 *  @brief �ֶ�����UI�¼�
		 *  @param dwID, �����¼��Ŀؼ�ID
		 *  @param pParam �¼�����
		 */
		virtual void SendUIEvent(DWORD dwID, IVariableParam* pParam) = 0;
	};

	/**
	 * @ingroup tenuilib
	 * @brief UILib�¼��ӿ�
	 *
	 * <p>
	 *���� ���¼���������Ҫ���տؼ��¼���ʱ����Ҫ�̳в�ʵ�� IUILibEvent �ӿڡ�<br>
	 *     ��ͨ�� IUILib::RegisterEventHandle() ���Լ�ע�ᵽUILib�С�����ӿؼ�û��<br>
	 *	ע���¼������ߣ������������¼����ϴ��ݣ����͸����ؼ����¼������ߡ��������ơ�<br>
	 * </p>
	 *
	 * @sa IUILib 
	 */
	class IUILibEvent : public IEvent
	{
	public:
		/**
		 *  @brief �ؼ��¼�֪ͨ
		 *  @param dwID �����¼��Ŀؼ���ID
		 *  @param pParem ��Ϣ�������������²�����ɣ�<br>
		 *				EPARAMID_EVENTID		�¼�ID<br>
		 *				EPARAMID_CONTROLNAME	�����¼��Ŀؼ���<br>
		 *				EPARAMID_xxxxxx			�����¼����������¼����;���<br>
		 *  @return 
		 *  @sa IVariableParam 
		 */	
		virtual void OnControlMessage(DWORD dwID, IVariableParam* pParam) = 0;
	};
}		
#endif