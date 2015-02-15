// WaveletTransform.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "WaveletTransform.h"
#include "math.h"
#include "dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveletTransform

CWaveletTransform::CWaveletTransform()
{
}

CWaveletTransform::~CWaveletTransform()
{
}


BEGIN_MESSAGE_MAP(CWaveletTransform, CWnd)
	//{{AFX_MSG_MAP(CWaveletTransform)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWaveletTransform message handlers

/***********************************************************************
* image denoised based on discrete wavelet transform
* 
* INPUT
*   pData - pointer of image
*   heightOld - the height of original image
*   widthOld - the width of original image
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                 
************************************************************************/

int CWaveletTransform::DWT_WaveletFilter2(HDIB hDIB)
{
	long i,j,temp;	// ѭ������

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;	
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);
		// �ж��Ƿ���24-bppλͼ
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// ��ʾ�û�
		MessageBox("С���任ֻ֧��8λͼ", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// ����
		return 0;
	}

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);

	// DIB�Ŀ��
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	long lSize = lWidth*lHeight;

	unsigned char *pImage = new unsigned char[lSize];

	long lLineBytes = WIDTHBYTES(lWidth*8);
			for(i = 0; i< lHeight; i++)
			{
				for(j = 0; j<lWidth; j++)
				{
					//for the ith Line and the jth column
					temp = i*lWidth + j;
					pImage[temp] = *((unsigned char *)(lpDIBBits + lLineBytes*(lHeight-1-i)+j));					
				}
			}
	
	
	int height,width;	// ��չ��ͼ��ĸߺͿ�
	int scale;	// �ֽ�߶�
	int m;	// ������, 2^scale
	
	scale = 3;//��ʾ�ֽ�3��
	m = (int)pow(2,scale);

	// ������չ��ͼ��ĳߴ磬ʹ��ͼ��ĸߺͿ��ܱ�m����
	if(0 == lHeight%m)
	{
		height = lHeight;
	}
	else
	{
		height = (m - lHeight%m) + lHeight;
	}
	
	if(0 == lWidth%m)
	{
		width = lWidth;
	}
	else
	{
		width = (m - lWidth%m) + lWidth;
	}

	double **tempData;	// ��ʱ���������ͼ������
	tempData = new double *[height];

	for(i=0; i<height; i++)
	{
		tempData[i] = new double[width];
	}

	

	// ��ʼ��,��չ���ֲ�����������
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			if(i >= lHeight)
			{
				if(j >= lWidth)
					tempData[i][j] = (double)pImage[(i-lHeight)*lWidth + j-lWidth];
				else
					tempData[i][j] = (double)pImage[(i-lHeight)*lWidth + j];
			}
			else
			{
				if(j >= lWidth)
					tempData[i][j] = (double)pImage[i*lWidth + j-lWidth];
				else
					tempData[i][j] = (double)pImage[i*lWidth + j];
			}				
		}
	}

	
	

	
	// ȡ���˲���ϵ��
	char* wavelet = "db4";	// ȡ����С��
	int filterlength;		// �˲�������
	if(1 != Wfilters(wavelet, &filterlength))
	{
		return 0;
	}
	
 
	// �ֽ�
	int heightNew = height;
	int widthNew = width;
	for(i=1; i<=scale; i++)
	{
		DWT2(tempData,heightNew,widthNew,filterlength);

		// ��Ƶ��ͼ��ĸߺͿ�
		heightNew /= 2;
		widthNew /= 2;
	}
		
	// �˲�
	Denoise2(tempData,height,width,scale);
	
	// �ع�
	for(i=scale; i>=1; i--)
	{
		//�ع�ͼ��ĸߺͿ�
		heightNew += heightNew;
		widthNew += widthNew;

		IDWT2(tempData,heightNew,widthNew,filterlength);
	}
	

	
	// ����˲����
	for(i=0; i<lHeight; i++)
	{
		for(j=0; j<lWidth; j++)
		{
			if(tempData[i][j] > 255)
			{
				pImage[i*lWidth + j] = (byte)255;
			}
			else if(tempData[i][j] < 0)
			{
				pImage[i*lWidth + j] = (byte)0;
			}
			else
			{
				pImage[i*lWidth + j] = (byte)tempData[i][j];
			}
		}
	}

	temp=0;
	for(i = 0; i< lHeight; i++)
			{
				for(j = 0; j<lWidth; j++)
				{
					//for the ith Line and the jth column
					*((unsigned char *)(lpDIBBits + lLineBytes*(lHeight-1-i)+j))=(unsigned char)(pImage[temp++]);			
					
				}
		}
	
	// �������
	::GlobalUnlock((HGLOBAL)hDIB);

	delete[] pImage;

	// ɾ��������ڴ�
	for(i=0; i<height; i++)
	{
		delete []tempData[i];
	}
	delete []tempData;

	return 1;
}

