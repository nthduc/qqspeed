#pragma once

//-----------------------------------------------------------------------------
//  Login 相关错误码
//-----------------------------------------------------------------------------

#define ERRORID_QQLOGINPASSWORDERROR		0x00000001		// QQ 密码验证错误
#define ERRORID_QQLOGINTIMEOUT				0x00000002		// QQ 密码验证登录超时
#define ERRORID_QQLOGIN_SERVERREPLYTIMEOUT	0x00000003      // 服务器在规定时间内没响应
#define ERRORID_QQLOGIN_CONNECTFAIL			0x00000004      // 建立连接失败
#define ERRORID_QQLOGIN_UPDATESIGNALFAIL	0x00000005      // QQ签名更新失败
#define ERRORID_QQLOGIN_VERIFYCODEERROR		0x00000006      // QQ签名更新失败
#define ERRORID_QQLOGIN_UNKNOWN				0x000000011

// 服务器主动关闭连接
#define ERRORID_QQLOGIN_CLOSESOCKET			0x00000004
#define ERRORID_QQLOGIN_SERVERERROR			0x00000010

//-----------------------------------------------------------------------------
// [0x00006xxx && 0xF0006xxx] Download 错误码 made by gongwei,正确使用0x00016xxx,错误使用0xf0016xxx
//-----------------------------------------------------------------------------
#define ERROR_DOWNLOAD_SUCCESS						(0x00006000)
#define ERROR_DOWNLOAD_ADD_TASK_SUCCESS				(ERROR_DOWNLOAD_SUCCESS+1)

#define ERROR_DOWNLOAD_FAIL							(0x00006100)

// 连接失败
#define ERROR_DOWNLOAD_CONNECT_HOST_FAIL			(ERROR_DOWNLOAD_FAIL+1)

// 收到未知消息
#define ERROR_DOWNLOAD_RESPONSE_FAIL				(ERROR_DOWNLOAD_FAIL+2)

// 下载超时
#define ERROR_DOWNLOAD_TIMEOUT						(ERROR_DOWNLOAD_FAIL+3)

// 删除任务失败
#define ERROR_DOWNLOAD_TASK_REMOVE					(ERROR_DOWNLOAD_FAIL+4)

// 下载参数无效
#define ERROR_DOWNLOAD_PARAM_ERROR					(ERROR_DOWNLOAD_FAIL+5)

// 写硬盘错误
#define ERROR_DOWNLOAD_WRITEBUFFER_ERROR			(ERROR_DOWNLOAD_FAIL+6)

// 服务器文件大小变化(文件更新), 重新下载.(用户收不到此消息)
#define ERROR_DOWNLOAD_SERVER_FILE_SIZE_CHANGED		(ERROR_DOWNLOAD_FAIL+7)

// 文件未下载完，Socket被关闭
#define ERROR_DOWNLOAD_HOST_CLOSE					(ERROR_DOWNLOAD_FAIL+8)

// 创建文件失败
#define ERROR_DOWNLOAD_CREATEFAIL_FAIL				(ERROR_DOWNLOAD_FAIL+9)

// 磁盘剩余空间不足
#define ERROR_DOWNLOAD_DISKFREESPACENOTENOUGH		(ERROR_DOWNLOAD_FAIL+10)

// 下载参数含有非法路径
#define ERROR_DOWNLOAD_PATHINVALID		            (ERROR_DOWNLOAD_FAIL+11)

// 删除旧文件失败
#define ERROR_DOWNLOAD_DELETEFILE_FAIL		        (ERROR_DOWNLOAD_FAIL+12)

// 添加下载任务失败事件
#define ERROR_DOWNLOAD_ADD_TASK_FAIL			    (ERROR_DOWNLOAD_FAIL+13)

// 下载任务同步执行失败事件
#define ERROR_DOWNLOAD_SYNTASK_FAIL					(ERROR_DOWNLOAD_FAIL+14)

// p2p添加启动失败
#define ERROR_DOWNLOAD_STARTTASK_FAIL		        (ERROR_DOWNLOAD_FAIL+15)

// 下载参数无效
#define ERROR_DOWNLOAD_PARAM_INVALIDATE				(ERROR_DOWNLOAD_FAIL+16)

// 创建IPGroup窗口失败
#define ERROR_DOWNLOAD_CREATEIPGROUPWINDOW_FAIL		(ERROR_DOWNLOAD_FAIL+17)

// 测速失败
#define ERROR_DOWNLOAD_DETECTSPEED_FAIL		        (ERROR_DOWNLOAD_FAIL+18)

// 代理无效
#define ERROR_DOWNLOAD_PROXY_INVALIDATE		        (ERROR_DOWNLOAD_FAIL+19)

// 没有找到对应的配置器
#define ERROR_DOWNLOAD_READCONFIG_FAIL		        (ERROR_DOWNLOAD_FAIL+20)

// 未知下载错误
#define ERROR_DOWNLOAD_UNKNOW_ERROR					(ERROR_DOWNLOAD_FAIL+21)

#define ERROR_DOWNLOAD_SENDMESSAGE_ERROR			(ERROR_DOWNLOAD_FAIL+22)

// 连接超时
#define ERROR_CONNECT_TIMEOUT						(ERROR_DOWNLOAD_FAIL+23)

// 所有ip均试过，连接失败
#define ERROR_CONNECT_ALLIPFAIL						(ERROR_DOWNLOAD_FAIL+24)


