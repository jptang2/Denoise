// TJPGRDoc.h : interface of the CTJPGRDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TJPGRDOC_H__FBEA09AC_FC50_11D5_B668_0010DCA65D26__INCLUDED_)
#define AFX_TJPGRDOC_H__FBEA09AC_FC50_11D5_B668_0010DCA65D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dib.h"
#include "freqfilter.h"

class CTJPGRDoc : public CDocument
{
protected: // create from serialization only
	CTJPGRDoc();
	DECLARE_DYNCREATE(CTJPGRDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTJPGRDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);
	CDIB m_clsDIB;
	CRectTracker m_Tracker;
	int m_nColorIndex;
	COLORREF m_refColorBKG;		// ±³¾°É«
	CPoint m_ptPoint_I;
	CPoint m_ptPoint_II;

	virtual ~CTJPGRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPalette* m_palDIB;
	CSize m_sizeDoc;


// Generated message map functions
public:
	CString m_strCurrentFile;
	HDIB m_hDIB;
	BOOL m_bIsFrequency;
	int m_nRgnNum;
	CRect m_rect[1000];
	float m_fZoomRatio;
	//{{AFX_MSG(CTJPGRDoc)
	afx_msg void OnFileReopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJPGRDOC_H__FBEA09AC_FC50_11D5_B668_0010DCA65D26__INCLUDED_)
