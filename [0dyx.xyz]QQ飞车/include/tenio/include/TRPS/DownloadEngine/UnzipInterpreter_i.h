//
#ifndef __UNZIPINTERPRETER_I_H__
#define __UNZIPINTERPRETER_I_H__
#include "commandinterpreter_i.h"

namespace Tenio
{
	// 所有压缩命令均是解压成功后删除压缩文件
	class IUnzipInterpreter : public ICommandInterpreter
	{
		
	};
}