
// ABASectionDlg.h: 头文件
//

#pragma once
#include "Section.h"

// CABASectionDlg 对话框
class CABASectionDlg : public CDialogEx
{
// 构造
public:
	CABASectionDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABASECTION_DIALOG };
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
	//截面列表
	vector<CSection> m_SecList;
	void InitialSecList();
	//截面选择
	CComboBox m_cComboxList;
	void InitialCombxList();
	afx_msg void OnCbnSelchangeComboList();

	//截面参数
	CListCtrl m_cListPar;
	void InitialSecPar(BOOL bFlag = TRUE);
	CSection m_cSec;
	SECTION_SHAPE m_Shape;
	//截面参数修改
	CEdit m_cEdit;
	int m_Row;
	int m_Col;
	afx_msg void OnNMDblclkListPar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEdit0();

	//相关数据生成
	CString m_sSetName;
	CString m_sMatName;
	afx_msg void OnBnClickedButtonGenaba();
	double m_fAngle;
	
	CStatic m_cPic;
	void OnPaint(int num);
	void OnPaint(CString sPath);
};
