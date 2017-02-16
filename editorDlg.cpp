
// editorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "editor.h"
#include "editorDlg.h"
#include "editorParser.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CeditorDlg dialog




CeditorDlg::CeditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CeditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CeditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_SCROLLBAR1, m_ScrollBar);
	//DDX_Control(pDX, IDC_SCROLLBAR2, m_vScb);
	DDX_Control(pDX, IDC_SLIDER1, m_vslider);
}

BEGIN_MESSAGE_MAP(CeditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CeditorDlg::OnBnClickedOk)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDCANCEL, &CeditorDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CeditorDlg message handlers
 PEditorParser cparser = NULL ;
BOOL CeditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	//cparser = CreateEditorParser("java");
	m_vslider.SetRange(0,8000);
	m_vslider.SetPos(0);
	//m_ScrollBar.setscro
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CeditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		CPaintDC dc(this); // device context for painting
		 DrawLines(&dc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CeditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


 
void CeditorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnOK();
	CFileDialog  fdlg(TRUE,0,0,6,L"C Style(*.c;*.cpp;*java;*.h)|*.c;*.cpp;*.java;*.h|C Style(*.h)|*.h|C Style(*.c)|*.c|CPP Style(*.cpp)|*.cpp|Java(*java)|*.java||" );
	fdlg.DoModal();

	CStdioFile fd(fdlg.GetPathName(),CFile::modeRead/*|CFile::typeBinary*/);
   
	if(cparser)
		ReleaseEditorParser(cparser);
    CString str = fdlg.GetFileExt();
	cparser = CreateEditorParser( str.GetBuffer());
	
	//CString str;
	CString strString;
	while(fd.ReadString(strString))
	{
		TRACE(L"%s\n",strString);
	
		AddTextLineToParser(cparser,strString.GetBuffer());
	}

	///*AddTextLineToParser(cparser,strString.GetBuffer());
	ProcessParserLines(cparser);
	/*
	char *pText = new char[fd.GetLength()];
	int nsize =  fd.Read(pText, fd.GetLength());
	
	LoadTextToParser(cparser,pText,nsize);
	delete pText;*/
	

    CDC *pDC =GetDlgItem(IDC_STATIC4)->GetDC();
	 DrawLines(pDC);
	GetDlgItem(IDC_STATIC4)->ReleaseDC(pDC);

	
}


void CeditorDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	TRACE("nPos : %d\n",nPos);
	 CDC *pDC =GetDlgItem(IDC_STATIC4)->GetDC();
	 DrawLines(pDC);
	GetDlgItem(IDC_STATIC4)->ReleaseDC(pDC);
}


void CeditorDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


int CeditorDlg::DrawLines(CDC *pDC)
{
	if( !cparser) return  -1;
	PEditorLine line = cparser->line;
	int n = cparser->nline;
	if(n <= 0) return 0;
	int x = 15,y = 5;
	y -=m_vslider.GetPos();
	//CDC *pDC = GetDC();
	CMemDC dc(*pDC,GetDlgItem(IDC_STATIC4));
	CDC &ddc = dc.GetDC();
	CRect cliRect;
	GetClientRect(&cliRect);
	ddc.FillRect(cliRect,CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH)));
	while( line)
	{
		CString str(line->rawLine);
		ddc.SetTextColor( RGB(0,0,0));
		ddc.TextOut(x,y,str);

		PLineDressUp p = line->cookies;
		while(p)
		{
			int cy = y;
			int cx = x;
			if(p->ntype != 1) // no tab char cookie
			{
			if(p->beginLoc)
				cx += ddc.GetTextExtent(line->rawLine,p->beginLoc).cx;
			ddc.SetTextColor(p->color);
			ddc.TextOut( cx,cy,line->rawLine + p->beginLoc ,p->endLoc - p->beginLoc);
			}
			p = p->next;

		}

		line = line->next;
		y += 15;
	}
	 
	//ReleaseDC(pDC);
	return 0;
}


void CeditorDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(cparser)
		ReleaseEditorParser(cparser);
	CDialog::OnClose();
}
