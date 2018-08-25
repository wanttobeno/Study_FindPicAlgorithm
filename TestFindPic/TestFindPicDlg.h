
// TestFindPicDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "PicDlg.h"

// CTestFindPicDlg dialog
class CTestFindPicDlg : public CDialog
{
// Construction
public:
	CTestFindPicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTFINDPIC_DIALOG };

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
	afx_msg void OnBnClickedBtnFindPic();
	CStatic m_pTxtPos;
	CPicDlg  m_picDlg;
	afx_msg void OnBnClickedBtnFindDesk();

	void FindPicTest(HWND hWnd,LPCTSTR pFindFile);

};
