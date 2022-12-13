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
	 * @brief ��״̬ͼƬ��ͼ��
	 *
	 * ��Ϊͼ������#STATEID_NORMAL ��#STATEID_HOVER ��#STATEID_PRESSED ��#STATEID_DISABLE ����״̬��ͼƬ��
	 * ������Ҫת��ͼ���״̬
	 * @sa IJuneLayer , IComponent , IMultiState
	 */
	class IMultiStateImageBox : 
		public IJuneLayer, 
		public IComponent,
		public IMultiState
	{
	public:
		/**
		 * @brief ����ͼ������
		 * @param pParam ͼ������
		 * @return �������óɹ�������TRUE�����򷵻�FALSE
		 * @sa IVariableParam
		 */
		virtual BOOL SetProperty(IVariableParam* pParam) = 0;
	};
};

#endif

// InterfaceName:MultiStateImageBox
// SubFolderName:Layer
