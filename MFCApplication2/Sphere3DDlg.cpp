#include "pch.h"
#include "framework.h"
//#include "Sphere3D.h"
#include "Sphere3DDlg.h"
#include "afxdialogex.h"
#include <cmath>
#include <gdiplus.h>
using namespace Gdiplus;

const double M_PI = 3.14159265358979323846;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSphere3DDlg::CSphere3DDlg(CWnd* pParent)
    : CDialogEx(IDD_SPHERE3D_DIALOG, pParent)
{
    // 初始化参数
    m_xRot = -45.0f;
    m_yRot = 0.0f;
    m_zRot = 200.0f;
    m_xTrans = 0.0f;
    m_yTrans = 0.0f;
    m_zTrans = -5.0f;
    m_zoom = 1.0f;
    m_bDragging = false;
    m_pDC = nullptr;
    m_hRC = nullptr;
}

CSphere3DDlg::~CSphere3DDlg()
{
    if (m_hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
    }
    if (m_pDC) delete m_pDC;
}

void CSphere3DDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_PIC, m_glWindow);
    DDX_Control(pDX, IDC_ROTATION_SCROLL, m_rotationScroll);
}

BEGIN_MESSAGE_MAP(CSphere3DDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_DESTROY()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEWHEEL()
    ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CSphere3DDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置滚动条范围 (0-360度)
    m_rotationScroll.SetScrollRange(0, 360);
    m_rotationScroll.SetScrollPos(static_cast<int>(m_yRot));

    // 初始化OpenGL
    InitializeOpenGL();

    return TRUE;
}

void CSphere3DDlg::InitializeOpenGL()
{
    // 确保只初始化一次
    if (m_pDC != nullptr) return;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    m_pDC = new CClientDC(&m_glWindow);

    int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
    if (pixelFormat == 0) {
        AfxMessageBox(_T("ChoosePixelFormat failed"));
        return;
    }

    if (!SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd)) {
        AfxMessageBox(_T("SetPixelFormat failed"));
        return;
    }

    m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
    if (m_hRC == NULL) {
        AfxMessageBox(_T("wglCreateContext failed"));
        return;
    }

    if (!wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) {
        AfxMessageBox(_T("wglMakeCurrent failed"));
        return;
    }

    // 初始化OpenGL状态 - 白色背景
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色背景
    glEnable(GL_DEPTH_TEST);  // 启用深度测试保持遮挡效果

    // 设置初始投影
    CRect rect;
    m_glWindow.GetClientRect(&rect);
    SetupProjection(rect.Width(), rect.Height());

    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void CSphere3DDlg::SetupProjection(int width, int height)
{
    if (height == 0) height = 1;

    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLdouble)width / (GLdouble)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CSphere3DDlg::DrawScene()
{
    if (!m_pDC || !m_hRC) return;

    wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 应用变换
    glTranslatef(m_xTrans, m_yTrans, m_zTrans);
    glScalef(m_zoom, m_zoom, m_zoom);
    glRotatef(m_xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(m_yRot, 0.0f, 1.0f, 0.0f);
    glRotatef(m_zRot, 0.0f, 0.0f, 1.0f);

    // 绘制坐标轴和球体
    DrawAxes();
    //GenerateSphereData(10, 36);
    DrawSphere();

    SwapBuffers(m_pDC->GetSafeHdc());

    // 使用GDI+绘制坐标轴标签和刻度
    DrawAxisLabels();
}

// 将3D坐标转换为2D屏幕坐标
CPoint CSphere3DDlg::ProjectPoint(float x, float y, float z)
{
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLdouble winX, winY, winZ;
    gluProject(x, y, z, modelview, projection, viewport, &winX, &winY, &winZ);

    // 转换y坐标（OpenGL原点在左下，Windows在左上）
    winY = viewport[3] - winY;

    return CPoint(static_cast<int>(winX), static_cast<int>(winY));
}

void CSphere3DDlg::DrawAxisLabels()
{
    CDC* pDC = CDC::FromHandle(m_pDC->GetSafeHdc());
    if (!pDC) return;

    Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);

    FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
    SolidBrush textBrush(Color(0, 0, 0)); // 黑色文本

    StringFormat stringFormat;
    stringFormat.SetAlignment(StringAlignmentCenter);
    stringFormat.SetLineAlignment(StringAlignmentCenter);

    // 获取坐标轴端点的屏幕坐标
    CPoint origin = ProjectPoint(0, 0, 0);
    CPoint xEnd = ProjectPoint(2.0f, 0, 0);
    CPoint yEnd = ProjectPoint(0, 2.0f, 0);
    CPoint zEnd = ProjectPoint(0, 0, 2.0f);

    // 计算坐标轴长度（像素）
    int axisLengthX = static_cast<int>(sqrt(pow(xEnd.x - origin.x, 2) + pow(xEnd.y - origin.y, 2)));
    int axisLengthY = static_cast<int>(sqrt(pow(yEnd.x - origin.x, 2) + pow(yEnd.y - origin.y, 2)));
    int axisLengthZ = static_cast<int>(sqrt(pow(zEnd.x - origin.x, 2) + pow(zEnd.y - origin.y, 2)));

    // 确定刻度间隔（基于坐标轴长度）
    int maxAxisLength = max(axisLengthX, max(axisLengthY, axisLengthZ));
    int tickCount = max(4, maxAxisLength / 50); // 每50像素一个刻度

    // 绘制坐标轴标签
    DrawAxisLabel(graphics, font, textBrush, xEnd, L"X", axisLengthX);
    DrawAxisLabel(graphics, font, textBrush, yEnd, L"Y", axisLengthY);
    DrawAxisLabel(graphics, font, textBrush, zEnd, L"Z", axisLengthZ);

    // 绘制刻度线标签
    DrawAxisTicks(graphics, font, textBrush, origin, xEnd, tickCount, true, false, false);  // X轴
    DrawAxisTicks(graphics, font, textBrush, origin, yEnd, tickCount, false, true, false); // Y轴
    DrawAxisTicks(graphics, font, textBrush, origin, zEnd, tickCount, false, false, true); // Z轴
}

