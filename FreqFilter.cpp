// FreqFilter.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "FreqFilter.h"


#include "FreqFilter.h"
#include "FreqCalculate.h"
#include <complex>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreqFilter

CFreqFilter::CFreqFilter()
{
}

CFreqFilter::~CFreqFilter()
{
}


BEGIN_MESSAGE_MAP(CFreqFilter, CWnd)
	//{{AFX_MSG_MAP(CFreqFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFreqFilter message handlers

void CFreqFilter::ButterworthL(HDIB hDIB, float fD)
{
    // 临时变量
	LONG	i;
	LONG	j,TI,TJ;

	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

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

	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate;

	// 保存频域数据
	complex<double> *FD, *TD,*TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD = new complex<double>[w * h * 3];

	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// 获取时域数值
				unsigned char Value = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			
				// 时域赋值
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// 否则补零
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// 进行频谱分析 时域->频域
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// 释放内存
	delete[] TD;

		for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// 保存转换后的频谱图像
			TempD[i * w * 3 + j] = FD[TI * w * 3 + TJ];
		}
	}

	// 当前频率
	float fDN;

	// 对图象进行低通滤波
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// 计算距离
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			// 构造巴特沃斯低通滤波器并滤波,fd是输入的
			TempD[i * 3 * w + j] *= complex<double>(1 / (1 + 0.414 * (fDN / fD) * (fDN / fD)), 0.0f);
		}
	}

		for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// 保存转换后的频谱图像
			FD[i * w * 3 + j] = TempD[TI * w * 3 + TJ];
		}
	}

	// 进行频谱分析 频域->时域
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;



	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 释放内存
	delete[] FD;

	// 恢复光标
	EndWaitCursor();
}

void CFreqFilter::Lpfilter(HDIB hDIB,int var) 
 //理想低通滤波器
{
	  // 临时变量
	LONG	i;
	LONG	j,TI,TJ;

	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

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

	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate;

	// 保存频域数据
	complex<double> *FD, *TD,*TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD= new complex<double>[w * h * 3];

	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// 获取时域数值
				unsigned char Value = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			
				// 时域赋值
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// 否则补零
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// 进行频谱分析 时域->频域
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// 释放内存
	delete[] TD;

	// 将原点放置于图像中心位置
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// 保存转换后的频谱图像
			TempD[i * w * 3 + j] = FD[TI * w * 3 + TJ];
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w*3;j++)
		{
			int x=abs(h/2-i);
			int y=abs(w/2-j);
			if((x*x+y*y)>var*var)
				TempD[i * 3 * w + j] = complex<double>(0.0,0.0);
		}
	}

		// 将原点放置于图像中心位置
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// 保存转换后的频谱图像
			FD[i * w * 3 + j] = TempD[TI * w * 3 + TJ];
		}
	}
	// 进行频谱分析 频域->时域
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 释放内存
	delete[] FD;

	// 恢复光标
	EndWaitCursor();
}