
#include "MainFrm.h"
#include "stdafx.h"
#include "TJPGR.h"

#include "TJPGRDoc.h"
#include "TJPGRView.h"
#include "grayproc.h"
#include "FreqCalculate.h"
#include "DLGSetFreq.h"
#include "FreqFilter.h"
#include "MedFilter.h"
#include "Noise.h"
#include "WaveletTransform.h"

#include "DlgPointStre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTJPGRView

IMPLEMENT_DYNCREATE(CTJPGRView, CView)

BEGIN_MESSAGE_MAP(CTJPGRView, CView)
	//{{AFX_MSG_MAP(CTJPGRView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_FREQ_FOUR, OnFreqFour)
	ON_COMMAND(ID_BUTTERWORTH_L, OnButterworthL)
	ON_COMMAND(ID_LPFILTER, OnLpfilter)
	ON_COMMAND(ID_SALT_NOISE, OnSaltNoise)
	ON_COMMAND(ID_RANDOM_NOISE, OnRandomNoise)
	ON_COMMAND(IDM_MEDIAN, OnMedian)
	ON_COMMAND(IDM_ADAPT, OnAdapt)
	ON_COMMAND(IDM_IMPROVE, OnImprove)
	ON_COMMAND(ID_DWT, OnDwt)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTJPGRView construction/destruction

CTJPGRView::CTJPGRView()
{
	// TODO: add construction code here

}

CTJPGRView::~CTJPGRView()
{
}

BOOL CTJPGRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTJPGRView drawing

void CTJPGRView::OnDraw(CDC* pDC)
{
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// ��ȡDIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
		
		// ��ȡDIB���
		int cxDIB = (int) pDoc->m_clsDIB.DIBWidth(lpDIB);
		
		// ��ȡDIB�߶�
		int cyDIB = (int) pDoc->m_clsDIB.DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// �ж��Ƿ��Ǵ�ӡ
		if (pDC->IsPrinting())
		{
			// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��
			
			// ��ȡ��ӡҳ���ˮƽ���(����)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// ��ȡ��ӡ��ÿӢ��������
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// �����ӡͼ��λ�ã���ֱ���У�
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
		}
		else // �Ǵ�ӡ
		{
			// ��������ͼ��
			rcDest = rcDIB;
		}
		// ���DIB
		pDoc->m_clsDIB.PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	// �ָ��������
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CTJPGRView printing

BOOL CTJPGRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTJPGRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTJPGRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTJPGRView diagnostics

#ifdef _DEBUG
void CTJPGRView::AssertValid() const
{
	CView::AssertValid();
}

void CTJPGRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTJPGRDoc* CTJPGRView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTJPGRDoc)));
	return (CTJPGRDoc*)m_pDocument;
}
#endif //_DEBUG




BOOL CTJPGRView::OnEraseBkgnd(CDC* pDC) 
{
	
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();
    
	// ����һ��Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
                                                                                  
	// ������ǰ��Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// ��ȡ�ػ�����
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// �ػ�
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);

	// �ָ���ǰ��Brush
	pDC->SelectObject(pOldBrush);
	return 1;
}

void CTJPGRView::OnPointStre() 
{
// ѭ������
	int i;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc;
	clsGrayProc.GetIntensity(pDoc->GetHDIB(), fPs_Y);
	
	// �����Ի���
	CDlgPointStre dlg;
	dlg.m_pDoc = pDoc;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ�Ի������û����лҶ����߱任
	dlg.DoModal();	
}

void CTJPGRView::OnPointEqua() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ���лҶȷֲ����⻯����
	CGrayProc clsGrayProc;
	clsGrayProc.PointEqua(pDoc->GetHDIB());
	
	// �ػ���ͼ
	Invalidate();

	// ��������
    pDoc->SetModifiedFlag(TRUE);
}


///////////////////////////////////////////
//	��ͼ��ʵʩ����Ҷ�任����			 //	
///////////////////////////////////////////
void CTJPGRView::OnFreqFour() 
{
    // ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();
	
	CFreqCalculate clsFreqCalculate;

	// ����DIBFourier()�������и���Ҷ�任
	if (clsFreqCalculate.DIBFourier(pDoc->GetHDIB()))
	{
		// ��ʶΪƵ��ͼ
		pDoc->m_bIsFrequency = TRUE;

		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ������ͼ
		//pDoc->UpdateAllViews(NULL);
		Invalidate();
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}	
}


///////////////////////////////////////////
//	��ͼ��ʵʩ������˹��ͨ�˲�			 //	
///////////////////////////////////////////
void CTJPGRView::OnButterworthL() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.SetFlag(2);
//	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter;
		clsFreqFilter.ButterworthL(pDoc->GetHDIB(), dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�������Ե�ͨ�˲�			 //	
///////////////////////////////////////////
void CTJPGRView::OnLpfilter() 
{
	CTJPGRDoc* pDoc = GetDocument();
    CDLGSetFreq dlg;
	dlg.SetFlag(1);
	if(dlg.DoModal()==IDOK)
	{
		BeginWaitCursor();
		UpdateData(TRUE);	

		CFreqFilter clsLpfilter;
		clsLpfilter.Lpfilter(pDoc->GetHDIB(), dlg.var);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
		UpdateWindow();						
		EndWaitCursor();
	}			
}

void CTJPGRView::OnSaltNoise() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ��ͼ����а�����˹��ͨ�˲�
	CNoise clsNoise;
	clsNoise.SaltNoise(pDoc->GetHDIB());

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
}

void CTJPGRView::OnRandomNoise() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ��ͼ����а�����˹��ͨ�˲�
	CNoise clsNoise;
	clsNoise.RandomNoise(pDoc->GetHDIB());

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
}

void CTJPGRView::OnMedian() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ��������ƽ���˲�����
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Median(pDoc->GetHDIB());

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
	
}

void CTJPGRView::OnAdapt() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ��������ƽ���˲�����
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Adapt(pDoc->GetHDIB());

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
	
}

void CTJPGRView::OnImprove() 
{
	// ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();

	// ��������ƽ���˲�����
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Improve(pDoc->GetHDIB());

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
	
}

void CTJPGRView::OnDwt() 
{
    // ��ȡ�ĵ�
	CTJPGRDoc* pDoc = GetDocument();
	
	// ���Ĺ����״
	BeginWaitCursor();		
	
	//ASSERT(pImageData!=NULL);

	// ����С��
	waveletTransform.DWT_WaveletFilter2(pDoc->GetHDIB());	

	// �ָ����
	EndWaitCursor();
	

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
    
}
