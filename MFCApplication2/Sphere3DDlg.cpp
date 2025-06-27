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
    m_xRot = -80.0f;
    m_yRot = 0.0f;
    m_zRot = 225.0f;
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

    //GenerateSphereData(36, 36);

    // 设置滚动条范围 (0-360度)
    m_rotationScroll.SetScrollRange(0, 360);
    m_rotationScroll.SetScrollPos(static_cast<int>(m_yRot));

    //计算数据最值
    for (int i = 0; i < m_spherePoints.size(); i++)
    {
        for (int j = 0; j < m_spherePoints[i].size(); j++)
        {
            if (i == 0 && j == 0)
            {
                minX = maxX = m_spherePoints[i][j].x;
                minY = maxY = m_spherePoints[i][j].y;
                minZ = maxZ = m_spherePoints[i][j].z;
            }
            else
            {
                minX = min(minX, m_spherePoints[i][j].x);
                maxX = max(maxX, m_spherePoints[i][j].x);

                minY = min(minY, m_spherePoints[i][j].y);
                maxY = max(maxY, m_spherePoints[i][j].y);

                minZ = min(minZ, m_spherePoints[i][j].z);
                maxZ = max(maxZ, m_spherePoints[i][j].z);
            }
        }
    }

    //计算缩放比例
    float org = 1.5f;
    //org = maxX;
    scaleX = org / maxX;
    scaleY = org / maxY;
    scaleZ = org / maxZ;

    //计算坐标轴长度
    axisLengthX = maxX * 1.5f * scaleX;
    axisLengthY = maxY * 1.5f * scaleY;
    axisLengthZ = maxZ * 1.5f * scaleZ;
    axisLengthXNeg = abs(minX) * 1.5f * scaleX;
    axisLengthYNeg = abs(minY) * 1.5f * scaleY;
    axisLengthZNeg = abs(minZ) * 1.5f * scaleZ;

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

    // 根据窗口宽高比调整透视投影
    float aspect = (float)width / (float)height;
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

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

    // 绘制曲面
    DrawSphere();

    DrawAxes();

    SwapBuffers(m_pDC->GetSafeHdc());

    //绘制坐标轴和标签
    DrawAxisLabels(); // 使用GDI+绘制坐标轴标签和刻度

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
    if (gluProject(x, y, z, modelview, projection, viewport, &winX, &winY, &winZ)) {
        // 转换y坐标（OpenGL原点在左下，Windows在左上）
        winY = viewport[3] - winY;
        return CPoint(static_cast<int>(winX), static_cast<int>(winY));
    }

    // 投影失败时返回无效点
    return CPoint(INT_MIN, INT_MIN);
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
    CPoint xEnd = ProjectPoint(axisLengthX, 0, 0);
    CPoint xNegEnd = ProjectPoint(-axisLengthXNeg, 0, 0);
    CPoint yEnd = ProjectPoint(0, axisLengthY, 0);
    CPoint yNegEnd = ProjectPoint(0, -axisLengthYNeg, 0);
    CPoint zEnd = ProjectPoint(0, 0, axisLengthZ);
    CPoint zNegEnd = ProjectPoint(0, 0, -axisLengthZNeg);

    // 计算坐标轴长度（像素）
    int axisLengthX = static_cast<int>(sqrt(pow(xEnd.x - xNegEnd.x, 2) + pow(xEnd.y - xNegEnd.y, 2)));
    int axisLengthY = static_cast<int>(sqrt(pow(yEnd.x - yNegEnd.x, 2) + pow(yEnd.y - yNegEnd.y, 2)));
    int axisLengthZ = static_cast<int>(sqrt(pow(zEnd.x - zNegEnd.x, 2) + pow(zEnd.y - zNegEnd.y, 2)));

    // 绘制坐标轴标签
    DrawAxisLabel(graphics, font, textBrush, xEnd, L"X+", axisLengthX);
    DrawAxisLabel(graphics, font, textBrush, xNegEnd, L"X-", axisLengthX);
    DrawAxisLabel(graphics, font, textBrush, yEnd, L"Y+", axisLengthY);
    DrawAxisLabel(graphics, font, textBrush, yNegEnd, L"Y-", axisLengthY);
    DrawAxisLabel(graphics, font, textBrush, zEnd, L"Z+", axisLengthZ);
    DrawAxisLabel(graphics, font, textBrush, zNegEnd, L"Z-", axisLengthZNeg);

    // 绘制刻度线标签（正负方向）
    DrawAxisTicks(graphics, font, textBrush, xNegEnd, xEnd, true, false, false);  // X轴
    DrawAxisTicks(graphics, font, textBrush, yNegEnd, yEnd, false, true, false);  // Y轴
    DrawAxisTicks(graphics, font, textBrush, zNegEnd, zEnd, false, false, true);  // Z轴
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
    CPoint startPoint2D, CPoint endPoint2D,
    bool isXAxis, bool isYAxis, bool isZAxis)
{
    float tickLength = 8;
    float spacing = 1.0f;
    float axisLengthWorld = 1.0f;
    float axisLengthWorldNeg = 1.0f;
    float scale = 1.0f;

    if (isXAxis) {
        axisLengthWorldNeg = axisLengthXNeg / scaleX;
        axisLengthWorld = axisLengthX / scaleX;
        spacing = ComputeTickSpacing(axisLengthWorld + axisLengthWorldNeg);
        scale = scaleX;
    }
    else if (isYAxis) {
        axisLengthWorldNeg = axisLengthYNeg / scaleX;
        axisLengthWorld = axisLengthY / scaleY;
        spacing = ComputeTickSpacing(axisLengthWorld + axisLengthWorldNeg);
        scale = scaleY;
    }
    else if (isZAxis) {
        axisLengthWorldNeg = axisLengthZNeg / scaleX;
        axisLengthWorld = axisLengthZ / scaleZ;
        spacing = ComputeTickSpacing(axisLengthWorld + axisLengthWorldNeg) * 10;
        scale = scaleZ;
    }

    float start = ceil(-axisLengthWorldNeg / spacing) * spacing;
    float end = floor(axisLengthWorld / spacing) * spacing;

    for (float v = start; v <= end; v += spacing)
    {
        float x = 0, y = 0, z = 0;
        if (isXAxis) x = v;
        else if (isYAxis) y = v;
        else if (isZAxis) z = v;

        CPoint pt = ProjectPoint(x * scaleX, y * scaleY, z * scaleZ);

        int dx = 0, dy = 0;
        if (isXAxis) dy = tickLength;
        else if (isYAxis) dx = tickLength;
        else if (isZAxis) { dx = tickLength; dy = tickLength; }

        Pen pen(Color(100, 100, 100), 1.0f);
        graphics.DrawLine(&pen, pt.x - dx, pt.y - dy, pt.x + dx, pt.y + dy);

        if (fabs(v) < 1e-4f) continue;

        CString label;
        if (fabs(v) < 0.001f) label = _T("0");
        else if (fabs(v) < 0.1f) label.Format(_T("%.4f"), v);
        else if (fabs(v) < 1.0f) label.Format(_T("%.3f"), v);
        else if (fabs(v) < 10.0f) label.Format(_T("%.2f"), v);
        else if (fabs(v) < 1000.0f) label.Format(_T("%.1f"), v);
        else label.Format(_T("%.0f"), v);

        RectF rect(static_cast<REAL>(pt.x + dx + 2), static_cast<REAL>(pt.y + dy + 2), 50, 20);
        graphics.DrawString(label, -1, &font, rect, &StringFormat(), &brush);
    }
}


