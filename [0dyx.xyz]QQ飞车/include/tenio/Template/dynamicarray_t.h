#pragma once

namespace Tenio
{

	const int g_nDefaultSize = 5;
	const int g_nIncSize = 5;

	class TDynamicArrayManipulator
	{
	protected:
		template<typename ElementType>
		void AllocDefaultDynamicArray(ElementType** &pDynamciArray, int &nSize)
		{
			nSize = g_nDefaultSize;
				
			pDynamciArray = new ElementType*[g_nDefaultSize];
		}
		
		template<typename ElementType>
		void FreeDynamicArray(ElementType** &pDynamciArray, int &nCount, int &nSize)
		{
			if (pDynamciArray != NULL) 
			{
				delete [] pDynamciArray;
				pDynamciArray = NULL;
				nCount = 0;
				nSize = 0;
			}
		}

		template<typename ElementType>
		void DestoryDynamicArray(ElementType** &pDynamciArray, int &nCount, int &nSize)
		{
			if (pDynamciArray != NULL) 
			{
				for(int i = 0; i < nCount; i++)
				{
					pDynamciArray[i]->Free();
				}
				
				delete [] pDynamciArray;
				pDynamciArray = NULL;
				nCount = 0;
				nSize = 0;
			}
		}
		
		template<typename ElementType>
		void IncDynamicArray(ElementType** &pDynamciArray, int nCount, int &nSize)
		{
			nSize += g_nIncSize; // Increase size
			
			// Cache old array pointer
			ElementType** pTempDynamicArray = pDynamciArray;
			
			pDynamciArray = new ElementType*[nSize];
			
			// Copy exist elements 
			for(int i = 0; i < nCount; i++)
			{
				pDynamciArray[i] = pTempDynamicArray[i];
			}
			
			// Free old array memory
			int nDummy = 0;
			FreeDynamicArray(pTempDynamicArray, nCount, nDummy);
		}
		
		template<typename ElementType>
		void AppendDynamicArrayElement(
			ElementType* pElement, 
			ElementType** &pDynamciArray, 
			int &nCount, 
			int &nSize
			)
		{
			if (pElement != NULL) 
			{
				if (nCount == nSize) 
				{
					IncDynamicArray(pDynamciArray, nCount, nSize);
				}
				
				pDynamciArray[nCount] = pElement;
				nCount++;
			}
		}
	};

};