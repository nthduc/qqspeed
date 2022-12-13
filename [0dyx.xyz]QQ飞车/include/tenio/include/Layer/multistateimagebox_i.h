#ifndef __MULTISTATEIMAGEBOX_I_H__
#define __MULTISTATEIMAGEBOX_I_H__

#include "../teniointerface.h"
#include "layer_i.h"
#include "multistate_i.h"
#include "uilib/variableparam_i.h"

namespace Tenio
{
	class IMultiStateImageBox;

	TENIO_MAKE_TYPEID_MAP(IMultiStateImageBox, COMPONENTID_MULTISTATEIMAGEBOX);

	/**
	 * @ingroup tenuilib
	 * @brief 多状态图片框图层
	 *
	 * 可为图层设置#STATEID_NORMAL 、#STATEID_HOVER 、#STATEID_PRESSED 、#STATEID_DISABLE 四种状态的图片，
	 * 并按需要转换图层的状态
	 * @sa IJuneLayer , IComponent , IMultiState
	 */
	class IMultiStateImageBox : 
		public IJuneLayer, 
		public IComponent,
		public IMultiState
	{
	public:
		/**
		 * @brief 设置图层属性
		 * @param pParam 图层属性
		 * @return 属性设置成功，返回TRUE，否则返回FALSE
		 * @sa IVariableParam
		 */
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
};

#endif

// InterfaceName:MultiStateImageBox
// SubFolderName:Layer
