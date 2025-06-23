#pragma once
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gdiplus.h>

struct Point3D {
    double x, y, z;
    Point3D(double _x = 0, double _y = 0, double _z = 0) : x(_x), y(_y), z(_z) {}
};

class COpenGLControl : public CWnd {
public:
    COpenGLControl();
    virtual ~COpenGLControl();

    bool Create(CRect rect, CWnd* parent);
    void InitOpenGL();
    void Render();
    void Resize(int width, int height);

    void GenerateSphereData(int slices, int stacks);
    std::vector<std::vector<Point3D>> m_spherePoints;
protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

private:
    HGLRC m_hRC;
    CDC* m_pDC;
    ULONG_PTR m_gdiplusToken;  // GDI+≥ı ºªØ¡Ó≈∆

    
    GLuint m_sphereDL;

    bool m_bDragging;
    CPoint m_ptLastPos;
    float m_rotX, m_rotY, m_rotZ;
    float m_zoom;
    float m_transX, m_transY;

    bool SetupPixelFormat();
    void GLSetup();
    void DrawSphere();
    void DrawAnnotations(CDC* pDC);
    void DrawArrow(Graphics& graphics, PointF start, PointF end, Color color, REAL size);

};