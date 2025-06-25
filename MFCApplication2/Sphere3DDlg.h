#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class CSphere3DDlg : public CDialogEx
{
public:
    CSphere3DDlg(CWnd* pParent = nullptr);
    ~CSphere3DDlg();

    enum { IDD = IDD_SPHERE3D_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

public:
    CStatic m_glWindow;
    CScrollBar m_rotationScroll;

private:
    HGLRC m_hRC;
    CDC* m_pDC;

    // 3D变换参数
    float m_xRot, m_yRot, m_zRot;
    float m_xTrans, m_yTrans, m_zTrans;
    float m_zoom;

    // 鼠标状态
    CPoint m_prevPoint;
    bool m_bDragging;

    // 球体参数
    GLUquadricObj* m_sphere;

    void InitializeOpenGL();
    void SetupProjection(int width, int height);
    void DrawScene();
    void DrawAxes();
    void DrawSphere();

public:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDestroy();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};