
#ifndef __UIPROTOCOL_H__
#define __UIPROTOCOL_H__

#include "gameui.h"
#include "ov_cproto.h"


//��Ϸ���͸���������Ϣ
//wparam :��������,lparam:���Ӳ���
//wparam==0: player����ֵ�仯
//wparam==1: ��Ʒ���������Ǯ�����ص���Ŀ�仯 
//wparam==2: �̵�����ѡ��������Ŀ�仯(lparam=ѡ������Ŀ)
//wparam==3: ��ҽ��ף��Է�����Ǯ���ı仯(lparam=Ǯ��)
//wparam==4: ��ҽ��ף��Լ�����Ǯ���ı仯(lparam=Ǯ��)
//wparam==5: ��ͼ���ֱ仯
#define GM_UPDATEVALUE    (WM_USER+100) 

#define GM_POPMSGBOX      (WM_USER+101) //wparam = pinfo,  lparam = msgbox_id
#define GM_UPDATEITEMINFO (WM_USER+102) //��Ʒ��ϸ��Ϣ����

//���µĴ���Ϸ�Ի���������wparam == 0xffffffff(-1),  ���ǹرմ˴���
//�����ø��ԵĲ�����
#define GM_OPENNPCDLG     (WM_USER+202) //�򿪺�NPC�ĶԻ���   lparam = stDialogInfo *

#define GM_OPENSHOP       (WM_USER+203) //�򿪺�npc����player���������棨�̵꣩, wparam = 0(������)��1(������), 2(ֻ����)��3(ֻ����) lparam=���ٸ���Ʒ

#define GM_DEALREQUEST    (WM_USER+204) //�����������ˣ������Ƿ�ͬ��ĶԻ���, lparam = �������˵�����

#define GM_OPENDEAL       (WM_USER+205) //��ʼ���ף��������׵Ľ���, lparam = ���׶Է������֣��Լ����ֿ�ͨ��GetSelfName�õ���

#define GM_OPENNUMINPUT   (WM_USER+206) //����ֵ�������, wparam = ����16λ-�����Ŀ�� ��16λ-ȱʡֵ��, lparam = �����ַ���

#define GM_OPENSTORE      (WM_USER+207) //�򿪱��ֿܲ�, wparam = 0(С����), 1(������)

#define GM_OPENPLAYERMENU (WM_USER+208) //�Ҽ����������ң�������һ����������, 
                                        //wparam = �������(char *), lparam = �˵�����λ��(x=LOWWORD, y=HIWORD)

#define GM_DEALUPDATE     (WM_USER+209) //���½���״̬�� wparam = ��Щ��ֵ��λ�������Լ�ͬ��1, �Է�ͬ��2, �Լ��ɽ�4���Է��ɽ�8

#define GM_TEAMJOIN       (WM_USER+210) //������������, wparam = ���������֣� lparam = ������Ϣ
#define GM_TEAMINVITE     (WM_USER+211) //���������飬  wparam = ����ӳ������֣� lparam = ������Ϣ
#define GM_TEAMUPDATE     (WM_USER+212) //�����Ա�仯, wparam = (0,1)�Ƿ���Ҫ����ӽ���, lparam=(0,1)��Ա��Ŀ�Ƿ����

#define GM_OPENSKILLWIN   (WM_USER+213) //�򿪼��ܽ���, wparam=0:��ͨ�� 1:�������棬 (-1):�ر�
#define GM_SKILLUPDATE    (WM_USER+214) //���ܵı仯��������Ӧ����Ϣˢ�¼��ܽ���

#define GM_OPENEQUIPWIN   (WM_USER+215) //�򿪻��߹ر�(wparam==-1) װ������
#define GM_OPENWAREWIN    (WM_USER+216) //�򿪻��߹ر�(wparam==-1) ��������, wparam=0:����Ʒ�� 1:�ռ�Ʒ�� 2:����װ��

#define GM_TASKUPDATE     (WM_USER+217) //�������, wparam = �ڼ��������б�(1,2,3)

#define GM_OPENITEMMOD    (WM_USER+218) //������Ʒ�ı����, wparam=0:�ϳɣ� 1:������ 2:��Ƕ, 3:��ѯ�ϳɣ�4:��ѯ����, lparam=listnum
#define GM_UPDATEITEMMOD  (WM_USER+219) //ѡ��ĳһ��Ҫ�ϳɾ�������Ʒ, wparam=������Ŀ,  lparam=�۸�

#define GM_MASTER_REQUEST (WM_USER+220) //��ʦ��ͽ����   wparam=0:��ʦ, 1:��ͽ,  lparam = �����ˣ�������ͽ�ܣ�����
#define GM_MASTER_ACCEPT  (WM_USER+221) //�Է����հ�ʦ��ͽ����,  wparam = bit0-0��ʦ:1��ͽ, bit1-0�ܾ�:1����, lparam = �Է�����

#define GM_GOSSIPMSGUPDATE (WM_USER+250) //�Ƿ���˽����Ϣ:  wparam = ��������Ϣ�� 0��ʾû����Ϣ
#define GM_GOSSIPTALK     (WM_USER+251) //˽����Ϣ���Լ�˵��,  wparam=��������,  lparam=��Ϣ
#define GM_GOSSIPREPLY    (WM_USER+252) //˽����Ϣ���Է��ش�,  wparam=��������,  lparam=��Ϣ
#define GM_GMBUDDYUPDATE  (WM_USER+253) //��Ϸ���ѱ䶯�������ߵȵȣ�wparam = ���Ѹ���, lparam=bit0:�Ƿ����� bit1:�Ƿ���ʾ�ȼ�
#define GM_GMBUDDYJOIN    (WM_USER+254) //�������������Ϸ����,  wparam = ����������
#define GM_BEGINWHISPER   (WM_USER+255) //��ʼ��ĳ������,   wparam = �Է�����
//{add by novaliu  qq message popup
#define GM_QQMSG		  GM_GOSSIPMSGUPDATE //(WM_USER+256) //QQ��Ϣ������GM_GOSSIPMSGUPDATE 
#define GM_QQBUDDYJOIN	  (WM_USER+257) //��QQ����
#define GM_GOSSIPSYS      (WM_USER+258) //˽��ϵͳ��Ϣ
//}

#define GM_OPENTRANSPORT  (WM_USER+260) //�򿪴����б� wparam = ���͵���Ŀ�� (-1)��ʾ�ر��б�

#define GM_OPENMAILLIST   (WM_USER+261) //���ʼ��б���� wparam = �ʼ���Ŀ
#define GM_OPENMAIL       (WM_USER+262) //���ʼ���ϸ���ݵĽ��� wparam = �ڼ����ʼ�(0, 1, 2...)
#define GM_WRITEMAIL      (WM_USER+263) //��д�ʼ��Ľ���, wparam = �ʼ�����(0, 1, 2, 3), lparam = char *[],  [0]���Ͷ���(����Ϊnull), [1]��������

