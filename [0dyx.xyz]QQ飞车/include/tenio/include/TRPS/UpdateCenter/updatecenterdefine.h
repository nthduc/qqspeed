#pragma once

namespace Tenio
{
	// 共内、外部共用的数据结构定义

	// 模块版本信息
	typedef struct tagModuleVersionInfo
	{
		tstring strModuleID; // 模块的唯一标志，内部使用
		tstring strModuleName; // 模块的描述名称，可以显示
		DWORD dwVersion;  // 主版本号
		DWORD dwPatch;		// 已安装最大SP号
		DWORD dwKB;  // 已打最大补丁号 
	}MODULEVERSIONINFO;

	// 客户端版本信息
	typedef MODULEVERSIONINFO CLIENTVERSIONINFO;


	
}