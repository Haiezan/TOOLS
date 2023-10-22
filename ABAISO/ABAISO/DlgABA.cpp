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
	DDX_Control(pDX, IDC_LIST2, m_cListData);
}


BEGIN_MESSAGE_MAP(CDlgABA, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgABA::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CDlgABA 消息处理程序


BOOL CDlgABA::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetData();
	

	OnLbnSelchangeList1();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgABA::SetData()
{
	m_cList.ResetContent();
	m_cList.AddString(L"Elasticity");

	if (m_ISO.IsPlastic()) m_cList.AddString(L"Plasticity");
	if (m_ISO.IsDamping()) m_cList.AddString(L"Damping");
	m_cList.SetCurSel(0);
}

void CDlgABA::OnLbnSelchangeList1()
{
	int iSel = m_cList.GetCurSel();
	m_cList.GetText(iSel, m_sNote);

	m_cListData.DeleteAllItems();
	m_cListData.DeleteColumn(1);
	m_cListData.DeleteColumn(2);
	m_cListData.DeleteColumn(3);
	m_cListData.DeleteColumn(4);
	//int aa = m_cListData.GetItemCount();
	/*for (int i = 0; i < m_cListData.GetItemCount(); i++)
	{

	}*/

	//m_cListData.ModifyStyle(0, LVS_REPORT);               // 报表模式  
	//m_cListData.SetExtendedStyle(m_cListData.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	//1、初始化列表样式。参数为：整行选择、网格线
	m_cListData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//2、建立表头，即为每列起名字。注意列是从1开始
	m_cListData.InsertColumn(1, L"", LVCFMT_CENTER, 50);
	m_cListData.InsertColumn(2, L"D11", LVCFMT_CENTER, 50);
	m_cListData.InsertColumn(3, L"D22", LVCFMT_CENTER, 50);
	m_cListData.InsertColumn(4, L"D33", LVCFMT_CENTER, 50);

	//3、先创建行，再为改行每列赋值。注意列是从0开始
	m_cListData.InsertItem(0, L"1");
	//m_cListData.SetItemText(0, 0, L"1");
	m_cListData.SetItemText(0, 1, L"0.3");
	m_cListData.SetItemText(0, 2, L"0.4");
	m_cListData.SetItemText(0, 3, L"0.4");

	UpdateData(FALSE);
}
