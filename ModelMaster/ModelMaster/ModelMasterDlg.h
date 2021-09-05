
// ModelMasterDlg.h: 头文件
//
#include <vector>
#include"Project.h"
#include "ConfigDlg.h"

#pragma once

using namespace std;

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

private:
	//配置文件
	CString sIni;
	void GetConfig();
	void WriteConfig();

	//软件路径
	CString m_sSoftwSSG;

public:
	CString m_sPath;
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButton1();
	void ScanFile(CString Dir);  //搜索文件
	void AddModel(CFileInfo file);  //添加模型文件
	void ShowTree();

	vector<Project> m_ProjectList;  //项目列表

	CTreeCtrl m_cTree;
	CString m_sTargetExt;
	int SplitString(LPCTSTR lpszStr, LPCTSTR lpszSplit, CStringArray& rArrString, BOOL bAllowNullString);
	afx_msg void OnBnClickedButtonBackup();
	int m_iFileTime;
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRightOpen();

	HTREEITEM m_hCurItem;
	afx_msg void OnRightTxt();

	CFileInfo GetFileInfo(HTREEITEM m_hCurItem);
	afx_msg void OnBnClickedButtonWrite();
	afx_msg void OnBnClickedButtonConfig();
};
