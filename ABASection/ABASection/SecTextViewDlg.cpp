// SecTextViewDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ABASection.h"
#include "SecTextViewDlg.h"
#include "afxdialogex.h"


// CSecTextViewDlg 对话框

IMPLEMENT_DYNAMIC(CSecTextViewDlg, CDialogEx)

CSecTextViewDlg::CSecTextViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALO_SECVIEW, pParent)
	, m_sText(_T(""))
{

}

CSecTextViewDlg::~CSecTextViewDlg()
{
}

void CSecTextViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sText);
}


BEGIN_MESSAGE_MAP(CSecTextViewDlg, CDialogEx)
END_MESSAGE_MAP()


// CSecTextViewDlg 消息处理程序
