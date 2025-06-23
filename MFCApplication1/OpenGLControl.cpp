#include "pch.h"
#include "OpenGLControl.h"
#include <cmath>
#include <sstream>
#include <iomanip>

const double PI = 3.14159265358979323846;

COpenGLControl::COpenGLControl()
    : m_hRC(NULL), m_pDC(NULL),
    m_gdiplusToken(0),
    m_bDragging(false),
    m_rotX(220.0f), m_rotY(220.0f), m_rotZ(0.0f),
    m_zoom(1.0f), m_transX(0.0f), m_transY(0.0f),
    m_sphereDL(0) {
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

COpenGLControl::~COpenGLControl() {
    if (m_hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
    }
    if (m_pDC) delete m_pDC;
    if (m_sphereDL) glDeleteLists(m_sphereDL, 1);

    // 关闭GDI+
    GdiplusShutdown(m_gdiplusToken);
}

bool COpenGLControl::Create(CRect rect, CWnd* parent) {
    CString className = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)::GetStockObject(BLACK_BRUSH),
        ::LoadIcon(NULL, IDI_APPLICATION));

    return CreateEx(0, className, _T("OpenGL Control"),
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        rect, parent, 0);
}

void COpenGLControl::InitOpenGL() {
    m_pDC = new CClientDC(this);
    if (!m_pDC || !SetupPixelFormat()) {
        AfxMessageBox(_T("OpenGL initialization failed"));
        return;
    }

    m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
    if (!m_hRC || !wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {
        AfxMessageBox(_T("Rendering context creation failed"));
        return;
    }

    GLSetup();
    //GenerateSphereData(36, 18);

    m_sphereDL = glGenLists(1);
    glNewList(m_sphereDL, GL_COMPILE);
    DrawSphere();
    glEndList();
}

bool COpenGLControl::SetupPixelFormat() {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        32, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
    return pixelFormat && SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd);
}

void COpenGLControl::GLSetup() {
    // 设置白色背景
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // 禁用光照（线框模式不需要光照）
    glDisable(GL_LIGHTING);

    // 设置线框颜色为黑色
    glColor3f(0.0f, 0.0f, 0.0f);
}

void COpenGLControl::OnPaint() {
    CPaintDC dc(this);

    // 先渲染OpenGL内容
    Render();

    // 然后使用GDI+绘制标注
    DrawAnnotations(&dc);
}

void COpenGLControl::Render() {
    if (!wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 设置视点 - Z轴朝上，X/Y轴朝前
    gluLookAt(3, 3, 3,  // 摄像机位置 (X,Y,Z)
        0, 0, 0,  // 观察目标点
        0, 0, 1); // 上方向向量 (Z轴朝上)

    glTranslatef(m_transX, m_transY, 0.0f);
    glScalef(m_zoom, m_zoom, m_zoom);
    glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);

    if (m_sphereDL) glCallList(m_sphereDL);

    SwapBuffers(m_pDC->GetSafeHdc());
}

