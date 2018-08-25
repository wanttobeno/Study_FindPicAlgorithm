// PicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestFindPic.h"
#include "PicDlg.h"


// CPicDlg dialog

IMPLEMENT_DYNAMIC(CPicDlg, CDialog)

CPicDlg::CPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicDlg::IDD, pParent)
{

}

CPicDlg::~CPicDlg()
{
}

void CPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicDlg, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CPicDlg message handlers

BOOL CPicDlg::OnEraseBkgnd(CDC* pDC)
{
	//return CDialog::OnEraseBkgnd(pDC);
	CBrush backBrush(0xff8080);
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

LRESULT CPicDlg::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	return HTCAPTION;
	//return CDialog::OnNcHitTest(point);
}