/***********************************************************************
* denoising noise of wavelet coefficients
* 
* INPUT
*   coeff - pointer of wavelet coefficients
*   height - the height of coefficients matrix
*	width - the width of coefficients matrix
*	scale - the scale of decomposition
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                  
************************************************************************/
int CWaveletTransform::Denoise2(double **coeff, int height, int width, int scale)
{
	int i,j,k;	// ѭ������
	int r,c;	// �к���
	double delta;	// ����ϵ���ı�׼��
	
	for(k=1; k<=scale; k++)
	{
		if(1 == k)
		{
			// ���öԽ��Ӵ���HH����������׼��
			r = (int)(height/pow(2,k));	// �Ӵ�����
			c = (int)(width/pow(2,k));	// �Ӵ�����
			double *temp = new double[r * c];	// ��ʱ����Ӵ����ݣ���ȡ��ֵ
			for(i=0; i<r; i++)
				for(j=0; j<c; j++)
					temp[i*c + j] = ABS(coeff[r+i][c+j]);
				delta = prhap(temp,r*c) / 0.6745;	//���鹫ʽ��detla=MAD/0.6745,MAD��С��ϵ������ֵ�ľ�ֵ
				
				delete []temp;
		}
		
		// ��ֵ����
		r = (int)(height/pow(2,k-1));	//	LL,LH,HL,HH�ĸ���ͼ����ɵ�ͼ�����
		c = (int)(width/pow(2,k-1));	//	LL,LH,HL,HH�ĸ���ͼ����ɵ�ͼ�����
		
		Thresholding2(coeff, r/2, c/2, r, c, delta);	//HH
		Thresholding2(coeff, r/2, 0, r, c/2, delta);	//HL
		Thresholding2(coeff, 0, c/2, r/2, c, delta);	//LH
	}
	
	return 1;


}
/***********************************************************************
* denoising noise of wavelet coefficients
* 
* INPUT
*   coeff - pointer of wavelet coefficients
*   rTop - top row of coefficients matrix
*	cleft - left column of coefficients matrix
*   rBottom - bottom row of coefficients matrix
*	cRight - right column of coefficients matrix
*	delta - standard deviation of noise
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                 
************************************************************************/
int CWaveletTransform::Thresholding2(double **coeff, int rTop, int cLeft, int rBottom, int cRight, double delta)
{
	int i,j;
	int nr = 3;	// ���ڰ뾶
	double sum1 = 0.0;
	double sum2 = 0.0;
	int num = 0;
	double mean,var,T;
	
	for(i=rTop; i<rBottom; i++)
	{
		for(j=cLeft; j<cRight; j++)
		{
//			sum1 = 0.0; sum2 = 0.0; num = 0;
//			for(m=-nr; m<=nr; m++)
//			{
//				for(n=-nr; n<=nr; n++)
//				{
//					if((i+m >= rTop) && (i+m < rBottom) && (j+n >= cLeft) && (j+n < cRight))
//					{
						num += 1;
						sum1 += coeff[i][j];
						sum2 += coeff[i][j]*coeff[i][j];
//					}
//				}
//			}

		}
	}
	mean = sum1/num;      //mean��ֵ
	var = sum2/num - mean*mean;    //var����
	if(var <= delta*delta)
	{
		T = 1000;	// ȡ�ܴ��ֵ��ʹ������ϵ��Ϊ0
	}
	else
	{
		T = sqrt(2) * delta*delta/sqrt(var-delta*delta);
	}

	for(i=rTop; i<rBottom; i++)
	{
		for(j=cLeft; j<cRight; j++)
		{				
				// ����ֵ����
				mean = ABS(coeff[i][j]) - T;
				if(mean <= 0)
					coeff[i][j] = 0;
				else
					coeff[i][j] = coeff[i][j]*mean / (mean+T);
		}
	}
	
	
	return 1;	


}
// protected
/***********************************************************************
* 1-D discrete wavelet transform
* 
* INPUT
*   data - pointer of data
*   length - the length of data
*   filterlength - the length of filter
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                  
************************************************************************/
int CWaveletTransform::DWT(double *data, int length, int filterlength)
{
	int i,k;	// ѭ������
	int n = filterlength/2;	// �˲�������λ��

	double sum1,sum2;	// ��ʱ����

	double *temp = new double[length];	// ��ʱ��������ű任���
	for(i=0; i<length; i+=2)	//�²���
	{
		// ��ʼ��
		sum1 = 0.0;
		sum2 = 0.0;

		// �����˲������������˲�������λ�ú�i�غ�
		for(k=0; k<filterlength; k++)
		{
			if(i-n+k < 0)	// �߽紦��������������
			{
				sum1 += data[i-n+k + length]*m_lo_D[k];	// ��ͨ
				sum2 += data[i-n+k + length]*m_hi_D[k];	// ��ͨ
			}
			else if(i-n+k >=length)	// �߽紦��������������
			{
				sum1 += data[i-n+k - length]*m_lo_D[k];	// ��ͨ
				sum2 += data[i-n+k - length]*m_hi_D[k];	// ��ͨ
			}
			else
			{
				sum1 += data[i-n+k]*m_lo_D[k];	// ��ͨ
				sum2 += data[i-n+k]*m_hi_D[k];	// ��ͨ
			}
		}

		// ���
		temp[i/2] = sum1;	// ��ò��Ϣ
		temp[length/2 + i/2] = sum2;	// ϸ����Ϣ
	}

	// ����
	for(i=0; i<length; i++)
		data[i] = temp[i];

	delete []temp;

	return 1;

}
/***********************************************************************
* 1-D inverse discrete wavelet transform
* 
* INPUT
*   coeff - pointer of wavelet coefficients
*   length - the length of data
*   filterlength - the length of filter
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                  
************************************************************************/
int CWaveletTransform::IDWT(double *coeff, int length, int filterlength)
{
	int i,k;	// ѭ������
	int n = filterlength/2;	// �˲�������λ��
	
	double sum1,sum2;	// ��ʱ����
	
	double *temp = new double[length];	// ��ʱ��������ű任���
	for(i=0; i<length; i++)
	{
		// ��ʼ��
		sum1 = 0.0;
		sum2 = 0.0;
		
		// �����˲���������
		for(k=0; k<filterlength; k++)
		{
			if(i-n+k+1 < 0)	// �߽紦��������������
			{
				if(0 == (i-n+k+1 + length)%2)	// �ϲ���
				{
					sum1 += coeff[(i-n+k+1 + length)/2]*m_lo_R[k];	// ��ͨ
					sum2 += coeff[length/2 + (i-n+k+1 + length)/2]*m_hi_R[k];	// ��ͨ
				}
			}
			else if(i-n+k+1 >=length)	// �߽紦��������������
			{
				if(0 == (i-n+k+1 - length)%2)	// �ϲ���
				{
					sum1 += coeff[(i-n+k+1 - length)/2]*m_lo_R[k];	// ��ͨ
					sum2 += coeff[length/2 + (i-n+k+1 - length)/2]*m_hi_R[k];	// ��ͨ
				}
			}
			else
			{
				if(0 == (i-n+k+1)%2)	// �ϲ���
				{
					sum1 += coeff[(i-n+k+1)/2]*m_lo_R[k];	// ��ͨ
					sum2 += coeff[length/2 + (i-n+k+1)/2]*m_hi_R[k];	// ��ͨ
				}
			}
		}
		
		// ���
		temp[i] = sum1 + sum2;	
	}
	
	// ����
	for(i=0; i<length; i++)
		coeff[i] = temp[i];
	
	delete []temp;

	return 1;

}

