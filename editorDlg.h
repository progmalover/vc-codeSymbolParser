
// editorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CeditorDlg dialog
class CeditorDlg : public CDialog
{
// Construction
public:
	CeditorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	CScrollBar m_ScrollBar;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCancel();
	CScrollBar m_vScb;
	CSliderCtrl m_vslider;
	int DrawLines(CDC *pDC);
	afx_msg void OnClose();
};
