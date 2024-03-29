﻿
// ABAISODlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ABAISO.h"
#include "ABAISODlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CABAISODlg 对话框



CABAISODlg::CABAISODlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABAISO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pDlgSSG = NULL;
}
CABAISODlg::~CABAISODlg()
{
	if (m_pDlgSSG != NULL) delete m_pDlgSSG;
	if (m_pDlgABA != NULL) delete m_pDlgABA;
}

void CABAISODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_cTab);
	DDX_Control(pDX, IDC_STATIC_PANEL, m_cGroup);
}

BEGIN_MESSAGE_MAP(CABAISODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CABAISODlg::OnTcnSelchangeTab)
END_MESSAGE_MAP()


// CABAISODlg 消息处理程序

BOOL CABAISODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	/*m_cPanel.GetWindowRect(&m_cRect);
	ScreenToClient(&m_cRect);

	GetWindowRect(&m_cDlg);
	ScreenToClient(&m_cDlg);*/

	CRect rect;
	GetDlgItem(IDC_STATIC_PANEL)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_cTab.InsertItem(0, L"SAUSG");
	m_cTab.InsertItem(1, L"ABAQUS");
	m_cTab.SetCurSel(1);

	if (m_pDlgSSG == NULL)
	{
		m_pDlgSSG = new CDlgSSG();
		m_pDlgSSG->Create(IDD_DIALOG_SSG, this);
		m_pDlgSSG->m_ISO = m_ISO;
		
		CRect rectDlg;
		m_pDlgSSG->GetWindowRect(&rectDlg);
		m_pDlgSSG->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}
	if (m_pDlgABA == NULL)
	{
		m_pDlgABA = new CDlgABA();
		m_pDlgABA->Create(IDD_DIALOG_ABA, this);
		m_pDlgABA->m_ISO = m_ISO;

		CRect rectDlg;
		m_pDlgABA->GetWindowRect(&rectDlg);
		m_pDlgABA->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
	}
	ViewDlg();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CABAISODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CABAISODlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CABAISODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CABAISODlg::OnTcnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)
{
	ViewDlg();
	*pResult = 0;
}
void CABAISODlg::ViewDlg()
{
	int iSel = m_cTab.GetCurSel();
	if (iSel == 0)
	{
		m_pDlgSSG->ShowWindow(SW_SHOW);
		m_pDlgABA->ShowWindow(SW_HIDE);
	}
	else if (iSel == 1)
	{
		m_pDlgSSG->UpdateData();
		m_pDlgSSG->ShowWindow(SW_HIDE);

		m_pDlgABA->m_ISO = m_ISO = m_pDlgSSG->m_ISO;
		m_pDlgABA->OnInitDialog();
		m_pDlgABA->ShowWindow(SW_SHOW);
	}
}