void COpenGLControl::DrawAnnotations(CDC* pDC) {
    Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    CRect rect;
    GetClientRect(&rect);

    // 1. 绘制标题
    FontFamily fontFamily(L"Arial");
    Gdiplus::Font titleFont(&fontFamily, 16, FontStyleBold, UnitPixel);
    SolidBrush titleBrush(Color(255, 0, 0, 0));

    StringFormat titleFormat;
    titleFormat.SetAlignment(StringAlignmentCenter);
    graphics.DrawString(L"3D球体线框模型（三色坐标轴）", -1, &titleFont,
        RectF(0, 10, rect.Width(), 30),
        &titleFormat, &titleBrush);

    // 2. 绘制坐标轴系统
    Gdiplus::Font axisFont(&fontFamily, 10, FontStyleRegular, UnitPixel);
    SolidBrush xBrush(Color(255, 255, 0, 0));   // X轴-红色
    SolidBrush yBrush(Color(255, 0, 255, 0));   // Y轴-绿色
    SolidBrush zBrush(Color(255, 0, 0, 255));   // Z轴-蓝色
    SolidBrush blackBrush(Color(255, 0, 0, 0)); // 黑色

    // 获取当前变换矩阵
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // 定义坐标轴参数
    GLdouble axisLength = 1.5; // 坐标轴长度
    GLdouble arrowSize = 0.1;  // 箭头大小
    int tickCount = 5;         // 刻度数量（每轴）

    // 原点坐标
    GLdouble origin[3] = { 0, 0, 0 };
    GLdouble ox, oy, oz;
    gluProject(origin[0], origin[1], origin[2], modelview, projection, viewport, &ox, &oy, &oz);
    oy = rect.Height() - oy;

    // 绘制X轴系统（红色）
    {
        Pen xPen(Color(255, 255, 0, 0), 2.0f);

        // 正X轴
        GLdouble xPos[3] = { axisLength, 0, 0 };
        GLdouble sx, sy, sz;
        gluProject(xPos[0], xPos[1], xPos[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;

        // 绘制轴线
        graphics.DrawLine(&xPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);

        // 绘制箭头
        DrawArrow(graphics, PointF((REAL)ox, (REAL)oy), PointF((REAL)sx, (REAL)sy),
            Color(255, 255, 0, 0), 8.0f);

        // 绘制刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = axisLength * i / tickCount;
            GLdouble tick[3] = { tickPos, 0, 0 };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            // 刻度线
            graphics.DrawLine(&xPen, (REAL)sx, (REAL)(sy - 5), (REAL)sx, (REAL)(sy + 5));

            // 刻度值
            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)sx, (REAL)(sy + 10)), &xBrush);
        }

        // 负X轴
        GLdouble xNeg[3] = { -axisLength, 0, 0 };
        gluProject(xNeg[0], xNeg[1], xNeg[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawLine(&xPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);

        // 负刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = -axisLength * i / tickCount;
            GLdouble tick[3] = { tickPos, 0, 0 };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            graphics.DrawLine(&xPen, (REAL)sx, (REAL)(sy - 5), (REAL)sx, (REAL)(sy + 5));

            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)sx, (REAL)(sy + 10)), &xBrush);
        }

        // X轴标签
        gluProject(axisLength * 1.1, 0, 0, modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawString(L"X", -1, &axisFont, PointF((REAL)sx, (REAL)sy), &xBrush);
    }

    // 绘制Y轴系统（绿色）
    {
        Pen yPen(Color(255, 0, 255, 0), 2.0f);

        // 正Y轴
        GLdouble yPos[3] = { 0, axisLength, 0 };
        GLdouble sx, sy, sz;
        gluProject(yPos[0], yPos[1], yPos[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;

        graphics.DrawLine(&yPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);
        DrawArrow(graphics, PointF((REAL)ox, (REAL)oy), PointF((REAL)sx, (REAL)sy),
            Color(255, 0, 255, 0), 8.0f);

        // 刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = axisLength * i / tickCount;
            GLdouble tick[3] = { 0, tickPos, 0 };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            graphics.DrawLine(&yPen, (REAL)(sx - 5), (REAL)sy, (REAL)(sx + 5), (REAL)sy);

            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)(sx + 10), (REAL)sy), &yBrush);
        }

        // 负Y轴
        GLdouble yNeg[3] = { 0, -axisLength, 0 };
        gluProject(yNeg[0], yNeg[1], yNeg[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawLine(&yPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);

        // 负刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = -axisLength * i / tickCount;
            GLdouble tick[3] = { 0, tickPos, 0 };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            graphics.DrawLine(&yPen, (REAL)(sx - 5), (REAL)sy, (REAL)(sx + 5), (REAL)sy);

            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)(sx + 10), (REAL)sy), &yBrush);
        }

        // Y轴标签
        gluProject(0, axisLength * 1.1, 0, modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawString(L"Y", -1, &axisFont, PointF((REAL)sx, (REAL)sy), &yBrush);
    }

    // 绘制Z轴系统（蓝色）
    {
        Pen zPen(Color(255, 0, 0, 255), 2.0f);

        // 正Z轴
        GLdouble zPos[3] = { 0, 0, axisLength };
        GLdouble sx, sy, sz;
        gluProject(zPos[0], zPos[1], zPos[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;

        graphics.DrawLine(&zPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);
        DrawArrow(graphics, PointF((REAL)ox, (REAL)oy), PointF((REAL)sx, (REAL)sy),
            Color(255, 0, 0, 255), 8.0f);

        // 刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = axisLength * i / tickCount;
            GLdouble tick[3] = { 0, 0, tickPos };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            graphics.DrawLine(&zPen, (REAL)(sx - 5), (REAL)(sy - 5), (REAL)(sx + 5), (REAL)(sy + 5));
            graphics.DrawLine(&zPen, (REAL)(sx - 5), (REAL)(sy + 5), (REAL)(sx + 5), (REAL)(sy - 5));

            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)(sx + 10), (REAL)(sy + 10)), &zBrush);
        }

        // 负Z轴
        GLdouble zNeg[3] = { 0, 0, -axisLength };
        gluProject(zNeg[0], zNeg[1], zNeg[2], modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawLine(&zPen, (REAL)ox, (REAL)oy, (REAL)sx, (REAL)sy);

        // 负刻度
        for (int i = 1; i <= tickCount; ++i) {
            GLdouble tickPos = -axisLength * i / tickCount;
            GLdouble tick[3] = { 0, 0, tickPos };
            gluProject(tick[0], tick[1], tick[2], modelview, projection, viewport, &sx, &sy, &sz);
            sy = rect.Height() - sy;

            graphics.DrawLine(&zPen, (REAL)(sx - 5), (REAL)(sy - 5), (REAL)(sx + 5), (REAL)(sy + 5));
            graphics.DrawLine(&zPen, (REAL)(sx - 5), (REAL)(sy + 5), (REAL)(sx + 5), (REAL)(sy - 5));

            std::wstringstream ss;
            ss << tickPos;
            graphics.DrawString(ss.str().c_str(), -1, &axisFont,
                PointF((REAL)(sx + 10), (REAL)(sy + 10)), &zBrush);
        }

        // Z轴标签
        gluProject(0, 0, axisLength * 1.1, modelview, projection, viewport, &sx, &sy, &sz);
        sy = rect.Height() - sy;
        graphics.DrawString(L"Z", -1, &axisFont, PointF((REAL)sx, (REAL)sy), &zBrush);
    }

    // 3. 绘制状态信息和图例
    Gdiplus::Font infoFont(&fontFamily, 10, FontStyleRegular, UnitPixel);
    SolidBrush infoBrush(Color(255, 0, 100, 0)); // 深绿色

    std::wstringstream ss;
    ss << L"旋转: X=" << std::fixed << std::setprecision(1) << m_rotX
        << L"°, Y=" << m_rotY << L"°, Z=" << m_rotZ << L"°\n"
        << L"缩放: " << std::setprecision(2) << m_zoom
        << L" | 平移: X=" << m_transX << L", Y=" << m_transY
        << L"\n使用鼠标左键旋转, Ctrl+左键平移, 滚轮缩放";

    graphics.DrawString(ss.str().c_str(), -1, &infoFont,
        RectF(10, rect.Height() - 80, rect.Width() - 20, 70),
        NULL, &infoBrush);

    // 图例
    graphics.DrawString(L"坐标轴颜色:", -1, &infoFont, PointF(10, 50), &blackBrush);
    graphics.DrawString(L"X轴: 红色", -1, &infoFont, PointF(20, 70), &xBrush);
    graphics.DrawString(L"Y轴: 绿色", -1, &infoFont, PointF(20, 90), &yBrush);
    graphics.DrawString(L"Z轴: 蓝色", -1, &infoFont, PointF(20, 110), &zBrush);
}

