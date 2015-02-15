// ************************************************************************
//  �ļ�����FreqCalculate.cpp
//
//  ͼ�������任�����⣺
//
//	FFT()				- һά���ٸ���Ҷ�任
//  IFFT()				- һά���ٸ���Ҷ��任
//  Fourier()			- ��ά���ٸ���Ҷ�任
//  IFourier()			- ��ά���ٸ���Ҷ��任
//  DCT()				- һά������ɢ���ұ任
//  IDCT()				- һά������ɢ������任
//	FreqDCT()			- ��ά������ɢ���ұ任
//  IFreqDCT()			- ��ά������ɢ������任
//  WALSH()				- һά�ֶ�ʲ��������任
//  IWALSH()			- һά�ֶ�ʲ����������任
//	FreqWALSH()			- ��ά�ֶ�ʲ��������任
//	IFreqWALSH()		- ��ά�ֶ�ʲ����������任
//	DWT()				- ��ά�����С���ֽ�
//	IDWT()				- ��ά�����С���ع�
//	
//  DIBFourier()		- ͼ��ĸ���Ҷ�任
//  DIBDCT()			- ͼ�����ɢ���ұ任
//  DIBWalsh()			- ͼ����ֶ�ʲ��������任
//  DIBDWT()			- ͼ��Ķ�ά��ɢС���任
//
//*************************************************************************

#include "stdafx.h"
#include "TJPGR.h"
#include "FreqCalculate.h"
#include "math.h"
#include <complex>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// ������
#define PI 3.1415926535

/////////////////////////////////////////////////////////////////////////////
// CFreqCalculate

CFreqCalculate::CFreqCalculate()
{
}

CFreqCalculate::~CFreqCalculate()
{
}


BEGIN_MESSAGE_MAP(CFreqCalculate, CWnd)
	//{{AFX_MSG_MAP(CFreqCalculate)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFreqCalculate message handlers


/*************************************************************************
 * �������ƣ�
 *   FFT()
 * ����:
 *   complex<double> * TD	- ָ��ʱ�������ָ��
 *   complex<double> * FD	- ָ��Ƶ�������ָ��
 *   r						��2������������������
 * ����ֵ:
 *   �ޡ�
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 ************************************************************************/

