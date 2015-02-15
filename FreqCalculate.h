#if !defined(AFX_FREQCALCULATE_H__EE8EA061_0A59_11D6_B669_0010DCA65D26__INCLUDED_)
#define AFX_FREQCALCULATE_H__EE8EA061_0A59_11D6_B669_0010DCA65D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreqCalculate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFreqCalculate window
#include "dib.h"
#include <complex>
using namespace std;

class CFreqCalculate : public CWnd
{
// Construction
public:
	CFreqCalculate();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreqCalculate)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DIBIDWT(HDIB hDIB);
	void IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	void DIBDWT(HDIB hDIB);
	int a(int nX, int nXSize);
	void DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	BOOL IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL DIBFourier(HDIB hDIB);
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
	void FFT(complex<double> * TD, complex<double> * FD, int r);
	virtual ~CFreqCalculate();

	// Generated message map functions
protected:
	CDIB m_clsDIB;

	//{{AFX_MSG(CFreqCalculate)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREQCALCULATE_H__EE8EA061_0A59_11D6_B669_0010DCA65D26__INCLUDED_)