float CSphere3DDlg::ComputeTickSpacing(float axisWorldLength)
{
    float desiredScreenSpacing = 300.0f; // 最小像素间隔（可调）
    float approxTickCount = (axisWorldLength * m_zoom) / desiredScreenSpacing;
    if (approxTickCount < 1) approxTickCount = 1;

    float rawSpacing = axisWorldLength / approxTickCount;

    // 规整为漂亮的数字：1, 2, 5, 10, 20, 50, ...
    float mag = pow(10.0f, floor(log10(rawSpacing)));
    float norm = rawSpacing / mag;
    float spacing;
    if (norm < 1.5f) spacing = 1.0f;
    else if (norm < 3.0f) spacing = 2.0f;
    else if (norm < 7.0f) spacing = 5.0f;
    else spacing = 10.0f;

    return spacing * mag;
}


void CSphere3DDlg::DrawAxes()
{
    glLineWidth(2.0f);

    // X轴 (红色)
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.2f, 0.2f); // 深红色
    glVertex3f(-axisLengthXNeg, 0.0f, 0.0f); // 负方向
    glVertex3f(axisLengthX, 0.0f, 0.0f);  // 正方向
    glEnd();

    // 绘制X轴箭头
    DrawArrow(-axisLengthXNeg, 0.0f, 0.0f, axisLengthX, 0.0f, 0.0f, 0.8f, 0.2f, 0.2f);

    // Y轴 (绿色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.8f, 0.2f); // 深绿色
    glVertex3f(0.0f, -axisLengthYNeg, 0.0f); // 负方向
    glVertex3f(0.0f, axisLengthY, 0.0f);  // 正方向
    glEnd();

    // 绘制Y轴箭头
    DrawArrow(0.0f, -axisLengthYNeg, 0.0f, 0.0f, axisLengthY, 0.0f, 0.2f, 0.8f, 0.2f);

    // Z轴 (蓝色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.2f, 0.8f); // 深蓝色
    glVertex3f(0.0f, 0.0f, -axisLengthZNeg); // 负方向
    glVertex3f(0.0f, 0.0f, axisLengthZ);  // 正方向
    glEnd();

    // 绘制Z轴箭头
    DrawArrow(0.0f, 0.0f, -axisLengthZNeg, 0.0f, 0.0f, axisLengthZ, 0.2f, 0.2f, 0.8f);
}

