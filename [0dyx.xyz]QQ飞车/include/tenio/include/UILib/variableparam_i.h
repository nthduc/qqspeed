#ifndef __VARIABLEPARAM_I_H__
#define __VARIABLEPARAM_I_H__

namespace Tenio
{
	class IVariableParam;
	TENIO_MAKE_TYPEID_MAP(IVariableParam, COMPONENTID_VARIABLEPARAM);

	/**
	 * @ingroup tenuilib
	 * @brief 参数集合类
	 *
	 * <p>
	 *　　 可以存储五种类型的参数信息(int, DWORD, BYTE, float和字符串)，BOOL类型按int类型存放
	 * </p>
	 *
	 * @sa 
	 */	
	class IVariableParam : public IComponent
	{
	public:

		/**
		 *  @brief 设置参数值, 如果该参数ID已存在，则修改之
		 *  @param dwPropertyID 参数ID
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */		
		virtual BOOL SetValue(DWORD dwPropertyID, int nValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, LPCTSTR lpszValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, DWORD dwValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, BYTE byValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, float fValue) = 0;

		/**
		 *  @brief 根据ID获得参数值
		 *  @param dwPropertyID 参数的ID
		 *  @param bDelete 是否在取出值以后删除对象中存放的参数值
		 *  @return 返回取出的值
		 *  @sa 
		 */	
		virtual int	  GetInt(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual BYTE  GetByte(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual DWORD GetDword(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual float GetFloat(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual void  GetString(DWORD dwPropertyID, LPTSTR lpszValue, UINT nMaxLen, BOOL bDelete) = 0;
		virtual int   GetStringLen(DWORD dwPropertyID) = 0;
		
		/**
		 *  @brief 判断参数是否存在
		 *  @param dwPropertyID 被判断是否存在的参数ID
		 *  @return 存在则返回TRUE，不存在则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL IsIntPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsStringPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsDwordPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsBytePropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsFloatPropertyExist(DWORD dwPropertyID) = 0;

		/**
		 *  @brief 清除所有参数值
		 *  @sa 
		 */		
		virtual void Clear() = 0;

		/**
		 *  @brief 删除参数
		 *  @param dwPropertyID 被删除的参数ID
		 *  @sa 
		 */
		virtual void DeleteInt(DWORD dwPropertyID) = 0;
		virtual void DeleteByte(DWORD dwPropertyID) = 0;
		virtual void DeleteDword(DWORD dwPropertyID) = 0;
		virtual void DeleteFloat(DWORD dwPropertyID) = 0;
		virtual void DeleteString(DWORD dwPropertyID) = 0;

		/**
		 *  @brief 将一个对象中的所有参数拷贝到另一个对象中
		 *  @param pDstParam 目的参数对象指针
		 *  @param bClearFirst 是否在拷贝前清空目的参数对象中的所有参数
		 *  @sa 
		 */
		virtual void CopyTo(IVariableParam* pDstParam, BOOL bClearFirst) = 0;

		/**
		 *  @brief 拷贝单个参数
		 *  @param pDstParam 目的参数对象的指针
		 *  @param dwPropertyID 被拷贝参数的ID
		 *  @return 成功则返回TRUE，失败则返回FALSE
		 *  @sa 
		 */	
		virtual BOOL CopyInt(IVariableParam* pDstParam, DWORD dwPropertyID) = 0;
		virtual BOOL CopyByte(IVariableParam* pDstParam, DWORD dwPropertyID) = 0;
		virtual BOOL CopyDword(IVariableParam* pDstParam, DWORD dwPropertyID) = 0;
		virtual BOOL CopyFloat(IVariableParam* pDstParam, DWORD dwPropertyID) = 0;
		virtual BOOL CopyString(IVariableParam* pDstParam, DWORD dwPropertyID) = 0;		
	};
}
#endif