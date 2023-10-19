// DlgNL2.cpp: 实现文件
//

#include "pch.h"
#include "ABAISO.h"
#include "DlgNL2.h"
#include "afxdialogex.h"


// CDlgNL2 对话框

IMPLEMENT_DYNAMIC(CDlgNL2, CDialogEx)

CDlgNL2::CDlgNL2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NL2, pParent)
	, m_f1(0)
{

}

CDlgNL2::~CDlgNL2()
{
}

void CDlgNL2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_f1);
	DDX_Text(pDX, IDC_EDIT2, m_f2);
	DDX_Text(pDX, IDC_EDIT3, m_f3);
}


BEGIN_MESSAGE_MAP(CDlgNL2, CDialogEx)
END_MESSAGE_MAP()


// CDlgNL2 消息处理程序


BOOL CDlgNL2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