// 绘制坐标轴箭头
void CSphere3DDlg::DrawArrow(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float r, float g, float b)
{
    // 计算轴方向向量
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;

    // 归一化方向向量
    float length = sqrt(dx * dx + dy * dy + dz * dz);
    if (length < 0.001f) return;

    dx /= length;
    dy /= length;
    dz /= length;

    // 箭头参数
    float baseArrowSize = 0.1f;
    float arrowLength = baseArrowSize * m_zoom;
    float arrowWidth = (baseArrowSize * 0.5f) * m_zoom;

    // 箭头尖端位置
    float tipX = x2;
    float tipY = y2;
    float tipZ = z2;

    // 箭头基部位置
    float baseX = tipX - dx * arrowLength;
    float baseY = tipY - dy * arrowLength;
    float baseZ = tipZ - dz * arrowLength;

    // 计算垂直向量
    float perpX = 0.0f, perpY = 0.0f, perpZ = 0.0f;

    // 确保选择的垂直向量方向不与原向量平行
    if (fabs(dx) <= fabs(dy) && fabs(dx) <= fabs(dz)) {
        // dx最小，选择X轴方向作为初始垂直向量
        perpX = 1.0f;
        perpY = 0.0f;
        perpZ = 0.0f;
    }
    else if (fabs(dy) <= fabs(dx) && fabs(dy) <= fabs(dz)) {
        // dy最小，选择Y轴方向作为初始垂直向量
        perpX = 0.0f;
        perpY = 1.0f;
        perpZ = 0.0f;
    }
    else {
        // dz最小，选择Z轴方向作为初始垂直向量
        perpX = 0.0f;
        perpY = 0.0f;
        perpZ = 1.0f;
    }

    // 计算叉积得到垂直向量
    float ux = dy * perpZ - dz * perpY;
    float uy = dz * perpX - dx * perpZ;
    float uz = dx * perpY - dy * perpX;

    // 归一化垂直向量
    length = sqrt(ux * ux + uy * uy + uz * uz);
    if (length > 0.001f) {
        ux /= length;
        uy /= length;
        uz /= length;
    }

    // 计算另一个垂直向量
    float vx = dy * uz - dz * uy;
    float vy = dz * ux - dx * uz;
    float vz = dx * uy - dy * ux;

    // 归一化
    length = sqrt(vx * vx + vy * vy + vz * vz);
    if (length > 0.001f) {
        vx /= length;
        vy /= length;
        vz /= length;
    }

    // 绘制箭头
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);

    // 箭头尖端
    glVertex3f(tipX, tipY, tipZ);
    glVertex3f(baseX + ux * arrowWidth, baseY + uy * arrowWidth, baseZ + uz * arrowWidth);
    glVertex3f(baseX + vx * arrowWidth, baseY + vy * arrowWidth, baseZ + vz * arrowWidth);

    glVertex3f(tipX, tipY, tipZ);
    glVertex3f(baseX - ux * arrowWidth, baseY - uy * arrowWidth, baseZ - uz * arrowWidth);
    glVertex3f(baseX + vx * arrowWidth, baseY + vy * arrowWidth, baseZ + vz * arrowWidth);

    glVertex3f(tipX, tipY, tipZ);
    glVertex3f(baseX + ux * arrowWidth, baseY + uy * arrowWidth, baseZ + uz * arrowWidth);
    glVertex3f(baseX - vx * arrowWidth, baseY - vy * arrowWidth, baseZ - vz * arrowWidth);

    glVertex3f(tipX, tipY, tipZ);
    glVertex3f(baseX - ux * arrowWidth, baseY - uy * arrowWidth, baseZ - uz * arrowWidth);
    glVertex3f(baseX - vx * arrowWidth, baseY - vy * arrowWidth, baseZ - vz * arrowWidth);
    glEnd();
}

