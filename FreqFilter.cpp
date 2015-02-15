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
    // ��ʱ����
	LONG	i;
	LONG	j,TI,TJ;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

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

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
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

	// ����Ƶ������
	complex<double> *FD, *TD,*TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				unsigned char Value = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
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

			// ����ת�����Ƶ��ͼ��
			TempD[i * w * 3 + j] = FD[TI * w * 3 + TJ];
		}
	}

	// ��ǰƵ��
	float fDN;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt( i * i + k * k);		

			// ���������˹��ͨ�˲������˲�,fd�������
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

			// ����ת�����Ƶ��ͼ��
			FD[i * w * 3 + j] = TempD[TI * w * 3 + TJ];
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;



	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}

void CFreqFilter::Lpfilter(HDIB hDIB,int var) 
 //�����ͨ�˲���
{
	  // ��ʱ����
	LONG	i;
	LONG	j,TI,TJ;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

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

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
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

	// ����Ƶ������
	complex<double> *FD, *TD,*TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD= new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				unsigned char Value = *((unsigned char *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ԭ�������ͼ������λ��
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

			// ����ת�����Ƶ��ͼ��
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

		// ��ԭ�������ͼ������λ��
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

			// ����ת�����Ƶ��ͼ��
			FD[i * w * 3 + j] = TempD[TI * w * 3 + TJ];
		}
	}
	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ͷ��ڴ�
	delete[] FD;

	// �ָ����
	EndWaitCursor();
}