VOID CFreqCalculate::FFT(complex<double> * TD, complex<double> * FD, int r)
{	
	LONG	count;				// ����Ҷ�任�ܵ���	
	int		i,j,k;				// ѭ������
	int		N,p;	            //NΪ��ǰÿ�����
	double	w;			    	// ����	
	complex<double> *W,*X1,*X2,*X;
	
	count = 1 << r;				// ���㸵��Ҷ�任�ܵ�����rΪ����
	
	// ������������洢��
	W  = new complex<double>[count / 2];                    //countΪ���յ���(256��512��)
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// �����Ȩϵ��(��ת����W),wΪ����
	for(i = 0; i < count / 2; i++) 
	{
		w = -i * PI * 2 / count;
		W[i] = complex<double> (cos(w), sin(w));            //cosʵ��
	}
	
	// ��ʱ���д��X1,X1Ϊ���룬X2Ϊ���
	memcpy(X1, TD, sizeof(complex<double>) * count);        
	
	// ���ٸ���Ҷ�任(��2Ƶ�ʳ�ȡ),p144ҳ,k�Ե�r-k-1
	//��->ÿ�����->����->���->���
	for(k = 0; k < r; k++)                                  //kΪ��ǰ�ݣ�k=0Ϊ��1��
	{   
		N = 1 << (r-k);                                     //NΪ��ǰ��ÿ�������Ƶ�ʳ�ȡ��һ�����,k=0,N=2^r��512-256-128....2
	  //for(j = 0; j < 1 << k; j++)
		for(j = 0; j < count/N; j++)                         //jΪ��ǰ�ݵ�������k=0ʱ��1����1��(j=0)��k=1ʱ��2����2��
		{
			                                 
			for(i = 0; i < N / 2; i++)                      //iΪ��ǰ��ÿ���㣬���Ժ�����������һ����
			{
				p = j * N;									//pΪ��ǰ�������������㵽��j��ʱ������ܵ���(���)
				X2[i + p] = X1[i + p] + X1[i + p + N / 2];  //ÿ���ϰ룬N/2Ϊ�����ڸ���256-128....1				
				X2[i + p + N / 2] = (X1[i + p] - X1[i + p + N / 2]) * W[i * (1<<k)];//ÿ���°룬[]��ΪW�ϱ�,Ϊÿ����*����
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������,�����λ�����λ�����������ÿһλi��Ȼ��Ѹ�λ��ֵ��������������Ӧλ
	//�ڵ�������Ϊ��r-i-1λ��rΪ������λ���������������ұ�Ϊ��λ
	
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)    //�����Ƶ�iλ��0...r-1
		{    
			if (j&(1<<i))         //j��λ��λ��������0�����ѭ��
			{   
				p+=1<<(r-i-1);    //�����������λi��ֵΪ1����������r-i-1����Ӧλ��1��p+���ۼ���������λ��1
			}
		}
		FD[j]=X1[p];			  //��λ
	}
	


	delete W;
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IFFT()
 *
 * ����:
 *   complex<double> * FD	- ָ��Ƶ��ֵ��ָ��
 *   complex<double> * TD	- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ��任��
 *
 ************************************************************************/

void CFreqCalculate::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ѭ������
	int		i;
	
	complex<double> *X;
	
	// ���㸶��Ҷ�任����
	LONG N = 1<<r;
	
	// ������������洢��
	X = new complex<double>[N];
	
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * N);
	
	// ����
	for (i = 0; i < N; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for (i = 0; i < N; i++)
	{
		TD[i] = complex<double> (TD[i].real() / N, -TD[i].imag() / N);
	}
	
	// �ͷ��ڴ�
	delete X;
}


/*************************************************************************
 *
 * �������ƣ�
 *   Fourier()
 *
 * ����:
 *   complex* TD		- �����ʱ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ�任��
 *
 ************************************************************************/

BOOL CFreqCalculate::Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	         
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];
	
	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempT[j] = TD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		FFT(TempT, TempF, hp);

		// ����任���
		for (j = 0; j < h; j++)
			TD[j * w * 3 + i] = TempF[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	// �����ڴ�
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempT[j] = TD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			FFT(TempT, TempF, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempF[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IFourier()
 *
 * ����:
 *   LPBYTE TD			- ���صĿ�������
 *   LONG lWidth		- ����ͼ��Ŀ��
 *	 LONG lHeight		- ����ͼ��ĸ߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ��任��
 *
 ************************************************************************/

BOOL CFreqCalculate::IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// ѭ������
	LONG	i;
	LONG	j;
	LONG    k;

	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempF[j] = FD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IFFT(TempF, TempT, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempT[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;
	
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempF[j] = FD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IFFT(TempF, TempT, hp);

		// ����任���
		for (j = 0; j < h; j++)
			FD[j * w * 3 + i] = TempT[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(TD + i * lLineBytes + j) = FD[i * w * 3 + j].real();
		}
	}

	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBFourier()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����и���Ҷ�任��
 *
 ************************************************************************/

BOOL CFreqCalculate::DIBFourier(HDIB hDIB)
{
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;

	// �м����
	double	dTemp;
	LONG TI,TJ;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ָ��DIB��ָ��
	LPBYTE	lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE    lpDIBBits;
	
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
		return FALSE;
	}

	// ���Ĺ����״
	BeginWaitCursor();
	
	// DIB�Ŀ��
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	// �����ڴ�
	complex<double> *FD, *TD, *TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD =  new complex<double>[w * h * 3];
	
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

	// ����Ƶ�׷���
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return FALSE;

	// �ͷ��ڴ�
	delete []TD;

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

	// ��
	for(i = (int)(h - lHeight) / 2; i < (int)(h + lHeight) / 2; i++)
	{
		// ��
		for(j = (int)(w * 3 - lLineBytes) / 2; j < (int)(w * 3 + lLineBytes) / 2; j += 3)
		{
			// ����Ƶ��
			dTemp = sqrt(TempD[w * 3 * i + j].real() * TempD[w * 3 * i + j].real() + 
				         TempD[w * 3 * i + j].imag() * TempD[w * 3 * i + j].imag()) / 100;

			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				dTemp = 255;
			}

			// ����Ϊԭͼ��С��Χ
			TI = i - (h - lHeight) / 2;
			TJ = j / 3 - (w - lWidth) / 2;
			
			// ��Ӧ����ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * TI + TJ * 3;

			// ����Դͼ��
			* (lpSrc) = (BYTE) (dTemp);
			* (lpSrc + 1) = (BYTE) (dTemp);
			* (lpSrc + 2) = (BYTE) (dTemp);
		}
	}

	// �������
	::GlobalUnlock(hDIB);

	// ɾ����ʱ����
	delete []FD;
	delete []TempD;
	
	// �ָ����
	EndWaitCursor();
	
	// ����
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////
//																	     //
//							С���任									 //
//																		 //
///////////////////////////////////////////////////////////////////////////

#define DD 13
float h[DD] = { -0.00332761f,0.00569794f,	0.0196637f,	-0.0482603f,	-0.0485391f,
				0.292562f,	0.564406f,	0.292562f,	-0.0485391f,	-0.0482602f,
				-0.0196637f,	0.00569794f,	-0.0033276f};
float g[DD] = {0.00332761f,  0.00569794f,	-0.0196637f,	-0.0482603f,	0.0485391f,
			   0.292562f,	-0.564406f,	0.292562f,	0.0485391f,	-0.0482602f,
			   0.0196637f,	0.00569794f,	0.0033276f};
float hi[DD];
float gi[DD];

int CFreqCalculate::a(int nX, int nXSize)
{
	if (nX < 0)
		nX = -nX;
	if (nX >= nXSize)
		nX = nXSize * 2 - nX - 2;
	return nX;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά���������ɢС���任��
 *
 ************************************************************************/

void CFreqCalculate::DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// ѭ������
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* BufferX;
	float* BufferY;

	BufferX = new float[nXSize];
	BufferY = new float[nYSize];

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (i = 0; i < nX; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + n * nX + a(i + j, nX)) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + n * nX + a(i + j, nX)) * g[j + (DD - 1) / 2 - 1];
			BufferX[i / 2] = temp1;
			BufferX[i / 2 + nX / 2] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = BufferX[k];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (i = 0; i < nY; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + a(i + j, nY) * nX + n) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + a(i + j, nY) * nX + n) * g[j + (DD - 1) / 2 - 1];
			BufferY[i / 2] = temp2;
			BufferY[i / 2 + nY / 2] = temp1;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = BufferY[k];
	}
	
	delete[] BufferX;
	delete[] BufferY;
}