//-----------------------------------------------------------------------------
// [0x00006xxx && 0xF0006xxx] Download 错误码 made by gongwei,正确使用0x00016xxx,错误使用0xf0016xxx
//-----------------------------------------------------------------------------

#define ERROR_UPDATE_FAIL							(0x00006200)

// 执行文件操作命令时，删除路径失败
#define ERROR_UPDATE_DELETEPATH_FAIL		        (ERROR_UPDATE_FAIL+1)

// 没有匹配的命令解释器
#define ERROR_UPDATE_NOINTERPRETER					(ERROR_UPDATE_FAIL+3)

// 拷贝文件时原文件不存在
#define ERROR_COPYFILE_SOURCEINEXIST				(ERROR_UPDATE_FAIL+4)

// 执行文件操作命令时，拷贝文件时原文件不存在
#define ERROR_MOVEFILE_FAIL							(ERROR_UPDATE_FAIL+5)

// 执行文件操作命令时，拷贝文件时无法设置文件属性
#define ERROR_COPYFILE_SETFILEATTRIBUTESFAIL		(ERROR_UPDATE_FAIL+6)

// 创建目录失败
#define ERROR_CREATEDIRECTORYFAIL					(ERROR_UPDATE_FAIL+7)

// 执行COPY命令失败，且回滚失败
#define ERROR_COPYFILE_ROLLBACKFAIL					(ERROR_UPDATE_FAIL+8)

// 解压文件时，文件名不是*.zip的形式
#define ERROR_UNZIP_NOTZIPFILE						(ERROR_UPDATE_FAIL+9)

// 解压组件创建失败
#define ERROR_UNZIP_CREATEZIPUTILITYFAIL			(ERROR_UPDATE_FAIL+10)

// 调用IZipUtilityThread的UnzipFile()函数失败
#define ERROR_UNZIP_INVOKEUNZIPFILEFAIL				(ERROR_UPDATE_FAIL+11)

// IZipUtilityThread返回解压失败事件
#define ERROR_UNZIP_ONDECOMPRESSFAIL				(ERROR_UPDATE_FAIL+12)

// 添加更新任务失败事件
#define ERROR_UPDATE_ADD_TASK_FAIL					(ERROR_UPDATE_FAIL+13)

// 下载任务同步执行失败事件
#define ERROR_UPDATE_SYNTASK_FAIL					(ERROR_UPDATE_FAIL+14)

// 回滚失败事件
#define ERROR_UPDATE_ROLLBACK_SUCCESS				(ERROR_UPDATE_FAIL+15)
 
#define ERROR_UPDATE_ERROR							(ERROR_UPDATE_FAIL+16)

// #define ERROR_DOWNLOAD_UPDATE_ERROR					(ERROR_DOWNLOAD_FAIL+10)
// #define ERROR_UPDATE_UNZUP_ERROR						(ERROR_DOWNLOAD_FAIL+11)
// #define ERROR_UPDATE_REPLACEFILE_ERROR				(ERROR_DOWNLOAD_FAIL+12)
// #define ERROR_UPDATE_DELETEFILE_ERROR				(ERROR_DOWNLOAD_FAIL+13)
#define ERROR_BATCHUPDATE_SYNCERROR					(ERROR_UPDATE_FAIL+17)

//-----------------------------------------------------------------------------
// [0x000001xx] TRPS 错误码 made by olivezhang
//-----------------------------------------------------------------------------
//------安装错误------
#define ERROR_TRPS_FAIL										0x00000100

//安装成功
#define ERROR_TRPS_UPDATE_SETUP_SUCESS						(ERROR_TRPS_FAIL + 1)

//安装被用户按取消键中止
#define ERROR_TRPS_UPDATE_USER_ABORT						(ERROR_TRPS_FAIL + 2)

//磁盘空间不足
#define ERROR_TRPS_UPDATE_SPACE_NOT_ENOUGH					(ERROR_TRPS_FAIL + 3)

//安装失败，一般是要打包的文件不存在
#define ERROR_TRPS_UPDATE_SETUP_FAILS						(ERROR_TRPS_FAIL + 4)

//安装程序正在运行中
#define ERROR_TRPS_UPDATE_ENM_SETUPEXE_RUNING				(ERROR_TRPS_FAIL + 5)

//更新的进程正在运行中
#define ERROR_TRPS_UPDATE_ENM_UPDATEPROCESS_RUNNING			(ERROR_TRPS_FAIL + 6)

//------下载错误（作为下载引擎自定义错误的补充）------

//更新包MD5_HASH校验失败,且当前更新包没有被依赖
#define  ERROR_TRPS_DOWNLOAD_MD5CHECKFAILED					(ERROR_TRPS_FAIL + 7)

//更新包MD5_HASH校验失败,且当前更新包被依赖
#define  ERROR_TRPS_DOWNLOAD_MD5CHECKFAILED_DEPENDED		(ERROR_TRPS_FAIL + 8)

//------升级信息获取错误------
// 获取更新信息失败
#define ERROR_TRPS_RETRIEVE_FAILURE							(ERROR_TRPS_FAIL + 9)

//------安装错误------
//启动安装包失败
#define ERROR_TRPS_UPDATE_OPEN_PACKAGE_FAIL					(ERROR_TRPS_FAIL + 10)

//安装包超时未返回成功消息
#define ERROR_TRPS_UPDATE_INSTALL_TIME_OUT					(ERROR_TRPS_FAIL + 11)