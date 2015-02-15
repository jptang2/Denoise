#if !defined(AFX_MEDFILTER_H__AC2E5B3D_A77E_4CC7_B50F_33F57CD94618__INCLUDED_)
#define AFX_MEDFILTER_H__AC2E5B3D_A77E_4CC7_B50F_33F57CD94618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MedFilter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MedFilter window
#include "dib.h"

class CMedFilter : public CWnd
{
// Construction
public:
	CMedFilter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MedFilter)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Improve(HDIB hDIB);
	void Adapt(HDIB hDIB);
	void Median(HDIB hDIB);
	virtual ~CMedFilter();

	// Generated message map functions
protected:
	CDIB m_clsDIB;
	//{{AFX_MSG(MedFilter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEDFILTER_H__AC2E5B3D_A77E_4CC7_B50F_33F57CD94618__INCLUDED_)
