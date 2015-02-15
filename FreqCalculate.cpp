// ************************************************************************
//  文件名：FreqCalculate.cpp
//
//  图像正交变换函数库：
//
//	FFT()				- 一维快速付立叶变换
//  IFFT()				- 一维快速付立叶逆变换
//  Fourier()			- 二维快速傅立叶变换
//  IFourier()			- 二维快速傅立叶逆变换
//  DCT()				- 一维快速离散余弦变换
//  IDCT()				- 一维快速离散余弦逆变换
//	FreqDCT()			- 二维快速离散余弦变换
//  IFreqDCT()			- 二维快速离散余弦逆变换
//  WALSH()				- 一维沃尔什－哈达玛变换
//  IWALSH()			- 一维沃尔什－哈达玛逆变换
//	FreqWALSH()			- 二维沃尔什－哈达玛变换
//	IFreqWALSH()		- 二维沃尔什－哈达玛逆变换
//	DWT()				- 二维点阵的小波分解
//	IDWT()				- 二维点阵的小波重构
//	
//  DIBFourier()		- 图像的付立叶变换
//  DIBDCT()			- 图像的离散余弦变换
//  DIBWalsh()			- 图像的沃尔什－哈达玛变换
//  DIBDWT()			- 图象的二维离散小波变换
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


// 常数π
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
 * 函数名称：
 *   FFT()
 * 参数:
 *   complex<double> * TD	- 指向时域数组的指针
 *   complex<double> * FD	- 指向频域数组的指针
 *   r						－2的幂数，即迭代次数
 * 返回值:
 *   无。
 * 说明:
 *   该函数用来实现快速傅里叶变换。
 ************************************************************************/

VOID CFreqCalculate::FFT(complex<double> * TD, complex<double> * FD, int r)
{	
	LONG	count;				// 傅里叶变换总点数	
	int		i,j,k;				// 循环变量
	int		N,p;	            //N为当前每组点数
	double	w;			    	// 弧度	
	complex<double> *W,*X1,*X2,*X;
	
	count = 1 << r;				// 计算傅里叶变换总点数，r为总幂
	
	// 分配运算所需存储器
	W  = new complex<double>[count / 2];                    //count为接收点数(256，512等)
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// 计算加权系数(旋转因子W),w为弧度
	for(i = 0; i < count / 2; i++) 
	{
		w = -i * PI * 2 / count;
		W[i] = complex<double> (cos(w), sin(w));            //cos实部
	}
	
	// 将时域点写入X1,X1为输入，X2为输出
	memcpy(X1, TD, sizeof(complex<double>) * count);        
	
	// 快速傅里叶变换(基2频率抽取),p144页,k对调r-k-1
	//幂->每组点数->组数->半点->跨度
	for(k = 0; k < r; k++)                                  //k为当前幂，k=0为第1级
	{   
		N = 1 << (r-k);                                     //N为当前幂每组点数，频率抽取第一级最多,k=0,N=2^r，512-256-128....2
	  //for(j = 0; j < 1 << k; j++)
		for(j = 0; j < count/N; j++)                         //j为当前幂的组数，k=0时第1级有1组(j=0)，k=1时第2级有2组
		{
			                                 
			for(i = 0; i < N / 2; i++)                      //i为当前幂每组半点，所以后面两个半组一起算
			{
				p = j * N;									//p为当前幂组外间隔，即算到第j组时跨过的总点数(跨度)
				X2[i + p] = X1[i + p] + X1[i + p + N / 2];  //每组上半，N/2为间组内隔，256-128....1				
				X2[i + p + N / 2] = (X1[i + p] - X1[i + p + N / 2]) * W[i * (1<<k)];//每组下半，[]内为W上标,为每组半点*组数
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// 重新排序,从最低位到最高位检查正序数的每一位i，然后把该位的值赋给倒序数的相应位
	//在倒序数中为第r-i-1位，r为序数的位数。二进制正序右边为低位
	
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)    //二进制的i位，0...r-1
		{    
			if (j&(1<<i))         //j按位与位数，若非0则进入循环
			{   
				p+=1<<(r-i-1);    //如果正序数中位i的值为1，则倒序数中r-i-1的相应位置1。p+是累加其他所有位的1
			}
		}
		FD[j]=X1[p];			  //对位
	}
	


	delete W;
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * 函数名称：
 *   IFFT()
 *
 * 参数:
 *   complex<double> * FD	- 指向频域值的指针
 *   complex<double> * TD	- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶逆变换。
 *
 ************************************************************************/

void CFreqCalculate::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 循环变量
	int		i;
	
	complex<double> *X;
	
	// 计算付立叶变换点数
	LONG N = 1<<r;
	
	// 分配运算所需存储器
	X = new complex<double>[N];
	
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * N);
	
	// 求共轭
	for (i = 0; i < N; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for (i = 0; i < N; i++)
	{
		TD[i] = complex<double> (TD[i].real() / N, -TD[i].imag() / N);
	}
	
	// 释放内存
	delete X;
}


/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   complex* TD		- 输入的时域序列
 *	 LONG lWidth		- 图象宽度
 *	 LONG lHeight		- 图象高度
 *	 complex* FD		- 输出的频域序列
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数进行二维快速付立叶变换。
 *
 ************************************************************************/

BOOL CFreqCalculate::Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// 循环变量
	LONG	i;
	LONG	j;
	LONG    k;

	// 更改光标形状
	BeginWaitCursor();
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
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
		
	// 分配内存
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];
	
	// 对y方向进行快速付立叶变换
	for (i = 0; i < w * 3; i++)
	{
		// 抽取数据
		for (j = 0; j < h; j++)
			TempT[j] = TD[j * w * 3 + i];
		
		// 一维快速傅立叶变换
		FFT(TempT, TempF, hp);

		// 保存变换结果
		for (j = 0; j < h; j++)
			TD[j * w * 3 + i] = TempF[j];
	}
	
	// 释放内存
	delete TempT;
	delete TempF;

	// 分配内存
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// 对x方向进行快速付立叶变换
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// 抽取数据
			for (j = 0; j < w; j++)
				TempT[j] = TD[i * w * 3 + j * 3 + k];

			// 一维快速傅立叶变换
			FFT(TempT, TempF, wp);

			// 保存变换结果
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempF[j];
		}
	}

	// 释放内存
	delete TempT;
	delete TempF;

	return TRUE;
}


