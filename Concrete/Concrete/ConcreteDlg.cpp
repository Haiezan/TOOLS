
// ConcreteDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Concrete.h"
#include "ConcreteDlg.h"
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


// CConcreteDlg 对话框



CConcreteDlg::CConcreteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CONCRETE_DIALOG, pParent)
	, m_fFc(20.10)
	, m_fEc(3.0e4)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConcreteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FC, m_fFc);
	DDV_MinMaxFloat(pDX, m_fFc, 0, FLT_MAX);
	DDX_Text(pDX, IDC_EDIT_EC, m_fEc);
	DDV_MinMaxFloat(pDX, m_fEc, 0, FLT_MAX);
	DDX_Control(pDX, IDC_CUSTOM1, m_ChartCtrl);
}

BEGIN_MESSAGE_MAP(CConcreteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLOT, &CConcreteDlg::OnBnClickedButtonPlot)
END_MESSAGE_MAP()


// CConcreteDlg 消息处理程序

BOOL CConcreteDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConcreteDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConcreteDlg::OnPaint()
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
HCURSOR CConcreteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

double CConcreteDlg::GetDc(double e, double Ec, double fcr, double ecr, double ac)
{
	double dc = 0.f;

	double pc = fcr / Ec / ecr;
	double n = Ec * ecr / (Ec*ecr - fcr);
	double x = e / ecr;

	if (x <= 1.f)
	{
		dc = 1 - pc * n / (n - 1 + pow(x, n));
	}
	else
	{
		dc = 1 - pc / (ac*(x - 1)*(x - 1) + x);
	}
	return dc;
}
double CConcreteDlg::GetSigmaC(double e, double dc, double Ec)
{
	double sigma = (1 - dc)*Ec*e;
	return sigma;
}

void CConcreteDlg::OnBnClickedButtonPlot()
{
	// TODO: 在此添加控件通知处理程序代码
	int N = 100;
	double de = 0.0001f;
	double ee = 0, ss = 0;

	double* e = new double[N];
	double* s = new double[N];


	for (int i = 0; i < N; i++)
	{
		ee = i * de;
		double dc = GetDc(ee, m_fEc, m_fFc, ecr, ac);
		ss = GetSigmaC(ee, dc, m_fEc);

		e[i] = ee;
		s[i] = ss;
	}

	//清空曲线
	m_ChartCtrl.RemoveAllSeries();
	m_ChartCtrl.GetTitle()->RemoveAll();

	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//背景颜色
	m_ChartCtrl.SetBackColor(RGB(255, 255, 255));

	//图标标题
	//m_ChartCtrl.GetTitle()->AddString(_T("P-M1"));

	//坐标轴标题
	m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText(_T("e"));
	m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText(_T("sigma"));

	CChartLineSerie* pLineSerie = m_ChartCtrl.CreateLineSerie();
	pLineSerie->SetSeriesOrdering(poNoOrdering);//设置为无序
	pLineSerie->SetPoints(e, s, N);
	pLineSerie->SetWidth(1);
	pLineSerie->SetColor(RGB(0, 255, 0));
	//pLineSerie->SetName(name);

	//pPMx1->uSerieId = pLineSerie1->GetSerieId();
	m_ChartCtrl.EnableRefresh(true);

	delete[]e, s;

}
