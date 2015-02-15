// TJPGRDoc.cpp : implementation of the CTJPGRDoc class
//

#include "stdafx.h"
#include "TJPGR.h"

#include "TJPGRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTJPGRDoc

IMPLEMENT_DYNCREATE(CTJPGRDoc, CDocument)

BEGIN_MESSAGE_MAP(CTJPGRDoc, CDocument)
	//{{AFX_MSG_MAP(CTJPGRDoc)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTJPGRDoc construction/destruction

CTJPGRDoc::CTJPGRDoc()
{
	// Ĭ�ϱ���ɫ����ɫ
	m_refColorBKG = 0x00808080;	

	
	// ��ʼ������
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1, 1);
	m_ptPoint_I = CPoint(0, 0);
	m_ptPoint_II = CPoint(0, 0);
	m_fZoomRatio = 1.0f;
	m_nRgnNum = 0;
	m_bIsFrequency = FALSE;
	m_strCurrentFile = "";

	m_Tracker.m_rect.SetRect (0, 0, 10, 10);
	m_Tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine;
}

CTJPGRDoc::~CTJPGRDoc()
{
	// �ж�DIB�����Ƿ����
	if (m_hDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	
	// �жϵ�ɫ���Ƿ����
	if (m_palDIB != NULL)
	{
		// �����ɫ��
		delete m_palDIB;
	}
}

BOOL CTJPGRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strCurrentFile = "";

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTJPGRDoc serialization

void CTJPGRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTJPGRDoc diagnostics

#ifdef _DEBUG
void CTJPGRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTJPGRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTJPGRDoc commands


///////////////////////////////////////////
//	���´��ĵ�						 //
///////////////////////////////////////////

void CTJPGRDoc::OnFileReopen() 
{
 	// ���´�ͼ�񣬷��������޸�
 
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�
/*	if (IsModified())
	{
		// ��ʾ�û��ò�������ʧ���е�ǰ���޸�
		if (MessageBox(NULL, "���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", "ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// �û�ȡ��������ֱ�ӷ���
			return;
		}
	}
*/	
	CFile file;
 	CFileException fe;
 
 	CString strPathName;
  	
	// ��ȡ��ǰ�ļ�·��
 	strPathName = GetPathName();
 
 	// ���´��ļ�
 	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
 	{
 		// ʧ��
 		ReportSaveLoadException(strPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 
		// ����
 		return;
 	}
 	
 	// ���Ĺ����״
 	BeginWaitCursor();
 	
 	// ���Ե���ReadDIBFile()��ȡͼ��
 	TRY
 	{
 		m_hDIB = m_clsDIB.ReadDIBFile(file);
 	}
 	CATCH (CFileException, eLoad)
 	{
 		// ��ȡʧ��
 		file.Abort();
 		
 		// �ָ������״
 		EndWaitCursor();
 		
 		// ����ʧ��
 		ReportSaveLoadException(strPathName, eLoad,	FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		// ����DIBΪ��
 		m_hDIB = NULL;
 		
 		// ����
 		return;
 	}
 	END_CATCH
 	
 	// ��ʼ��DIB
 	InitDIBData();
 	
 	// �ж϶�ȡ�ļ��Ƿ�ɹ�
 	if (m_hDIB == NULL)
 	{
 		// ʧ�ܣ����ܷ�BMP��ʽ
 		CString strMsg;
 		strMsg = "��ȡͼ��ʱ���������Ƿ�BMP��ʽ��ͼ���ļ���";
 		
 		// ��ʾ����
 		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
  		
 		// �ָ������״
 		EndWaitCursor();
 		
 		// ����
 		return;
 	}
 	
 	// ��ʼ������ΪFALSE
 	SetModifiedFlag(FALSE);
 	
 	// ˢ��
 	UpdateAllViews(NULL);
  	
 	// �ָ������״
 	EndWaitCursor();
	
 	// ����
 	return;
}


///////////////////////////////////////////
//	���ĵ�							 //
///////////////////////////////////////////

BOOL CTJPGRDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	m_strCurrentFile = CString(lpszPathName);

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// ����FALSE
		return FALSE;
	}
	
	// ����ĵ������е�����
	DeleteContents();
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���Ե���ReadDIBFile()��ȡͼ�񣬲������쳣
	TRY
	{
		m_hDIB = m_clsDIB.ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// ��ȡʧ��
		file.Abort();
		
		// �ָ������״
		EndWaitCursor();
		
		// ����ʧ��
		ReportSaveLoadException(lpszPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// ����DIBΪ��
		m_hDIB = NULL;
		
		// ����FALSE
		return FALSE;
	}
	END_CATCH
	
	// ��ʼ��DIB
	InitDIBData();
	
	// �ָ������״
	EndWaitCursor();
	
	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (m_hDIB == NULL)
	{
		// ʧ�ܣ����ܷ�BMP��ʽ
		CString strMsg;
		strMsg = "��ȡͼ��ʱ���������Ƿ�BMP��ʽ��ͼ���ļ���";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����FALSE
		return FALSE;
	}
	
	// �����ļ�·����ӵ����ʹ�ã�MRU�����ļ��б�
	SetPathName(lpszPathName);
	
	// ��ʼ���ͱ��ΪFALSE
	SetModifiedFlag(FALSE);
	
	// ����TRUE
	return TRUE;
}


///////////////////////////////////////////
//	�����ĵ�							 //
///////////////////////////////////////////

BOOL CTJPGRDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;

	m_strCurrentFile = CString(lpszPathName);

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeCreate | 
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);
		
		// ����FALSE
		return FALSE;
	}

	// ���Ե���SaveDIB����ͼ��
	BOOL bSuccess = FALSE;
	TRY
	{		
		// ���Ĺ����״
		BeginWaitCursor();

		// ���Ա���ͼ��
		bSuccess = m_clsDIB.SaveDIB(m_hDIB, file);

		// �ر��ļ�
		file.Close();
	}			
	CATCH (CException, eSave)
	{
		// ʧ��
		file.Abort();
		
		// �ָ������״
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		// ����FALSE
		return FALSE;
	}
	END_CATCH

	// �ָ������״
	EndWaitCursor();
	
	// ��������ΪFALSE
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// ����ʧ�ܣ�������������ʽ��DIB�����Զ�ȡ���ǲ��ܱ���
		// ������SaveDIB��������

		CString strMsg;
		strMsg = "�޷�����BMPͼ��";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	}
	
	return bSuccess;
}


