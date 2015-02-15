// Noise.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "Noise.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoise

CNoise::CNoise()
{
}

CNoise::~CNoise()
{
}


BEGIN_MESSAGE_MAP(CNoise, CWnd)
	//{{AFX_MSG_MAP(CNoise)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNoise message handlers



void CNoise::RandomNoise(HDIB hDIB)
{   // 循环变量
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
	
	// 生成伪随机种子
	srand((unsigned)time(NULL));

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// 取随机值
			LONG Temp = rand();

			// 对像素各颜色分量进行随机加噪处理
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = R * 224 / 256 + Temp / 1024;
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = G * 224 / 256 + Temp / 1024;
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = B * 224 / 256 + Temp / 1024;
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();

}



void CNoise::SaltNoise(HDIB hDIB)
{   // 循环变量
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
	
	// 生成伪随机种子
	srand((unsigned)time(NULL));

	// 对各像素进行灰度转换
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// 取随机值
			if (rand() > 32500)
			{
				// 对像素各颜色分量进行随机加噪处理
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
			}
		}
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();

}
