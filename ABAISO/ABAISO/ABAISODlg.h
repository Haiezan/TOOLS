
// ABAISODlg.h: 头文件
//

#pragma once
#include "DlgSSG.h"

// CABAISODlg 对话框
class CABAISODlg : public CDialogEx
{
// 构造
public:
	CABAISODlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CABAISODlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABAISO_DIALOG };
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
	CTabCtrl m_cTab;

	CDlgSSG* m_pDlgSSG;
	CStatic m_cGroup;
	CRect m_cGroupRect;
	CRect m_cDlgRect;
};