/***********************************************************************
* 2-D discrete wavelet transform
* 
* INPUT
*   data - pointer of image data
*   height - the height of image data
*   width - the width of image data
*   filterlength - the length of filter
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                  
************************************************************************/
int CWaveletTransform::DWT2(double **data, int height, int width, int filterlength)
{
	int i,j;	// ѭ������
	double *temp = new double[height];	// ��ʱ���ÿһ�е�����

	// �зֽ�
	for(i=0; i<height; i++)
		DWT(data[i],width,filterlength);

	// �зֽ�
	for(j=0; j<width; j++)
	{
		// ��ת��
		for(i=0; i<height; i++)
		{
			temp[i] = data[i][j];
		}
		DWT(temp,height,filterlength);
		// ��ת��
		for(i=0; i<height; i++)
		{
			data[i][j] = temp[i];
		}
	}

	delete []temp;

	return 1;

}
/***********************************************************************
* 2-D inverse discrete wavelet transform
* 
* INPUT
*   coeff - pointer of wavelet coefficients
*   height - the height of image data
*   width - the width of image data
*   filterlength - the length of filter
* RETURN
*   1 - success
*   0 - false
*
* Writer
*   tjp
* DATE
*   2011-1-31                                                                 
************************************************************************/
int CWaveletTransform::IDWT2(double **coeff, int height, int width, int filterlength)
{
	int i,j;	// ѭ������
	double *temp = new double[height];	// ��ʱ���ÿһ�е�����
	
	// ���ع�
	for(j=0; j<width; j++)
	{
		// ��ת��
		for(i=0; i<height; i++)
		{
			temp[i] = coeff[i][j];
		}
		IDWT(temp,height,filterlength);
		// ��ת��
		for(i=0; i<height; i++)
		{
			coeff[i][j] = temp[i];
		}
	}

	// ���ع�
	for(i=0; i<height; i++)
		IDWT(coeff[i],width,filterlength);
	
	return 1;

}


