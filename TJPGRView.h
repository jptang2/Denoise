// TJPGRView.h : interface of the CTJPGRView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TJPGRVIEW_H__281D1DD1_A7A5_4090_8981_C56588A65ADD__INCLUDED_)
#define AFX_TJPGRVIEW_H__281D1DD1_A7A5_4090_8981_C56588A65ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WaveletTransform.h"


class CTJPGRView : public CView
{
protected: // create from serialization only
	CTJPGRView();
	DECLARE_DYNCREATE(CTJPGRView)

// Attributes
public:
	CTJPGRDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTJPGRView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDIB m_clsDIB;
	CWaveletTransform waveletTransform;
	virtual ~CTJPGRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	

// Generated message map functions
protected:
	//{{AFX_MSG(CTJPGRView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPointStre();
	afx_msg void OnPointEqua();
	afx_msg void OnFreqFour();
	afx_msg void OnButterworthL();
	afx_msg void OnLpfilter();
	afx_msg void OnSaltNoise();
	afx_msg void OnRandomNoise();
	afx_msg void OnMedian();
	afx_msg void OnAdapt();
	afx_msg void OnImprove();
	afx_msg void OnDwt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in TJPGRView.cpp
inline CTJPGRDoc* CTJPGRView::GetDocument()
   { return (CTJPGRDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJPGRVIEW_H__281D1DD1_A7A5_4090_8981_C56588A65ADD__INCLUDED_)
