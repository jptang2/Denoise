// ************************************************************************
//  �ļ�����dib.cpp
//
//  DIB(Independent Bitmap) �����⣺
//
//  PaintDIB()          - ����DIB����
//  CreateDIBPalette()  - ����DIB�����ɫ��
//  FindDIBBits()       - ����DIBͼ��������ʼλ��
//  DIBWidth()          - ����DIB���
//  DIBHeight()         - ����DIB�߶�
//  PaletteSize()       - ����DIB��ɫ���С
//  DIBNumColors()      - ����DIB��ɫ����ɫ��Ŀ
//  CopyHandle()        - �����ڴ��
//	DIBBitCount()		- �ú�������DIBBitCount	
//  SaveDIB()           - ��DIB���浽ָ���ļ���
//  ReadDIBFile()       - ��ָ���ļ��ж�ȡDIB����
//	SetRgnColor()		- ��ָ������ɫ���ָ��������
//	SetCircleColor()	- ��ָ������ɫ���ָ����Բ������
//
// ************************************************************************

#include "stdafx.h"
#include "dib.h"
#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

//	Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

CDIB::CDIB()
{
	
}

CDIB::~CDIB()
{
}

////////////////////////////////////////////////////////////////////////////
//							����ģ�鿪ʼ								  //
////////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *
 * �������ƣ�
 *   PaintDIB()
 *
 * ����:
 *   HDC hDC            - �豸�������
 *   LPRECT lpDCRect    - ��������
 *   HDIB hDIB          - ָ��DIB�����ָ��
 *   LPRECT lpDIBRect   - �����DIB����
 *   CPalette* pPal     - ��ɫ��ָ��
 *
 * ����ֵ:
 *   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   ����DIB����
 *
 ************************************************************************/

BOOL CDIB::PaintDIB(HDC     hDC,
					LPRECT  lpDCRect,
					HDIB    hDIB,
					LPRECT  lpDIBRect,
					CPalette* pPal)
{
	LPBYTE    lpDIBINFOHEADER;            // BITMAPINFOHEADERָ��
	LPBYTE    lpDIBBits;				  // DIB����ָ��
	BOOL	  bRet = FALSE;				  // ���سɹ���־
	HPALETTE  hNewPal = NULL;			  // ��ǰ�ĵ�ɫ��
	HPALETTE  hOldPal = NULL;			  // ��ǰ�ĵ�ɫ��

	// �ж�DIB�����Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		// ����
		return FALSE;
	}

	// ����DIB
	lpDIBINFOHEADER  = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = FindDIBBits(lpDIBINFOHEADER);

	// ��ȡDIB��ɫ�壬��ѡ��
	if (pPal != NULL)
	{
		hNewPal = (HPALETTE) pPal->m_hObject;

		// ѡ�е�ɫ��
		hOldPal = ::SelectPalette(hDC, hNewPal, TRUE);
	}

	// ������ʾģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
	if ((RECTWIDTH(lpDCRect)  == RECTWIDTH(lpDIBRect)) &&
	   (RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
	{
		// ԭʼ��С���������졣
		bRet = ::SetDIBitsToDevice(hDC,						    // hDC
								   lpDCRect->left,				// DestX
								   lpDCRect->top,			    // DestY
								   RECTWIDTH(lpDCRect),			// nDestWidth
								   RECTHEIGHT(lpDCRect),		// nDestHeight
								   lpDIBRect->left,				// SrcX
								   (int)DIBHeight(lpDIBINFOHEADER) -
									  lpDIBRect->top -
									  RECTHEIGHT(lpDIBRect),	// SrcY
								   0,							// nStartScan
								   (WORD)DIBHeight(lpDIBINFOHEADER),  // nNumScans
								   lpDIBBits,					// lpBits
								   (LPBITMAPINFO)lpDIBINFOHEADER,     // lpBitsInfo
								   DIB_RGB_COLORS);				// wUsage
	}
    else
	{
		// ��ԭʼ��С�����졣
		bRet = ::StretchDIBits(hDC,								// hDC
							   lpDCRect->left,					// DestX
							   lpDCRect->top,					// DestY
							   RECTWIDTH(lpDCRect),				// nDestWidth
							   RECTHEIGHT(lpDCRect),			// nDestHeight
							   lpDIBRect->left,					// SrcX
							   lpDIBRect->top,					// SrcY
							   RECTWIDTH(lpDIBRect),			// wSrcWidth
							   RECTHEIGHT(lpDIBRect),			// wSrcHeight
							   lpDIBBits,						// lpBits
							   (LPBITMAPINFO)lpDIBINFOHEADER,   // lpBitsInfo
							   DIB_RGB_COLORS,					// wUsage
							   SRCCOPY);						// dwROP
	}
	
    // �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// �ָ���ǰ�ĵ�ɫ��
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}
	
	// ����
	return bRet;
}


