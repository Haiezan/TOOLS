#pragma once
#include "OpenGLControl.h"

// C3DSurfaceDlg 对话框

class C3DSurfaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(C3DSurfaceDlg)

public:
	C3DSurfaceDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~C3DSurfaceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_3DSURFACE_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()

public:
    COpenGLControl m_glControl;
};
