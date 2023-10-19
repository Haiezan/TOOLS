// CDlgNL1.cpp: 实现文件
//

#include "pch.h"
#include "ABAISO.h"
#include "DlgNL1.h"
#include "afxdialogex.h"


// CDlgNL1 对话框

IMPLEMENT_DYNAMIC(CDlgNL1, CDialogEx)

CDlgNL1::CDlgNL1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NL1, pParent)
	, m_f1(0)
	, m_f2(0)
{

}

CDlgNL1::~CDlgNL1()
{
}

void CDlgNL1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_f1);
	DDX_Text(pDX, IDC_EDIT2, m_f2);
}


BEGIN_MESSAGE_MAP(CDlgNL1, CDialogEx)
END_MESSAGE_MAP()


// CDlgNL1 消息处理程序


BOOL CDlgNL1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
