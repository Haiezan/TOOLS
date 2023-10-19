#pragma once


// CDlgSSG 对话框

class CDlgSSG : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSSG)

public:
	CDlgSSG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSSG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SSG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bU[6];
	float m_fK[6], m_fC[6];
	BOOL m_bUN[6];
	float m_fNL[8];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButtonUn1();
};
