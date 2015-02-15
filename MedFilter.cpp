// MedFilter.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "MedFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MedFilter

CMedFilter::CMedFilter()
{
}

CMedFilter::~CMedFilter()
{
}


BEGIN_MESSAGE_MAP(CMedFilter, CWnd)
	//{{AFX_MSG_MAP(MedFilter)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MedFilter message handlers

void CMedFilter::Median(HDIB hDIB)
{   
	// 循环变量
	LONG i;
	LONG j;
	LONG k;
	LONG l;

	// 掩码模版
	unsigned char mask_r[9];
	unsigned char mask_g[9];
	unsigned char mask_b[9];
	COLORREF mask[9];

	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 24)
	{
		// 提示用户
		MessageBox("请先将其转换为24位色位图，再进行处理！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 申请并分配中间缓存
	HLOCAL hLocal = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal == NULL)
		return;
	LPBYTE m_temp = (LPBYTE)LocalLock(hLocal);

	// 复制图象数据到中间缓存
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

	// 模版滤波
	for (i = 0; i < lWidth; i++)			//被处理像素在i列
	{
		for (j = 0; j < lHeight; j++)		//被处理像素在j行
		{
			// 索引
			int	id = 0;

			// 进行小区域模版滤波
			for (k = i - 1; k < i + 2; k++)
			{
				for(l = j - 1; l < j + 2; l++)
				{
					// 防止内存溢出
					if (k >= 0 && l >= 0 && k < lWidth && l < lHeight)
					{
						mask_r[id] = *((unsigned char *)lpDIBBits + l * lLineBytes + k * 3);
						mask_g[id] = *((unsigned char *)lpDIBBits + l * lLineBytes + k * 3 + 1);
						mask_b[id] = *((unsigned char *)lpDIBBits + l * lLineBytes + k * 3 + 2);
				
						mask[id] = RGB(mask_r[id], mask_g[id], mask_b[id]);
						id++;
					}
				}
			}
			
			// 中间变量
			unsigned char T;
			
			// 冒泡排序法
			for (k = 0; k < 8; k++)		
			{
				for (l = 8; l > k; l--)					
				{
					if (mask_r[l] < mask_r[l - 1])
					{
						T = mask_r[l];
						mask_r[l] = mask_r[l - 1];
						mask_r[l - 1] = T;
					}
					if (mask_g[l] < mask_g[l - 1])
					{
						T = mask_g[l];
						mask_g[l] = mask_g[l - 1];
						mask_g[l - 1] = T;
					}
					if (mask_b[l] < mask_b[l - 1])
					{
						T = mask_b[l];
						mask_b[l] = mask_b[l - 1];
						mask_b[l - 1] = T;
					}
				}
			}
			m_temp[j * lLineBytes + i * 3]     = mask_r[4];
			m_temp[j * lLineBytes + i * 3 + 1] = mask_g[4];
			m_temp[j * lLineBytes + i * 3 + 2] = mask_b[4];

		

		}
	}

	// 将转换后的中间缓存数据回存到DIB
/*	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
		}
	}
*/	
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = m_temp[i * lLineBytes + j];
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = m_temp[i * lLineBytes + j];
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = m_temp[i * lLineBytes + j];
		}
	}
	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	LocalUnlock(hLocal);
	LocalFree(hLocal);

	// 恢复光标
	EndWaitCursor();

}
/************************************************************************************/
void CMedFilter::Adapt(HDIB hDIB)
{	
	// 循环变量
	LONG i;
	LONG j;
	LONG k;
	LONG l;

	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 24)
	{
		// 提示用户
		MessageBox("请先将其转换为24位色位图，再进行处理！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 申请并分配中间缓存
	HLOCAL hLocal = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal == NULL)
		return;
	LPBYTE min = (LPBYTE)LocalLock(hLocal);

	HLOCAL hLocal2 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal2 == NULL)
		return;
	LPBYTE med = (LPBYTE)LocalLock(hLocal2);

	HLOCAL hLocal3 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal3 == NULL)
		return;
	LPBYTE max = (LPBYTE)LocalLock(hLocal3);

	HLOCAL hLocal4 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal4 == NULL)
		return;
	LPBYTE m_temp = (LPBYTE)LocalLock(hLocal4);

	// 复制图象数据到中间缓存
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(m_temp + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}
	
	int maxwindowwidth = 9;
	int pos = (maxwindowwidth-1)/2;