// 新增：绘制箭头函数
void COpenGLControl::DrawArrow(Graphics& graphics, PointF start, PointF end, Color color, REAL size) {
    Pen arrowPen(color, 2.0f);

    // 计算箭头方向
    REAL dx = end.X - start.X;
    REAL dy = end.Y - start.Y;
    REAL length = sqrt(dx * dx + dy * dy);
    if (length < 0.0001) return;

    // 标准化方向向量
    dx /= length;
    dy /= length;

    // 计算箭头两侧点
    PointF arrowPoint1(end.X - size * dx + size * dy * 0.5f,
        end.Y - size * dy - size * dx * 0.5f);
    PointF arrowPoint2(end.X - size * dx - size * dy * 0.5f,
        end.Y - size * dy + size * dx * 0.5f);

    // 绘制箭头
    graphics.DrawLine(&arrowPen, end, arrowPoint1);
    graphics.DrawLine(&arrowPen, end, arrowPoint2);
}

void COpenGLControl::Resize(int width, int height) {
    if (height == 0) height = 1;
    if (!wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) return;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy) {
    CWnd::OnSize(nType, cx, cy);
    if (cx > 0 && cy > 0 && m_pDC && m_hRC)
    {
        Resize(cx, cy);
        Invalidate(FALSE);  // 触发重绘
    }
}

