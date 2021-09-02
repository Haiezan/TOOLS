
// ConcreteDlg.h: 头文件
//

#pragma once
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartPointsSerie.h"

// CConcreteDlg 对话框
class CConcreteDlg : public CDialogEx
{
// 构造
public:
	CConcreteDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONCRETE_DIALOG };
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
	// 抗压强度
	float m_fFc;
	// 弹性模量
	float m_fEc;

	float ecr = 1471.8e-6f;
	float ac = 0.75f;
	float ecu2ecr = 2.99f;

	float GetDc(float e, float Ec, float fcr, float ecr, float ac);
	float GetSigmaC(float e, float dc, float Ec);
	afx_msg void OnBnClickedButtonPlot();
	CChartCtrl m_ChartCtrl;
};
