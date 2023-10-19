#pragma once


// CDlgNL1 对话框

class CDlgNL1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNL1)

public:
	CDlgNL1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgNL1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NL1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	float m_f1;
	float m_f2;
};
