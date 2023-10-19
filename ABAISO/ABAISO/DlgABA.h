#pragma once


// CDlgABA 对话框

class CDlgABA : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgABA)

public:
	CDlgABA(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgABA();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ABA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_cList;
	CString m_sNote;
	afx_msg void OnLbnSelchangeList1();
	BOOL m_bLinear;
};