//add by lincoln [2005:12:10] [14:02]
//{{{ȥ����consign
/*
#define GM_OPENCONSIGNCONFIG (WM_USER+265) //�򿪼�����Ʒ������۸񴰿�
#define GM_OPENCONSIGN    (WM_USER+266) //�򿪼��۴���, wparam=0: ����, 1:����, 2:״̬, lparam=0, 1, 2,3: �л����ڼ�������
#define GM_UPDATECONSIGN  (WM_USER+267) //���¼���״̬����, wparam = 0:����,1:����,2:����,3:�Լ���Ʒ,  lparam=��Ʒ����
#define GM_UPDATECONSIGNLIST (WM_USER+268) //���µȴ�ȷ�ϵļ��ۻ��߹�����Ʒ�б�, wparam = ��Ʒ����
*/
//}}}

//add by lincoln [2005:12:10] [14:36]
//{{{�㿨����
#define GM_OPENCONSIGN    (WM_USER+266) //�򿪵㿨���۴���, wparam=0: ����ʵ�忨, 1:���۰󶨿�, 2:���򣨵㿨����ƽ̨��
//}}}

#define GM_OPENPLAYERSHOP (WM_USER+280)  //������Լ����̵�, wparam==0:������Ʒ,wparam=1:������Ʒ,wparam=2:(lparam=0:�ҵĳ����̵�,lparam=1:�ҵ��չ��̵�)
#define GM_OPENSHOPITEM   (WM_USER+281)  //����Ʒϸ�ڵĴ���, wparam = id|(�����Ŀ<<16),  lparam = ��Ʒ����(char *)
#define GM_UPDATEPLAYERSHOP (WM_USER+282) //��������Լ����̵꣬wparam=0:׼������Ʒ, 1:׼���չ���Ʒ, 2:�����̵���Ʒ�б�(��Ҫ������ť) lparam=��Ʒ����

#define GM_NEWBIETIPS     (WM_USER+283)  //������ʾ, 1:����Ϣ�� 0:����Ϣ
#define GM_SYSMSGUPDATE   (WM_USER+284)  //ϵͳ��ʾ, ��������Ϣ�� 0:����Ϣ
#define GM_MAILMSGUPDATE  (WM_USER+285)  //�ʼ���ʾ��1:���ʼ�����, 0:�����ʼ�

#define GM_TARGETSTATE    (WM_USER+290)  //ѡ��Ŀ��仯, ������npc, ����ȣ�wparam = pselect_actor
#define GM_CHANGEFIGURE   (WM_USER+291)  //�ı䷢�ͷ�ɫ, wparam = 0:��ɫ�� 1:����,  lparam = ѡ����Ŀ
#define GM_OPENSKILLLEARN (WM_USER+292)  //�򿪼���ѧϰ����, wparam=ְҵ(0:�����)
#define GM_SKILLLEARNUPDATE (WM_USER+293) //����ѧϰ����ĸ���

#define GM_OPENFAMILYLIST (WM_USER+300)     //�򿪼����б����
#define GM_FAMILYJOIN     (WM_USER+301)     //֪ͨ ĳ��������뱾����
#define GM_FAMILYINVITE   (WM_USER+302)     //֪ͨ ĳ�������뵱ǰ���
#define GM_FAMILYALLY   (WM_USER+303)       //֪ͨ �����������������
#define GM_FAMILYDISHOSTILE (WM_USER+304)   //֪ͨ ��������������ȡ���ж�
#define GM_OPENCREATEFAMILY (WM_USER+305)   //֪ͨ �򿪴����������
#define GM_UPDATEFAMILYDETAILINFO (WM_USER+306)   //֪ͨ ���������Ϣ��Ҫ����
#define GM_OPENFMMANAGE (WM_USER+307) // ֪ͨ �򿪹رռ���������
#define GM_UPDATEMASTERPRENTICE (WM_USER+308) // ֪ͨ��ʦͽ�б�,ʦ���Ǽ��б��

#define GM_FAMSTASHAUTH (WM_USER+309) // ֪ͨ ����ʹ�ü���ֿ����Ȩ
#define GM_OPENFAMSTASH (WM_USER+310) //wparam=0: �������� (������֤�������)
                                      //wparam=1: �״���������
                                      //wparam=2: �޸�����
                                      //wparam=3: �򿪹����б�,num
                                      //wparam=4: �������� (ͬ����Ȩ)


#define GM_OPENAUTHWIN  (WM_USER+315) // ��ͼƬ��֤����,  wparam=-1(�ر�)
#define GM_OPENREPAIRITEM (WM_USER+316) // ��������, wparam=-1(�ر�)

#define GM_OPENINPUTMONEY (WM_USER+317) // ������Ǯ�ĶԻ���

#define GM_OPENEQUIPOTHER (WM_USER+318) // ����鿴����ҵ�װ������

#define GM_UPDATETASKLIST (WM_USER+319) // ��������ƺ��б�

#define GM_OPENWORKTIP (WM_USER+320)    // �򿪴�ѡ��˵�
                                        // wparam = 1 ; ��ʾ��ʾ  lparam = 1 ; ��ʾ���� lparam = 2 ; ��ʾ�ڿ�
                                        // wparam = 0 ; ��ʾ�ر�

#define GM_ENTERMAP   (WM_USER+400)     //�����µĵ�ͼ wparam=0:��һ�ν����ͼ�� 1:�л���ͼ

//add by lincoln [2005:12:6] [19:57]
//{{{�������
#define GM_OPENADVERTISE   (WM_USER+410) //�򿪹������
//}}}


//add by lincoln [2005:12:12] [10:41]
//{{{�Ʋ���ȫ��
#define GM_OPENSECULOCK   (WM_USER+411) //�򿪲Ʋ���ȫ������
//}}}


//add by lincoln [2005:12:12] [10:41]
//{{{��ֵ��ʾ
#define GM_OPENOWEOPT   (WM_USER+412) //�򿪳�ֵ��ʾ����
//}}}

//end ����Ϸ�Ի�������

#define GM_TEAMCREATE	(WM_USER+480)
#define GM_MEMBERUPDATE (WM_USER+481)
#define GM_REGIST		(WM_USER+482)
#define GM_TEAMDESTORY  (WM_USER+483)
#define GM_TEAMLEAVE	(WM_USER+484)
#define GM_TEAMREADY	(WM_USER+485)
#define GM_PLAYERUPDATE  (WM_USER+486)
#define GM_TEAMKICK		(WM_USER+487)
#define GM_SHOWBROKER	(WM_USER+488)

//msgbox����(��ʾ��Ϣ,��ʾͼ��,�رհ�ť,ȷ����ť,ȡ����ť)
//����,ȷ��(ȷ��,����,�ɽ�...)
//����,ȡ��(ȡ��,�ܾ�,����...)
//UI���������ö�ٱ���,�ڽű���ȷ��msgbox����ʾ���.
enum UIMsgId
{
	//������UI�Լ�ʹ�õ���Ϣid
	//UI_MSGID_DEFAULT=0;��gameui.h���Ѿ�����
	//��ˣ�client�����msgid�����1��ʼ

