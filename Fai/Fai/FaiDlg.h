
// FaiDlg.h: 头文件
//

#pragma once


// CFaiDlg 对话框
class CFaiDlg : public CDialogEx
{
// 构造
public:
	CFaiDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FAI_DIALOG };
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
	CComboBox m_cComboxCatg;
	int m_iCatg; //截面分类
	char m_cCatg = 'a';
	float m_fCxb; //长细比
	float m_fFy; //屈服强度
	float m_fE; //弹性模量
	float m_fFai; //稳定系数
	float GetFai(int iCag, float fCxb, float fFy, float fE); //计算稳定系数

	CString m_sMsg;
	afx_msg void OnBnClickedButton1();
	CString GetTime();
	
	afx_msg void OnCbnSelchangeComboCatg();
};