/*************************************************************************
 *
 * �������ƣ�
 *   CreateDIBPalette()
 *
 * ����:
 *   HDIB hDIB          - ָ��DIB�����ָ��
 *   CPalette* pPal     - ��ɫ��ָ��
 *
 * ����ֵ:
 *   BOOL               - �����ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   ����DIB�����ɫ��
 *
 ************************************************************************/

BOOL CDIB::CreateDIBPalette(HDIB hDIB, CPalette* pPal)
{
	// ָ���߼���ɫ���ָ��
	LPLOGPALETTE lpPal;
	
	// �߼���ɫ��ľ��
	HANDLE hLogPal;
	
	// ��ɫ��ľ��
	HPALETTE hPal = NULL;
	
	// ѭ������
	int i;
	
	// ��ɫ���е���ɫ��Ŀ
	int nNumColors;
	
	// ָ��DIB��ָ��
	LPBYTE lpbi;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// �������
	BOOL bRet = FALSE;
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		// ����FALSE
		return FALSE;
	}
	
	// ����DIB
	lpbi = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpbi;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpbi;
	
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	nNumColors = DIBNumColors(lpbi);
	
	if (nNumColors != 0)
	{
		// ����Ϊ�߼���ɫ���ڴ�
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* nNumColors);
		
		// ����ڴ治�㣬�˳�
		if (hLogPal == 0)
		{
			// �������
			::GlobalUnlock((HGLOBAL) hDIB);
			
			// ����FALSE
			return FALSE;
		}
		
		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);
		
		// ���ð汾��
		lpPal->palVersion = PALVERSION;
		
		// ������ɫ��Ŀ
		lpPal->palNumEntries = (WORD)nNumColors;
		
		// �ж��Ƿ���WIN3.0��DIB
		bWinStyleDIB = IS_WIN30_DIB(lpbi);

		// ��ȡ��ɫ��
		for (i = 0; i < nNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;
				
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;
				
				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}
		
		// �����߼���ɫ�崴����ɫ�壬������ָ��
		bRet = pPal->CreatePalette(lpPal);
		
		// �������
		::GlobalUnlock((HGLOBAL) hLogPal);
		
		// �ͷ��߼���ɫ��
		::GlobalFree((HGLOBAL) hLogPal);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	
	// ���ؽ��
	return bRet;
}


/*************************************************************************
 *
 * �������ƣ�
 *   FindDIBBits()
 *
 * ����:
 *   LPBYTE lpbi        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   LPBYTE             - ָ��DIBͼ��������ʼλ��
 *
 * ˵��:
 *   ����DIBͼ��������ʼλ��
 *
 ************************************************************************/