void CSphere3DDlg::DrawAxisLabel(Gdiplus::Graphics& graphics, Gdiplus::Font& font, SolidBrush& brush,
    CPoint endPoint, const wchar_t* label, int axisLength)
{
    // 计算标签位置（在轴端点外偏移）
    int offset = max(15, axisLength / 20);

    // 计算偏移方向（基于轴方向）
    int dx = endPoint.x - ProjectPoint(0, 0, 0).x;
    int dy = endPoint.y - ProjectPoint(0, 0, 0).y;

    // 归一化方向向量
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx = static_cast<int>(dx / length * offset);
        dy = static_cast<int>(dy / length * offset);
    }

    RectF rect(endPoint.x + dx - 15, endPoint.y + dy - 15, 30, 30);
    graphics.DrawString(label, -1, &font, rect, &StringFormat(), &brush);
}

void CSphere3DDlg::DrawAxisTicks(Graphics& graphics, Gdiplus::Font& font, SolidBrush& brush,
    CPoint startPoint, CPoint endPoint, int tickCount,
    bool isXAxis, bool isYAxis, bool isZAxis)
{
    // 计算轴向量
    float dx = static_cast<float>(endPoint.x - startPoint.x);
    float dy = static_cast<float>(endPoint.y - startPoint.y);

    // 计算刻度方向（垂直于轴）
    float perpX = -dy;
    float perpY = dx;

    // 归一化垂直向量
    float length = sqrt(perpX * perpX + perpY * perpY);
    if (length > 0) {
        perpX /= length;
        perpY /= length;
    }

    // 刻度线长度
    int tickLength = 8;

    // 绘制刻度线和标签
    for (int i = 1; i <= tickCount; i++) {
        // 计算刻度位置（0.0到2.0之间）
        float t = static_cast<float>(i) / tickCount;
        float value = t * 2.0f;

        // 计算屏幕位置
        float px = startPoint.x + dx * t;
        float py = startPoint.y + dy * t;

        // 绘制刻度线
        CPoint tickStart(static_cast<int>(px), static_cast<int>(py));
        CPoint tickEnd(static_cast<int>(px + perpX * tickLength),
            static_cast<int>(py + perpY * tickLength));

        Pen pen(Color(100, 100, 100), 1.0f);
        graphics.DrawLine(&pen, tickStart.x, tickStart.y, tickEnd.x, tickEnd.y);

        // 绘制刻度值
        CString valueStr;
        valueStr.Format(_T("%.1f"), value);

        // 计算文本位置（在刻度线外偏移）
        int textOffsetX = static_cast<int>(perpX * (tickLength + 5));
        int textOffsetY = static_cast<int>(perpY * (tickLength + 5));

        RectF rect(tickEnd.x + textOffsetX - 20,
            tickEnd.y + textOffsetY - 10,
            40, 20);
        graphics.DrawString(valueStr, -1, &font, rect, &StringFormat(), &brush);
    }
}

void CSphere3DDlg::DrawAxes()
{
    glLineWidth(2.0f);

    // X轴 (红色)
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.2f, 0.2f); // 深红色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);
    glEnd();

    // Y轴 (绿色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.8f, 0.2f); // 深绿色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glEnd();

    // Z轴 (蓝色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.2f, 0.8f); // 深蓝色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);
    glEnd();
}

