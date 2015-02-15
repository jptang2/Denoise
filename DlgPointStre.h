#if !defined(AFX_DLGPOINTSTRE_H__A250D1D6_9873_4AEB_B185_25A0D7BB138A__INCLUDED_)
#define AFX_DLGPOINTSTRE_H__A250D1D6_9873_4AEB_B185_25A0D7BB138A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPointStre.h : header file
//

#include "TJPGRDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre dialog

class CDlgPointStre : public CDialog
{
// Construction
public:
	CDlgPointStre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPointStre)
	enum { IDD = IDD_DLG_POINTSTRE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPointStre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	public:
	CTJPGRDoc* m_pDoc; 

	// 各个灰度值的计数
	float m_fIntensity[256];

protected:
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动第一点，2表示正在拖动第二点。
	int		m_iIsDraging;

	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	// 两个转折点坐标
	BYTE	m_bX1;
	BYTE	m_bY1;
	BYTE	m_bX2;
	BYTE	m_bY2;

	// Generated message map functions
	//{{AFX_MSG(CDlgPointStre)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOINTSTRE_H__A250D1D6_9873_4AEB_B185_25A0D7BB138A__INCLUDED_)