/*************************************************************************
 *
 * 函数名称：
 *   IFourier()
 *
 * 参数:
 *   LPBYTE TD			- 返回的空域数据
 *   LONG lWidth		- 空域图象的宽度
 *	 LONG lHeight		- 空域图象的高度
 *	 complex* FD		- 输入的频域数据
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数进行二维快速付立叶逆变换。
 *
 ************************************************************************/

BOOL CFreqCalculate::IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	// 循环变量
	LONG	i;
	LONG	j;
	LONG    k;

	// 更改光标形状
	BeginWaitCursor();
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 分配内存
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// 对x方向进行快速付立叶变换
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// 抽取数据
			for (j = 0; j < w; j++)
				TempF[j] = FD[i * w * 3 + j * 3 + k];

			// 一维快速傅立叶变换
			IFFT(TempF, TempT, wp);

			// 保存变换结果
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempT[j];
		}
	}

	// 释放内存
	delete TempT;
	delete TempF;
	
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];

	// 对y方向进行快速付立叶变换
	for (i = 0; i < w * 3; i++)
	{
		// 抽取数据
		for (j = 0; j < h; j++)
			TempF[j] = FD[j * w * 3 + i];
		
		// 一维快速傅立叶变换
		IFFT(TempF, TempT, hp);

		// 保存变换结果
		for (j = 0; j < h; j++)
			FD[j * w * 3 + i] = TempT[j];
	}
	
	// 释放内存
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
 * 函数名称：
 *   DIBFourier()
 *
 * 参数:
 *   HDIB	hDIB		- 待处理的DIB
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/

