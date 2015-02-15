#if !defined(AFX_WAVELETTRANSFORM_H__011FFDAA_46EE_45BC_912F_AA11D8F7D523__INCLUDED_)
#define AFX_WAVELETTRANSFORM_H__011FFDAA_46EE_45BC_912F_AA11D8F7D523__INCLUDED_

#include "Dib.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveletTransform.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaveletTransform window

class CWaveletTransform : public CWnd
{
// Construction
public:
	CWaveletTransform();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaveletTransform)
	//}}AFX_VIRTUAL

// Implementation
public:
	double prhap(double *p,int n);
	double ABS(double num);
	int Wfilters(char* wavelet, int* length);
	int DWT2(double **data, int height, int width, int filterlength);
	int IDWT2(double **coeff, int height, int width, int filterlength);
	int Denoise2(double **coeff, int height, int width, int scale);
	int DWT_WaveletFilter2(HDIB hDIB);
	CDIB m_clsDIB;
	virtual ~CWaveletTransform();

	// Generated message map functions
protected:
	int IDWT(double *coeff, int length, int filterlength);
	int DWT(double *data, int length, int filterlength);
private:
	int Thresholding2(double **coeff, int rTop, int cLeft, int rBottom, int cRight, double delta);
private:
	void rsift(double *p,int i,int n);
	int SetFilters(double *db, int length);
	double * m_lo_D;	// 低通分解滤波器系数
	double * m_hi_D;	// 高通分解滤波器系数
	double * m_lo_R;	// 低通重构滤波器系数
	double * m_hi_R;	// 高通重构滤波器系数
	//{{AFX_MSG(CWaveletTransform)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVELETTRANSFORM_H__011FFDAA_46EE_45BC_912F_AA11D8F7D523__INCLUDED_)
