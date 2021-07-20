
// FaiDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Fai.h"
#include "FaiDlg.h"
#include "afxdialogex.h"
#include "function.h"

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


// CFaiDlg 对话框



CFaiDlg::CFaiDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FAI_DIALOG, pParent)
	, m_fCxb(100.f)
	, m_fFy(345.f)
	, m_sMsg(_T(""))
	, m_fE(206000.f)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//更改应用程序图标
	HICON ico = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//SetIcon(ico, TRUE);//设置大图标
	//SetIcon(ico, FALSE);//设置小图标
}

void CFaiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CATG, m_cComboxCatg);
	DDX_Text(pDX, IDC_EDIT_CXB, m_fCxb);
	DDV_MinMaxFloat(pDX, m_fCxb, 0, FLT_MAX);
	DDX_Text(pDX, IDC_EDIT_FY, m_fFy);
	DDV_MinMaxFloat(pDX, m_fFy, 0, FLT_MAX);
	DDX_Text(pDX, IDC_EDIT_MSG, m_sMsg);
	DDX_Text(pDX, IDC_EDIT_E, m_fE);
	DDV_MinMaxFloat(pDX, m_fE, 0, FLT_MAX);
}

BEGIN_MESSAGE_MAP(CFaiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFaiDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_CATG, &CFaiDlg::OnCbnSelchangeComboCatg)
	ON_BN_CLICKED(IDC_BUTTON_TABLE, &CFaiDlg::OnBnClickedButtonTable)
END_MESSAGE_MAP()


// CFaiDlg 消息处理程序

BOOL CFaiDlg::OnInitDialog()
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
	m_cComboxCatg.AddString(_T("a类"));
	m_cComboxCatg.AddString(_T("b类"));
	m_cComboxCatg.AddString(_T("c类"));
	m_cComboxCatg.AddString(_T("d类"));
	m_cComboxCatg.SetCurSel(0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFaiDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFaiDlg::OnPaint()
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
HCURSOR CFaiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFaiDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	
	m_fFai = GetFai(m_iCatg, m_fCxb, m_fFy, m_fE);
	
	CString strTime = GetTime();
	CString str;
	float lagdn = m_fCxb / sqrt(235.f / m_fFy);
	str.Format(_T("[%s] Q%.0f\tλ=%.0f\tλ/εk=%.0f\r\nφ=%5.3f\r\n"), strTime, m_fFy, m_fCxb, lagdn, m_fFai);
	
	m_sMsg += str;

	UpdateData(FALSE);
}

void CFaiDlg::OnCbnSelchangeComboCatg()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCatg = m_cComboxCatg.GetCurSel();
	m_cCatg = 'a' + m_iCatg;
}

CString CFaiDlg::GetTime()
{
	CTime time = CTime::GetCurrentTime();   ///构造CTime对象
	int m_nYear = time.GetYear();      ///年
	int m_nMonth = time.GetMonth();      ///月
	int m_nDay = time.GetDay();      ///日
	int m_nHour = time.GetHour();      ///小时
	int m_nMinute = time.GetMinute();   ///分钟
	int m_nSecond = time.GetSecond();   ///秒

	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");
	return m_strTime;
}

#include "DlgTable.h"
void CFaiDlg::OnBnClickedButtonTable()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgTable cDlg;
	cDlg.DoModal();
}