	UI_MSGID_YESNO			= 1,//��һ��YES��ť��һ��NO��ť(�ǣ���)
	UI_MSGID_OKCANCEL		= 2,//��һ��OK��ť��һ��CANCEL��ť(ȷ����ȡ��)
	UI_MSGID_AGREERESIST	= 3,//��һ��AGREE��ť��һ��RESIST��ť��ͬ�⣬�ܾ���

	UI_MSGID_AFFIRM			= 4,//��һ��AFFIRM��ť(ȷ��)
	UI_MSGID_CLOSE			= 5,//��һ��CLOSE��ť(�ر�)
	UI_MSGID_MINICLOSE		= 6,//��һ�����Ͻ�СCLOSE��ť(�ر�)
	UI_MSGID_SENTENCE		= 7,//��һ���ı�
	
	//100������Clientʹ�õ���Ϣid
	UI_MSGID_LOGIN			= 101,//��½ʧ��(��ʾ��Ϣ,��ʾͼ��,�رհ�ť)
	UI_MSGID_NETERROR       = 102,//��Ϸ���������Ӵ���
	
	UI_MSGID_SKILLREVIVE    = 103,//��ɫ����, ʹ�ü��ܸ���
	UI_MSGID_ITEMREVIVE     = 104,//��ɫ����, ʹ�õ��߸���
	UI_MSGID_REVIVE         = 105,//��ɫ����, ����

	UI_MSGID_ENDWORK        = 106,//�Ƿ�Ҫ������

	UI_MSGID_QUIT			= 107,//�Ƿ��˳�

	UI_MSGID_CONN           = 108,//��������, �Ƿ�ȡ��(ֻ��һ��cancel��ť��ʡ�Ժŵĵ�����С����ѭ������)
    UI_MSGID_CONN_NOBTN     = 109,//���ڽ�����...(�ް�ť��ʡ�Ժŵĵ�����С����ѭ������)
    UI_MSGID_CANCEL         = 110,//ֻ��һ��cancel��ť
	
	UI_MSGID_BIND		    = 111,//װ����
	UI_MSGID_DROPLOST		= 112,//������ʧ��ʾ

	UI_MSGID_MONEYALERT		= 113,//�ʼ���Ǯ������ʾ
	
	//{{{ add by novaliu, 20060224
	UI_MSGID_UPDATE         = 114, //ѯ������Ƿ���Ҫ����
	//}}}

	//add by lincoln [2006:2:27] [15:41]
	//{{{�ɼ�����
	UI_MSGID_COLLECT		= 115,
	//}}}
};


enum UIMsgSubId
{
    UI_MSGSUBID_DEFAULT     = 0,    // Ĭ��
    UI_MSGSUBID_CONNPSW     = 1,    // �����������������
    UI_MSGSUBID_CHECKPSW    = 2,    // ������֤�ʺ�����
    UI_MSGSUBID_CONNDIR     = 3,    // ��������Ŀ¼������
    UI_MSGSUBID_READDIR     = 4,    // ���ڶ�ȡ������Ŀ¼
    UI_MSGSUBID_CONNZONE    = 5,    // ����������Ϸ������
    UI_MSGSUBID_LOGINZONE   = 6,    // ���ڵ�½��Ϸ������
    UI_MSGSUBID_READROLES   = 7,    // ���ڶ�ȡ��ɫ�б�
    UI_MSGSUBID_ENTERWORLD  = 8,    // ���ڽ�����Ϸ�¼�(��ȡ����ť)
    UI_MSGSUBID_CREATEROLE  = 9,    // ���ڴ�����ɫ
    UI_MSGSUBID_CONNPSWERR  = 10,   // �����������������
    UI_MSGSUBID_PSWERR      = 11,   // �û��������
    UI_MSGSUBID_CONNDIRERR  = 12,   // ����dir����������
    UI_MSGSUBID_READDIRERR  = 13,   // ��ȡdir����������
};

enum UICustomState
{	
	
	UI_CUSTOM_STATE_DEFAULT		= 0,
	UI_CUSTOM_STATE_LARGE		= 1,
		
};

enum UICustomStyle
{	
	
	UI_CUSTOM_STYLE_DEFAULT		= 0,
		
};

enum UISpriteState
{	
	
	UI_SPRITE_STATE_NORMAL		= 0,//���ƾ�ֹ����
	UI_SPRITE_STATE_IDLE		= 1,//���ƾ�ֹ����������С����
	UI_SPRITE_STATE_ACTIVE		= 2,//���Ʊ���
	UI_SPRITE_STATE_DISABLE		= 3,//���Ʊ�ҵľ�ֹ����
	UI_SPRITE_STATE_INVISIBLE	= 4,//������
		
};


enum UISpriteDirect
{	
	UI_SPRITE_DIRECT_FRONT		= 0,
	UI_SPRITE_DIRECT_BACK		= 1,
	UI_SPRITE_DIRECT_RIGHT		= 2,
	UI_SPRITE_DIRECT_LEFT		= 3,
	
};

	
enum UIItemType
{

	//UI_ITEM_NONE = -1,//defined in gameui.h

	UI_ITEM_ITEM = 0,
	UI_ITEM_WEAPON,
	UI_ITEM_ARMOR,
	UI_ITEM_MEDICINE,
	UI_ITEM_WARE,	
	UI_ITEM_SPECIAL,

};


//���ͻ��˲�ѯ��ȫ�ֱ����ı�־
enum UIGlobalQuery
{
	GlobalQuery_None				=0,
	GlobalQuery_MailListOpen		=1,
	
	//add by lincoln [2005:12:10] [14:02]
	//{{{ȥ����consign
	//GlobalQuery_ConsignListOpen     =2,
	//}}}

	GlobalQuery_ItemModOpen         =3,
	GlobalQuery_WriteMail           =4,
	GlobalQuery_PlayerDeal          =5,
	GlobalQuery_Msgbox				=6,
	GlobalQuery_PlayerShop          =7,
	GlobalQuery_WriteSmallPack      =8,
	GlobalQuery_WriteBigPack        =9,
	GlobalQuery_Probe				=10,
    GlobalQuery_RegMPList		    =11,
	GlobalQuery_FAMILYList		    =12,
	GlobalQuery_AccelBar		    =13,
	GlobalQuery_SketchMap		    =14,
	GlobalQuery_Gossip			    =15,
	GlobalQuery_OtherEquip		    =16,

	GlobalQuery_End
};

enum UIQueryType
{

	//(gameui.h)
	//#define UI_QUERY_TYPE_NONE	-1
	UI_QUERY_TYPE_AUTH	=0,
	UI_QUERY_TYPE_VOICE	=1,

};

enum UIDragType
{	
	
