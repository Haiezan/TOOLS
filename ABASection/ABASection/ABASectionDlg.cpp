
// ABASectionDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ABASection.h"
#include "ABASectionDlg.h"
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


// CABASectionDlg 对话框



CABASectionDlg::CABASectionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABASECTION_DIALOG, pParent)
	, m_sSetName(_T("CS0+600X300X600X300-Q235"))
	, m_sMatName(_T("Q235"))
	, m_fAngle(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CABASectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAR, m_cListPar);
	DDX_Control(pDX, IDC_EDIT0, m_cEdit);
	DDX_Text(pDX, IDC_EDIT_SET, m_sSetName);
	DDX_Text(pDX, IDC_EDIT_MAT, m_sMatName);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fAngle);
	DDX_Control(pDX, IDC_COMBO_LIST, m_cComboxList);
	DDX_Control(pDX, IDC_STATIC_PIC, m_cPic);
}

BEGIN_MESSAGE_MAP(CABASectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PAR, &CABASectionDlg::OnNMDblclkListPar)
	ON_EN_KILLFOCUS(IDC_EDIT0, &CABASectionDlg::OnEnKillfocusEdit0)
	ON_BN_CLICKED(IDC_BUTTON_GENABA, &CABASectionDlg::OnBnClickedButtonGenaba)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST, &CABASectionDlg::OnCbnSelchangeComboList)
END_MESSAGE_MAP()


// CABASectionDlg 消息处理程序

BOOL CABASectionDlg::OnInitDialog()
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


	InitialSecList();
	m_cEdit.ShowWindow(SW_HIDE);

	InitialCombxList();


	m_cListPar.InsertColumn(0, _T("参数值"), LVCFMT_LEFT, 60, -1);
	m_cListPar.InsertColumn(0, _T("名称"), LVCFMT_LEFT, 150, -1);
	m_cListPar.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40, -1);
	m_cListPar.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_AUTOSIZECOLUMNS);
	InitialSecPar();



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CABASectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CABASectionDlg::OnPaint()
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
HCURSOR CABASectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CABASectionDlg::InitialSecList()
{
	m_SecList.push_back(CSection(TRACT_PILLAR_ST_CROSS));
	m_SecList.push_back(CSection(TRACT_PILLAR_ST_I));
	//m_SecList[0].ID = IDB_BITMAP1;

}

void CABASectionDlg::InitialCombxList()
{
	for (unsigned  int i = 0; i < m_SecList.size(); i++)
	{
		CString str;
		str.Format(_T("%d-%s"), i, m_SecList[i].sName);
		m_cComboxList.AddString(str);
	}
	m_cComboxList.SetCurSel(0);
}

void CABASectionDlg::InitialSecPar(BOOL bFlag)
{
	if (bFlag)
	{
		m_cListPar.DeleteAllItems();

		m_cSec = CSection(m_Shape);

		for (unsigned  int i = 0; i < m_cSec.Pars.size(); i++)
		{
			sPar par = m_cSec.Pars[i];
			CString str1, str2, str3;
			str1.Format(_T("%d"), par.num);
			str2 = par.sDes;
			str3.Format(_T("%0.3f"), par.fValue);

			m_cListPar.InsertItem(i, str1);
			m_cListPar.SetItemText(i, 1, str2);
			m_cListPar.SetItemText(i, 2, str3);
		}
		//IDB_BITMAP1
		OnPaint(m_cSec.ID);
	}
	else
	{
		int num=m_cListPar.GetItemCount();
		//m_cSec.Pars.clear();
		for (int i = 0; i < num; i++)
		{
			CString str = m_cListPar.GetItemText(i, 2);
			double fvalue = _ttof(str);
			m_cSec.Pars[i].fValue = fvalue;
		}
	}

}


void CABASectionDlg::OnNMDblclkListPar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;//获得选中的行
	m_Col = pNMListView->iSubItem;//获得选中列

	if (m_Col != 2) return;


	if (pNMListView->iSubItem != 0) //如果选择的是子项;
	{
		m_cListPar.GetSubItemRect(m_Row, m_Col, LVIR_LABEL, rc);//获得子项的RECT；
		m_cEdit.SetParent(&m_cListPar);//转换坐标为列表框中的坐标
		m_cEdit.MoveWindow(rc);//移动Edit到RECT坐在的位置;
		m_cEdit.SetWindowText(m_cListPar.GetItemText(m_Row, m_Col));//将该子项中的值放在Edit控件中；
		m_cEdit.ShowWindow(SW_SHOW);//显示Edit控件；
		m_cEdit.SetFocus();//设置Edit焦点
		m_cEdit.ShowCaret();//显示光标
		m_cEdit.SetSel(-1);//将光标移动到最后
	}
	*pResult = 0;
}

void CABASectionDlg::OnEnKillfocusEdit0()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tem;
	m_cEdit.GetWindowText(tem);    //得到用户输入的新的内容
	double fvalue = _ttof(tem);
	tem.Format(_T("%0.3f"), fvalue);
	m_cListPar.SetItemText(m_Row, m_Col, tem);   //设置编辑框的新内容
	m_cEdit.ShowWindow(SW_HIDE);                //隐藏编辑框
}

#include"SecTextViewDlg.h"
void CABASectionDlg::OnBnClickedButtonGenaba()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	InitialSecPar(FALSE);
	
	CString sText = m_cSec.GenSecINP(m_sSetName, m_sMatName);
	CString sT;

	//增加角度
	if (m_fAngle > 0.01)
	{
		double v[3];
		v[0] = cos(m_fAngle*3.14 / 180);
		v[1] = sin(m_fAngle*3.14 / 180);
		v[2] = 0;
		sT.Format(_T("%8.4f,\t%8.4f,\t%8.4f\r\n"), v[0], v[1], v[2]);
		sText += sT;
	}

	CSecTextViewDlg cDlg;
	cDlg.m_sText = sText;
	cDlg.DoModal();

}


void CABASectionDlg::OnCbnSelchangeComboList()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_cComboxList.GetCurSel();
	m_Shape = m_SecList[index].Shape;
	InitialSecPar();
}

void CABASectionDlg::OnPaint(int num)
{
	CPaintDC dc(this); // device context for painting

	CDC *pDC = m_cPic.GetDC();

	CRect re;
	m_cPic.GetClientRect(&re);

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&re);
	pDC->SelectClipRgn(&rgn);
	//加载图片
	CBitmap bitmap;
	bitmap.LoadBitmapW(num);
	//得到图片信息
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	//创建兼容DC
	CDC CompatibleDC;
	CompatibleDC.CreateCompatibleDC(pDC);
	CompatibleDC.SelectObject(bitmap);
	pDC->StretchBlt(0, 0, re.Width(), re.Height(), &CompatibleDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	rgn.DeleteObject();
	if (pDC) ReleaseDC(pDC);
}