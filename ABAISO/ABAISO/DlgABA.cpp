// DlgABA.cpp: 实现文件
//

#include "pch.h"
#include "ABAISO.h"
#include "DlgABA.h"
#include "afxdialogex.h"


// CDlgABA 对话框

IMPLEMENT_DYNAMIC(CDlgABA, CDialogEx)

CDlgABA::CDlgABA(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ABA, pParent)
	, m_sNote(_T(""))
	, m_bLinear(FALSE)
{

}

CDlgABA::~CDlgABA()
{
}

void CDlgABA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_cList);
	DDX_Text(pDX, IDC_STATIC_NOTE, m_sNote);
	DDX_Radio(pDX, IDC_RADIO_LINEAR, m_bLinear);
}


BEGIN_MESSAGE_MAP(CDlgABA, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgABA::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CDlgABA 消息处理程序


BOOL CDlgABA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cList.AddString(L"Elasticity");
	m_cList.AddString(L"Plasticity");
	m_cList.AddString(L"Damping");
	m_cList.SetCurSel(0);

	OnLbnSelchangeList1();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgABA::OnLbnSelchangeList1()
{
	int iSel = m_cList.GetCurSel();
	m_cList.GetText(iSel, m_sNote);

	UpdateData(FALSE);
}