	//(gameui.h)
	#define UI_DRAG_TYPE_NONE	-1
    UI_DRAG_TYPE_ITEM = 0,
	UI_DRAG_TYPE_SHOP = 1,
	UI_DRAG_TYPE_DRESSROOM_ITEM = 2,
	UI_DRAG_TYPE_DRESSROOM_TEMPLATE = 3,
	UI_DRAG_TYPE_GAMELOGIC = 4,
/*	UI_DRAG_TYPE_SHOP			= 0,//"���"
	UI_DRAG_TYPE_WARE_MEDICINE	= 1,//"��Ʒ����"ҩƷ"��
	UI_DRAG_TYPE_WARE_WARE		= 2,//"��Ʒ����"��Ʒ"��
	UI_DRAG_TYPE_WARE_EQUIP		= 3,//"��Ʒ����"װ��"��
	UI_DRAG_TYPE_WARE_SPECIAL   = 4,//"��Ʒ����"����"��
	UI_DRAG_TYPE_EQUIPED		= 5,//"�Ѿ�װ���õ�װ��---��װ��ʼ"

	UI_DRAG_TYPE_EQUIPOTHER		= 6,//"������ҵ�װ��"

	UI_DRAG_TYPE_STORE			= 7,//"���ֿ߲�"

	UI_DRAG_TYPE_BUYLIST       = 8,//��npc���̵꣩---�����Ʒ�б�
	UI_DRAG_TYPE_SELBUYLIST    = 9,//��npc���̵꣩---�����ѡ�����Ʒ�б�
//	UI_DRAG_TYPE_SELLLIST       = 10, //��npc���̵꣩---������Ʒ�б�
//	UI_DRAG_TYPE_SELSELLLIST    = 11, //��npc���̵꣩---������ѡ�����Ʒ�б�
	UI_DRAG_TYPE_DEAL_MINE      = 12,//����ҽ��׵��Լ��ⷽ����Ʒ�б�
	UI_DRAG_TYPE_DEAL_OTHER     = 13,//����ҽ��׵ı����ṩ����Ʒ�б�
	UI_DRAG_TYPE_SYN_ITEMS      = 14,//��Ʒ�ϳɵ�ԭ����Ʒ
	UI_DRAG_TYPE_SYN_LIST       = 15,//��Ʒ���Ժϳɳ������б�
	UI_DRAG_TYPE_ITEMHOLE       = 16,//��Ʒ��Ƕ
	//UI_DRAG_TYPE_MAILITEM       = 17, //�ʼ������������Ʒ�б�
	
	//add by lincoln [2005:12:10] [14:02]
	//{{{ȥ����consign
	/*
	UI_DRAG_TYPE_CONSIGN_ITEM   = 18, //������Ʒ�б�--����
	UI_DRAG_TYPE_CONSIGN_WEAPON = 19, //������Ʒ�б�--����
	UI_DRAG_TYPE_CONSIGN_ARMOR  = 20, //������Ʒ�б�--����
	UI_DRAG_TYPE_CONSIGN_MINE   = 21, //������Ʒ�б�--���˼��۵�������Ʒ
	UI_DRAG_TYPE_CONSIGN_LIST   = 22, //������Ʒѡ���ȴ�ȷ����list

	//}}}

	//add by lincoln [2005:12:11] [11:50]
	//{{{�㿨����
	UI_DRAG_TYPE_CONSIGN_ACCCARD = 18, //�󶨿�����
	UI_DRAG_TYPE_CONSIGN_LIST	 = 19, //�㿨����ƽ̨
	//}}}
	

	UI_DRAG_TYPE_SHOP           = 23, //����̵���Ʒ�б�
	UI_DRAG_TYPE_PETS           = 24, //��ҳ����б�
	UI_DRAG_TYPE_SMALLPACK_RECV = 25, //�յ�С��������Ʒ�б�
	UI_DRAG_TYPE_SMALLPACK_WRITE = 26,//����С��������Ʒ�б� 
	UI_DRAG_TYPE_BIGPACK_RECV = 27,   //�յ����������Ʒ�б�
	UI_DRAG_TYPE_BIGPACK_WRITE = 28,  //���ʹ��������Ʒ�б�

	//add by jension [2006:01:06] [14:13]
	//{{{���﻽��
	UI_DRAG_TYPE_PETS_WAKE		= 29,
	//}}}
	
	UI_MAX_PACKTYPE,

	UI_DRAG_TYPE_SKILL_PET      = 99,   //���＼��
	UI_DRAG_TYPE_SKILL_MAIN     = 100, //ְҵ����
	UI_DRAG_TYPE_SKILL_OTHER    = 101, //��������
	UI_DRAG_TYPE_SKILL_LIFE     = 102, //�����
	UI_DRAG_TYPE_OPENTASK       = 103, //δ�������
	UI_DRAG_TYPE_WORLDTASK      = 104, //��ɵ���������
	UI_DRAG_TYPE_STORYTASK      = 105, //��ɵĹ�������
	UI_DRAG_TYPE_TASKHINT       = 106, //�����hint�б�
	UI_DRAG_TYPE_GMBUDDY        = 107, //��Ϸ����
	UI_DRAG_TYPE_QQBUDDY        = 108, //qq����
	UI_DRAG_TYPE_TRANSPORT      = 109, //���͵��б�
	UI_DRAG_TYPE_MAILLIST       = 110, //�ʼ��б�
	UI_DRAG_TYPE_IDOBJ          = 111, //��id�����ֵ���Ʒ
	UI_DRAG_TYPE_DLGNPCPIC      = 112, //�Ի���npc������
	UI_DRAG_TYPE_SCREEN			= 113, //��Ļ
	UI_DRAG_TYPE_TEAM           = 114, //��Ӷ�Ա
	UI_DRAG_TYPE_MAILREJECT     = 115, //�ʼ������б�
	UI_DRAG_TYPE_HELP           = 116, //��Ϸ������״��Ϣ
	UI_DRAG_TYPE_STORE2			= 117,//"���ֿ߲�ڶ�ҳ"
	UI_DRAG_TYPE_FIGURE			= 118, //��ɫѡ���б�
	UI_DRAG_TYPE_SKILLLEARN_MAIN  = 119, //����ѧϰ-ְҵ����
	UI_DRAG_TYPE_SKILLLEARN_OTHER = 120, //����ѧϰ-��������
	UI_DRAG_TYPE_SKILLLEARN_LIFE  = 121, //����ѧϰ-�����
	UI_DRAG_TYPE_SKETCHMAP        = 122, //С��ͼ

	UI_DRAG_TYPE_CURPETSKILL      = 124, //��ǰ���＼��

    UI_DRAG_TYPE_EMBLEMFAMILYLIST = 125, //������� �б� ID

	UI_DRAG_TYPE_QQGROUP          = 126,  //qqȺ

	//add+lincoln
	//[2005:7:5] [10:52]
	UI_DRAG_TYPE_GARAGE           = 127,  //�ֿ��б�
	UI_DRAG_TYPE_MASTERREG        = 128,  //ʦ���Ǽ��б�
	UI_DRAG_TYPE_MASTERLIST       = 129,  //ʦ��ͬ���б�
	UI_DRAG_TYPE_PRENTICELIST     = 130,  //ͽ���б�

    UI_DRAG_TYPE_STORE3			  = 132,//"���ֿ߲����ҳ"
    UI_DRAG_TYPE_STORE4			  = 133,//"���ֿ߲����ҳ"
    UI_DRAG_TYPE_STORE5			  = 134,//"���ֿ߲����ҳ"

    UI_DRAG_TYPE_FAMMEMLIST       = 135,//�����Ա�б��ڽ�������ϵģ�
	
	UI_DRAG_TYPE_FILTERLIST       = 136,//��������б��ڽ�������ϵģ�
	*/
};


