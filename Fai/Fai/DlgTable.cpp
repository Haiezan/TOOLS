// DlgTable.cpp: 实现文件
//

#include "stdafx.h"
#include "Fai.h"
#include "DlgTable.h"
#include "afxdialogex.h"
#include "function.h"


// CDlgTable 对话框

IMPLEMENT_DYNAMIC(CDlgTable, CDialogEx)

CDlgTable::CDlgTable(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TABLE, pParent)
{

}

CDlgTable::~CDlgTable()
{
}

void CDlgTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combox);
	DDX_Control(pDX, IDC_LIST_TABLE, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgTable, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgTable::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlgTable 消息处理程序


BOOL CDlgTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Combox.AddString(_T("a类"));
	m_Combox.AddString(_T("b类"));
	m_Combox.AddString(_T("c类"));
	m_Combox.AddString(_T("d类"));
	m_Combox.SetCurSel(0);

	InitialTable();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgTable::InitialTable()
{
	int iCatg = m_Combox.GetCurSel();
	char cCatg = 'a' + iCatg;

	CString sText;
	switch (iCatg)
	{
	case 0:
		sText = "表D．0．1 a类截面轴心受压构件的稳定系数φ";
		break;
	case 1:
		sText = "表D．0．2 b类截面轴心受压构件的稳定系数φ";
		break;
	case 2:
		sText = "表D．0．3 c类截面轴心受压构件的稳定系数φ";
		break;
	case 3:
		sText = "表D．0．4 d类截面轴心受压构件的稳定系数φ";
		break;
	default:
		break;
	}
	GetDlgItem(IDC_STATIC)->SetWindowText(sText);

	float X[10] = { 0,1,2,3,4,5,6,7,8,9 };
	float Y[25] = { 0,10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200,210,220,230,240 };

	m_ListCtrl.DeleteAllItems();
	int nIndex = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	if (nIndex < 1)
	{
		m_ListCtrl.InsertColumn(0, _T("λ/εk"), LVCFMT_CENTER, 400, -1);
		for (int i = 0; i < 10; i++)
		{
			CString str;
			str.Format(L"%.0f", X[i]);
			m_ListCtrl.InsertColumn(i + 1, str, LVCFMT_CENTER, 400, -1);
		}
	}

	
	for (int i = 0; i < 25; i++)
	{
		CString str1,str2;
		str1.Format(L"%.0f", Y[i]);
		m_ListCtrl.InsertItem(i, str1);

		for (int j = 0; j < 10; j++)
		{
			float fcxb2etak = X[j] + Y[i];
			float fai = GetFai(iCatg, fcxb2etak);
			str2.Format(L"%.3f", fai);
			m_ListCtrl.SetItemText(i, j + 1, str2);
		}
	}


	m_ListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_AUTOSIZECOLUMNS);

}

void CDlgTable::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	InitialTable();
}
