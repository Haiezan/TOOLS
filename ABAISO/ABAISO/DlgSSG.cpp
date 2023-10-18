// DlgSSG.cpp: 实现文件
//

#include "pch.h"
#include "ABAISO.h"
#include "DlgSSG.h"
#include "afxdialogex.h"


// CDlgSSG 对话框

IMPLEMENT_DYNAMIC(CDlgSSG, CDialogEx)

CDlgSSG::CDlgSSG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SSG, pParent)
	, m_bU1(FALSE)
	, m_fK1(0)
	, m_fC1(0)
{

}

CDlgSSG::~CDlgSSG()
{
}

void CDlgSSG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_U1, m_bU1);
	DDX_Text(pDX, IDC_EDIT_K1, m_fK1);
	DDX_Text(pDX, IDC_EDIT_C1, m_fC1);
}


BEGIN_MESSAGE_MAP(CDlgSSG, CDialogEx)
END_MESSAGE_MAP()


// CDlgSSG 消息处理程序
