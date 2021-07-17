#pragma once


// CDlgTable 对话框

class CDlgTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTable)

public:
	CDlgTable(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combox;
	virtual BOOL OnInitDialog();
	void InitialTable();
	CListCtrl m_ListCtrl;
	afx_msg void OnCbnSelchangeCombo1();
};
