#pragma once

//-----------------------------------------------------------------------------
//  Login ��ش�����
//-----------------------------------------------------------------------------

#define ERRORID_QQLOGINPASSWORDERROR		0x00000001		// QQ ������֤����
#define ERRORID_QQLOGINTIMEOUT				0x00000002		// QQ ������֤��¼��ʱ
#define ERRORID_QQLOGIN_SERVERREPLYTIMEOUT	0x00000003      // �������ڹ涨ʱ����û��Ӧ
#define ERRORID_QQLOGIN_CONNECTFAIL			0x00000004      // ��������ʧ��
#define ERRORID_QQLOGIN_UPDATESIGNALFAIL	0x00000005      // QQǩ������ʧ��
#define ERRORID_QQLOGIN_VERIFYCODEERROR		0x00000006      // QQǩ������ʧ��
#define ERRORID_QQLOGIN_UNKNOWN				0x000000011

// �����������ر�����
#define ERRORID_QQLOGIN_CLOSESOCKET			0x00000004
#define ERRORID_QQLOGIN_SERVERERROR			0x00000010

//-----------------------------------------------------------------------------
// [0x00006xxx && 0xF0006xxx] Download ������ made by gongwei,��ȷʹ��0x00016xxx,����ʹ��0xf0016xxx
//-----------------------------------------------------------------------------
#define ERROR_DOWNLOAD_SUCCESS						(0x00006000)
#define ERROR_DOWNLOAD_ADD_TASK_SUCCESS				(ERROR_DOWNLOAD_SUCCESS+1)

#define ERROR_DOWNLOAD_FAIL							(0x00006100)

// ����ʧ��
#define ERROR_DOWNLOAD_CONNECT_HOST_FAIL			(ERROR_DOWNLOAD_FAIL+1)

// �յ�δ֪��Ϣ
#define ERROR_DOWNLOAD_RESPONSE_FAIL				(ERROR_DOWNLOAD_FAIL+2)

// ���س�ʱ
#define ERROR_DOWNLOAD_TIMEOUT						(ERROR_DOWNLOAD_FAIL+3)

// ɾ������ʧ��
#define ERROR_DOWNLOAD_TASK_REMOVE					(ERROR_DOWNLOAD_FAIL+4)

// ���ز�����Ч
#define ERROR_DOWNLOAD_PARAM_ERROR					(ERROR_DOWNLOAD_FAIL+5)

// дӲ�̴���
#define ERROR_DOWNLOAD_WRITEBUFFER_ERROR			(ERROR_DOWNLOAD_FAIL+6)

// �������ļ���С�仯(�ļ�����), ��������.(�û��ղ�������Ϣ)
#define ERROR_DOWNLOAD_SERVER_FILE_SIZE_CHANGED		(ERROR_DOWNLOAD_FAIL+7)

// �ļ�δ�����꣬Socket���ر�
#define ERROR_DOWNLOAD_HOST_CLOSE					(ERROR_DOWNLOAD_FAIL+8)

// �����ļ�ʧ��
#define ERROR_DOWNLOAD_CREATEFAIL_FAIL				(ERROR_DOWNLOAD_FAIL+9)

// ����ʣ��ռ䲻��
#define ERROR_DOWNLOAD_DISKFREESPACENOTENOUGH		(ERROR_DOWNLOAD_FAIL+10)

// ���ز������зǷ�·��
#define ERROR_DOWNLOAD_PATHINVALID		            (ERROR_DOWNLOAD_FAIL+11)

// ɾ�����ļ�ʧ��
#define ERROR_DOWNLOAD_DELETEFILE_FAIL		        (ERROR_DOWNLOAD_FAIL+12)

// �����������ʧ���¼�
#define ERROR_DOWNLOAD_ADD_TASK_FAIL			    (ERROR_DOWNLOAD_FAIL+13)

// ��������ͬ��ִ��ʧ���¼�
#define ERROR_DOWNLOAD_SYNTASK_FAIL					(ERROR_DOWNLOAD_FAIL+14)

// p2p�������ʧ��
#define ERROR_DOWNLOAD_STARTTASK_FAIL		        (ERROR_DOWNLOAD_FAIL+15)

// ���ز�����Ч
#define ERROR_DOWNLOAD_PARAM_INVALIDATE				(ERROR_DOWNLOAD_FAIL+16)

// ����IPGroup����ʧ��
#define ERROR_DOWNLOAD_CREATEIPGROUPWINDOW_FAIL		(ERROR_DOWNLOAD_FAIL+17)

// ����ʧ��
#define ERROR_DOWNLOAD_DETECTSPEED_FAIL		        (ERROR_DOWNLOAD_FAIL+18)

// ������Ч
#define ERROR_DOWNLOAD_PROXY_INVALIDATE		        (ERROR_DOWNLOAD_FAIL+19)

// û���ҵ���Ӧ��������
#define ERROR_DOWNLOAD_READCONFIG_FAIL		        (ERROR_DOWNLOAD_FAIL+20)

// δ֪���ش���
#define ERROR_DOWNLOAD_UNKNOW_ERROR					(ERROR_DOWNLOAD_FAIL+21)

#define ERROR_DOWNLOAD_SENDMESSAGE_ERROR			(ERROR_DOWNLOAD_FAIL+22)

// ���ӳ�ʱ
#define ERROR_CONNECT_TIMEOUT						(ERROR_DOWNLOAD_FAIL+23)

// ����ip���Թ�������ʧ��
#define ERROR_CONNECT_ALLIPFAIL						(ERROR_DOWNLOAD_FAIL+24)


