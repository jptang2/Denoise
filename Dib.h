// dib.h

#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// DIB���
DECLARE_HANDLE(HDIB);

// DIB����
#define PALVERSION   0x300

/* DIB�� */

// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// �����������Ŀ��
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// �����������ĸ߶�
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)


class CDIB
{

public:
	void SetCircleColor(HDIB hDIB, CRect rect, COLORREF col);
	void SetRgnColor(HDIB hDIB, CRect rect, COLORREF col);
	CDIB();
	~CDIB();

	// ����ԭ��
	BOOL	PaintDIB(HDC hDC,LPRECT lpDCRect,HDIB hDIB,LPRECT lpDIBRect,CPalette* pPal);
	BOOL    CreateDIBPalette(HDIB hDIB, CPalette* cPal);
	LPBYTE   FindDIBBits (LPBYTE lpbi);
	DWORD   DIBWidth (LPBYTE lpDIB);
	DWORD   DIBHeight (LPBYTE lpDIB);
	WORD    PaletteSize (LPBYTE lpbi);
	WORD    DIBNumColors (LPBYTE lpbi);
	WORD	DIBBitCount(LPBYTE lpbi);
	HGLOBAL CopyHandle (HGLOBAL h);

	BOOL    SaveDIB (HDIB hDib, CFile& file);
	HDIB    ReadDIBFile(CFile& file);
};
#endif //!_INC_DIBAPI