//  for (int m = pos; m < lHeight-pos; m++)     第二种方法，边缘未处理
	for (int m = 0; m < lHeight; m++)		//被处理像素在i列
	{
	  //for (int n = pos; n < lWidth-pos;n++)   第二种方法，边缘未处理
		for (int n = 0; n < lWidth;n++)		//被处理像素在j行
		{
			int start = 3;     //开始为3*3模板
			while (start <= maxwindowwidth)
			{ 
				int ak = (start-1)/2;
				int	id = 0;	
				int mnumber = start*start;
				int* mask_r=new int[mnumber];
				int* mask_g=new int[mnumber];
				int* mask_b=new int[mnumber];

				 // 进行小区域模版滤波,第二种方法，边缘未处理
/*				 for(int i=-ak;i < ak+1;i++)
				 {

					for (int j= -ak;j < ak+1;j++)
					{	
							mask_r[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3);
							mask_g[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3 + 1);
							mask_b[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3 + 2);
							//mask[id] = RGB(mask_r[id], mask_g[id], mask_b[id]);
						    id++;											
					}
				 }
*/
				

			// 进行小区域模版滤波,同3*3方法
			for (k = m - ak; k < m + ak+1; k++)
			{
				for(l = n - ak; l < n + ak+1; l++)
				{
					// 防止内存溢出
					if (k >= 0 && l >= 0 && l < lWidth && k < lHeight)
					{
						mask_r[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3);
						mask_g[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3 + 1);
						mask_b[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3 + 2);						
						id++;
					}
				}
			}



				// 中间变量
				unsigned char T;
			
				// 冒泡排序法
				for (k = 0; k < mnumber-1; k++)		
				{
					for (l = mnumber-1; l > k; l--)					
					{
						if (mask_r[l] < mask_r[l - 1])
						{
							T = mask_r[l];
							mask_r[l] = mask_r[l - 1];
							mask_r[l - 1] = T;
						}
						if (mask_g[l] < mask_g[l - 1])
						{
							T = mask_g[l];
							mask_g[l] = mask_g[l - 1];
							mask_g[l - 1] = T;
						}
						if (mask_b[l] < mask_b[l - 1])
						{
							T = mask_b[l];
							mask_b[l] = mask_b[l - 1];
							mask_b[l - 1] = T;
						}
					}
				}

/*	int min_r,max_r,med_r;
	int min_g,max_g,med_g;
	int min_b,max_b,med_b;
*/	int A1,A2,A3,A4,A5,A6,B1,B2,B3,B4,B5,B6;

				//给临时变量赋值
				min[m * lLineBytes + n * 3]     = mask_r[0];
				min[m * lLineBytes + n * 3 + 1] = mask_g[0];
				min[m * lLineBytes + n * 3 + 2] = mask_b[0];

				med[m * lLineBytes + n * 3]     = mask_r[(mnumber-1)/2];
				med[m * lLineBytes + n * 3 + 1] = mask_g[(mnumber-1)/2];
				med[m * lLineBytes + n * 3 + 2] = mask_b[(mnumber-1)/2];

				max[m * lLineBytes + n * 3]     = mask_r[mnumber-1];
				max[m * lLineBytes + n * 3 + 1] = mask_g[mnumber-1];
				max[m * lLineBytes + n * 3 + 2] = mask_b[mnumber-1];


				// 将转换后的中间缓存数据回存到DIB
				
				//*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(m_temp + lLineBytes * m + n*3);			
				//*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(m_temp + lLineBytes * m + n*3+1);			
				//*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(m_temp + lLineBytes * m + n*3+2);

				//开始与最大最小值判断
				A1 = med[m * lLineBytes + n * 3]     -min[m * lLineBytes + n * 3];
				A2 = med[m * lLineBytes + n * 3 + 1] -min[m * lLineBytes + n * 3 + 1];
				A3 = med[m * lLineBytes + n * 3 + 2] -min[m * lLineBytes + n * 3 + 2];				
				
				A4 = med[m * lLineBytes + n * 3]     -max[m * lLineBytes + n * 3];
				A5 = med[m * lLineBytes + n * 3 + 1] -max[m * lLineBytes + n * 3 + 1];
				A6 = med[m * lLineBytes + n * 3 + 2] -max[m * lLineBytes + n * 3 + 2];

				if (A1 > 0 && A2 > 0 && A3 > 0 && A4 < 0 && A5 < 0 && A6< 0)
				{
					//B1 = inbuffer[m*width+n] - min;
					B1 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -min[m * lLineBytes + n * 3];
					B2 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-min[m * lLineBytes + n * 3+1];
					B3 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-min[m * lLineBytes + n * 3+2];

					//B2 = inbuffer[m*width+n] - max;
					B4 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -max[m * lLineBytes + n * 3];
					B5 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-max[m * lLineBytes + n * 3+1];
					B6 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-max[m * lLineBytes + n * 3+2];
					
					
					if (B1 > 0 && B2 > 0 && B3 > 0 && B4 < 0 && B5 < 0 && B6< 0)
					{
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(m_temp + lLineBytes * m + n*3);						
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(m_temp + lLineBytes * m + n*3+1);						
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(m_temp + lLineBytes * m + n*3+2);
						delete[] mask_r;
						delete[] mask_g;
						delete[] mask_b;
						break;
					}
					else
					{
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(med + lLineBytes * m + n*3);						
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(med + lLineBytes * m + n*3+1);						
						*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(med + lLineBytes * m + n*3+2);
						delete[] mask_r;
						delete[] mask_g;
						delete[] mask_b;
						break;
					}

				}
	
      		start += 2;			
			delete[] mask_r;
			delete[] mask_g;
			delete[] mask_b;
		}
	}}

