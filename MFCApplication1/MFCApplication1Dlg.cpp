
// MFCApplication1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
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


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序
#include "3DSurfaceDlg.h"
BOOL CMFCApplication1Dlg::OnInitDialog()
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
	LoadPointDataFromFile(L"D:\\Github\\TOOLS\\MFCApplication1\\B8527\\PMM_RD.dat");
	C3DSurfaceDlg dlg;
	dlg.m_glControl.m_spherePoints = m_pointCloud;
	dlg.DoModal();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
// 从文本文件加载点数据
bool CMFCApplication1Dlg::LoadPointDataFromFile(const std::wstring& filename) {
	// 清空现有数据
	m_pointCloud.clear();
	m_pointCloud.resize(36);
	// 打开文件
	std::wifstream file(filename);
	if (!file.is_open()) {
		AfxMessageBox(_T("无法打开文件"));
		return false;
	}

	// 设置UTF-8本地化（支持中文路径）
	//std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	//file.imbue(utf8_locale);

	std::wstring line;
	int lineNum = 0;
	std::getline(file, line);

	while (std::getline(file, line)) {
		lineNum++;

		// 跳过空行和注释行（以#开头）
		if (line.empty() || line[0] == L'#') {
			continue;
		}

		std::wistringstream iss(line);
		Point3D point;
		wchar_t sep;

		// 尝试读取三种常见格式：
		// 1. x y z
		// 2. x,y,z
		// 3. x; y; z

		// 尝试空格分隔
		for (int ii = 0; ii < 36; ii++)
		{
			if (iss >> point.x >> point.y >> point.z) {
				m_pointCloud[ii].push_back(point);
			}
		}
		continue;

		// 清除错误状态
		iss.clear();
		iss.seekg(0);

		// 尝试逗号分隔
		if (iss >> point.x >> sep >> point.y >> sep >> point.z && (sep == L',' || sep == L';')) {
			m_pointCloud[0].push_back(point);
			continue;
		}

		// 如果都不成功，报告错误行
		CString msg;
		msg.Format(_T("第%d行数据格式错误: %s"), lineNum, line.c_str());
		AfxMessageBox(msg);
	}

	file.close();

	if (m_pointCloud.empty()) {
		AfxMessageBox(_T("文件未包含有效数据"));
		return false;
	}


	return true;
}