#include "pch.h"
#include "OpenGLControl.h"
#include <cmath>

const double PI = 3.14159265358979323846;
const float AXIS_LENGTH = 2.0f;  // 坐标轴长度

COpenGLControl::COpenGLControl()
    : m_hRC(NULL), m_pDC(NULL),
    m_bDragging(false),
    m_rotX(30.0f), m_rotY(0.0f), m_rotZ(0.0f),
    m_zoom(1.0f), m_transX(0.0f), m_transY(0.0f),
    m_sphereDL(0) {}

COpenGLControl::~COpenGLControl() {
    if (m_hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
    }
    if (m_pDC) delete m_pDC;
    if (m_sphereDL) glDeleteLists(m_sphereDL, 1);
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
    GenerateSphereData(36, 18);

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
    // 白色背景
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // 启用深度测试以实现遮挡效果
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // 禁用光照
    glDisable(GL_LIGHTING);
}

void COpenGLControl::OnPaint() {
    CPaintDC dc(this);
    Render();
}

void COpenGLControl::Render() {
    if (!wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC)) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 设置视点
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // 应用变换
    glTranslatef(m_transX, m_transY, 0.0f);
    glScalef(m_zoom, m_zoom, m_zoom);
    glRotatef(m_rotX, 1.0f, 0.0f, 0.0f);
    glRotatef(m_rotY, 0.0f, 1.0f, 0.0f);
    glRotatef(m_rotZ, 0.0f, 0.0f, 1.0f);

    // 绘制坐标轴（先绘制，这样球体线框可以遮挡它）
    DrawCoordinateAxes();

    // 绘制球体
    if (m_sphereDL) glCallList(m_sphereDL);

    SwapBuffers(m_pDC->GetSafeHdc());
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
    if (cx > 0 && cy > 0 && m_pDC) Resize(cx, cy);
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

    // 黑色线框
    glColor3f(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < stacks; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= slices; ++j) {
            Point3D p1 = m_spherePoints[i][j];
            glVertex3f(p1.x, p1.y, p1.z);

            Point3D p2 = m_spherePoints[i + 1][j];
            glVertex3f(p2.x, p2.y, p2.z);
        }
        glEnd();
    }

    // 恢复填充模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void COpenGLControl::DrawCoordinateAxes() {
    // 设置线宽（坐标轴比球体线框粗一些）
    glLineWidth(2.0f);

    glBegin(GL_LINES);

    // X轴 (红色)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(AXIS_LENGTH, 0.0f, 0.0f);

    // Y轴 (绿色)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, AXIS_LENGTH, 0.0f);

    // Z轴 (蓝色)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, AXIS_LENGTH);

    glEnd();

    // 恢复默认线宽
    glLineWidth(1.0f);
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
        Invalidate(FALSE);
    }
    CWnd::OnMouseMove(nFlags, point);
}

BOOL COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    m_zoom += zDelta * 0.001f;
    m_zoom = max(0.1f, min(5.0f, m_zoom));
    Invalidate(FALSE);
    return TRUE;
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()