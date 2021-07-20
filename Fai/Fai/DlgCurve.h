#pragma once
#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartLineSerie.h"

// CDlgCurve 对话框

class CDlgCurve : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCurve)

public:
	CDlgCurve(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgCurve();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CURVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CChartCtrl m_ChartCtrl;
	virtual BOOL OnInitDialog();
	void PlotLineSerie(CChartLineSerie* pLineSerie, double* x, double* y, int num, int width, COLORREF color, const TChartString& name);
};