/*	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
			j++;
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = *(m_temp + lLineBytes * i + j);
		}
	}
*/	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	LocalUnlock(hLocal);
	LocalFree(hLocal);
	LocalUnlock(hLocal2);
	LocalFree(hLocal2);
	LocalUnlock(hLocal3);
	LocalFree(hLocal3);
	LocalUnlock(hLocal4);
	LocalFree(hLocal4);

	// 恢复光标
	EndWaitCursor();
}
/************************************************************************************/
void CMedFilter::Improve(HDIB hDIB)
{	
	// 循环变量
	int i,j,k,l;

	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 24)
	{
		// 提示用户
		MessageBox("请先将其转换为24位色位图，再进行处理！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 申请并分配中间缓存
	HLOCAL hLocal = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal == NULL)
		return;
	LPBYTE min = (LPBYTE)LocalLock(hLocal);

/*	HLOCAL hLocal2 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal2 == NULL)
		return;
	LPBYTE med = (LPBYTE)LocalLock(hLocal2);
*/
	HLOCAL hLocal3 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal3 == NULL)
		return;
	LPBYTE max = (LPBYTE)LocalLock(hLocal3);

	HLOCAL hLocal4 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal4 == NULL)
		return;
	LPBYTE org = (LPBYTE)LocalLock(hLocal4);

