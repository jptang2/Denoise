; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWaveletTransform
LastTemplate=generic CWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TJPGR.h"
LastPage=0

ClassCount=13
Class1=CTJPGRApp
Class2=CTJPGRDoc
Class3=CTJPGRView
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_TJPGRTYPE
Resource2=IDD_ABOUTBOX
Resource3=IDR_DIP_SYTYPE
Resource4=IDD_DLG_POINTSTRE
Class5=CChildFrame
Class6=CAboutDlg
Class7=CDlgPointStre
Class8=CGrayProc
Resource5=IDR_MAINFRAME
Class9=CDLGSetFreq
Class10=CFreqFilter
Class11=CNoise
Class12=MedFilter
Class13=CWaveletTransform
Resource6=IDD_DLG_SETFREQ

[CLS:CTJPGRApp]
Type=0
HeaderFile=TJPGR.h
ImplementationFile=TJPGR.cpp
Filter=N

[CLS:CTJPGRDoc]
Type=0
HeaderFile=TJPGRDoc.h
ImplementationFile=TJPGRDoc.cpp
Filter=N

[CLS:CTJPGRView]
Type=0
HeaderFile=TJPGRView.h
ImplementationFile=TJPGRView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CTJPGRView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=TJPGR.cpp
ImplementationFile=TJPGR.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_FILE_REOPEN
CommandCount=9

[MNU:IDR_TJPGRTYPE]
Type=1
Class=CTJPGRView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[MNU:IDR_DIP_SYTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_POINT_STRE
Command12=ID_POINT_EQUA
Command13=IDM_MEDIAN
Command14=IDM_ADAPT
Command15=IDM_IMPROVE
Command16=ID_FREQ_FOUR
Command17=ID_LPFILTER
Command18=ID_BUTTERWORTH_L
Command19=ID_DWT
Command20=ID_RANDOM_NOISE
Command21=ID_SALT_NOISE
Command22=ID_WINDOW_NEW
Command23=ID_WINDOW_TILE_HORZ
Command24=ID_APP_ABOUT
CommandCount=24

[DLG:IDD_DLG_POINTSTRE]
Type=1
Class=CDlgPointStre
ControlCount=1
Control1=IDC_COORD,static,1342308352

[CLS:CDlgPointStre]
Type=0
HeaderFile=DlgPointStre.h
ImplementationFile=DlgPointStre.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgPointStre

[CLS:CGrayProc]
Type=0
HeaderFile=GrayProc.h
ImplementationFile=GrayProc.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CGrayProc

[DLG:IDD_DLG_SETFREQ]
Type=1
Class=CDLGSetFreq
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDC_VALUE,edit,1350631552
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352

[CLS:CDLGSetFreq]
Type=0
HeaderFile=DLGSetFreq.h
ImplementationFile=DLGSetFreq.cpp
BaseClass=CDialog
Filter=D
LastObject=CDLGSetFreq
VirtualFilter=dWC

[CLS:CFreqFilter]
Type=0
HeaderFile=FreqFilter.h
ImplementationFile=FreqFilter.cpp
BaseClass=CWnd
Filter=W
LastObject=CFreqFilter
VirtualFilter=WC

[CLS:CNoise]
Type=0
HeaderFile=Noise.h
ImplementationFile=Noise.cpp
BaseClass=CWnd
Filter=W
LastObject=CNoise
VirtualFilter=WC

[CLS:MedFilter]
Type=0
HeaderFile=MedFilter.h
ImplementationFile=MedFilter.cpp
BaseClass=generic CWnd
Filter=W
LastObject=ID_APP_ABOUT

[CLS:CWaveletTransform]
Type=0
HeaderFile=WaveletTransform.h
ImplementationFile=WaveletTransform.cpp
BaseClass=generic CWnd
Filter=W
LastObject=CWaveletTransform