BOOL CFreqCalculate::DIBFourier(HDIB hDIB)
{
	// 指向源图像的指针
	unsigned char*	lpSrc;

	// 中间变量
	double	dTemp;
	LONG TI,TJ;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 指向DIB的指针
	LPBYTE	lpDIB;
	
	// 指向DIB象素指针
	LPBYTE    lpDIBBits;
	
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
		return FALSE;
	}

	// 更改光标形状
	BeginWaitCursor();
	
	// DIB的宽度
	LONG lWidth = m_clsDIB.DIBWidth(lpDIB);
	
	// DIB的高度
	LONG lHeight = m_clsDIB.DIBHeight(lpDIB);

	// 计算图像每行的字节数
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	// 分配内存
	complex<double> *FD, *TD, *TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD =  new complex<double>[w * h * 3];
	
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

	// 进行频谱分析
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return FALSE;

	// 释放内存
	delete []TD;

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

	// 行
	for(i = (int)(h - lHeight) / 2; i < (int)(h + lHeight) / 2; i++)
	{
		// 列
		for(j = (int)(w * 3 - lLineBytes) / 2; j < (int)(w * 3 + lLineBytes) / 2; j += 3)
		{
			// 计算频谱
			dTemp = sqrt(TempD[w * 3 * i + j].real() * TempD[w * 3 * i + j].real() + 
				         TempD[w * 3 * i + j].imag() * TempD[w * 3 * i + j].imag()) / 100;

			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}

			// 限制为原图大小范围
			TI = i - (h - lHeight) / 2;
			TJ = j / 3 - (w - lWidth) / 2;
			
			// 对应象素指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * TI + TJ * 3;

			// 更新源图像
			* (lpSrc) = (BYTE) (dTemp);
			* (lpSrc + 1) = (BYTE) (dTemp);
			* (lpSrc + 2) = (BYTE) (dTemp);
		}
	}

	// 解除锁定
	::GlobalUnlock(hDIB);

	// 删除临时变量
	delete []FD;
	delete []TempD;
	
	// 恢复光标
	EndWaitCursor();
	
	// 返回
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////
//																	     //
//							小波变换									 //
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
 * 函数名称：
 *   DWT()
 *
 * 参数:
 *   HDIB	hDIB		- 待处理的DIB
 *
 * 返回值:
 *   void	            - 无返回值
 *
 * 说明:
 *   该函数对二维点阵进行离散小波变换。
 *
 ************************************************************************/

void CFreqCalculate::DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// 循环变量
	int i, j, k, n;

	// 中间变量
	float temp1, temp2;
	float* BufferX;
	float* BufferY;

	BufferX = new float[nXSize];
	BufferY = new float[nYSize];

	// 水平方向
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

		// 回存数据
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = BufferX[k];
	}

	// 垂直方向
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

		// 回存数据
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = BufferY[k];
	}
	
	delete[] BufferX;
	delete[] BufferY;
}


/*************************************************************************
 *
 * 函数名称：
 *   IDWT()
 *
 * 参数:
 *   HDIB	hDIB		- 待处理的DIB
 *
 * 返回值:
 *   void	            - 无返回值
 *
 * 说明:
 *   该函数对二维点阵进行小波重构。
 *
 ************************************************************************/

void CFreqCalculate::IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize)
{
	// 循环变量
	int i, j, k, n;

	// 中间变量
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

	// 垂直方向
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

		// 回存数据
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = Buffer2[k];
	}

	// 水平方向
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

		// 回存数据
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = Buffer2[k] * 4;
	}

	delete[] Buffer1;
	delete[] Buffer2;
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBDWT()
 *
 * 参数:
 *   HDIB	hDIB		- 待处理的DIB
 *
 * 返回值:
 *   void	            - 无返回值
 *
 * 说明:
 *   该函数对图象进行二维离散小波变换。
 *
 ************************************************************************/

void CFreqCalculate::DIBDWT(HDIB hDIB)
{
	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox("请先将其转换为8位色位图，再进行处理！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
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

	// 进行小波分解
	DWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}


/*************************************************************************
 *
 * 函数名称：
 *   DIBIDWT()
 *
 * 参数:
 *   HDIB	hDIB		- 待处理的DIB
 *
 * 返回值:
 *   void	            - 无返回值
 *
 * 说明:
 *   该函数对图象进行二维小波重构。
 *
 ************************************************************************/

void CFreqCalculate::DIBIDWT(HDIB hDIB)
{
	// 指向DIB的指针
	LPBYTE lpDIB;
	
	// 指向DIB象素指针
	LPBYTE lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// 找到DIB图像象素起始位置
	lpDIBBits = m_clsDIB.FindDIBBits(lpDIB);
	
	// 判断是否是24-bpp位图
	if (m_clsDIB.DIBBitCount(lpDIB) != 8)
	{
		// 提示用户
		MessageBox("请先将其转换为8位色位图，再进行处理！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
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

	// 进行小波重构	
	IDWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	// 解除锁定
	::GlobalUnlock((HGLOBAL) hDIB);

	// 恢复光标
	EndWaitCursor();
}
