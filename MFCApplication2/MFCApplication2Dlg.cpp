
// MFCApplication2Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2Dlg 对话框



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMFCApplication2Dlg 消息处理程序
#include "Sphere3DDlg.h"
BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OnBnClickedButton1();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication2Dlg::OnPaint()
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
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	LoadPointDataFromFile(L"D:\\Github\\TOOLS\\MFCApplication1\\B8527\\PMM_RD.dat");
	LoadForceDataFromFile(L"D:\\Github\\TOOLS\\MFCApplication1\\B8527\\FIN_FILE.dat");

	nLongitudeLines = m_pointCloud.size();
	nLatitudeLines = m_pointCloud[0].size();

	InterpilateSurface(nLongitudeLines, nLatitudeLines);

	CSphere3DDlg dlg;
	dlg.m_spherePoints = m_pointSurface;
	dlg.m_forcePoints = m_pointForce;
	dlg.DoModal();
}

#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iomanip>

// 从文本文件加载点数据
bool CMFCApplication2Dlg::LoadPointDataFromFile(const std::wstring& filename) {
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

bool CMFCApplication2Dlg::LoadForceDataFromFile(const std::wstring& filename) {
	// 清空现有数据
	m_pointForce.clear();
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
	std::getline(file, line);
	std::getline(file, line);
	std::getline(file, line);
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
		float time, F2, F3, F1, M2, M3, M1;
		if (iss >> time >> F2 >> F3 >> F1 >> M2 >> M3 >> M1) {
			m_pointForce.push_back(Point3D(M2, M3, F1));
		}
	}

	file.close();

	if (m_pointCloud.empty()) {
		AfxMessageBox(_T("文件未包含有效数据"));
		return false;
	}


	return true;
}

bool CMFCApplication2Dlg::InterpilateSurface(int nLongitudeLines, int nLatitudeLines)
{
	//计算极值
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	for (int i = 0; i < m_pointCloud.size(); i++)
	{
		for (int j = 0; j < m_pointCloud[i].size(); j++)
		{
			if (i == 0 && j == 0)
			{
				minX = maxX = m_pointCloud[i][j].x;
				minY = maxY = m_pointCloud[i][j].y;
				minZ = maxZ = m_pointCloud[i][j].z;
			}
			else
			{
				minX = min(minX, m_pointCloud[i][j].x);
				maxX = max(maxX, m_pointCloud[i][j].x);

				minY = min(minY, m_pointCloud[i][j].y);
				maxY = max(maxY, m_pointCloud[i][j].y);

				minZ = min(minZ, m_pointCloud[i][j].z);
				maxZ = max(maxZ, m_pointCloud[i][j].z);
			}
		}
	}

	//计算坐标
	m_pointSurface.clear();
	m_pointSurface.resize(nLongitudeLines);

	float dZ = (maxZ - minZ) / (nLatitudeLines - 1);
	for (int i = 0; i < nLongitudeLines; ++i)
	{
		m_pointSurface[i].resize(nLatitudeLines);

		std::vector<float> xx, yy, zz;
		xx.clear();
		yy.clear();
		zz.clear();
		for (int j = 0; j < nLatitudeLines; j++)
		{
			xx.push_back(m_pointCloud[i][j].x);
			yy.push_back(m_pointCloud[i][j].y);
			zz.push_back(m_pointCloud[i][j].z);
		}

		float Z = minZ;
		for (int j = 0; j < nLatitudeLines; j++)
		{
			float X = LinearInterpolation(zz, xx, Z);
			float Y = LinearInterpolation(zz, yy, Z);
			m_pointSurface[i][j] = Point3D(X, Y, Z);
			Z += dZ;
		}
	}

	//写文件
	std::ofstream outFile("Surface");
	if (!outFile.is_open())
	{
		return FALSE;
	}

	// 设置输出精度
	outFile.precision(6);
	outFile << std::fixed;

	// 写入表头
	outFile << "Longitude Index | Latitude Index | X Coordinate | Y Coordinate | Z Coordinate\n";
	outFile << "----------------------------------------------------------------------------\n";

	// 写入插值结果
	for (size_t i = 0; i < m_pointSurface.size(); ++i)
	{
		for (size_t j = 0; j < m_pointSurface[i].size(); ++j)
		{
			const Point3D& pt = m_pointSurface[i][j];
			outFile << std::setw(16) << j << " | "
				<< std::setw(15) << i << " | "
				<< std::setw(13) << pt.x << " | "
				<< std::setw(13) << pt.y << " | "
				<< std::setw(13) << pt.z << "\n";
		}
	}

	outFile.close();

	return 1;
}

float CMFCApplication2Dlg::LinearInterpolation(const std::vector<float>& x,
	const std::vector<float>& y, float xi) {
	// 检查输入有效性
	if (x.size() != y.size()) {
		throw std::invalid_argument("x and y vectors must be of the same size");
	}
	if (x.empty()) {
		throw std::invalid_argument("Input vectors cannot be empty");
	}
	if (xi < x.front()) {
		return y.front();
	}
	if (xi > x.back()) {
		return y.back();
	}

	// 查找xi所在的区间
	size_t i = 0;
	while (i < x.size() - 1 && x[i + 1] < xi) {
		++i;
	}

	// 如果是正好等于某个点，直接返回对应的y值
	if (xi == x[i]) {
		return y[i];
	}
	if (i < x.size() - 1 && xi == x[i + 1]) {
		return y[i + 1];
	}

	// 线性插值公式: y = y0 + (y1-y0)*(x-x0)/(x1-x0)
	double slope = (y[i + 1] - y[i]) / (x[i + 1] - x[i]);
	return y[i] + slope * (xi - x[i]);
}