LPBYTE CDIB::FindDIBBits(LPBYTE lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBWidth()
 *
 * ����:
 *   LPBYTE lpbi        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIBλ����
 *
 * ˵��:
 *   ��ȡDIBλͼ�Ŀ��
 *
 ************************************************************************/

DWORD CDIB::DIBWidth(LPBYTE lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBHeight()
 *
 * ����:
 *   LPBYTE lpDIB       - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   DWORD              - DIBλͼ�߶�
 *
 * ˵��:
 *   ��ȡDIBλͼ�߶�
 *
 ************************************************************************/

DWORD CDIB::DIBHeight(LPBYTE lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	// ����DIB��ͼ��Ŀ��
	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   PaletteSize()
 *
 * ����:
 *   LPBYTE lpbi        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - DIB�е�ɫ��Ĵ�С
 *
 * ˵��:
 *   ����DIB��ɫ���С
 *
 ************************************************************************/

WORD CDIB::PaletteSize(LPBYTE lpbi)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB (lpbi))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBNumColors()
 *
 * ����:
 *   LPBYTE lpbi        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   ����DIB��ɫ����ɫ��Ŀ
 *
 ************************************************************************/

WORD CDIB::DIBNumColors(LPBYTE lpbi)
{
	int nBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��
	
	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡdwClrUsedֵ
		DWORD dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		
		if (dwClrUsed != 0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// �������ص�λ��������ɫ��Ŀ
	switch (nBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;
		default:
			return 0;
	}
}


/*************************************************************************
 *
 * �������ƣ�
 *   DIBBitCount()
 *
 * ����:
 *   LPBYTE lpbi        - ָ��DIB�����ָ��
 *
 * ����ֵ:
 *   WORD               - ���ص�ɫ������ɫ������
 *
 * ˵��:
 *   �ú�������DIBBitCount��
 *
 ************************************************************************/

WORD CDIB::DIBBitCount(LPBYTE lpbi)
{
	int nBitCount;

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}
	
	// ����nBitCount
	return nBitCount;
}


/*************************************************************************
 *
 * �������ƣ�
 *   CopyHandle()
 *
 * ����:
 *   HGLOBAL h          - Ҫ���Ƶ��ڴ�����
 *
 * ����ֵ:
 *   HGLOBAL            - ���ƺ�����ڴ�����
 *
 * ˵��:
 *   �����ڴ��
 *
 ************************************************************************/

HGLOBAL CDIB::CopyHandle (HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	// ��ȡָ���ڴ������С
	DWORD dwLen = ::GlobalSize((HGLOBAL) h);
	
	// �������ڴ�ռ�
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);
	
	// �жϷ����Ƿ�ɹ�
	if (hCopy != NULL)
	{
		// ����
		void* lpCopy = ::GlobalLock((HGLOBAL) hCopy);
		void* lp     = ::GlobalLock((HGLOBAL) h);
		
		// ����
		memcpy(lpCopy, lp, dwLen);
		
		// �������
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}


/*************************************************************************
 *
 * �������ƣ�
 *   SaveDIB()
 *
 * ����:
 *   HDIB hDib          - Ҫ�����DIB
 *   CFile& file        - �����ļ�CFile
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE����CFileException
 *
 * ˵��:
 *   ��DIB���浽ָ���ļ���
 *
 *************************************************************************/

BOOL CDIB::SaveDIB(HDIB hDib, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;

	if (hDib == NULL)
	{
		// ���DIBΪ�գ�����FALSE
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) hDib);
	
	if (lpBI == NULL)
	{
		// Ϊ�գ�����FALSE
		return FALSE;
	}
	
	// �ж��Ƿ���WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// ��֧���������͵�DIB����
		
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// ����FALSE
		return FALSE;
	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType = DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��
	
	// �ļ�ͷ��С����ɫ���С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
	DWORD dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);
	
	// ����ͼ���С
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ��СΪWidth * Height
		DWORD dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;
		
		// �����DIB�����Ĵ�С
		dwDIBSize += dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage = dwBmBitsSize;
	}


	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);
	
	// ����������
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ���С
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize + PaletteSize((LPBYTE)lpBI);

	// ����д�ļ�
	TRY
	{
		// д�ļ�ͷ
		file.Write((LPBYTE)&bmfHdr, sizeof(BITMAPFILEHEADER));
		
		// дDIBͷ������
		file.WriteHuge(lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		// �������
		::GlobalUnlock((HGLOBAL) hDib);
		
		// �׳��쳣
		THROW_LAST();
	}
	END_CATCH
	
	// �������
	::GlobalUnlock((HGLOBAL) hDib);
	
	// ����TRUE
	return TRUE;
}


