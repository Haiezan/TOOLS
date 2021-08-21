
// ModelMasterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ModelMaster.h"
#include "ModelMasterDlg.h"
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


// CModelMasterDlg 对话框



CModelMasterDlg::CModelMasterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELMASTER_DIALOG, pParent)
	, m_sPath(_T("D:\\04 Support"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModelMasterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_sPath);
	DDX_Control(pDX, IDC_TREE1, m_cTree);
}

BEGIN_MESSAGE_MAP(CModelMasterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CModelMasterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CModelMasterDlg 消息处理程序

BOOL CModelMasterDlg::OnInitDialog()
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

void CModelMasterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CModelMasterDlg::OnPaint()
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
HCURSOR CModelMasterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CModelMasterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	ScanFile(m_sPath);
	ShowTree();

}

void CModelMasterDlg::ScanFile(CString Dir)
{
	CFileFind finder;
	CString Add = L"\\*";
	CString DirSpec = Dir + Add;                        //补全要遍历的文件夹的目录
	BOOL bWorking = finder.FindFile(DirSpec);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDots())              //扫描到的不是节点
		{
			if (finder.IsDirectory())           //扫描到的是文件夹
			{
				CString strDirectory = finder.GetFilePath();
				ScanFile(strDirectory);           //递归调用ScanFile（）
			}
			else                               //扫描到的是文件
			{
				//CString strFile = finder.GetFilePath();     //得到文件的全路径

				FileInfo file;
				file.FileName = finder.GetFileName();
				file.FilePath = finder.GetFilePath();
				file.FileTitle = finder.GetFileTitle();
				file.FileURL = finder.GetFileURL();
				file.Root = finder.GetRoot();

				CString str = L".ssg";
				if (file.FilePath.Find(str) >= 0)
				{
					AddModel(file);
				}
			}
		}
	}
	finder.Close();
}

void CModelMasterDlg::AddModel(FileInfo file)
{
	CString sFile = file.FilePath;
	CString str = sFile.Right(sFile.GetLength() - m_sPath.GetLength() - 1);
	CString sModelName = str.Left(str.Find('\\'));
	CString sModelPath = m_sPath + "\\" + sModelName;

	CString sFileName = sFile.Right(sFile.Find('\\'));

	for (int i = 0; i < m_ProjectList.size(); i++)
	{
		if (sModelName.Compare(m_ProjectList[i].sName) == 0)
		{
			m_ProjectList[i].FileList.push_back(file);
			return;
		}
	}

	Project project;
	project.sName = sModelName;
	project.sPath = sModelPath;
	project.FileList.push_back(file);
	m_ProjectList.push_back(project);
}

void CModelMasterDlg::ShowTree()
{
	if (m_ProjectList.size() < 1) return;

	HTREEITEM hRoot;     // 树的根节点的句柄   
	HTREEITEM hCataItem; // 可表示任一分类节点的句柄

	for (int m = 0; m < m_ProjectList.size(); m++)
	{
		//switch (m_PmmData[i].MatType)
		Project project = m_ProjectList[m];
		hRoot = m_cTree.InsertItem(project.sName, TVI_ROOT);

		for (int i = 0; i < project.FileList.size(); i++)
		{
			FileInfo file = project.FileList[i];
			CString str = file.FileName;

			hCataItem = m_cTree.InsertItem(str, hRoot, TVI_LAST);

			m_cTree.SetCheck(hCataItem, FALSE);
			BOOL bCheck = m_cTree.GetCheck(hCataItem);
		}

		//展开树状菜单
		m_cTree.Expand(hRoot, TVE_EXPAND);
	}
}