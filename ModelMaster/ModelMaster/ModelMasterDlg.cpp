
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
	, m_sTargetExt(_T(".ssg,.SSG,.yjk,.jws,.JWS,.rar,.zip"))
	, m_iFileTime(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CModelMasterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATH, m_sPath);
	DDX_Control(pDX, IDC_TREE1, m_cTree);
	DDX_Text(pDX, IDC_EDIT_EXT, m_sTargetExt);
	DDX_Radio(pDX, IDC_RADIO1, m_iFileTime);
}

BEGIN_MESSAGE_MAP(CModelMasterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CModelMasterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_BACKUP, &CModelMasterDlg::OnBnClickedButtonBackup)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CModelMasterDlg::OnBnClickedButtonPath)
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
	m_ProjectList.clear();
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
				FileInfo file;  //储存文件信息
				file.Length = finder.GetLength();
				file.FileName = finder.GetFileName();
				file.FilePath = finder.GetFilePath();
				file.FileTitle = finder.GetFileTitle();
				file.FileURL = finder.GetFileURL();
				file.Root = finder.GetRoot();
				
				finder.GetLastWriteTime(file.LastWriteTime);
				finder.GetLastAccessTime(file.LastAccessTime);
				finder.GetCreationTime(file.CreationTime);

				//目标文件后缀
				CStringArray szList;
				CString szSplit = _T(",");
				int n = SplitString(m_sTargetExt, szSplit, szList, FALSE);
				for (int i = 0; i < n; i++)
				{
					CString str = szList.GetAt(i);
					if (file.FilePath.Find(str) >= 0)
					{
						AddModel(file);
					}
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
			m_ProjectList[i].InsertFile(file);
			return;
		}
	}

	Project project;
	project.sName = sModelName;
	project.sPath = sModelPath;
	project.InsertFile(file);
	m_ProjectList.push_back(project);
	return;
}

void CModelMasterDlg::ShowTree()
{
	m_cTree.DeleteAllItems();

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
		//m_cTree.Expand(hRoot, TVE_EXPAND);
	}
}

int CModelMasterDlg::SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString, BOOL bAllowNullString)
{
	rArrString.RemoveAll();
	CString szStr = lpszStr;
	szStr.TrimLeft();
	szStr.TrimRight();
	if (szStr.GetLength() == 0)
	{
		return 0;
	}
	CString szSplit = lpszSplit;
	if (szSplit.GetLength() == 0)
	{
		rArrString.Add(szStr);
		return 1;
	}
	CString s;
	int n;
	do {
		n = szStr.Find(szSplit);
		if (n > 0)
		{
			rArrString.Add(szStr.Left(n));
			szStr = szStr.Right(szStr.GetLength() - n - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else if (n == 0)
		{
			if (bAllowNullString)
				rArrString.Add(_T(""));
			szStr = szStr.Right(szStr.GetLength() - szSplit.GetLength());
			szStr.TrimLeft();
		}
		else
		{
			if ((szStr.GetLength() > 0) || bAllowNullString)
				rArrString.Add(szStr);
			break;
		}
	} while (1);
	return rArrString.GetSize();
}

void CModelMasterDlg::OnBnClickedButtonBackup()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CString path;

	// 获取特定文件夹的LPITEMIDLIST，可以将之理解为HANDLE
	// 所谓的特定文件夹,你可以用CSIDL_XXX来检索之。
	LPITEMIDLIST rootLoation;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &rootLoation);
	if (rootLoation == NULL) {
		// unkown error
		return;
	}
	// 配置对话框
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.pidlRoot = rootLoation; // 文件夹对话框之根目录，不指定的话则为我的电脑
	bi.lpszTitle = _T("选择备份目录"); // 可以不指定
	// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	// 打开对话框, 有点像DoModal
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);
	if (targetLocation != NULL) {
		TCHAR targetPath[MAX_PATH];
		SHGetPathFromIDList(targetLocation, targetPath);

		path = targetPath;
		CTime tm = CTime::GetCurrentTime();
		CString stm;
		stm.Format(_T("%04d%02d%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay());

		path = path + "\\Backup" + stm;

		CString str = L"选择备份目录为:" + path;
		MessageBox(str,L"备份目录");

		if (!PathIsDirectory(path)) CreateDirectory(path, 0);
		
		//备份项目
		for each (Project project in m_ProjectList)
		{
			CString dir = path + "\\" + project.sName;

			if (!PathIsDirectory(dir))
			{
				CreateDirectory(dir, 0);//不存在则创建
			}
			for each (FileInfo file in project.FileList)
			{
				CString target = dir + "\\" + file.FileName;
				bool m = CopyFile(file.FilePath, target, m_iFileTime); //覆盖重复文件
			}
		}
	}
}


void CModelMasterDlg::OnBnClickedButtonPath()
{
	// TODO: 在此添加控件通知处理程序代码

	// 获取特定文件夹的LPITEMIDLIST，可以将之理解为HANDLE
	// 所谓的特定文件夹,你可以用CSIDL_XXX来检索之。
	LPITEMIDLIST rootLoation;
	SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &rootLoation);
	if (rootLoation == NULL) {
		// unkown error
		return;
	}
	// 配置对话框
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.pidlRoot = rootLoation; // 文件夹对话框之根目录，不指定的话则为我的电脑
	bi.lpszTitle = _T("选择备份目录"); // 可以不指定
	// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	// 打开对话框, 有点像DoModal
	LPITEMIDLIST targetLocation = SHBrowseForFolder(&bi);
	if (targetLocation != NULL) {
		TCHAR targetPath[MAX_PATH];
		SHGetPathFromIDList(targetLocation, targetPath);

		m_sPath = targetPath;
		UpdateData(FALSE);
	}
}
