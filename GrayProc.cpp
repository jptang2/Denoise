// GrayProc.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "GrayProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrayProc

CGrayProc::CGrayProc()
{
}

CGrayProc::~CGrayProc()
{
}


BEGIN_MESSAGE_MAP(CGrayProc, CWnd)
	//{{AFX_MSG_MAP(CGrayProc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrayProc message handlers

void CGrayProc::GrayStretch(HDIB hDIB, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
	// 循环变量
	LONG i;
	LONG j;

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

	// 灰度映射表
	BYTE	bMap[256];

	// 计算灰度映射表
	for (i = 0; i <= bX1; i++)	//[0 —— X1]
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)		//(X1 —— X2]
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)		//(X2 —— 256)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			bMap[i] = 255;
		}
	}
	
	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// 对像素各颜色分量进行灰度映射处理
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[R];
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[G];
			j++;

			// 在此如用lWidth * 3则有彩色效果，如用lLineBytes则为普通灰度处理
			unsigned char B = *((unsigned char *)lpDIBBits + lWidth * 3 * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[B];
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, int *nNs_Y)
{
	// 循环变量
	LONG i;
	LONG j;

	// 变量初始化
	memset(nNs_Y, 0, sizeof(nNs_Y));

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

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);

			// 计算灰度值
			unsigned char Y = (9798 * R + 19235 * G + 3735 * B) / 32768;

			// 灰度统计计数
			nNs_Y[Y]++;		
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, float *fPs_R, float *fPs_G, float *fPs_B)
{
	// 循环变量
	LONG i;
	LONG j;

	// 灰度计数
	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];

	// 变量初始化
	memset(nNs_R,0,sizeof(nNs_R));
	memset(nNs_G,0,sizeof(nNs_G));
	memset(nNs_B,0,sizeof(nNs_B));

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

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// 对各像素进行灰度统计
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			nNs_R[R]++;		
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			nNs_G[G]++;
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			nNs_B[B]++;
		}
	}

	// 计算灰度分布密度
	for(i=0;i<256;i++)
	{
		fPs_R[i] = nNs_R[i] / (lHeight * lWidth * 1.0f);
		fPs_G[i] = nNs_G[i] / (lHeight * lWidth * 1.0f);
		fPs_B[i] = nNs_B[i] / (lHeight * lWidth * 1.0f);
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}


void CGrayProc::PointEqua(HDIB hDIB)
{
	// 循环变量
	LONG i;
	LONG j;

	// 灰度分布密度
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// 中间变量
	float temp_r[256];
	float temp_g[256];
	float temp_b[256];

	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];
	
	// 初始化
	memset(temp_r, 0, sizeof(temp_r));
	memset(temp_g, 0, sizeof(temp_g));
	memset(temp_b, 0, sizeof(temp_b));

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
	
	// 获取图像的灰度分布密度
	GetIntensity(hDIB, fPs_R, fPs_G, fPs_B);
	
	// 进行均衡化处理 
	for(i = 0; i < 256; i++)
	{
		if(i == 0)
		{
			temp_r[0] = fPs_R[0];
			temp_g[0] = fPs_G[0];
			temp_b[0] = fPs_B[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + fPs_R[i];
			temp_g[i] = temp_g[i-1] + fPs_G[i];
			temp_b[i] = temp_b[i-1] + fPs_B[i];
		}
		nNs_R[i] = (int)(255.0f * temp_r[i] + 0.5f);
		nNs_G[i] = (int)(255.0f * temp_g[i] + 0.5f);
		nNs_B[i] = (int)(255.0f * temp_b[i] + 0.5f);
	}

	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// 将搬移后的灰度分布写入DIB图像
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = nNs_R[R];
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = nNs_G[G];
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = nNs_B[B];
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, float *fPs_Y)
{
	// 循环变量
	LONG i;
	LONG j;

	// 灰度计数
	int nNs_Y[256];

	// 变量初始化
	memset(nNs_Y,0,sizeof(nNs_Y));

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

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);

			// 计算灰度值
			unsigned char Y = (9798 * R + 19235 * G + 3735 * B) / 32768;

			// 灰度统计计数
			nNs_Y[Y]++;		
		}
	}

	// 计算灰度分布密度
	for(i=0;i<256;i++)
		fPs_Y[i] = nNs_Y[i] / (lHeight * lWidth * 1.0f);

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}
