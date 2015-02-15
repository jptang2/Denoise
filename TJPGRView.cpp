
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
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
		
		// 获取DIB宽度
		int cxDIB = (int) pDoc->m_clsDIB.DIBWidth(lpDIB);
		
		// 获取DIB高度
		int cyDIB = (int) pDoc->m_clsDIB.DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// 判断是否是打印
		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面
			
			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
		}
		else // 非打印
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}
		// 输出DIB
		pDoc->m_clsDIB.PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	// 恢复正常光标
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
	
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();
    
	// 创建一个Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
                                                                                  
	// 保存以前的Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// 获取重绘区域
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// 重绘
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);

	// 恢复以前的Brush
	pDC->SelectObject(pOldBrush);
	return 1;
}

void CTJPGRView::OnPointStre() 
{
// 循环变量
	int i;

	// 各颜色分量的灰度分布密度
	float fPs_Y[256];

	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 计算灰度分布密度
	CGrayProc clsGrayProc;
	clsGrayProc.GetIntensity(pDoc->GetHDIB(), fPs_Y);
	
	// 创建对话框
	CDlgPointStre dlg;
	dlg.m_pDoc = pDoc;

	// 传递灰度分布密度数据给面板类
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// 显示对话框，由用户进行灰度折线变换
	dlg.DoModal();	
}

void CTJPGRView::OnPointEqua() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 进行灰度分布均衡化处理
	CGrayProc clsGrayProc;
	clsGrayProc.PointEqua(pDoc->GetHDIB());
	
	// 重画视图
	Invalidate();

	// 设置脏标记
    pDoc->SetModifiedFlag(TRUE);
}


///////////////////////////////////////////
//	对图像实施傅立叶变换处理			 //	
///////////////////////////////////////////
void CTJPGRView::OnFreqFour() 
{
    // 获取文档
	CTJPGRDoc* pDoc = GetDocument();
	
	CFreqCalculate clsFreqCalculate;

	// 调用DIBFourier()函数进行付立叶变换
	if (clsFreqCalculate.DIBFourier(pDoc->GetHDIB()))
	{
		// 标识为频谱图
		pDoc->m_bIsFrequency = TRUE;

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		//pDoc->UpdateAllViews(NULL);
		Invalidate();
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}	
}


///////////////////////////////////////////
//	对图像实施巴特沃斯低通滤波			 //	
///////////////////////////////////////////
void CTJPGRView::OnButterworthL() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 由对话框输入截止频率
	CDLGSetFreq dlg;
	dlg.SetFlag(2);
//	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// 更新数据
		UpdateData(TRUE);

		// 对图象进行巴特沃斯低通滤波
		CFreqFilter clsFreqFilter;
		clsFreqFilter.ButterworthL(pDoc->GetHDIB(), dlg.m_fValue);

		// 重画视图
		Invalidate();

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	对图像实施理想线性低通滤波			 //	
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

		// 重画视图
		Invalidate();

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		UpdateWindow();						
		EndWaitCursor();
	}			
}

void CTJPGRView::OnSaltNoise() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 对图象进行巴特沃斯低通滤波
	CNoise clsNoise;
	clsNoise.SaltNoise(pDoc->GetHDIB());

	// 重画视图
	Invalidate();

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
}

void CTJPGRView::OnRandomNoise() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 对图象进行巴特沃斯低通滤波
	CNoise clsNoise;
	clsNoise.RandomNoise(pDoc->GetHDIB());

	// 重画视图
	Invalidate();

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
}

void CTJPGRView::OnMedian() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 进行线形平滑滤波处理
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Median(pDoc->GetHDIB());

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	Invalidate();
	
}

void CTJPGRView::OnAdapt() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 进行线形平滑滤波处理
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Adapt(pDoc->GetHDIB());

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	Invalidate();
	
}

void CTJPGRView::OnImprove() 
{
	// 获取文档
	CTJPGRDoc* pDoc = GetDocument();

	// 进行线形平滑滤波处理
	CMedFilter clsSpaceFilter;
	clsSpaceFilter.Improve(pDoc->GetHDIB());

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	Invalidate();
	
}

void CTJPGRView::OnDwt() 
{
    // 获取文档
	CTJPGRDoc* pDoc = GetDocument();
	
	// 更改光标形状
	BeginWaitCursor();		
	
	//ASSERT(pImageData!=NULL);

	// 进行小波
	waveletTransform.DWT_WaveletFilter2(pDoc->GetHDIB());	

	// 恢复光标
	EndWaitCursor();
	

	// 重画视图
	Invalidate();

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
    
}