BOOL COpenGLControl::OnEraseBkgnd(CDC* pDC) {
    return TRUE;
}

void COpenGLControl::GenerateSphereData(int slices, int stacks) {
    m_spherePoints.resize(stacks + 1);

    for (int i = 0; i <= stacks; ++i) {
        double phi = PI / 2 - i * PI / stacks;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);

        m_spherePoints[i].resize(slices + 1);

        for (int j = 0; j <= slices; ++j) {
            double theta = j * 2 * PI / slices;
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);

            double x = cosPhi * cosTheta;
            double y = cosPhi * sinTheta;
            double z = sinPhi;

            m_spherePoints[i][j] = Point3D(x, y, z);
        }
    }
}

void COpenGLControl::DrawSphere() {
    int stacks = m_spherePoints.size() - 1;
    if (stacks < 2) return;
    int slices = m_spherePoints[0].size() - 1;
    if (slices < 2) return;

    // 设置线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置线宽
    glLineWidth(1.0f);

    // 保持黑色线框
    glColor3f(0.0f, 0.0f, 0.0f);

    float amp = 1. / 1000;
    for (int i = 0; i < stacks+1; ++i) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            Point3D p1 = m_spherePoints[i% stacks][j];
            glVertex3f(p1.x * amp, p1.y * amp, p1.z * amp * 0.1);

            Point3D p2 = m_spherePoints[(i + 1)% stacks][j];
            glVertex3f(p2.x * amp, p2.y * amp, p2.z * amp * 0.1);
        }
        glEnd();
    }

    /*
    for (int i = 0; i < slices; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= stacks; ++j) {
            Point3D p1 = m_spherePoints[j][i];
            glVertex3f(p1.x * amp, p1.y * amp, p1.z * amp);

            Point3D p2 = m_spherePoints[j][i + 1];
            glVertex3f(p2.x * amp, p2.y * amp, p2.z * amp);
        }
        glEnd();
    }*/

    // 恢复填充模式（可选）
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// 鼠标交互实现
void COpenGLControl::OnLButtonDown(UINT nFlags, CPoint point) {
    m_bDragging = true;
    m_ptLastPos = point;
    SetCapture();
    CWnd::OnLButtonDown(nFlags, point);
}

void COpenGLControl::OnLButtonUp(UINT nFlags, CPoint point) {
    m_bDragging = false;
    ReleaseCapture();
    CWnd::OnLButtonUp(nFlags, point);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point) {
    if (m_bDragging) {
        CSize offset = point - m_ptLastPos;

        if (nFlags & MK_CONTROL) {
            m_transX += offset.cx * 0.01f;
            m_transY -= offset.cy * 0.01f;
        }
        else {
            m_rotY += offset.cx * 0.5f;
            m_rotX += offset.cy * 0.5f;
        }

        m_ptLastPos = point;
        Invalidate(FALSE);  // 触发重绘，包括标注更新
    }
    CWnd::OnMouseMove(nFlags, point);
}

BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    m_zoom += zDelta * 0.001f;
    m_zoom = max(0.1f, min(5.0f, m_zoom));
    Invalidate(FALSE);
    return TRUE;
}

void COpenGLControl::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CWnd::OnShowWindow(bShow, nStatus);
    if (bShow && m_hRC)
    {
        // 强制更新视口和投影矩阵
        CRect rect;
        GetClientRect(&rect);
        Resize(rect.Width(), rect.Height());
        Invalidate(FALSE);
    }
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()