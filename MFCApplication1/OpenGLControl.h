#pragma once
#include <vector>
#include <gl/GL.h>
#include <gl/GLU.h>

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

    void GenerateSurfaceData(int width, int height);
    void SetRotation(float x, float y, float z);
    void SetZoom(float zoom);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    HGLRC m_hRC;
    CDC* m_pDC;

    std::vector<std::vector<Point3D>> m_surfacePoints;
    float m_rotX, m_rotY, m_rotZ;
    float m_zoom;
    GLuint m_surfaceDL;

    bool SetupPixelFormat();
    void GLSetup();
    void DrawSurface();
};