#if !defined(AFX_NOISE_H__69C207F2_B76B_44C4_87EB_CEC86BF1F618__INCLUDED_)
#define AFX_NOISE_H__69C207F2_B76B_44C4_87EB_CEC86BF1F618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Noise.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNoise window

#include "dib.h"

class CNoise : public CWnd
{
// Construction
public:
	CNoise();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoise)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaltNoise(HDIB hDIB);
	void RandomNoise(HDIB hDIB);
	virtual ~CNoise();

	// Generated message map functions
protected:
	CDIB m_clsDIB;
	//{{AFX_MSG(CNoise)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOISE_H__69C207F2_B76B_44C4_87EB_CEC86BF1F618__INCLUDED_)
