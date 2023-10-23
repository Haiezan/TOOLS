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
	, m_bHard1(FALSE)
	, m_bHard2(FALSE)
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
	DDX_Check(pDX, IDC_CHECK_F1, m_bU[0]);
	DDX_Check(pDX, IDC_CHECK_F2, m_bU[1]);
	DDX_Check(pDX, IDC_CHECK_F3, m_bU[2]);
	DDX_Check(pDX, IDC_CHECK_HARD1, m_bHard1);
	DDX_Check(pDX, IDC_CHECK_HARD2, m_bHard2);
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
	//if (m_ISO.IsDamping()) m_cList.AddString(L"Damping");
	m_cList.SetCurSel(0);
}

void CDlgABA::OnLbnSelchangeList1()
{
	int iSel = m_cList.GetCurSel();
	m_cList.GetText(iSel, m_sNote);

	//清空表格数据
	m_cListData.DeleteAllItems();
	for (int i = 0; i < 4; i++) m_cListData.DeleteColumn(0);
	//初始化列表样式。参数为：整行选择、网格线
	m_cListData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//弹性参数
	if (m_sNote == L"Elasticity")
	{
		m_bLinear = TRUE;
		memcpy(m_bU, m_ISO.m_bU, 6 * sizeof(BOOL));

		//建立表头，即为每列起名字。注意列是从1开始
		m_cListData.InsertColumn(1, L"", LVCFMT_CENTER, 30);
		m_cListData.InsertColumn(2, L"D11", LVCFMT_CENTER, 50);
		m_cListData.InsertColumn(3, L"D22", LVCFMT_CENTER, 50);
		m_cListData.InsertColumn(4, L"D33", LVCFMT_CENTER, 100);

		//先创建行，再为改行每列赋值。注意列是从0开始
		CString str;
		m_cListData.InsertItem(0, L"1");
		//m_cListData.SetItemText(0, 0, L"1");
		str.Format(L"%g", m_ISO.m_fK[1]);
		m_cListData.SetItemText(0, 1, str);
		str.Format(L"%g", m_ISO.m_fK[2]);
		m_cListData.SetItemText(0, 2, str);
		str.Format(L"%g", m_ISO.m_fK[0]);
		m_cListData.SetItemText(0, 3, str);
	}
	else if (m_sNote == L"Plasticity")
	{
		m_bLinear = FALSE;
		memset(m_bU, 0, 6 * sizeof(BOOL));
		m_bU[0] = m_ISO.m_bUN[1];
		m_bU[1] = m_ISO.m_bUN[2];

		m_bHard1 = FALSE;
		m_bHard2 = TRUE;

		float K0 = m_ISO.m_fNL[2]; //初始刚度
		float Fb0 = m_ISO.m_fNL[3]; //屈服力
		float B0 = m_ISO.m_fNL[4]; //屈服后刚度比

		float Fb1 = (1.0 - Fb0 / K0) * K0 * B0 + Fb0; //应变为1时对应内力
		float PlasticStrain = 1.f - Fb1 / K0; //应变为1时对应塑性应变

		//建立表头，即为每列起名字。注意列是从1开始
		m_cListData.InsertColumn(1, L"", LVCFMT_CENTER, 30);
		m_cListData.InsertColumn(2, L"Yield Force//Moment", LVCFMT_CENTER, 100);
		m_cListData.InsertColumn(3, L"Plastic Motion", LVCFMT_CENTER, 100);
		//m_cListData.InsertColumn(4, L"Rate", LVCFMT_CENTER, 100);

		//先创建行，再为改行每列赋值。注意列是从0开始
		CString str;
		m_cListData.InsertItem(0, L"1");
		str.Format(L"%g", Fb0);
		m_cListData.SetItemText(0, 1, str);
		m_cListData.SetItemText(0, 2, L"0.0");

		m_cListData.InsertItem(1, L"2");
		str.Format(L"%g", Fb1);
		m_cListData.SetItemText(1, 1, str);
		str.Format(L"%g", PlasticStrain);
		m_cListData.SetItemText(1, 2, str);
	}
	UpdateData(FALSE);
}
