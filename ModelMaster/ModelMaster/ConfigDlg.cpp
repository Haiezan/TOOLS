// ConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ModelMaster.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"
#include "Browse.h"


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CONFIG, pParent)
	, m_sSausg(_T(""))
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAUSG, m_sSausg);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


void CConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CConfigDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_sSausg = BrowseFile();
	UpdateData(FALSE);
}