//add+lincoln
//[2004:11:9] [21:30]
enum
{

	SCRIPT_FUNC_ON_UPDATEVALUE		=SYSTEM_SCIRPT_NUM,	
	SCRIPT_FUNC_ON_SHOWDIALOG		,
	SCRIPT_FUNC_ON_TEAMUPDATE		,
	SCRIPT_FUNC_ON_MEMBERUPDATE		,
	SCRIPT_FUNC_ON_SHOWSELFICON		,


	SCRIPT_FUNC_ON_TALKINFO			,
	SCRIPT_FUNC_UPDATEITEMINFO		,
	SCRIPT_FUNC_GLOBALQUERY			,

	SCRIPT_FUNC_ON_OPENSHOP			,
	SCRIPT_FUNC_ON_OPENNUMINPUT		,
	SCRIPT_FUNC_ON_OPENPLAYERMENU	,
	SCRIPT_FUNC_ON_OPENSTORE		,

	SCRIPT_FUNC_ON_OPENDEAL			,
	SCRIPT_FUNC_ON_DEALUPDATE		,




	SCRIPT_FUNC_ON_OPENSKILLWIN		,
	SCRIPT_FUNC_ON_SKILLUPDATE		,

	SCRIPT_FUNC_ON_OPENWAREWIN		,
	SCRIPT_FUNC_ON_OPENEQUIPWIN		,

	SCRIPT_FUNC_ON_TASKUPDATE		,
	
    SCRIPT_FUNC_OPENITEMMOD			,   //(int ����,   int �б���Ŀ)
	SCRIPT_FUNC_UPDATEITEMMOD       ,   //(int ���ͣ�  int ���Ͽ׶���Ŀ��  int �۸�)
	SCRIPT_FUNC_OWEUPDATE			,

	SCRIPT_FUNC_GOSSIPMSGUPDATE		,
	SCRIPT_FUNC_GOSSIPREPLY         ,
	SCRIPT_FUNC_GMBUDDYUPDATE		,
	SCRIPT_FUNC_BEGINWHISPER        ,

	SCRIPT_FUNC_OPENTRANSPORT		,

	SCRIPT_FUNC_OPENMAILLIST		,
	SCRIPT_FUNC_OPENMAIL		    ,
	SCRIPT_FUNC_WRITEMAIL		    ,

	//add by lincoln [2005:12:10] [14:02]
	//{{{ȥ����consign
	/*
	SCRIPT_FUNC_OPENCONSIGNCONFIG   ,
	SCRIPT_FUNC_OPENCONSIGN			,
	SCRIPT_FUNC_UPDATECONSIGN		,
	SCRIPT_FUNC_UPDATECONSIGNLIST   ,
	*/
	//}}}


	//add by lincoln [2005:12:10] [14:36]
	//{{{�㿨����
	SCRIPT_FUNC_OPENCONSIGN			,
	//}}}

	SCRIPT_FUNC_OPENPLAYERSHOP		,
	SCRIPT_FUNC_OPENSHOPITEM		,		
	SCRIPT_FUNC_UPDATEPLAYERSHOP	,

	SCRIPT_FUNC_NEWBIETIPS          ,
	SCRIPT_FUNC_SYSMSGUPDATE        ,
	SCRIPT_FUNC_MAILMSGUPDATE       ,
	SCRIPT_FUNC_TARGETSTATE         ,
	SCRIPT_FUNC_CHANGEFIGURE        ,
	SCRIPT_FUNC_OPENSKILLLEARN      ,
	SCRIPT_FUNC_SKILLLEARNUPDATE    ,
    
    SCRIPT_FUNC_OPENFAMILYLIST      ,  // �򿪼����б����
    SCRIPT_FUNC_OPENCREATEFAMILY    ,  // �򿪴����������
    SCRIPT_FUNC_UPDATEFMDETAILINFO  ,  // ���������Ϣ��Ҫ����

	SCRIPT_FUNC_UPDATEMASTERPRENTICE,  //wparam=1:��ʦ���Ǽ��б����,num;
									   //wparam=2:����ʦͽ����б�,nummaster,numprentice,mode(1:open, 2:update)
									   //wparam=3:��ʦ���Ǽ�ʱ������ܵĽ���

    SCRIPT_FUNC_OPENFAMSTASH,          //wparam=0: ��������
                                       //wparam=1: �״���������
                                       //wparam=2: �޸�����
                                       //wparam=3: �򿪹����б�,num����

	SCRIPT_FUNC_OPENAUTHWIN,           

	SCRIPT_FUNC_OPENREPAIRITEM,

    SCRIPT_FUNC_OPENINPUTMONEY,         // ������Ǯ�ĶԻ���
    
    SCRIPT_FUNC_OPENEQUIPOTHER,         // ����鿴����ҵ�װ������

    SCRIPT_FUNC_UPDATETASKLIST,         // ��������ƺ��б�

    SCRIPT_FUNC_OPENWORKTIP,            // �򿪴�ѡ��˵�
                                        // wparam = 1 ; ��ʾ��ʾ  lparam = 1 ; ��ʾ���� lparam = 2 ; ��ʾ�ڿ�
                                        // wparam = 0 ; ��ʾ�ر�

	SCRIPT_FUNC_ENTERMAP,               //�����ͼ, wparam=0: ��һ�Σ� =1:�л���ͼ

	SCRIPT_FUNC_OPENADVERTISE,          //�򿪹������

	SCRIPT_FUNC_OPENSECULOCK,          //�򿪲Ʋ���ȫ������--(bshow,nmode,pcaption,pinfo),nmode==0:�趨,nmode==1:�޸�,nmode==2:����

	SCRIPT_FUNC_OPENOWEOPT,            //�򿪳�ֵ��ʾ����--bshow
	
	//add by jension [2006:01:06] [15:56]
	//{{{���﻽��
	SCRIPT_FUNC_OPENWAKEPETDLG,			//�򿪳��﻽�ѽ���
	//}}}

};


/**
 * �������Ϣ
 */
enum
{
	CHAT_GENERAL = 0,   //������һ����Ϣ
	CHAT_WHISPER,       //����
	CHAT_TEAM,          //����
	CHAT_CLAN,          //����
	CHAT_BUDDY,         //����
	CHAT_SYSTEM,        //ϵͳ
	CHAT_MASTER,        //ʦͽ
};

enum
{
	UI_RICHFLAG_ICONNICKNAME=1,
};


struct stDialogInfo
{
	enum
	{
		MAX_INPUT = 4,
	};
	struct stOption
	{
		bool enddlg;  //��ѡ������Ի�
		char name[CS_MAX_OPT_LEN];
	};
	struct stInput
	{
		int inputtype; //0: ���֣�   1: �ַ���
		char varname[CS_MAX_VAR_LEN];
		char showname[CS_MAX_SHOW_LEN];
		int  minvalue, maxvalue;
	};

