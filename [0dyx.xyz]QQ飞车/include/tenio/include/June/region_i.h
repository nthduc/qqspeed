#ifndef __JUNEREGION_I_H__
#define __JUNEREGION_I_H__

namespace Tenio
{
	/**
	 * @ingroup tenuilib
	 * @brief 更新区域管理
	 *
	 * <p>
	 *　　 该对象管理更新区域的合并，剪裁等操作，更新区域即脏区域。\n
	 *	下面举例说明如何从中获得当前需更新的所有矩形:\n
	 *	\n
	 *		int nRectCount = pRegion->EnumRects(NULL, 0);\n
	 *	\n
	 *		RECT* prcList = NULL;\n
	 *		TTenio_AllocMemory(&prcList, sizeof(RECT)*nRectCount, TRUE);\n
	 *		pRegion->EnumRects(prcList, nRectCount);\n
	 *	 \n
	 *		// do something using prcList ...\n
	 *	\n	   
	 *		TTenio_ReleaseMemory(prcList);\n
	 *	\n
	 * </p>
	 *
	 * @sa 
	 */	
	class IJuneRegion
	{
	public:
		/**
		 *  @brief 设置是否自动进行区域合并
		 *  @param bAutoMixe 处于 AutoMix 状态中时, 插入的形状会自动剔除与本形状重复的部分
		 *  @sa 
		 */
		virtual void SetAutoMix(BOOL bAutoMix) = 0;

		/**
		 *  @brief 手动触发 Mix, 所有重复的部分都被剪除
		 */
		virtual void MixChildren() = 0;

		/**
		 *  @brief 把一个矩形加入到形状中
		 *  @param prcNewRect 待加入的新矩形指针
		 *  @return 成功则返回TRUE，失败返回FALSE(例如矩形指针为NULL)
		 *  @sa 
		 */
		virtual BOOL InsertRect(const RECT* prcNewRect) = 0;

		/**
		 *  @brief 当前形状移除掉一个矩形
		 *  @param prcToRemoveRect 待移除的矩形指针
		 *  @return 成功则返回TRUE，失败返回FALSE(例如矩形指针为NULL)
		 *  @sa 
		 */
		virtual BOOL RemoveRect(const RECT* prcToRemoveRect) = 0;

		/**
		 *  @brief 清空 Region
		 *  @return 如无意外返回TRUE
		 *  @sa 
		 */
		virtual BOOL ClearAll() = 0;

		/**
		 *  @brief 枚举当前所有矩形
		 *  @param pRectList 保存矩形的内存指针，由外部分配
		 *  @param nCount 该内存可存入矩形的个数
		 *  @return pRectList  = NULL 或 nCount <= 0 时返回 Rect 的个数 \n
					pRectList != NULL 且 nCount > 0  时返回这些 Rect
		 *  @sa 
		 */
		virtual int EnumRects(RECT* pRectList, int nCount) = 0;

		/**
		 *  @brief 当前管理的区域是否为空
		 *  @return 空则返回TRUE，不空则返回FALSE
		 *  @sa 
		 */
		virtual BOOL IsEmpty() = 0;

		/**
		 *  @brief 平移当前被管理的所有区域
		 *  @param x 平移的x轴距离
		 *  @param y 平移的y轴距离
		 *  @sa 
		 */	
		virtual void Offset(int x, int y) = 0;

		/**
		 *  @brief 判断一个矩形区是否完成在region之外
		 *  @param rc　矩形区域
		 *  @return 是返回TRUE, 在区域内或重叠则返回FALSE
		 */	
		virtual BOOL RectOutOfRegion(RECT& rc) = 0;
	};
};

#endif