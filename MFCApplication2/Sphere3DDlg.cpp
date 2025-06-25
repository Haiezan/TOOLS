#include "pch.h"
#include "framework.h"
//#include "Sphere3D.h"
#include "Sphere3DDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSphere3DDlg::CSphere3DDlg(CWnd* pParent)
    : CDialogEx(IDD_SPHERE3D_DIALOG, pParent)
{
    // 初始化参数
    m_xRot = 30.0f;
    m_yRot = 30.0f;
    m_zRot = 0.0f;
    m_xTrans = 0.0f;
    m_yTrans = 0.0f;
    m_zTrans = -5.0f;
    m_zoom = 1.0f;
    m_bDragging = false;
    m_sphere = nullptr;
    m_pDC = nullptr;
    m_hRC = nullptr;
}

CSphere3DDlg::~CSphere3DDlg()
{
    if (m_sphere)
        gluDeleteQuadric(m_sphere);
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

    // 创建球体
    m_sphere = gluNewQuadric();
    gluQuadricDrawStyle(m_sphere, GLU_LINE); // 线框模式

    // 设置初始投影
    CRect rect;
    m_glWindow.GetClientRect(&rect);
    SetupProjection(rect.Width(), rect.Height());
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
    DrawSphere();

    SwapBuffers(m_pDC->GetSafeHdc());
}

void CSphere3DDlg::DrawAxes()
{
    glLineWidth(2.0f);

    // X轴 (红色)
    glBegin(GL_LINES);
    glColor3f(0.8f, 0.2f, 0.2f); // 深红色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(2.0f, 0.0f, 0.0f);

    // 刻度线
    glColor3f(0.5f, 0.0f, 0.0f); // 稍浅的红色
    for (float i = 0.5f; i < 2.0f; i += 0.5f) {
        glVertex3f(i, -0.05f, 0.0f);
        glVertex3f(i, 0.05f, 0.0f);
    }
    glEnd();

    // Y轴 (绿色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.8f, 0.2f); // 深绿色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 2.0f, 0.0f);

    // 刻度线
    glColor3f(0.0f, 0.5f, 0.0f); // 稍浅的绿色
    for (float i = 0.5f; i < 2.0f; i += 0.5f) {
        glVertex3f(-0.05f, i, 0.0f);
        glVertex3f(0.05f, i, 0.0f);
    }
    glEnd();

    // Z轴 (蓝色)
    glBegin(GL_LINES);
    glColor3f(0.2f, 0.2f, 0.8f); // 深蓝色
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 2.0f);

    // 刻度线
    glColor3f(0.0f, 0.0f, 0.5f); // 稍浅的蓝色
    for (float i = 0.5f; i < 2.0f; i += 0.5f) {
        glVertex3f(-0.05f, 0.0f, i);
        glVertex3f(0.05f, 0.0f, i);
    }
    glEnd();

    // 绘制轴标签
    glColor3f(0.0f, 0.0f, 0.0f); // 黑色标签
    glRasterPos3f(2.1f, 0.0f, 0.0f);
    //m_glWindow.TextOut(0, 0, "X");
    glRasterPos3f(0.0f, 2.1f, 0.0f);
    //m_glWindow.TextOut(0, 0, "Y");
    glRasterPos3f(0.0f, 0.0f, 2.1f);
    //m_glWindow.TextOut(0, 0, "Z");
}

void CSphere3DDlg::DrawSphere()
{
    // 设置线框颜色为黑色
    glColor3f(0.0f, 0.0f, 0.0f);

    // 设置线宽
    glLineWidth(1.0f);

    // 绘制线框球体（已启用深度测试，自动处理遮挡）
    gluSphere(m_sphere, 0.8, 32, 32);
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
        m_yRot = static_cast<float>(pos);
        DrawScene();
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}