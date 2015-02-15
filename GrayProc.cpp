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
	// ѭ������
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

	// �Ҷ�ӳ���
	BYTE	bMap[256];

	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)	//[0 ���� X1]
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (BYTE) bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)		//(X1 ���� X2]
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)		//(X2 ���� 256)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}
	
	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// �����ظ���ɫ�������лҶ�ӳ�䴦��
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[R];
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[G];
			j++;

			// �ڴ�����lWidth * 3���в�ɫЧ��������lLineBytes��Ϊ��ͨ�Ҷȴ���
			unsigned char B = *((unsigned char *)lpDIBBits + lWidth * 3 * i + j);
			*((unsigned char *)lpDIBBits + lLineBytes * i + j) = bMap[B];
		}
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, int *nNs_Y)
{
	// ѭ������
	LONG i;
	LONG j;

	// ������ʼ��
	memset(nNs_Y, 0, sizeof(nNs_Y));

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

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);

			// ����Ҷ�ֵ
			unsigned char Y = (9798 * R + 19235 * G + 3735 * B) / 32768;

			// �Ҷ�ͳ�Ƽ���
			nNs_Y[Y]++;		
		}
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, float *fPs_R, float *fPs_G, float *fPs_B)
{
	// ѭ������
	LONG i;
	LONG j;

	// �Ҷȼ���
	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];

	// ������ʼ��
	memset(nNs_R,0,sizeof(nNs_R));
	memset(nNs_G,0,sizeof(nNs_G));
	memset(nNs_B,0,sizeof(nNs_B));

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

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// �Ը����ؽ��лҶ�ͳ��
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

	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
	{
		fPs_R[i] = nNs_R[i] / (lHeight * lWidth * 1.0f);
		fPs_G[i] = nNs_G[i] / (lHeight * lWidth * 1.0f);
		fPs_B[i] = nNs_B[i] / (lHeight * lWidth * 1.0f);
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}


void CGrayProc::PointEqua(HDIB hDIB)
{
	// ѭ������
	LONG i;
	LONG j;

	// �Ҷȷֲ��ܶ�
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// �м����
	float temp_r[256];
	float temp_g[256];
	float temp_b[256];

	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];
	
	// ��ʼ��
	memset(temp_r, 0, sizeof(temp_r));
	memset(temp_g, 0, sizeof(temp_g));
	memset(temp_b, 0, sizeof(temp_b));

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
	
	// ��ȡͼ��ĻҶȷֲ��ܶ�
	GetIntensity(hDIB, fPs_R, fPs_G, fPs_B);
	
	// ���о��⻯���� 
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

	// DIB�Ŀ��
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// �����ƺ�ĻҶȷֲ�д��DIBͼ��
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

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}

void CGrayProc::GetIntensity(HDIB hDIB, float *fPs_Y)
{
	// ѭ������
	LONG i;
	LONG j;

	// �Ҷȼ���
	int nNs_Y[256];

	// ������ʼ��
	memset(nNs_Y,0,sizeof(nNs_Y));

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

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			unsigned char R = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char G = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			j++;
			unsigned char B = *((unsigned char *)lpDIBBits + lLineBytes * i + j);

			// ����Ҷ�ֵ
			unsigned char Y = (9798 * R + 19235 * G + 3735 * B) / 32768;

			// �Ҷ�ͳ�Ƽ���
			nNs_Y[Y]++;		
		}
	}

	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		fPs_Y[i] = nNs_Y[i] / (lHeight * lWidth * 1.0f);

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}