//-----------------------------------------------------------------------------
// [0x00006xxx && 0xF0006xxx] Download ������ made by gongwei,��ȷʹ��0x00016xxx,����ʹ��0xf0016xxx
//-----------------------------------------------------------------------------

#define ERROR_UPDATE_FAIL							(0x00006200)

// ִ���ļ���������ʱ��ɾ��·��ʧ��
#define ERROR_UPDATE_DELETEPATH_FAIL		        (ERROR_UPDATE_FAIL+1)

// û��ƥ������������
#define ERROR_UPDATE_NOINTERPRETER					(ERROR_UPDATE_FAIL+3)

// �����ļ�ʱԭ�ļ�������
#define ERROR_COPYFILE_SOURCEINEXIST				(ERROR_UPDATE_FAIL+4)

// ִ���ļ���������ʱ�������ļ�ʱԭ�ļ�������
#define ERROR_MOVEFILE_FAIL							(ERROR_UPDATE_FAIL+5)

// ִ���ļ���������ʱ�������ļ�ʱ�޷������ļ�����
#define ERROR_COPYFILE_SETFILEATTRIBUTESFAIL		(ERROR_UPDATE_FAIL+6)

// ����Ŀ¼ʧ��
#define ERROR_CREATEDIRECTORYFAIL					(ERROR_UPDATE_FAIL+7)

// ִ��COPY����ʧ�ܣ��һع�ʧ��
#define ERROR_COPYFILE_ROLLBACKFAIL					(ERROR_UPDATE_FAIL+8)

// ��ѹ�ļ�ʱ���ļ�������*.zip����ʽ
#define ERROR_UNZIP_NOTZIPFILE						(ERROR_UPDATE_FAIL+9)

// ��ѹ�������ʧ��
#define ERROR_UNZIP_CREATEZIPUTILITYFAIL			(ERROR_UPDATE_FAIL+10)

// ����IZipUtilityThread��UnzipFile()����ʧ��
#define ERROR_UNZIP_INVOKEUNZIPFILEFAIL				(ERROR_UPDATE_FAIL+11)

// IZipUtilityThread���ؽ�ѹʧ���¼�
#define ERROR_UNZIP_ONDECOMPRESSFAIL				(ERROR_UPDATE_FAIL+12)

// ��Ӹ�������ʧ���¼�
#define ERROR_UPDATE_ADD_TASK_FAIL					(ERROR_UPDATE_FAIL+13)

// ��������ͬ��ִ��ʧ���¼�
#define ERROR_UPDATE_SYNTASK_FAIL					(ERROR_UPDATE_FAIL+14)

// �ع�ʧ���¼�
#define ERROR_UPDATE_ROLLBACK_SUCCESS				(ERROR_UPDATE_FAIL+15)
 
#define ERROR_UPDATE_ERROR							(ERROR_UPDATE_FAIL+16)

// #define ERROR_DOWNLOAD_UPDATE_ERROR					(ERROR_DOWNLOAD_FAIL+10)
// #define ERROR_UPDATE_UNZUP_ERROR						(ERROR_DOWNLOAD_FAIL+11)
// #define ERROR_UPDATE_REPLACEFILE_ERROR				(ERROR_DOWNLOAD_FAIL+12)
// #define ERROR_UPDATE_DELETEFILE_ERROR				(ERROR_DOWNLOAD_FAIL+13)
#define ERROR_BATCHUPDATE_SYNCERROR					(ERROR_UPDATE_FAIL+17)

//-----------------------------------------------------------------------------
// [0x000001xx] TRPS ������ made by olivezhang
//-----------------------------------------------------------------------------
//------��װ����------
#define ERROR_TRPS_FAIL										0x00000100

//��װ�ɹ�
#define ERROR_TRPS_UPDATE_SETUP_SUCESS						(ERROR_TRPS_FAIL + 1)

//��װ���û���ȡ������ֹ
#define ERROR_TRPS_UPDATE_USER_ABORT						(ERROR_TRPS_FAIL + 2)

//���̿ռ䲻��
#define ERROR_TRPS_UPDATE_SPACE_NOT_ENOUGH					(ERROR_TRPS_FAIL + 3)

//��װʧ�ܣ�һ����Ҫ������ļ�������
#define ERROR_TRPS_UPDATE_SETUP_FAILS						(ERROR_TRPS_FAIL + 4)

//��װ��������������
#define ERROR_TRPS_UPDATE_ENM_SETUPEXE_RUNING				(ERROR_TRPS_FAIL + 5)

//���µĽ�������������
#define ERROR_TRPS_UPDATE_ENM_UPDATEPROCESS_RUNNING			(ERROR_TRPS_FAIL + 6)

//------���ش�����Ϊ���������Զ������Ĳ��䣩------

//���°�MD5_HASHУ��ʧ��,�ҵ�ǰ���°�û�б�����
#define  ERROR_TRPS_DOWNLOAD_MD5CHECKFAILED					(ERROR_TRPS_FAIL + 7)

//���°�MD5_HASHУ��ʧ��,�ҵ�ǰ���°�������
#define  ERROR_TRPS_DOWNLOAD_MD5CHECKFAILED_DEPENDED		(ERROR_TRPS_FAIL + 8)

//------������Ϣ��ȡ����------
// ��ȡ������Ϣʧ��
#define ERROR_TRPS_RETRIEVE_FAILURE							(ERROR_TRPS_FAIL + 9)

//------��װ����------
//������װ��ʧ��
#define ERROR_TRPS_UPDATE_OPEN_PACKAGE_FAIL					(ERROR_TRPS_FAIL + 10)

//��װ����ʱδ���سɹ���Ϣ
#define ERROR_TRPS_UPDATE_INSTALL_TIME_OUT					(ERROR_TRPS_FAIL + 11)