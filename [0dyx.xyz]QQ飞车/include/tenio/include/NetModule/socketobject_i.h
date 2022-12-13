#ifndef __SOCKETOBJECT_I_H__
#define __SOCKETOBJECT_I_H__

#include "../teniointerface.h"
#include "../component_i.h"
#include "Sock5Def.h"
#include "HttpDef.h"
#include "SocketObjectDefine.h"
#include "../event_i.h"

namespace Tenio
{
	
	class ISocketObject;
	class ISocketEvent;

	class ITCPSocketObject;
	class IUDPSocketObject;
	class ISock5TCPProxyObject;
	class IHttpProxyObject;
	class ITCPSpeedLimitObject;
	class ISock5ProxySpeedLimitObject;
	class IHttpProxySpeedLimitObject;

	TENIO_MAKE_TYPEID_MAP(ITCPSocketObject, COMPONENTID_TCPCLIENT)
	TENIO_MAKE_TYPEID_MAP(IUDPSocketObject, COMPONENTID_UDPCLIENT)
	TENIO_MAKE_TYPEID_MAP(ISock5TCPProxyObject, COMPONENTID_SOCK5PROXY)
	TENIO_MAKE_TYPEID_MAP(IHttpProxyObject, COMPONENTID_HTTPPROXY)
	TENIO_MAKE_TYPEID_MAP(ITCPSpeedLimitObject, COMPONENTID_TCPSPEEDLIMITOBJECT)
	TENIO_MAKE_TYPEID_MAP(ISock5ProxySpeedLimitObject, COMPONENTID_SPEEDLIMITSOCK5PROXY)
	TENIO_MAKE_TYPEID_MAP(IHttpProxySpeedLimitObject, COMPONENTID_SPEEDLIMITHTTPPROXY)
	
	/**
	 * @ingroup network
	 * @brief 网络通讯模块
	 *
	 * <p>
	 *　　 本模块内部使用了多线程。
	 * </p>
	 *
	 * <p>
	 *　　 ISocketObject 是各种通讯组件的父类， ISocketObject 有多个子类，分别代表不同类型的
	 *     通讯组件，如 ITCPSocketObject , IUDPSocketObject ，分别代表使用 TCP 协议和 UDP 协
	 *     议通讯组件。不能直接创建 ISocketObject 类型的组件，只能创建各子类型通讯组件。
	 * </p>
	 *
	 * <p>
	 * ISocketObject 的子类型包括了：
	 * <ol>
	 *   <li>ITCPSocketObject － 使用TCP协议的通讯模块</li>
	 *   <li>IUDPSocketObject － 使用UDP协议的通讯模块</li>
	 *   <li>ISock5TCPProxyObject － 使用Sock5代理、TCP协议的通讯模块</li>
	 *   <li>IHttpProxyObject － 使用Http代理、TCP协议的通讯模块</li>
	 * </ol>
	 * </p>
	 *
	 * <p>
	 * 使用流程
	 * <ol>
	 *   <li>选择一种子类型的 SocketObject 进行创建</li>
	 *   <li>使用 Create() 函数创建, 传入代理相关信息，同时传入事件回调接口</li>
	 *   <li>使用 Connect() 函数连接服务器</li>
	 *   <li>在事件回调接口中收到 ISocketEvent::OnConnectSuccess() 消息，表示连接成功</li>
	 *   <li>使用 Send() 函数发送数据</li>
	 *   <li>如果有数据需要接收, 事件回调接口的 ISocketEvent::OnReceived() 会被调用, 在 ISocketEvent::OnReceived()
	 *      中收取数据</li>
	 *   <li>使用 Disconnect() 函数断开连接</li>
	 * </ol>
	 * </p>
	 *
	 * @sa ITCPSocketObject , IUDPSocketObject , ISock5TCPProxyObject , IHttpProxyObject
	 */
	class ISocketObject : public IComponent
	{
	public:
		/**
		 *  @brief 创建并初始化
		 *  @param lpParam 指向一个 SOCKETPARAM 类型结构体
		 *  @param pEventHandler 事件回调指针
		 *  @return 如果创建成功, 返回 TRUE
		 *  @sa 结构体定义:SOCKETPARAM （即: tagProxyParam ）
		 */
		virtual BOOL Create(LPVOID lpParam, ISocketEvent* pEventHandler) = 0;
		