void CSphere3DDlg::DrawSphere()
{
    // 设置线框颜色为黑色
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(1.0f);

    const int stacks = m_spherePoints.size();     // 经线数
    const int slices = m_spherePoints[0].size();     // 纬线数

    // 生成并绘制球体顶点
    for (int i = 0; i < stacks; ++i) 
    {
        // 绘制纬线（水平圆）
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j <= slices; ++j) 
        {
            float x = m_spherePoints[i][j % slices].x;
            float y = m_spherePoints[i][j % slices].y;
            float z = m_spherePoints[i][j % slices].z;

            x *= scaleX;
            y *= scaleY;
            z *= scaleZ;

            glVertex3f(x, y, z);
        }
        glEnd();
    }
    // 绘制经线（从北极到南极的弧线）
    for (int j = 0; j < slices; j++) 
    {
        glBegin(GL_LINE_STRIP);
        for (int k = 0; k <= stacks; ++k) 
        {
            float x = m_spherePoints[k % stacks][j].x;
            float y = m_spherePoints[k % stacks][j].y;
            float z = m_spherePoints[k % stacks][j].z;

            x *= scaleX;
            y *= scaleY;
            z *= scaleZ;

            glVertex3f(x, y, z);
        }
        glEnd();
    }
}

void CSphere3DDlg::GenerateSphereData(int slices, int stacks) {
    m_spherePoints.resize(stacks + 1);

    double radius = 2000;

    for (int i = 0; i <= stacks; ++i) {
        double phi = M_PI / 2 - i * M_PI / stacks;
        double cosPhi = cos(phi);
        double sinPhi = sin(phi);

        m_spherePoints[i].resize(slices + 1);

        for (int j = 0; j <= slices; ++j) {
            double theta = j * 2 * M_PI / slices;
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);

            double x = radius * cosPhi * cosTheta;
            double y = radius * cosPhi * sinTheta;
            double z = radius * sinPhi;

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

    if (IsWindow(m_glWindow))
    {
        // 设置OpenGL窗口位置和大小
        m_glWindow.SetWindowPos(NULL,
            0,
            0,
            cx,
            cy,
            SWP_NOZORDER);
    }

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

    //点击点位置判断
    int hitRadius = 10;
    CPoint xArrowScreen = ProjectPoint(axisLengthX, 0, 0); // 屏幕位置
    if (abs(point.x - xArrowScreen.x) <= hitRadius &&
        abs(point.y - xArrowScreen.y) <= hitRadius)
    {
        m_bDraggingXArrow = true;
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
    if (m_bDraggingXArrow) {
        m_bDraggingXArrow = false;
        ReleaseCapture();
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CSphere3DDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // 重置旋转和缩放
    m_xRot = -80.0f;
    m_yRot = 0.0f;
    m_zRot = 225.0f;
    m_zoom = 1.0f;
    m_rotationScroll.SetScrollPos(static_cast<int>(m_yRot));
    DrawScene();
    CDialogEx::OnRButtonDown(nFlags, point);
}

void CSphere3DDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        if (m_bDraggingXArrow) 
        {
            int dx = point.x - m_prevPoint.x;
            int dy = point.y - m_prevPoint.y;
            //if (abs(dx) > abs(dy)) m_yRot += dx * 0.5f; // 横向拖动：绕 Y 轴
            //else m_zRot += dy * 0.5f;                   // 纵向拖动：绕 Z 轴
            m_xRot += dx;
        }
        else if (nFlags & MK_CONTROL)
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
