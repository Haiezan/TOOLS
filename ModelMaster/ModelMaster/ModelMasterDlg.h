
// ModelMasterDlg.h: 头文件
//

#pragma once
#include <vector>
using namespace std;

struct Project
{
	CString sName;
	CString sPath;
	vector<CString> sModelList;
	vector<CFileFind> FileList;
};

// CModelMasterDlg 对话框
class CModelMasterDlg : public CDialogEx
{
// 构造
public:
	CModelMasterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELMASTER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_sPath;
	afx_msg void OnBnClickedButton1();

	void ScanFile(CString Dir);  //搜索文件
	void AddModel(CString sFile);  //添加模型文件
	void ShowTree();

	vector<Project> m_ProjectList;
	vector<CString>  m_FileList;
	
	CTreeCtrl m_cTree;
};

