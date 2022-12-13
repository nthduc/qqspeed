#if !defined(_UPDATE_COMMON_H_INCLUDE_)
#define _UPDATE_COMMON_H_INCLUDE_

//客户端和liveupdate公用结构，需要保持同步，所以放在一个文件里
#pragma warning(disable : 4786)
#include <vector>

#if defined(UPDATE_COMMON_NAMESPACE)
namespace UPDATE_COMMON_NAMESPACE {
#endif
	
#define CS_ATTR_DOWNLOAD_LIMIT_START_SIZE	 100   //5*1024*1024   当达到多少M的时候开始限制速度
#define CS_ATTR_DOWNLOAD_LIMIT_START_PERCENT 101   //10%           当达到%多少的时候开始限制速度
#define CS_ATTR_UPLOAD_LIMIT_START_SIZE	     102   //5*1024*1024   当达到多少M的时候开始限制速度
#define CS_ATTR_UPLOAD_LIMIT_START_PERCENT   103   //10%           当达到%多少的时候开始限制速度
#define CS_ATTR_DOWNLOADPERCENT			     104   //50            限制下载为峰值的百分之多少
#define CS_ATTR_UPLOADPERCENT			     105   //20            限制上传为峰值的百分之多少
#define	CS_ATTR_SHARE_NUM                    109   //3             共享数目

#define CS_ATTR_SHARE_SPEED_START_ID	     110   //2             共享1的速度限制
#define CS_ATTR_SHARE_MAX_NUM		         10    //5             共享2的速度限制
//last num = 120

#define DEF_ATTR_UPLOAD_LIMIT_START_SIZE       2*1024*1024
#define DEF_ATTR_UPLOAD_LIMIT_START_PERCENT    10
#define DEF_ATTR_DOWNLOAD_LIMIT_START_SIZE     2*1024*1024
#define DEF_ATTR_DOWNLOAD_LIMIT_START_PERCENT  10
#define DEF_ATTR_DOWNLOADPERCENT		       50
#define DEF_ATTR_UPLOADPERCENT				   20
#define DEF_ATTR_SHARE1_SPEED				   2
#define DEF_ATTR_SHARE2_SPEED                  10
#define DEF_ATTR_SHARE_NUM                     3

#define SERVER_URL_MAX_LENGTH	80
#define MAX_ATTR_NUM			128

typedef std::vector<std::string>		vecurl_t;

struct attrib_t
{
	int id;
	int value;
} ;

struct serverinfo_t
{
	void clear()	{ memset(this,0,sizeof(this)); }
	int  getsize1() { return sizeof(serverinfo_t)-getsize2(); }
	int  getsize2() { return sizeof(int)+MAX_ATTR_NUM*sizeof(attrib_t); }
	unsigned int cs_version;
	unsigned int low_version;           //unuse
	unsigned int high_version;         //unuse
	unsigned int update_version;
	int port;                           //unuse
	int length;                         //URL长度，如果长度为0，表示版本不要更新或现在无法更新
	char url[SERVER_URL_MAX_LENGTH];            //patch或文件列表文件的URL地址
	int torrent_length;                 
	char torrent_url [SERVER_URL_MAX_LENGTH];   //if len==0, then open http
	struct in_addr udp_ip;
	int udp_port;
	struct in_addr tcp_ip;
	int tcp_port;
	struct in_addr stun_ip;
	int stun_port;
	int attrib_count;
	attrib_t attrib[MAX_ATTR_NUM];
};

struct clientcfg_t
{
    void clear() {version=svport=weburl[0]=exepath[0]=appname[0]=0;vecaddr.clear();}
    unsigned int version;
    int  svport;
    //char svaddr[256];
    char weburl[256];
    char exepath[256];
    char appname[64];
    vecurl_t vecaddr;
};

struct clientinfo_t
{
	unsigned int current_version;
	unsigned int world_id;
	unsigned int uin;
	unsigned int flag;
	unsigned int reserved;
};

struct userinfo_t
{
	enum {VERSION = 100};
	unsigned int version;
	unsigned int uin;
	unsigned int word_id;
	unsigned int reserved;
};

#if defined(UPDATE_COMMON_NAMESPACE)
}
#endif

#endif