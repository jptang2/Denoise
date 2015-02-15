#if !defined(AFX_GRAYPROC_H__995674B4_3C63_421A_B0A3_E7B25C1CF5E3__INCLUDED_)
#define AFX_GRAYPROC_H__995674B4_3C63_421A_B0A3_E7B25C1CF5E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrayProc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrayProc window
#include "dib.h"

class CGrayProc : public CWnd
{
// Construction
public:
	CGrayProc();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrayProc)
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetIntensity(HDIB hDIB, float *fPs_Y);
	void GetIntensity(HDIB hDIB, int *fPs_Y);
	void GetIntensity(HDIB hDIB, float *fPs_R, float *fPs_G, float *fPs_B);

	void GrayStretch(HDIB hDIB, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);
	void PointEqua(HDIB hDIB);
	virtual ~CGrayProc();

	// Generated message map functions
protected:
	CDIB m_clsDIB;
	
	//{{AFX_MSG(CGrayProc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAYPROC_H__995674B4_3C63_421A_B0A3_E7B25C1CF5E3__INCLUDED_)
