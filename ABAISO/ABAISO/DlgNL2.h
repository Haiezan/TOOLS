#pragma once


// CDlgNL2 对话框

class CDlgNL2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNL2)

public:
	CDlgNL2(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgNL2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NL2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	float m_f1;
	float m_f2;
	float m_f3;
};
