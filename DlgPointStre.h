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

	// �����Ҷ�ֵ�ļ���
	float m_fIntensity[256];

protected:
	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶���һ�㣬2��ʾ�����϶��ڶ��㡣
	int		m_iIsDraging;

	// ��Ӧ����¼��ľ�������
	CRect	m_MouseRect;
	
	// ����ת�۵�����
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
