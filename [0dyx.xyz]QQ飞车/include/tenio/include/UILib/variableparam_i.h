#ifndef __VARIABLEPARAM_I_H__
#define __VARIABLEPARAM_I_H__

namespace Tenio
{
	class IVariableParam;
	TENIO_MAKE_TYPEID_MAP(IVariableParam, COMPONENTID_VARIABLEPARAM);

	/**
	 * @ingroup tenuilib
	 * @brief ����������
	 *
	 * <p>
	 *���� ���Դ洢�������͵Ĳ�����Ϣ(int, DWORD, BYTE, float���ַ���)��BOOL���Ͱ�int���ʹ��
	 * </p>
	 *
	 * @sa 
	 */	
	class IVariableParam : public IComponent
	{
	public:

		/**
		 *  @brief ���ò���ֵ, ����ò���ID�Ѵ��ڣ����޸�֮
		 *  @param dwPropertyID ����ID
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
		 *  @sa 
		 */		
		virtual BOOL SetValue(DWORD dwPropertyID, int nValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, LPCTSTR lpszValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, DWORD dwValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, BYTE byValue) = 0;
		virtual BOOL SetValue(DWORD dwPropertyID, float fValue) = 0;

		/**
		 *  @brief ����ID��ò���ֵ
		 *  @param dwPropertyID ������ID
		 *  @param bDelete �Ƿ���ȡ��ֵ�Ժ�ɾ�������д�ŵĲ���ֵ
		 *  @return ����ȡ����ֵ
		 *  @sa 
		 */	
		virtual int	  GetInt(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual BYTE  GetByte(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual DWORD GetDword(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual float GetFloat(DWORD dwPropertyID, BOOL bDelete) = 0;
		virtual void  GetString(DWORD dwPropertyID, LPTSTR lpszValue, UINT nMaxLen, BOOL bDelete) = 0;
		virtual int   GetStringLen(DWORD dwPropertyID) = 0;
		
		/**
		 *  @brief �жϲ����Ƿ����
		 *  @param dwPropertyID ���ж��Ƿ���ڵĲ���ID
		 *  @return �����򷵻�TRUE���������򷵻�FALSE
		 *  @sa 
		 */	
		virtual BOOL IsIntPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsStringPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsDwordPropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsBytePropertyExist(DWORD dwPropertyID) = 0;
		virtual BOOL IsFloatPropertyExist(DWORD dwPropertyID) = 0;

		/**
		 *  @brief ������в���ֵ
		 *  @sa 
		 */		
		virtual void Clear() = 0;

		/**
		 *  @brief ɾ������
		 *  @param dwPropertyID ��ɾ���Ĳ���ID
		 *  @sa 
		 */
		virtual void DeleteInt(DWORD dwPropertyID) = 0;
		virtual void DeleteByte(DWORD dwPropertyID) = 0;
		virtual void DeleteDword(DWORD dwPropertyID) = 0;
		virtual void DeleteFloat(DWORD dwPropertyID) = 0;
		virtual void DeleteString(DWORD dwPropertyID) = 0;

		/**
		 *  @brief ��һ�������е����в�����������һ��������
		 *  @param pDstParam Ŀ�Ĳ�������ָ��
		 *  @param bClearFirst �Ƿ��ڿ���ǰ���Ŀ�Ĳ��������е����в���
		 *  @sa 
		 */
		virtual void CopyTo(IVariableParam* pDstParam, BOOL bClearFirst) = 0;

		/**
		 *  @brief ������������
		 *  @param pDstParam Ŀ�Ĳ��������ָ��
		 *  @param dwPropertyID ������������ID
		 *  @return �ɹ��򷵻�TRUE��ʧ���򷵻�FALSE
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