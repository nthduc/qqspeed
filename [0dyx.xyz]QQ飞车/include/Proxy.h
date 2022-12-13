
#if !defined(_PROXY_SUPPORT_H_)
#define _PROXY_SUPPORT_H_

#include <winsock2.h>

enum {
    proxy_type_none=0,      //��ʹ�ô��������
    proxy_type_socks5=1,    //socks5 ����
    proxy_type_https=2,     //http   ����
    proxy_type_socks4=3     //socks4 ����
};

enum {
    proxy_cfg_server=1,     //cfg �� serverip ��Ч
    proxy_cfg_port=2,       //cfg �� port     ��Ч
    proxy_cfg_username=4,   //cfg �� username ��Ч
    proxy_cfg_password=8,   //cfg �� password ��Ч
    proxy_cfg_domain=16,    //cfg �� domain   ��Ч
    proxy_cfg_type=32       //cfg �� type     ��Ч
};


enum {
    error_proxy_none=0,
    error_proxy_error=-1,
    error_proxy_noproxy_connect_target,    //ֱ������Ŀ�ĵ�ַʱ��������
    error_proxy_type,                      //������������ʹ���

    error_socks5_errorbase = -1000,        //socks5 ������ʼ�����
    error_https_errorbase = -2000,         //https ������ʼ�����
    error_socks4_errorbase = -3000,        //socks4 ������ʼ����� 
    
    error_socks5_none=error_proxy_none,
    error_socks5_connect = error_socks5_errorbase,
    error_socks5_auth_method_nego,         //�ڷ�����֤��ʽЭ��ʱ��������
    error_socks5_auth_method_nego_replay,  //�ڽ�����֤��ʽЭ�̻�Ӧʱ��������
    error_socks5_send_auth_info,           //�ڷ�����֤��Ϣʱ��������
    error_socks5_receive_auth_info_replay, //�ڽ�����֤��Ӧʱ��������
    error_socks5_connect_target,           //��ͨ����������Ŀ�ĵ�ַʱ��������
    error_socks5_connect_target_replay,    //��ͨ����������Ŀ�ĵ�ַʱ��Ӧ����

    error_proxy_https_none = error_proxy_none,
    error_proxy_https_connect = error_https_errorbase,
    error_proxy_https_auth_auto,
    error_proxy_https_auth_none,
    error_proxy_https_auth_none_reply,
    error_proxy_https_auth_base,
    error_proxy_https_auth_base_reply,
    error_proxy_https_auth_ntlm_init,
    error_proxy_https_auth_ntlm,
    error_proxy_https_auth_ntlm_reply,
    error_proxy_https_auth_ntlm2,
    error_proxy_https_auth_ntlm2_reply,

    error_socks4_none = error_proxy_none,
    error_socks4_connect = error_socks5_errorbase,
    error_socks4_connect_target,
    error_socks4_connect_target_reply
};


#pragma pack(1)

typedef struct socks5cfg
{
    short           cfgmask;
    struct in_addr  ip;
    short           port;
    char            username[256];
    char            password[256];
}socks5cfg;

typedef struct socks4cfg
{
    short           cfgmask;
    struct in_addr  ip;
    short           port;
}socks4cfg;

typedef struct httpscfg
{
    short           cfgmask;
    struct in_addr  ip;
    short           port;
    char            username[256];
    char            password[256];
    char            domain[256];
}httpscfg;

typedef struct proxycfg
{
    short           type;
    socks5cfg       socks5;
    socks4cfg       socks4;
    httpscfg        https;
}proxycfg;

#pragma pack()

#if defined(__cplusplus)
extern "C" {
#endif

    /*utility*/
    int trim_space ( char* str );

    /*common use*/
    int connect_ex_load_config ( struct proxycfg* cfg, const char* cfgfile );
    int connect_ex_save_config ( struct proxycfg* cfg, const char* cfgfile );
    int connect_ex1 ( SOCKET* sock, struct sockaddr* addr, int addrlen, const char* cfgfile );
    int connect_ex2 ( SOCKET* sock, struct sockaddr* addr, int addrlen, struct proxycfg* cfg );

    /*expert use*/
    int connect_https ( SOCKET* sock, struct sockaddr* addr, int addrlen, struct httpscfg* cfg );
    int connect_socks4( SOCKET* sock, struct sockaddr* addr, int addrlen, struct socks4cfg* cfg );
    int connect_socks5( SOCKET* sock, struct sockaddr* addr, int addrlen, struct socks5cfg* cfg );

#if defined(__cplusplus)
}
#endif

#endif