	int type; //0: ֻ��һ�仰, 1: һ�仰+ѡ��,  2: һ�仰+��������ֵ
	char sentence[CS_MAX_STR_LEN];
	
	stOption options[CS_MAX_OPTS];
	int noption;

	stInput inputs[MAX_INPUT];
	int ninput;
};


enum
{

	CUSTOM_BUFFER_NONE			=  -1,
	CUSTOM_BUFFER_SKETCHMAP		=	0,
	CUSTOM_BUFFER_CHARACTER		=	1,
	CUSTOM_BUFFER_HEAD          =   2, //�Լ����߽��׶����ͷ��
	CUSTOM_BUFFER_TEAMHEAD      =   3, //���ѵ�ͷ��
	CUSTOM_BUFFER_CHARSTATE     =   4, //��ɫ״̬�������ͼ��
	CUSTOM_BUFFER_MAINHEAD      =   5, //�Լ����Ͻǵ�ͷ��
	CUSTOM_BUFFER_QQHEAD        =   6, //QQ���ѵ�ͷ��
	CUSTOM_BUFFER_FIGURE        =   7, //�ı������Ԥ��ͼ
	CUSTOM_BUFFER_HAIRHEAD      =   8, //���ͷ�ɫ��ͷ��
	CUSTOM_BUFFER_MONEY         =   9, //��Ǯ
	CUSTOM_BUFFER_CLOUD         =   10,//Ƭͷ
	CUSTOM_BUFFER_LOGO          =   11,//����
	CUSTOM_BUFFER_PET           =   12, //����
	CUSTOM_BUFFER_DIAGRAM       =   13, //����ͼ
    CUSTOM_BUFFER_EMBLEM        =   14, //�������
	CUSTOM_BUFFER_PETHEAD       =   15, //����ͷ��
	CUSTOM_BUFFER_PROBE	        =   16, //�������ͼƬ
    CUSTOM_BUFFER_TIMER	        =   17, //���������ѡ��ʱ�ĵ���ʱ
	CUSTOM_BUFFER_OTHERSTATE    =   18, //������ҵĽ�ɫ״̬����
	CUSTOM_BUFFER_STAMINA       =   19, //����������
	CUSTOM_BUFFER_PINGDELAY     =   20, //����pingֵ
	CUSTOM_BUFFER_LOGINAUTH     =   21, //��¼��֤��
};



struct stUICharacter
{
	//0-ѡ�˽����character��1-������ɫ��ѡ��ͼ,  2-������ɫ��preview character
	int style;

	int race;
	int gender;
	int hair;
	int head;
	int suitid;

	//��ͨ״̬����ֹ�������У����С������������״̬���˶�������Ч״̬����ң���ֹ�������ɼ�״̬�������ƣ�
	//normal,active,diable,invisible
	//�ο�UISpriteState����
	int state;
	

	//����(0-front,1-back,2-right,3-left)
	//�ο�UISpriteDirect����
	int direct;


	stUICharacter()
	{
		style=0;
		race=0;
		gender=0;
		hair=0;
		head=0;
		suitid=0;

		state=0;
		direct=0;

	}


	
};

struct stDrawSketchMap
{
	stRect rect;
	int  scalelevel;  //�����ŵȼ���Ϊ[0, 100],  0��ʾ��ͼ��С����С��100��ʾ��ͼ�Ŵ����
	bool bdrawtips;   //�Ƿ����е������ʾ
};

struct stDrawMainHead
{
	stRect rect;
};

struct stDrawFigure
{
	int index;
};

struct stDrawHairHead
{
	int gender;
	int hair;   //��ɫ(0..n)
	int head;   //����(0..n)
};

struct stDrawMoney
{
	int money;
	int style; //0:center,  1:left
};

struct stDrawTeamHead
{
	int index;
};

struct stDrawDiagram
{
	int mapid;
};



/*
Ŀǰ��Ϸ����UIģ�����Ϣ����:
1.ֻ��OnkeyDown�е�:
//VK_RETURN, VK_TAB, VK_ESCAPE , VK_BACK, 
//VK_HOME, VK_END, VK_DELETE, VK_INSERT
//VK_LEFT, VK_RIGHT,  VK_UP, VK_DOWN
//����


//2.ֻ��OnChar�е�:
//û��

//3.��OnChar,OnKeyDown�ж��е�:
//VK_SPACE,   '1',  'A',  'B',............

������onCharֻ�Ǵ�����buffer������ַ��� ��OnKeyDown�������ı�buffer
*/





