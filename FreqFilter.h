#if !defined(AFX_FREQFILTER_H__F094B924_9EFE_460C_8F72_12BAC57302DC__INCLUDED_)
#define AFX_FREQFILTER_H__F094B924_9EFE_460C_8F72_12BAC57302DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FreqFilter.h : header file
//
#include "dib.h"
/////////////////////////////////////////////////////////////////////////////
// CFreqFilter window

class CFreqFilter : public CWnd
{
// Construction
public:
	CFreqFilter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFreqFilter)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ButterworthL(HDIB hDIB, float fD);
	void Lpfilter(HDIB hDIB,int var);
	virtual ~CFreqFilter();

	// Generated message map functions
protected:
	CDIB m_clsDIB;
	//{{AFX_MSG(CFreqFilter)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREQFILTER_H__F094B924_9EFE_460C_8F72_12BAC57302DC__INCLUDED_)
