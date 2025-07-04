﻿
// MFCApplication1Dlg.h: 头文件
//

#pragma once
#include <string>
#include <vector>
#include "OpenGLControl.h"

// CMFCApplication1Dlg 对话框
class CMFCApplication1Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	std::vector < std::vector<Point3D>> m_pointCloud;
	bool LoadPointDataFromFile(const std::wstring& filename);

	std::vector < std::vector<Point3D>> m_pointSurface;
	int nLatitudeLines; //维度
	int nLongitudeLines; //经度
	bool InterpilateSurface(int nLongitudeLines, int nLatitudeLines);
	float LinearInterpolation(const std::vector<float>& x,
		const std::vector<float>& y, float xi);
};