/**
������Щ����ʱ����Ҫ���õĽű��ӿ�

SetInputNumber( int number );    //�����ƶ���Ʒ����Ҫ�������ֵ

int GetMoney( [int number] );    //�õ� [����] [����в��������ǵڼ����ֿ�] ��Ǯ�� 

int GetMaxWeight();              //�õ������

int GetPackWeight( [int itype] );//�õ� [����] [����в���������UI_DragType] ������---��ʾ����

int GetWeightAfterShop();        //�õ�Ԥ�Ƶ������Ժ���

int GetTotalPrice();             //�õ� [����ѡ���б�] ���ܵļ۸�

CloseStore();                    //�رձ��ֿܲ�---���ֿ����رհ�ť

UseItem( int type, int slot );   //ʹ��ĳ�����ͣ�ĳ����λ����Ʒ

SaveMoney( boolean bsave, int number ); //bsave������Ǯ�������䣬����ȡǮ

ToggleNpcShop( int mode );       //�л��̵�ģʽ�� mode = 0:�رգ�  1������ģʽ�� 2������ģʽ

ConfirmShopSale();               //ȷ�Ͻ��е�ǰģʽ���̵�����

CancelShopSale();                //ȡ����ǰģʽ���̵�����

ClearNpcShop();                  //����̵�ѡ���б�

CancelNpcShopItem( int i )       //ȡ���̵�ѡ���б��еĵڼ���(1...n)

DealRequest( char *pother );     //��������ύ������

AgreeDeal( boolean agree );      //�����˵Ľ������󵽴�Ƿ�ͬ����н���

CancelDeal();                    //ȡ������

LockDealItem( money );           //�������߽�����Ҫ���׵���Ʒ����Ǯ

CompleteDeal();                  //��ɽ�����ɽ���

InputDealMoney( int number );    //���뽻�׵Ľ�Ǯ��Ŀ

TeamCreate( char *pteamname, [char *pother, const char *pmsg] )  //�������� [������������ҹ�ͬ����]
TeamInvite( char *pother, char *pmsg ); //���������������������
TeamAcceptInvite( boolean accept ) //���ܼ�����������
TeamJoin( char *pother, char *pmsg );  //�������Ҫ��������Ķ���
TeamAcceptJoin( bool accept );   //����ĳ�˵ļ����������
TeamKickout( char *pname );      //�ߵ�����Ϊpname�ĳ�Ա
TeamLeave();                     //�뿪����
TeamDismiss();                   //��ɢ����
TeamFollow();                    //����ӳ��ж�
TeamGetInfo( [char *pname] );    //�����Լ�[����ĳ���]Table( name, nmember, isleader ), ���isleader��ʾ�Լ��Ƿ�ӳ�
TeamGetMember( int i );          //����Table( name, hp, maxhp, isleader, isonline )
AddNeighborTeam( char *pbuf, char *pname ) //�������ӵ�һ����������ȥ�� ���ؼ���������

SkillCanForget();                //�Ƿ������������
SkillForget( int type, int i );  //����ĳ���, type=0:�����ܣ� 1:��������,  i= (1...n)
SkillGetStatus( int type, int i );//����һ��table( blearned--�Ƿ��Ѿ�ѧ��, bupgrade--�ܷ�����, busable--�ܷ�ʹ��, bstudy--�Ƿ����ѧϰ, name--���֣�level--�ȼ�����char* )
SkillGetNumber( int type );      //�õ���ǰѧ�Ἴ�ܵ���Ŀ
SkillGetPoints();                //�õ���ǰ���ܵ���
SkillUpgrade( int type, int i ); //��������
SkillLearn( int type, int i );   //ϰ�ü���

SwitchSuit( int i );             //�л����ڼ�����װ

TaskRequireList();               //��ȡ�����б�
TaskCancel( int i );             //ȡ��δ�����������ĵڼ���
TaskGetNumber( int type );       //�õ�ĳ�������б���������ĸ���
TaskGetInfo( int type, int i );  //�õ�ĳ��(1-δ���,2-��������,3-��������)�����б�����, ĳ���������Ϣ
                                 //����Table( title, desc, nhint, hints ),  ����hints��һ��Table, hints[1]��hints[nhint]
								   ����String���͵���Ϣ
TaskGetHint( int type, int itask, int ihint ); //�õ�ĳ�������б��ĳ�������ĳ��hint, String����
TaskFindByName( int type, char *pstr );   //Ѱ�Ұ������ַ���������, ����task_index

Chat( char *pdest, char *pinfo );    //pdest = "����", "����", "����", "����" �����������
t, num = GetChatGoal();             //t[i=1...n] �ǵڼ���������������, num ���ٸ�����
bool IsChatEnable( int i );         //�õ�ĳ��chat��Ϣ�Ƿ����
EnableChat( int i, bool enable );  //enable or disable ����ĳ��chat��Ϣ
neatmsg, sender = GetNeatChatMsg( const char *pmsg ) //�ֽ�������Ϣ

-------�ϳɹ���-----------------------
1. �򿪺ϳɾ�����Ƕ����
SCRIPT_FUNC_OPENITEMMOD( type, �б���Ŀ ��
2. ItemModSelect( int i )            //ѡ��ϳɾ����ĵڼ���(1...n),  ��Ƕ����Ҫ����
3. SCRIPT_FUNC_UPDATEITEMMOD( ���ϻ��߿׶���Ŀ, �۸� )
4. ItemModConfirm( bool b )          //ȷ�ϻ���ȡ���ϳɾ�����Ƕ
-------�ϳɹ���-----------------------
ItemModSelect( int i )            //ѡ��ϳɾ����ĵڼ���(1...n), ��Ƕ����Ҫ����
ItemModConfirm( bool b )          //ȷ�ϻ���ȡ���ϳɾ�����Ƕ

MasterRequest( char *pname )      //�����ʦ����
ApprRequest( char *pname )        //�նԷ�Ϊͽ������
AgreeMasterRequest( bool b )      //�Ƿ���հ�ʦ����
AgreeApprRequest( bool b )        //�Ƿ������ͽ����
OweMaster()                       //��ʦ������ж�
MasterKickout( char *pname )      //�߳�ĳ��ͽ��
MasterBetray( char *pname )       //��ʦ
GetMasterName( char *pname )      //�õ�����Ϊpname���Ǹ��˵�ʦ������

PopQQMsg()                        //���QQ��Ϣ�� ����һ�����������Ϣ
GetNumQQBuddy()                   //�õ�QQ���Ѹ���
t = GetQQBuddy( int i, bool isindex=true ) //�õ��ڼ���(1...n)QQ���ѣ� 
			����table( char *name, int uin, bool bonline, int age, char *gender, char *nation, char *province, char *city )
QQBuddyAccept( bool b )           //����QQ���Ѽ�������
QQBuddyReqAdd( char *pname )      //�����ĳ��Ϊ����
QQBuddyTalk( int uin, char *pmsg )  //��qq_uin�������죬����UI��GM_QQBUDDYTALK��Ϣ
QQBuddyDel( int uin )               //ɾ��QQ����: uin
QQChangeStatus( int status )       //�ı�qq����״̬:  1-�뿪, 2-���ߣ�3-����,  4-����
QQBuddyRequireInfo( int uin )     //��Ҫqq������Ϣ

GetNumGameBuddy()                 //�õ���Ϸ���Ѹ���
t = GetGameBuddy( int i )         //�õ��ڼ���(1...n)��Ϸ���ѣ� 
		����table( char *name, bool bonline, bool bmaster, bool bmale, level, prof:ְҵ�� type:����, title:�ƺ� )
num,names,flags=GmBuddyGetAll(plisttxt); //���غ��Ѹ������ͺ�������, �Ƿ���plisttxt�е�����

GmBuddyReqAdd( char *pname )      //�����ĳ��Ϊ����
GmBuddyReqRemove( char *pname )   //����Ӻ�������ɾ��ĳ��
AcceptGmBuddyReq( bool b )        //�Ƿ���ܶԷ��ļ����������
GmBuddySetFlag( bool bhide, bool bshowlevel ) //�����Լ��Ƿ��ں������������������ʾ�ȼ�
bhide, bshowlevel = GmBuddyGetFlag() //�õ��Լ��Ƿ��ں������������������ʾ�ȼ�
ToggleMailReject( int i )         //�ı�ڼ�����Ϸ���ѵ��ʼ�����ѡ��(1...n)
ConfirmMailReject( bool b )       //true--ȷ�����ж���Ϸ���ѵ��ʼ�����ѡ��, false--ֱ�ӹر�ȡ��
MailReject( int i, bool b )       //���õڼ�����Ϸ���ѵ��ʼ�����ѡ��(1...n)

name, enable, price, desc = GetTransport( int i ) //�õ��ڼ���(1....n)���͵����Ϣ
TransportTo( int i )              //���͵��ڼ���(1....n)���͵�

WriteMail( int type, char *pto, char *ptitle )  //дĳ�����͵�email: 0--��ͨ�� 1--С������ 2--�����
SendMail( char *pto, char *ptitle, char *pbody, int money=0 ) //�����ʼ�
ReceiveMail( int i )              //�յڼ����ʼ�(1.....n)
GetMailDetail( int i )            //�õ��ڼ����ʼ�(1.....n)�����ݣ�����: char *pfrom, char *ptitle, char *pbody, int money, int type, int nitem
DeleteMail( int i )               //ɾ���ʼ�(1.....n)
GetMailFlag()
SetMailFlag()

int GetNumWorldServer()               //�õ����ٸ�worldserver
char *GetWorldServer( int i )         //�õ��ڼ���(1...n)worldserver������
SelectWorldServer( int i )            //ѡ��ڼ���(1...n)��Ϊ��ǰserver
int GetNumZoneServer()                //�õ����ٸ�zoneserver
char *GetZoneServer( int i )          //�õ��ڼ���(1...n)zoneserver������
int GetServerBusy( int i )            //�õ��ڼ���(1...n)zoneserver��æµ״̬, 1 �����㣩  2�����ã�  3����æ��   4��������
SelectZoneServer( int i )             //ѡ��ڼ���(1...n)��Ϊ��ǰserver
ConnectDirServer()                    //����dir server, client���浽ѡ��world, zone����
ConnectZoneServer()                   //����ѡ��õ�zoneserver, �����ӳɹ���client���潫����GameUIGotoScreen("entry")�����½����

ConsignItem( int price, int count, int discount ) //������Ʒ, count:���������Ŀ�����ܳ�����Χ,  discount:�ۿۣ�<=0��ʾ���ۿ�
ConfirmConsign( bool b )               //ȷ�ϼ���
BuyConsign( int type, int i )         //��ĳ���͵ĵڼ�����Ʒ(1...n)
CancelConsign( int i )                //ȡ���ڼ�����Ʒ����(1...n)
FindMyConsign()                       //�Լ����۵���Ʒ
FindConsign( char *pitemname, int minlevel, int maxlevel, int minprice, int maxprice, char *powner ) //���ϲ�ѯ, �������ֵ��ʾ�����Ǵ�����
name, price = ConsignItemInfo( int type, int i ) //��ĳ���͵ĵڼ�����Ʒ(1...n)�����ƣ��۸���Ϣ
int GetNumConsign( int type )         //�õ�ĳ���͵ļ�����Ʒ����
EndConsign()                          //�������ۻ���رռ���״̬����������Ʒʱ����Ҫ����
ClearConsign()                        //���������Ʒ

AddShopItem( int price, int number )  //���̵�������һ����Ʒ
DelShopItem( int i )                  //ɾ���ڼ�����Ʒ(1...n)
int GetNumShopItem()                      //�õ��̵������е���Ʒ����
name, price = ShopItemInfo( int i )   //�̵������еڼ�����Ʒ����Ϣ
CreateShop( char *pshopname )         //ѡ�����Ʒ��һ���̵꣬��Ʒ��UI_DRAG_TYPE_SHOP����
EndShop();                            //�ر��̵�

SetPlayerTitle( int type )            //���óƺ�ϵͳ��ѡ��: 1--ְҵ�ƺ�, 2--����ƺ�, 3--ʦͽ�ƺ�, 4--����ƺ�
int GetPlayerTitle()                  //�õ��ƺ�ϵͳ������: ...

char *PopNewbieTips()                 //����һ��tip, ���û���ˣ� ���ؿ��ַ���
char *PopSystemMsg()                  //����һ��ϵͳ��Ϣ, ���û���ˣ� ���ؿ��ַ���

ncolor, array = GetCtrlTextColor()    //�õ������ַ���Ӧ����ɫ��,  ncolor=���ٸ���ɫ��array[ncolor]=��ɫֵ(int)
bool CanMakeMaster(const char* playername);//�ܷ�����playernameΪʦ��
bool CanMakePrentice(const char* playername);//�ܷ������playernameΪͽ��
//����-1, ����ʾ�˵�
//0:�ɰ�Ϊʦ��
//1:����Ϊͽ��
//2:������ʦ
//3:�������ʦ��
int GetMasterMenuType( const char *playername);
//����-1, ����ʾ�˵�
//����0����ʾ��������ӡ�
//����1����ʾ��������ӡ�
//����2����ʾ��������ӡ�
//����3����ʾ���߳����顱
//����4, ��ʾ "����ӳ�"
int       GetTeamMenuType(const char* playername);
bool IsQQBuddy(const char* player);
bool IsGMBuddy(const char* player);
bool CanOweMaster(const char *player);  //�Ƿ��ܶ�ʦ���ж�

GetCharInitParam( int type[1,2,3])     //�õ�������ɫʱ��Ĳ���, ����table{ STR, DEX, VIT, AGI, INT, LUK, HP, SP }
FollowTeamLeader()                     //��ʼ����ӳ�
FollowPlayer( const char *pname )      //��ʼ����ĳ�����
GetCurMapName()                        //�õ���ǰ��ͼ����
IsPlayerLive()                         //��ǰ��ҽ�ɫ�Ƿ����
GetPlayerProf()                        //�õ���ǰ��ҵ�ְҵ
IsFollowOther()                        //�Ƿ�������
IsHoldPhone()                          //�Ƿ����

num,names,bselectflags=GetNeighborList(pcurlisttxt);
bselectflags[i]:name[i]�Ƿ��Ѿ������pcurlisttxt��
pcurlisttxt���磺delta;lincoln;qujianbiao

ChangePlayerFigure(int i)              //�ı䷢�ͷ�ɫ
GetActorTypeByName(char *pname)        //�������ֵõ�actor����
name, price = GetShopItemInfo( int dragtype, int i ) //�õ��̵�item����Ϣ,  i = (1....n)
GetHelpText()
GetLastUserName()
GetNumMail()
DeleteAllMail()
PushNewbieTips( int id )               //����������ʾ
ChangePlayerRace( int race )           //������ɫ����ı��ɫ���ͣ����ݣ������ȣ�

GetMainPetInfo()                       //�õ���ǰ�������Ϣ,������GetMainPlayerInfo, ����table{name=, CURHP=, ...}
SetPetNickName( const char *pname )    //���õ�ǰ�����ǳ�
bool HaveMapDiagram()                  //���ص�ǰ��ͼ�Ƿ�������ͼ
*/

struct UITeamInfo
{
	char szName[32];
	UINT TeamID;
	int TeamMode; //1 ���� 2 ����
};

struct UIMemberInfo
{
	char szName[32];
	UINT Uin;
	UINT PlayerID;
};
struct UIPlayerInfo
{
	char szName[32];
	UINT Uin;
	UINT PlayerID;
};
extern stDialogInfo g_DlgInfo;
extern std::vector<UITeamInfo> g_vecCurTeamlist;
extern std::vector<UIMemberInfo> g_vecMemberList;
extern std::vector<UIPlayerInfo> g_vecPlayerList;
extern UITeamInfo * g_MyTeam;

extern float GameUIGlobalQuery(int type,float defaultvalue=0.0f,void *pdata=0);
//{ modify by novaliu , 20050915
//extern void  GameUITalkInfo( int type, const char *pinfo, const char *pfrom, const char *pto );
extern void GameUITalkInfo ( int channel, int mode, const char* pinfo, const char*pfrom, const char* pto );
//}
extern int   GameUIOnWinMessage( UINT msg, WPARAM wparam, LPARAM lparam);


#endif