int CWaveletTransform::Wfilters(char *wavelet, int *length)
{
	char* waveletName;	// С�����ƣ�����Matlab��

	waveletName = "db4";	// db4С������ʧ��4

	if(0 == strcmp(wavelet,waveletName))
	{
		double db[8] = {
			0.23037781330886,   0.71484657055254,   0.63088076792959,  -0.02798376941698,
			-0.18703481171888,   0.03084138183599,   0.03288301166698,  -0.01059740178500
		};
		
		*length = 8;	// �˲�������
		SetFilters(db,*length);
	}
	return 1;

}

double CWaveletTransform::ABS(double num)
{
	if(num >= 0)
		return num;
	else
		return -num;
}

// ������
double CWaveletTransform::prhap(double *p, int n)
{
	int i,mm;
	double t;
	
	mm=n/2;
	for(i=mm-1; i>=0; i--)
		rsift(p,i,n-1);
	for(i=n-1; i>=1; i--)
	{ 
		t=p[0]; 
		p[0]=p[i]; 
		p[i]=t;
		rsift(p,0,i-1);
	}
	return p[n/2];

}


void CWaveletTransform::rsift(double *p, int i, int n)
{
		int j;
	double t;
	
	t=p[i]; 
	j=2*(i+1)-1;
	
	while (j<=n)
	{ 
		if ((j<n)&&(p[j]<p[j+1])) 
			j=j+1;
		if (t<p[j])
		{
			p[i]=p[j];
			i=j; 
			j=2*(i+1)-1;
		}
		else
			j=n+1;
	}
	p[i]=t;

	return;

}


int CWaveletTransform::SetFilters(double *db, int length)
{
	m_lo_D = new double[length];	
	m_hi_D = new double[length];	
	m_lo_R = new double[length];	
	m_hi_R = new double[length];	
	
	for(int i=0; i<length; i++)
	{
		m_lo_D[i] = db[length-1 - i];				// ��ͨ�ֽ��˲���ϵ��
		m_hi_D[i] = pow(-1,(i+1)%2) * db[i];		// ��ͨ�ֽ��˲���ϵ��
		m_lo_R[i] = db[i];							// ��ͨ�ع��˲���ϵ��
		m_hi_R[i] = pow(-1,i%2) * db[length-1 - i];	// ��ͨ�ع��˲���ϵ��
	}

	return 1;

}
