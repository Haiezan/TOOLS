#pragma once
#include "ISO.h"

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

	ISO m_ISO;
	void SetData();

	CListBox m_cList;
	CListCtrl m_cListData;
	CString m_sNote;
	afx_msg void OnLbnSelchangeList1();
	BOOL m_bLinear;
	

	BOOL m_bU[6];
	// 各向同性强化
	BOOL m_bHard1;
	// 随动强化
	BOOL m_bHard2;
};