/*************************************************************************
 *
 * �������ƣ�
 *   ReadDIBFile()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ��ļ�CFile
 *
 * ����ֵ:
 *   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   ��ָ���ļ��ж�ȡDIB����
 *
 *************************************************************************/

HDIB CDIB::ReadDIBFile(CFile& file)
{
	BITMAPFILEHEADER bmfHeader;
	HDIB hDIB;
	LPBYTE lpDIB;

	// ��ȡDIB���ļ������ȣ��ֽڣ�
	DWORD dwBitsSize = file.GetLength();

	// ���Զ�ȡDIB�ļ�ͷ
	if (file.Read((LPBYTE)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}

	// �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
	{
		// ��DIB���󣬷���NULL��
		return NULL;
	}

	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// ������
	if (file.ReadHuge(lpDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) !=
		dwBitsSize - sizeof(BITMAPFILEHEADER) )
	{
		// ��С���ԡ�
		
		// �������
		::GlobalUnlock((HGLOBAL) hDIB);
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		// ����NULL��
		return NULL;
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
	 
	// ����DIB���
	return hDIB;
}


/*************************************************************************
 *
 * �������ƣ�
 *   SetRgnColor()
 *
 * ����:	
 *   HDIB hDIB			- Ҫ��ȡ���ļ�
 *	 CRect rect			- ���õľ�����
 *	 COLORREF col		- ������ɫ
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�����ָ������ɫ���ָ��������
 *
 *************************************************************************/

void CDIB::SetRgnColor(HDIB hDIB, CRect rect, COLORREF col)
{
	// ѭ������
	LONG i;
	LONG j;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = FindDIBBits(lpDIB);

	// DIB�Ŀ��
	LONG lWidth = DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = DIBHeight(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// �����������ɫ���
	for (i = lHeight - rect.bottom; i < lHeight - rect.top; i++)
	{
		for (j = rect.left * 3; j < rect.right * 3; j++)
		{
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j) = GetBValue(col);
			j++;
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j) = GetGValue(col);
			j++;
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j) = GetRValue(col);
		}
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
}


/*************************************************************************
 *
 * �������ƣ�
 *   SetCircleColor()
 *
 * ����:	
 *   HDIB hDIB			- Ҫ��ȡ���ļ�
 *	 CRect rect			- ���õľ�����
 *	 COLORREF col		- ������ɫ
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�����ָ������ɫ���ָ����Բ������
 *
 *************************************************************************/

void CDIB::SetCircleColor(HDIB hDIB, CRect rect, COLORREF col)
{
	// ѭ������
	LONG i;
	LONG j;

	// ָ��DIB��ָ��
	LPBYTE lpDIB;
	
	// ָ��DIB����ָ��
	LPBYTE lpDIBBits;
	
	// ����DIB
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL) hDIB);

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = FindDIBBits(lpDIB);

	// DIB�Ŀ��
	LONG lWidth = DIBWidth(lpDIB);
	
	// DIB�ĸ߶�
	LONG lHeight = DIBHeight(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 24);

	// �м����
	int cx, cy, dr;

	// ��Բ�����������ɫ���
	for (i = lHeight - rect.bottom; i < lHeight - rect.top; i++)
	{
		for (j = rect.left; j < rect.right; j ++)
		{
			cx = abs(j - rect.left - 8);
			cy = abs(i - lHeight + rect.bottom - 8);
			dr = cx * cx + cy * cy;
			if (dr > 64 || i < 0 || i > lHeight || j < 0 || j > lWidth)
				continue;
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j * 3) = GetBValue(col);
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j * 3 + 1) = GetGValue(col);
			*(unsigned char *)(lpDIBBits + lLineBytes * i + j * 3 + 2) = GetRValue(col);
		}
	}

	// �������
	::GlobalUnlock((HGLOBAL) hDIB);
}
