#if !defined(AFX_DLGSETFREQ_H__0D7F31AF_4B84_44D5_9292_0275BADE45A5__INCLUDED_)
#define AFX_DLGSETFREQ_H__0D7F31AF_4B84_44D5_9292_0275BADE45A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSetFreq.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGSetFreq dialog

class CDLGSetFreq : public CDialog
{
// Construction
public:
	void SetFlag(int value);
	CDLGSetFreq(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDLGSetFreq)
	enum { IDD = IDD_DLG_SETFREQ };
	CButton	m_aaa;
	int flag;
	float	m_fValue;
	float	var;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSetFreq)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSetFreq)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETFREQ_H__0D7F31AF_4B84_44D5_9292_0275BADE45A5__INCLUDED_)