void CSphere3DDlg::DrawSphere()
{
    // 设置线框颜色为黑色
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);

    const float radius = 0.8f; // 球体半径
    const int stacks = m_spherePoints.size();     // 经度分段数
    const int slices = m_spherePoints[0].size();     // 纬度分段数

    // 生成并绘制球体顶点
    for (int i = 0; i < stacks; ++i) {
        // 当前纬度和下一纬度的角度
        float phi1 = static_cast<float>(i) * M_PI / static_cast<float>(stacks);
        float phi2 = static_cast<float>(i + 1) * M_PI / static_cast<float>(stacks);

        // 绘制纬线（水平圆）
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= slices; ++j) {
            float theta = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(slices);

            // 计算当前纬度的顶点坐标
            /*float x = radius * sin(phi1) * cos(theta);
            float y = radius * sin(phi1) * sin(theta);
            float z = radius * cos(phi1);*/
            float x = m_spherePoints[i][j % slices].x;
            float y = m_spherePoints[i][j % slices].y;
            float z = m_spherePoints[i][j % slices].z;

            glVertex3f(x/1000, y / 1000, z / 10000);
        }
        glEnd();
    }
    // 绘制经线（从北极到南极的弧线）
    for (int j = 0; j < slices; j++) {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k <= stacks; ++k) {
            float phi = static_cast<float>(k) * M_PI / static_cast<float>(stacks);
            float theta = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(slices);

            // 计算顶点坐标
            /*float x = radius * sin(phi) * cos(theta);
            float y = radius * sin(phi) * sin(theta);
            float z = radius * cos(phi);*/
            float x = m_spherePoints[k % stacks][j].x;
            float y = m_spherePoints[k % stacks][j].y;
            float z = m_spherePoints[k % stacks][j].z;

            glVertex3f(x / 1000, y / 1000, z / 10000);
        }
        glEnd();
    }

    // 绘制北极点
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, radius);
    glEnd();

    // 绘制南极点
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, -radius);
    glEnd();
}

void CSphere3DDlg::GenerateSphereData(int slices, int stacks) {
    m_spherePoints.resize(stacks + 1);

    for (int i = 0; i <= stacks; ++i) {
        double phi = M_PI / 2 - i * M_PI / stacks;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);

        m_spherePoints[i].resize(slices + 1);

        for (int j = 0; j <= slices; ++j) {
            double theta = j * 2 * M_PI / slices;
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);

            double x = cosPhi * cosTheta;
            double y = cosPhi * sinTheta;
            double z = sinPhi;

            m_spherePoints[i][j] = Point3D(x, y, z);
        }
    }
}

void CSphere3DDlg::OnPaint()
{
    CPaintDC dc(this);
    DrawScene();
}

BOOL CSphere3DDlg::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CSphere3DDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (m_glWindow.GetSafeHwnd() && m_pDC && m_hRC)
    {
        CRect rect;
        m_glWindow.GetClientRect(&rect);
        SetupProjection(rect.Width(), rect.Height());
        DrawScene();
    }
}

void CSphere3DDlg::OnDestroy()
{
    if (m_hRC)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
        m_hRC = NULL;
    }

    if (m_pDC)
    {
        delete m_pDC;
        m_pDC = NULL;
    }

    CDialogEx::OnDestroy();
}

void CSphere3DDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CRect rect;
    m_glWindow.GetWindowRect(&rect);
    ScreenToClient(&rect);

    if (rect.PtInRect(point)) {
        m_prevPoint = point;
        m_bDragging = true;
        SetCapture();
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CSphere3DDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging) {
        m_bDragging = false;
        ReleaseCapture();
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CSphere3DDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // 重置旋转和缩放
    m_xRot = 30.0f;
    m_yRot = 30.0f;
    m_zRot = 0.0f;
    m_zoom = 1.0f;
    m_rotationScroll.SetScrollPos(static_cast<int>(m_yRot));
    DrawScene();
    CDialogEx::OnRButtonDown(nFlags, point);
}

void CSphere3DDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        if (nFlags & MK_CONTROL)
        {
            // 平移
            m_xTrans += (point.x - m_prevPoint.x) * 0.01f;
            m_yTrans -= (point.y - m_prevPoint.y) * 0.01f;
        }
        else if (nFlags & MK_LBUTTON)
        {
            // 旋转
            m_xRot += (point.y - m_prevPoint.y);
            m_yRot += (point.x - m_prevPoint.x);

            if (m_xRot > 360) m_xRot -= 360;
            if (m_xRot < -360) m_xRot += 360;
            if (m_yRot > 360) m_yRot -= 360;
            if (m_yRot < -360) m_yRot += 360;

            m_rotationScroll.SetScrollPos(static_cast<int>(m_yRot));
        }

        m_prevPoint = point;
        DrawScene();
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CSphere3DDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    m_zoom += zDelta * 0.001f;
    m_zoom = max(0.1f, min(5.0f, m_zoom));
    Invalidate(FALSE);
    return TRUE;
}

void CSphere3DDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar == (CScrollBar*)&m_rotationScroll)
    {
        int pos = m_rotationScroll.GetScrollPos();

        switch (nSBCode)
        {
        case SB_LINELEFT:  pos -= 1; break;
        case SB_LINERIGHT: pos += 1; break;
        case SB_PAGELEFT:  pos -= 10; break;
        case SB_PAGERIGHT: pos += 10; break;
        case SB_THUMBTRACK: pos = nPos; break;
        }

        if (pos < 0) pos = 0;
        if (pos > 360) pos = 360;

        m_rotationScroll.SetScrollPos(pos);
        m_zRot = static_cast<float>(pos);
        DrawScene();
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
