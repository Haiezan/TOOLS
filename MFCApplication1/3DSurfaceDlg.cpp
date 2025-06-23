// 3DSurfaceDlg.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication1.h"
#include "3DSurfaceDlg.h"
#include "afxdialogex.h"


// C3DSurfaceDlg 对话框

IMPLEMENT_DYNAMIC(C3DSurfaceDlg, CDialogEx)

C3DSurfaceDlg::C3DSurfaceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_3DSURFACE_DIALOG, pParent)
{

}

C3DSurfaceDlg::~C3DSurfaceDlg()
{
}

void C3DSurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C3DSurfaceDlg, CDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL C3DSurfaceDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    CRect rect;
    GetClientRect(&rect);
    rect.DeflateRect(10, 10);
    m_glControl.Create(rect, this);
    m_glControl.InitOpenGL();

    return TRUE;
}

void C3DSurfaceDlg::OnSize(UINT nType, int cx, int cy) {
    CDialogEx::OnSize(nType, cx, cy);

    if (m_glControl.GetSafeHwnd()) {
        CRect rect;
        GetClientRect(&rect);
        rect.DeflateRect(10, 10);
        m_glControl.MoveWindow(rect);
    }
}