/*	HLOCAL hLocal5 = LocalAlloc(GHND, lLineBytes * lHeight);
	if (hLocal5 == NULL)
		return;
	LPBYTE final = (LPBYTE)LocalLock(hLocal4);
*/
	// 复制图象数据到中间缓存
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(min + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

/*	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(med + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}
*/
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(max + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}

	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			*(org + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(org + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			*(org + lLineBytes * i + j) = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
		}
	}
	
	int maxwindowwidth = 7;
    int msd1,msd2,msd3,th = 2; 
    //未污染点集
	int* untaint_r = new int[49]; 
	int* untaint_g = new int[49];
	int* untaint_b = new int[49];
	
//	int pos = (maxwindowwidth-1)/2;
//  for (int m = pos; m < lHeight-pos; m++)     第二种方法，边缘未处理
	for (int m = 0; m < lHeight; m++)		//被处理像素在i列
	{
	  //for (int n = pos; n < lWidth-pos;n++)   第二种方法，边缘未处理
		for (int n = 0; n < lWidth;n++)		//被处理像素在j行
		{
			int start = 3;     //开始为3*3模板
		    int untaintnum = 0;  //未污染点数
			//while (start <= maxwindowwidth)
			do{ 
				int ak = (start-1)/2;
				int	id = 0;	
				int mnumber = start*start;
				int* mask_r=new int[mnumber];
				int* mask_g=new int[mnumber];
				int* mask_b=new int[mnumber];

				 // 进行小区域模版滤波,第二种方法，边缘未处理,内存会溢出，初始m，n不能为0
/*				 for(int i=-ak;i < ak+1;i++)
				 {

					for (int j= -ak;j < ak+1;j++)
					{	
							mask_r[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3);
							mask_g[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3 + 1);
							mask_b[id] = *((unsigned char *)lpDIBBits + (m+i) * lLineBytes + (j+n) * 3 + 2);
							//mask[id] = RGB(mask_r[id], mask_g[id], mask_b[id]);
						    id++;											
					}
				 }
*/
				

			// 进行小区域模版滤波,同3*3方法
			for (k = m - ak; k < m + ak+1; k++)
			{
				for(l = n - ak; l < n + ak+1; l++)
				{
					// 防止内存溢出
					if (k >= 0 && l >= 0 && l < lWidth && k < lHeight )
					{
						mask_r[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3);
						mask_g[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3 + 1);
						mask_b[id] = *((unsigned char *)lpDIBBits + k * lLineBytes + l * 3 + 2);						
						id++;
					}
				}
			}

				// 中间变量
				unsigned char T;
			
				// 冒泡排序法
				for (k = 0; k < mnumber-1; k++)		
				{
					for (l = mnumber-1; l > k; l--)					
					{
						if (mask_r[l] < mask_r[l - 1])
						{
							T = mask_r[l];
							mask_r[l] = mask_r[l - 1];
							mask_r[l - 1] = T;
						}
						if (mask_g[l] < mask_g[l - 1])
						{
							T = mask_g[l];
							mask_g[l] = mask_g[l - 1];
							mask_g[l - 1] = T;
						}
						if (mask_b[l] < mask_b[l - 1])
						{
							T = mask_b[l];
							mask_b[l] = mask_b[l - 1];
							mask_b[l - 1] = T;
						}
					}
				}

/*	int min_r,max_r,med_r;
	int min_g,max_g,med_g;
	int min_b,max_b,med_b;
	int A1,A2,A3,A4,A5,A6;
*/	int	B1,B2,B3,B4,B5,B6;
	int	C1,C2,C3,C4,C5,C6;

				//给临时变量赋值
				min[m * lLineBytes + n * 3]     = mask_r[0];
				min[m * lLineBytes + n * 3 + 1] = mask_g[0];
				min[m * lLineBytes + n * 3 + 2] = mask_b[0];

	/*			med[m * lLineBytes + n * 3]     = mask_r[(mnumber-1)/2];
				med[m * lLineBytes + n * 3 + 1] = mask_g[(mnumber-1)/2];
				med[m * lLineBytes + n * 3 + 2] = mask_b[(mnumber-1)/2];

	*/			max[m * lLineBytes + n * 3]     = mask_r[mnumber-1];
				max[m * lLineBytes + n * 3 + 1] = mask_g[mnumber-1];
				max[m * lLineBytes + n * 3 + 2] = mask_b[mnumber-1];


	         	//B1 = inbuffer[m*width+n] - min;
				B1 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -min[m * lLineBytes + n * 3];
				B2 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-min[m * lLineBytes + n * 3+1];
				B3 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-min[m * lLineBytes + n * 3+2];

				//B2 = inbuffer[m*width+n] - max;
				B4 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -max[m * lLineBytes + n * 3];
				B5 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-max[m * lLineBytes + n * 3+1];
				B6 = *((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-max[m * lLineBytes + n * 3+2];

			//	int zz=*((unsigned char *)lpDIBBits + lLineBytes * m + n*3);
			//	int yy=med[m * lLineBytes + n * 3];
			//	int yy=RGB(0,255,0);
					
					
				if (B1 > 0 && B2 > 0 && B3 > 0 && B4 < 0 && B5 < 0 && B6< 0)
				{
				    *((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(org + lLineBytes * m + n*3);	
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(org + lLineBytes * m + n*3+1);
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(org + lLineBytes * m + n*3+2);
					delete[] mask_r;
					delete[] mask_g;
					delete[] mask_b;
					break;
				}
/*				else
					{			        	
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(med + lLineBytes * m + n*3);
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(med + lLineBytes * m + n*3+1);
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(med + lLineBytes * m + n*3+2);

					delete[] mask_r;
					delete[] mask_g;
					delete[] mask_b;
					break;
				}
  */   			
				
				
				
				else if(untaintnum == 0)//空					
				{
					for (k = m - ak; k < m + ak+1; k++)
					{
						for(l = n - ak; l < n + ak+1; l++)
						{
							// 防止内存溢出,非中心
							if (k >= 0 && l >= 0 && l < lWidth && k < lHeight  && (k!=m || l!=n))
							{							
								//C1 =inbuffer[(m+i)*width+n+j - min
								C1 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3)  -min[k * lLineBytes + l * 3];
								C2 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3+1)-min[k * lLineBytes + l * 3 + 1];
								C3 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3+2)-min[k * lLineBytes + l * 3 + 2];

								//C2 = inbuffer[(m+i)*width+n+j - max
								C4 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3)  -max[k * lLineBytes + l * 3];
								C5 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3+1)-max[k * lLineBytes + l * 3 + 1];
								C6 = *((unsigned char *)lpDIBBits + lLineBytes * k + l*3+2)-max[k * lLineBytes + l * 3 + 2];
							
								if(C1 > 0 && C2 > 0 && C3 > 0 && C4 < 0 && C5 < 0 && C6< 0)
								{   
									untaint_r[untaintnum]=*((unsigned char *)lpDIBBits + lLineBytes * k + l*3);
									untaint_g[untaintnum]=*((unsigned char *)lpDIBBits + lLineBytes * k + l*3+1);
									untaint_b[untaintnum]=*((unsigned char *)lpDIBBits + lLineBytes * k + l*3+2);
									untaintnum++;								
								}

							}
							
						}
					}
				}
				delete[] mask_r;
				delete[] mask_g;
				delete[] mask_b;
				start+=2;
				}while (start <= maxwindowwidth);

				if (untaintnum == 0)//空，颜色相同？
				{
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(org + lLineBytes * m + n*3);						
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(org + lLineBytes * m + n*3+1);						
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(org + lLineBytes * m + n*3+2);					
				}
				else//非空,未污染点排序
				{
				unsigned char T;
				// 冒泡排序法
				for (k = 0; k < untaintnum-1; k++)		
				{
					for (l = untaintnum-1; l > k; l--)					
					{
						if (untaint_r[l] < untaint_r[l - 1])
						{
							T = untaint_r[l];
							untaint_r[l] = untaint_r[l - 1];
							untaint_r[l - 1] = T;
						}
						if (untaint_g[l] < untaint_g[l - 1])
						{
							T = untaint_g[l];
							untaint_g[l] = untaint_g[l - 1];
							untaint_g[l - 1] = T;
						}
						if (untaint_b[l] < untaint_b[l - 1])
						{
							T = untaint_b[l];
							untaint_b[l] = untaint_b[l - 1];
							untaint_b[l - 1] = T;
						}
					}
				} 
				int A1,A2,A3,B1,B2,B3;
				//开始与最大最小值判断
				A1 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -untaint_r[0]);
				A2 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-untaint_g[0]);
				A3 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-untaint_b[0]);				
				
				B1 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)  -untaint_r[untaintnum-1]);
				B2 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1)-untaint_g[untaintnum-1]);
				B3 = abs(*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2)-untaint_b[untaintnum-1]);

				msd1 = A1 < B1 ? A1 : B1;
				msd2 = A2 < B2 ? A2 : B2;
				msd3 = A3 < B3 ? A3 : B3;

				if(msd1 <= th && msd2 <= th && msd3 <= th )//小于阈值的点保留
				{
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = *(org + lLineBytes * m + n*3);						
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = *(org + lLineBytes * m + n*3+1);						
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = *(org + lLineBytes * m + n*3+2);					
				}
				else
				{
				/*	final[m * lLineBytes + n * 3    ] = untaint_r[(untaintnum-1)/2];					
					final[m * lLineBytes + n * 3 + 1] = untaint_g[(untaintnum-1)/2];						
					final[m * lLineBytes + n * 3 + 2] = untaint_b[(untaintnum-1)/2];
//					int yy=untaintnum;
				*/
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3)   = untaint_r[(untaintnum-1)/2];				
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+1) = untaint_g[(untaintnum-1)/2];				
					*((unsigned char *)lpDIBBits + lLineBytes * m + n*3+2) = untaint_b[(untaintnum-1)/2];
                }

		
				}
}} 
		delete[] untaint_r;
		delete[] untaint_g;
		delete[] untaint_b;

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);
	LocalUnlock(hLocal);
	LocalFree(hLocal);
//	LocalUnlock(hLocal2);
//	LocalFree(hLocal2);
	LocalUnlock(hLocal3);
	LocalFree(hLocal3);
	LocalUnlock(hLocal4);
	LocalFree(hLocal4);

	// 恢复光标
	EndWaitCursor();
}
