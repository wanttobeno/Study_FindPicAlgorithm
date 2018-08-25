#pragma once


// CPicDlg dialog

class CPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CPicDlg)

public:
	CPicDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPicDlg();

// Dialog Data
	enum { IDD = IDD_DLG_PIC };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