///////////////////////////////////////////
//	�滻DIB���ڹ���ճ�����õ��ú���		 //
///////////////////////////////////////////

void CTJPGRDoc::ReplaceHDIB(HDIB hDIB)
{
	// �ж�DIB�Ƿ�Ϊ��
	if (m_hDIB != NULL)
	{
		// �ǿգ������
		::GlobalFree((HGLOBAL) m_hDIB);
	}

	// �滻���µ�DIB����
	m_hDIB = hDIB;
}


///////////////////////////////////////////
//	��ʼ��DIB����						 //
///////////////////////////////////////////

void CTJPGRDoc::InitDIBData()
{
	// �жϵ�ɫ���Ƿ�Ϊ��
	if (m_palDIB != NULL)
	{
		// ɾ����ɫ�����
		delete m_palDIB;

		// ���õ�ɫ��Ϊ��
		m_palDIB = NULL;
	}
	
	// ���DIB����Ϊ�գ�ֱ�ӷ���
	if (m_hDIB == NULL)
	{
		// ����
		return;
	}
	
	LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) m_hDIB);
	
	// �ж�ͼ���Ƿ����INT_MAX:2,147,483,647��
	if (m_clsDIB.DIBWidth(lpDIB) > INT_MAX || m_clsDIB.DIBHeight(lpDIB) > INT_MAX)
	{
		// ����DIB����
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		// �ͷ�DIB����
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// ����DIBΪ��
		m_hDIB = NULL;
		
		CString strMsg;
		strMsg = "BMPͼ��̫��";
		
		// ��ʾ�û�
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// �����ĵ���С
	m_sizeDoc = CSize((int) m_clsDIB.DIBWidth(lpDIB), (int) m_clsDIB.DIBHeight(lpDIB));
	
	// ����DIB����
	::GlobalUnlock((HGLOBAL) m_hDIB);
	
	// �����µ�ɫ��
	m_palDIB = new CPalette;
	
	// �ж��Ƿ񴴽��ɹ�
	if (m_palDIB == NULL)
	{
		// ʧ�ܣ��������ڴ治��
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// ����DIB����Ϊ��
		m_hDIB = NULL;
		
		// ����
		return;
	}
	
	// ����CreateDIBPalette��������ɫ��
	if (m_clsDIB.CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// ���ؿգ����ܸ�DIB����û�е�ɫ��
		
		// ɾ��
		delete m_palDIB;
		
		// ����Ϊ��
		m_palDIB = NULL;
		
		// ����
		return;
	}
}