/*************************************************************************
 *
 * �������ƣ�
 *   IDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά�������С���ع���
 *
 ************************************************************************/

void CFreqCalculate::IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// ѭ������
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* Buffer1;
	float* Buffer2;

	Buffer1 = new float[nXSize];
	Buffer2 = new float[nYSize];

	for (i = 0; i < DD; i++)
	{
		hi[i] = h[DD - 1 - i];
		gi[i] = g[DD - 1 - i];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + k * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i <  nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
			{
				temp1 = temp1 + Buffer1[a(i + j, nY)] * hi[j + (DD - 1) / 2];
				Buffer2[i] = temp1;
			}
		}
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + (k + nY / 2) * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i < nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + Buffer1[a(i + j, nY)] * gi[j + (DD - 1) / 2 + 1];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = Buffer2[k];
	}

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k);
			Buffer1[k * 2 + 1] = 0;
		}
		
		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + Buffer1[a(i + j, nX)] * hi[j + (DD - 1) / 2];
			Buffer2[i] = temp1;
		}
		
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k + nX / 2);
			Buffer1[k * 2 + 1] = 0;
		}

		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + gi[j + (DD - 1) / 2 + 1] * Buffer1[a(i + j, nX)];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = Buffer2[k] * 4;
	}

	delete[] Buffer1;
	delete[] Buffer2;
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�ά��ɢС���任��
 *
 ************************************************************************/

void CFreqCalculate::DIBDWT(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// ��ʾ�û�
		MessageBox("���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
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

	// ����С���ֽ�
	DWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBIDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�άС���ع���
 *
 ************************************************************************/

void CFreqCalculate::DIBIDWT(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// �ж��Ƿ���24-bppλͼ
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// ��ʾ�û�
		MessageBox("���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
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

	// ����С���ع�	
	IDWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);

	// �ָ����
	EndWaitCursor();
}
