// DlgCurve.cpp: 实现文件
//

#include "stdafx.h"
#include "Fai.h"
#include "DlgCurve.h"
#include "afxdialogex.h"
#include "function.h"


// CDlgCurve 对话框

IMPLEMENT_DYNAMIC(CDlgCurve, CDialogEx)

CDlgCurve::CDlgCurve(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CURVE, pParent)
{

}

CDlgCurve::~CDlgCurve()
{
}

void CDlgCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM1, m_ChartCtrl);
}


BEGIN_MESSAGE_MAP(CDlgCurve, CDialogEx)
END_MESSAGE_MAP()


// CDlgCurve 消息处理程序


BOOL CDlgCurve::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//清空曲线
	m_ChartCtrl.RemoveAllSeries();
	m_ChartCtrl.GetTitle()->RemoveAll();

	CChartAxis *pAxis = NULL;
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	pAxis->SetAutomatic(true);
	//pAxis->SetMinMax(m_fM1min*1.1f, m_fM1max*1.1f);
	pAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	pAxis->SetAutomatic(true);

	//背景颜色
	m_ChartCtrl.SetBackColor(RGB(255, 255, 255));

	//图标标题
	//m_ChartCtrl.GetTitle()->AddString(_T("P-M1"));

	//坐标轴标题
	m_ChartCtrl.GetBottomAxis()->GetLabel()->SetText(_T("λ"));
	m_ChartCtrl.GetLeftAxis()->GetLabel()->SetText(_T("φ"));

	//图例
	if (1)
	{
		CChartLegend* cChartLegend = m_ChartCtrl.GetLegend();
		cChartLegend->SetVisible(true);
		cChartLegend->DockLegend(cChartLegend->dsDockBottom);
		cChartLegend->SetHorizontalMode(true);
	}

	int num = 300;
	double* x = new double[num];
	double* a = new double[num];
	double* b = new double[num];
	double* c = new double[num];
	double* d = new double[num];
	for (int i = 0; i < num; i++)
	{
		x[i] = double(i);
		a[i] = GetFai(0, x[i]);
		b[i] = GetFai(1, x[i]);
		c[i] = GetFai(2, x[i]);
		d[i] = GetFai(3, x[i]);
	}
	CChartLineSerie* pLineSerieA = m_ChartCtrl.CreateLineSerie();
	PlotLineSerie(pLineSerieA, x, a, num, 2, RGB(0, 255, 0), _T("a类"));
	CChartLineSerie* pLineSerieB = m_ChartCtrl.CreateLineSerie();
	PlotLineSerie(pLineSerieB, x, b, num, 2, RGB(255, 255, 0), _T("b类"));
	CChartLineSerie* pLineSerieC = m_ChartCtrl.CreateLineSerie();
	PlotLineSerie(pLineSerieC, x, c, num, 2, RGB(255, 0, 0), _T("c类"));
	CChartLineSerie* pLineSerieD = m_ChartCtrl.CreateLineSerie();
	PlotLineSerie(pLineSerieD, x, d, num, 2, RGB(0, 0, 0), _T("d类"));

	m_ChartCtrl.EnableRefresh(true);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgCurve::PlotLineSerie(CChartLineSerie* pLineSerie, double* x, double* y, int num, int width, COLORREF color, const TChartString& name)
{
	pLineSerie->SetSeriesOrdering(poNoOrdering);//设置为无序
	pLineSerie->SetPoints(x, y, num);
	pLineSerie->SetWidth(width);
	pLineSerie->SetColor(color);
	pLineSerie->SetName(name);
}