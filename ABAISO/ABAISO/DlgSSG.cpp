﻿// DlgSSG.cpp: 实现文件
//

#include "pch.h"
#include "ABAISO.h"
#include "DlgSSG.h"
#include "afxdialogex.h"


// CDlgSSG 对话框.

const int ID[][5] = { IDC_CHECK_U1 ,IDC_EDIT_K1 ,IDC_EDIT_C1 ,IDC_CHECK_UN1 ,IDC_BUTTON_UN1,
		IDC_CHECK_U2 ,IDC_EDIT_K2 ,IDC_EDIT_C2 ,IDC_CHECK_UN2 ,IDC_BUTTON_UN2,
		IDC_CHECK_U3 ,IDC_EDIT_K3 ,IDC_EDIT_C3 ,IDC_CHECK_UN3 ,IDC_BUTTON_UN3,
		IDC_CHECK_U4 ,IDC_EDIT_K4 ,IDC_EDIT_C4 ,IDC_CHECK_UN4 ,IDC_BUTTON_UN4,
		IDC_CHECK_U5 ,IDC_EDIT_K5 ,IDC_EDIT_C5 ,IDC_CHECK_UN5 ,IDC_BUTTON_UN5,
		IDC_CHECK_U6 ,IDC_EDIT_K6 ,IDC_EDIT_C6 ,IDC_CHECK_UN6 ,IDC_BUTTON_UN6 };

IMPLEMENT_DYNAMIC(CDlgSSG, CDialogEx)

CDlgSSG::CDlgSSG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SSG, pParent)
{
	//m_bU[0] = m_bU[1] = m_bU[2] = TRUE;
	//m_bU[3] = m_bU[4] = m_bU[5] = FALSE;

	//m_fK[0] = 4.4e6f;
	//m_fK[1] = m_fK[2] = 2460.f;
	//m_fK[3] = m_fK[4] = m_fK[5] =0.f;

	//m_fC[0] = m_fC[3] = m_fC[4] = m_fC[5] = 0.f;
	//m_fC[1] = m_fC[2] = 23.f;

	//m_bUN[0] = m_bUN[1] = m_bUN[2] = TRUE;
	//m_bUN[3] = m_bUN[4] = m_bUN[5] = FALSE;

	//m_fNL[0] = 4.4e6f; //轴向压缩刚度
	//m_fNL[1] = 4.4e5f; //轴向拉伸刚度
	//m_fNL[2] = m_fNL[5] = 18970.f; //切向初始刚度
	//m_fNL[3] = m_fNL[6] = 203.f; //屈服力
	//m_fNL[4] = m_fNL[7] = 0.0769636f; //屈服后刚度比
}

CDlgSSG::~CDlgSSG()
{
}

void CDlgSSG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_ISO.m_sName);

	DDX_Check(pDX, IDC_CHECK_U1, m_ISO.m_bU[0]);
	DDX_Check(pDX, IDC_CHECK_U2, m_ISO.m_bU[1]);
	DDX_Check(pDX, IDC_CHECK_U3, m_ISO.m_bU[2]);
	DDX_Check(pDX, IDC_CHECK_U4, m_ISO.m_bU[3]);
	DDX_Check(pDX, IDC_CHECK_U5, m_ISO.m_bU[4]);
	DDX_Check(pDX, IDC_CHECK_U6, m_ISO.m_bU[5]);

	DDX_Text(pDX, IDC_EDIT_K1, m_ISO.m_fK[0]);
	DDX_Text(pDX, IDC_EDIT_K2, m_ISO.m_fK[1]);
	DDX_Text(pDX, IDC_EDIT_K3, m_ISO.m_fK[2]);
	DDX_Text(pDX, IDC_EDIT_K4, m_ISO.m_fK[3]);
	DDX_Text(pDX, IDC_EDIT_K5, m_ISO.m_fK[4]);
	DDX_Text(pDX, IDC_EDIT_K6, m_ISO.m_fK[5]);

	DDX_Text(pDX, IDC_EDIT_C1, m_ISO.m_fC[0]);
	DDX_Text(pDX, IDC_EDIT_C2, m_ISO.m_fC[1]);
	DDX_Text(pDX, IDC_EDIT_C3, m_ISO.m_fC[2]);
	DDX_Text(pDX, IDC_EDIT_C4, m_ISO.m_fC[3]);
	DDX_Text(pDX, IDC_EDIT_C5, m_ISO.m_fC[4]);
	DDX_Text(pDX, IDC_EDIT_C6, m_ISO.m_fC[5]);

	DDX_Check(pDX, IDC_CHECK_UN1, m_ISO.m_bUN[0]);
	DDX_Check(pDX, IDC_CHECK_UN2, m_ISO.m_bUN[1]);
	DDX_Check(pDX, IDC_CHECK_UN3, m_ISO.m_bUN[2]);
	DDX_Check(pDX, IDC_CHECK_UN4, m_ISO.m_bUN[3]);
	DDX_Check(pDX, IDC_CHECK_UN5, m_ISO.m_bUN[4]);
	DDX_Check(pDX, IDC_CHECK_UN6, m_ISO.m_bUN[5]);
	
}


BEGIN_MESSAGE_MAP(CDlgSSG, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_U1, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_U2, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_U3, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_U4, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_U5, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_U6, &CDlgSSG::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_BUTTON_UN1, &CDlgSSG::OnBnClickedButtonUn1)
	ON_BN_CLICKED(IDC_BUTTON_UN2, &CDlgSSG::OnBnClickedButtonUn2)
	ON_BN_CLICKED(IDC_BUTTON_UN3, &CDlgSSG::OnBnClickedButtonUn3)
END_MESSAGE_MAP()


// CDlgSSG 消息处理程序


BOOL CDlgSSG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgSSG::OnBnClickedCheck()
{
	UpdateData();
	SetData();
}

#include "DlgNL1.h"
void CDlgSSG::OnBnClickedButtonUn1()
{
	CDlgNL1 dlg;
	dlg.m_f1 = m_ISO.m_fNL[0];
	dlg.m_f2 = m_ISO.m_fNL[1];
	if (dlg.DoModal() == IDOK)
	{
		m_ISO.m_fNL[0] = dlg.m_f1;
		m_ISO.m_fNL[1] = dlg.m_f2;
	}
}

#include "DlgNL2.h"
void CDlgSSG::OnBnClickedButtonUn2()
{
	CDlgNL2 dlg;
	dlg.m_f1 = m_ISO.m_fNL[2];
	dlg.m_f2 = m_ISO.m_fNL[3];
	dlg.m_f3 = m_ISO.m_fNL[4];
	if (dlg.DoModal() == IDOK)
	{
		m_ISO.m_fNL[2] = dlg.m_f1;
		m_ISO.m_fNL[3] = dlg.m_f2;
		m_ISO.m_fNL[4] = dlg.m_f3;
	}
}


void CDlgSSG::OnBnClickedButtonUn3()
{
	CDlgNL2 dlg;
	dlg.m_f1 = m_ISO.m_fNL[5];
	dlg.m_f2 = m_ISO.m_fNL[6];
	dlg.m_f3 = m_ISO.m_fNL[7];
	if (dlg.DoModal() == IDOK)
	{
		m_ISO.m_fNL[5] = dlg.m_f1;
		m_ISO.m_fNL[6] = dlg.m_f2;
		m_ISO.m_fNL[7] = dlg.m_f3;
	}
}

void CDlgSSG::SetData()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			GetDlgItem(ID[i][j])->EnableWindow(m_bU[i]);
		}
	}
}