#ifndef __MATERIAL_I_H__
#define __MATERIAL_I_H__

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief ���л��ƶ��󹫹��ӿ�
	 *
	 * <p>
	 *���� 
	 * </p>
	 *
	 * @sa 
	 */
	
	class IMaterial
	{
	public:
		/**
		 *  @brief ��ø�ʽID
		 *  @return ���ظ�ʽID��ȡֵΪ enumIMAGEFORMATID 
		 *  @sa 
		 */
		virtual int GetFormatId() = 0;
	};
};

#endif // __MATERIAL_I_H__