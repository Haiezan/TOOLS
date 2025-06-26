#pragma once
#include "resource.h"
#include "afxdialogex.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>

struct Point3D 
{
    float x, y, z;
    Point3D(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};

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
    ULONG_PTR gdiplusToken;  // GDI+初始化令牌

    // 3D变换参数
    float m_xRot, m_yRot, m_zRot;
    float m_xTrans, m_yTrans, m_zTrans;
    float m_zoom;

    // 鼠标状态
    CPoint m_prevPoint;
    bool m_bDragging;

    void InitializeOpenGL();
    void SetupProjection(int width, int height);
    void DrawScene();
    CPoint ProjectPoint(float x, float y, float z);

    void DrawAxisLabels();
    void DrawAxisLabel(Gdiplus::Graphics& graphics, Gdiplus::Font& font, Gdiplus::SolidBrush& brush,
        CPoint endPoint, const wchar_t* label, int axisLength);
    void DrawAxisTicks(Gdiplus::Graphics& graphics, Gdiplus::Font& font, Gdiplus::SolidBrush& brush,
        CPoint startPoint2D, CPoint endPoint2D,
        bool isXAxis, bool isYAxis, bool isZAxis);
    float ComputeTickSpacing(float axisWorldLength);
    void DrawAxes();
    void DrawArrow(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float r, float g, float b);
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

    bool m_bDraggingXArrow = false;

    //曲面坐标
    void GenerateSphereData(int slices, int stacks);
    std::vector<std::vector<Point3D>> m_spherePoints;
    float maxX, minX, maxY, minY, maxZ, minZ;
    float scaleX, scaleY , scaleZ;
    float axisLengthX, axisLengthXNeg;
    float axisLengthY, axisLengthYNeg;
    float axisLengthZ, axisLengthZNeg;
};