#pragma once


// CSecTextViewDlg 对话框

class CSecTextViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSecTextViewDlg)

public:
	CSecTextViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSecTextViewDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALO_SECVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_sText;
};
