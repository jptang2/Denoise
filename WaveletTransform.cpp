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
	long i,j,temp;	// 循环变量

	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;	
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);
		// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox("小波变换只支持8位图", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL)hDIB);
		
		// 返回
		return 0;
	}

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);

	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
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
	
	
	int height,width;	// 扩展后图像的高和宽
	int scale;	// 分解尺度
	int m;	// 整除数, 2^scale
	
	scale = 3;//表示分解3层
	m = (int)pow(2,scale);

	// 计算扩展后图像的尺寸，使得图像的高和宽能被m整除
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

	double **tempData;	// 临时变量，存放图像数据
	tempData = new double *[height];

	for(i=0; i<height; i++)
	{
		tempData[i] = new double[width];
	}

	

	// 初始化,扩展部分采用周期延拓
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

	
	

	
	// 取得滤波器系数
	char* wavelet = "db4";	// 取何种小波
	int filterlength;		// 滤波器长度
	if(1 != Wfilters(wavelet, &filterlength))
	{
		return 0;
	}
	
 
	// 分解
	int heightNew = height;
	int widthNew = width;
	for(i=1; i<=scale; i++)
	{
		DWT2(tempData,heightNew,widthNew,filterlength);

		// 低频子图像的高和宽
		heightNew /= 2;
		widthNew /= 2;
	}
		
	// 滤波
	Denoise2(tempData,height,width,scale);
	
	// 重构
	for(i=scale; i>=1; i--)
	{
		//重构图像的高和宽
		heightNew += heightNew;
		widthNew += widthNew;

		IDWT2(tempData,heightNew,widthNew,filterlength);
	}
	

	
	// 输出滤波结果
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
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL)hDIB);

	delete[] pImage;

	// 删除分配的内存
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
	int i,j,k;	// 循环变量
	int r,c;	// 行和列
	double delta;	// 噪声系数的标准差
	
	for(k=1; k<=scale; k++)
	{
		if(1 == k)
		{
			// 利用对角子带（HH）求噪声标准差
			r = (int)(height/pow(2,k));	// 子带的行
			c = (int)(width/pow(2,k));	// 子带的列
			double *temp = new double[r * c];	// 临时存放子带数据，求取中值
			for(i=0; i<r; i++)
				for(j=0; j<c; j++)
					temp[i*c + j] = ABS(coeff[r+i][c+j]);
				delta = prhap(temp,r*c) / 0.6745;	//经验公式是detla=MAD/0.6745,MAD是小波系数绝对值的均值
				
				delete []temp;
		}
		
		// 域值处理
		r = (int)(height/pow(2,k-1));	//	LL,LH,HL,HH四个字图像组成的图像的行
		c = (int)(width/pow(2,k-1));	//	LL,LH,HL,HH四个字图像组成的图像的列
		
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
	int nr = 3;	// 窗口半径
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
	mean = sum1/num;      //mean均值
	var = sum2/num - mean*mean;    //var方差
	if(var <= delta*delta)
	{
		T = 1000;	// 取很大的值，使处理后的系数为0
	}
	else
	{
		T = sqrt(2) * delta*delta/sqrt(var-delta*delta);
	}

	for(i=rTop; i<rBottom; i++)
	{
		for(j=cLeft; j<cRight; j++)
		{				
				// 软域值处理
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
	int i,k;	// 循环变量
	int n = filterlength/2;	// 滤波器中心位置

	double sum1,sum2;	// 临时变量

	double *temp = new double[length];	// 临时变量，存放变换结果
	for(i=0; i<length; i+=2)	//下采样
	{
		// 初始化
		sum1 = 0.0;
		sum2 = 0.0;

		// 计算滤波器卷积结果，滤波器中心位置和i重合
		for(k=0; k<filterlength; k++)
		{
			if(i-n+k < 0)	// 边界处理，采用周期延拓
			{
				sum1 += data[i-n+k + length]*m_lo_D[k];	// 低通
				sum2 += data[i-n+k + length]*m_hi_D[k];	// 高通
			}
			else if(i-n+k >=length)	// 边界处理，采用周期延拓
			{
				sum1 += data[i-n+k - length]*m_lo_D[k];	// 低通
				sum2 += data[i-n+k - length]*m_hi_D[k];	// 高通
			}
			else
			{
				sum1 += data[i-n+k]*m_lo_D[k];	// 低通
				sum2 += data[i-n+k]*m_hi_D[k];	// 高通
			}
		}

		// 结果
		temp[i/2] = sum1;	// 概貌信息
		temp[length/2 + i/2] = sum2;	// 细节信息
	}

	// 更新
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
	int i,k;	// 循环变量
	int n = filterlength/2;	// 滤波器中心位置
	
	double sum1,sum2;	// 临时变量
	
	double *temp = new double[length];	// 临时变量，存放变换结果
	for(i=0; i<length; i++)
	{
		// 初始化
		sum1 = 0.0;
		sum2 = 0.0;
		
		// 计算滤波器卷积结果
		for(k=0; k<filterlength; k++)
		{
			if(i-n+k+1 < 0)	// 边界处理，采用周期延拓
			{
				if(0 == (i-n+k+1 + length)%2)	// 上采样
				{
					sum1 += coeff[(i-n+k+1 + length)/2]*m_lo_R[k];	// 低通
					sum2 += coeff[length/2 + (i-n+k+1 + length)/2]*m_hi_R[k];	// 高通
				}
			}
			else if(i-n+k+1 >=length)	// 边界处理，采用周期延拓
			{
				if(0 == (i-n+k+1 - length)%2)	// 上采样
				{
					sum1 += coeff[(i-n+k+1 - length)/2]*m_lo_R[k];	// 低通
					sum2 += coeff[length/2 + (i-n+k+1 - length)/2]*m_hi_R[k];	// 高通
				}
			}
			else
			{
				if(0 == (i-n+k+1)%2)	// 上采样
				{
					sum1 += coeff[(i-n+k+1)/2]*m_lo_R[k];	// 低通
					sum2 += coeff[length/2 + (i-n+k+1)/2]*m_hi_R[k];	// 高通
				}
			}
		}
		
		// 结果
		temp[i] = sum1 + sum2;	
	}
	
	// 更新
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
	int i,j;	// 循环变量
	double *temp = new double[height];	// 临时存放每一列的数据

	// 行分解
	for(i=0; i<height; i++)
		DWT(data[i],width,filterlength);

	// 列分解
	for(j=0; j<width; j++)
	{
		// 列转置
		for(i=0; i<height; i++)
		{
			temp[i] = data[i][j];
		}
		DWT(temp,height,filterlength);
		// 反转置
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
	int i,j;	// 循环变量
	double *temp = new double[height];	// 临时存放每一列的数据
	
	// 列重构
	for(j=0; j<width; j++)
	{
		// 列转置
		for(i=0; i<height; i++)
		{
			temp[i] = coeff[i][j];
		}
		IDWT(temp,height,filterlength);
		// 反转置
		for(i=0; i<height; i++)
		{
			coeff[i][j] = temp[i];
		}
	}

	// 行重构
	for(i=0; i<height; i++)
		IDWT(coeff[i],width,filterlength);
	
	return 1;

}


int CWaveletTransform::Wfilters(char *wavelet, int *length)
{
	char* waveletName;	// 小波名称，按照Matlab中

	waveletName = "db4";	// db4小波，消失矩4

	if(0 == strcmp(wavelet,waveletName))
	{
		double db[8] = {
			0.23037781330886,   0.71484657055254,   0.63088076792959,  -0.02798376941698,
			-0.18703481171888,   0.03084138183599,   0.03288301166698,  -0.01059740178500
		};
		
		*length = 8;	// 滤波器长度
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

// 堆排序
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
		m_lo_D[i] = db[length-1 - i];				// 低通分解滤波器系数
		m_hi_D[i] = pow(-1,(i+1)%2) * db[i];		// 高通分解滤波器系数
		m_lo_R[i] = db[i];							// 低通重构滤波器系数
		m_hi_R[i] = pow(-1,i%2) * db[length-1 - i];	// 高通重构滤波器系数
	}

	return 1;

}
