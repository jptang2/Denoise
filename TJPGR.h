// TJPGR.h : main header file for the TJPGR application
//

#if !defined(AFX_TJPGR_H__FBEA09A4_FC50_11D5_B668_0010DCA65D26__INCLUDED_)
#define AFX_TJPGR_H__FBEA09A4_FC50_11D5_B668_0010DCA65D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTJPGRApp:
// See TJPGR.cpp for the implementation of this class
//

class CTJPGRApp : public CWinApp
{
public:
	CTJPGRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTJPGRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTJPGRApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJPGR_H__FBEA09A4_FC50_11D5_B668_0010DCA65D26__INCLUDED_)
