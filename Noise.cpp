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
{   // ѭ������
	LONG i;
	LONG j;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_clsDIB.DIBBitCount(lpDIB) != 24)
	{
		// ��ʾ�û�
		MessageBox("���Ƚ���ת��Ϊ24λɫλͼ���ٽ��д���", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);
	
	// ����α�������
	srand((unsigned)time(NULL));

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// ȡ���ֵ
			LONG Temp = rand();

			// �����ظ���ɫ��������������봦��
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

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();

}



void CNoise::SaltNoise(HDIB hDIB)
{   // ѭ������
	LONG i;
	LONG j;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_clsDIB.DIBBitCount(lpDIB) != 24)
	{
		// ��ʾ�û�
		MessageBox("���Ƚ���ת��Ϊ24λɫλͼ���ٽ��д���", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);
	
	// ����α�������
	srand((unsigned)time(NULL));

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// ȡ���ֵ
			if (rand() > 32500)
			{
				// �����ظ���ɫ��������������봦��
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((unsigned char *)lpDIBBits + lLineBytes * i + j) = 0;
			}
		}
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();

}
