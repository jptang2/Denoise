// DLGSetFreq.cpp : implementation file
//

#include "stdafx.h"
#include "TJPGR.h"
#include "DLGSetFreq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSetFreq dialog


CDLGSetFreq::CDLGSetFreq(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSetFreq::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSetFreq)
	m_fValue = 250.0f;
	var = 550.0f;
	//}}AFX_DATA_INIT
}


void CDLGSetFreq::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSetFreq)
	DDX_Text(pDX, IDC_VALUE, m_fValue);
	DDV_MinMaxFloat(pDX, m_fValue, 0.f, 2000.f);
	DDX_Text(pDX, IDC_EDIT2, var);
	DDV_MinMaxFloat(pDX, var, 0.f, 2000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSetFreq, CDialog)
	//{{AFX_MSG_MAP(CDLGSetFreq)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDLGSetFreq::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	if(flag==1)
	{
		GetDlgItem(IDC_VALUE)->EnableWindow(FALSE);
		CWnd *pwnd=GetDlgItem(IDC_EDIT2);
		GotoDlgCtrl(pwnd);
	}
	else if(flag==2)
	{
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		CWnd *pwnd=GetDlgItem(IDC_VALUE);
		GotoDlgCtrl(pwnd);
	}
	else
	{
		CWnd *pwnd=GetDlgItem(IDC_EDIT2);
		GotoDlgCtrl(pwnd);
	}	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDLGSetFreq::SetFlag(int value)
{
flag=value;
}