		/**
		 *  @brief 连接远程服务器
		 *  @param lpszHost 服务器地址，可以是域名"www.qq.com"，也可以是IP "202.10.22.11"
		 *  @param ushPort 需要连接的端口
		 *  @return 如果成功地发出了连接请求, 返回 TRUE
		 */
		virtual BOOL Connect(LPTSTR lpszHost, USHORT ushPort) = 0;
		
		/**
		 *  @brief 连接远程服务器
		 *  @param ulHostIP 服务器IP，为网络字节序
		 *  @param ushPort 需要连接的端口
		 *  @return 如果成功地发出了连接请求, 返回 TRUE
		 */
		virtual BOOL Connect(ULONG ulHostIP, USHORT ushPort) = 0;
		
		/**
		 *  @brief 断开连接
		 *  @return 如果成功地断开了连接, 返回 TRUE
		 */
		virtual BOOL Disconnect() = 0;
		
		/**
		 *  @brief 发送数据
		 *  @return 发送动作成功，则返回 TRUE
		 */
		virtual BOOL Send(LPBYTE lpBytes, int nLen) = 0;

		/**
		 * @ingroup
		 * @brief 本函数在connect()之前调用才有效
		 * @param dwMode : socket模式。SOCKETMODE_DELAYRECEIVE : 延迟读数据
		 * @return
		 * @see
		 */
 		virtual void SetMode(DWORD dwMode) = 0;
 
		/**
		 * @brief 当模式为 SOCKETMODE_DELAYRECEIVE 时，调本函数，可以设置延迟时间(根据带宽计算)
		 * @param  dwBandWidth [in] : 带宽，单位：KB/S
		 * @see
		 */
		virtual void SetBandWidth(DWORD dwBandWidth) = 0;	
	};
	
	/**
	 * @ingroup network
	 * @brief 使用TCP协议的通讯模块
	 *
	 * 使用方法请参考 ISocketObject
	 */
	class ITCPSocketObject : public ISocketObject
	{
	};
	
	/**
	 * @ingroup network
	 * @brief 使用UDP协议的通讯模块
	 *
	 * 使用方法请参考 ISocketObject
	 */
	class IUDPSocketObject : public ISocketObject
	{
	};

	/**
	 * @ingroup network
	 * @brief 使用TCP协议，并通过 Sock5 代理进行通讯的通讯模块
	 *
	 * 使用方法请参考 ISocketObject
	 */
	class ISock5TCPProxyObject : public ISocketObject
	{
	};	

	/**
	 * @ingroup network
	 * @brief 使用TCP协议，并通过 Proxy 代理进行通讯的通讯模块
	 *
	 * 使用方法请参考 ISocketObject
	 */
	class IHttpProxyObject : public ISocketObject
	{
	};

	/**
	 * @ingroup network
	 * @brief ISocketObject 的事件回调接口
	 * @sa ISocketObject
	 */
	class ISocketEvent : public IEvent
	{
	public:
		/**
		 * @brief 连接成功
		 */
		virtual void OnConnectSuccess() = 0;

		/**
		 * @brief 连接失败
		 * @param nErrorCode 错误代码
		 */
		virtual void OnConnectFail(int nErrorCode) = 0;

		/**
		 * @brief 接收数据
		 * @param lpBytes 指向收到的数据
		 * @param nLen    接收到的数据的长度
		 */
		virtual void OnReceived(LPBYTE lpBytes, int nLen) = 0;

		/**
		 * @brief 连接被关闭
		 * @param nErrorCode 错误代码
		 */
		virtual void OnClosed(int nErrorCode) = 0;
	};
	
#define SOCKETOBJECT_SUBFOLDER					_T("Socket\\")
	
	
};
#endif
