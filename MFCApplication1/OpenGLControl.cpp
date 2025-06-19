#include "pch.h"
#include "OpenGLControl.h"

COpenGLControl::COpenGLControl()
    : m_hRC(NULL), m_pDC(NULL),
    m_bDragging(false),
    m_rotX(30.0f), m_rotY(0.0f), m_rotZ(0.0f),
    m_zoom(1.0f), m_transX(0.0f), m_transY(0.0f),
    m_surfaceDL(0) {}

COpenGLControl::~COpenGLControl() {
    if (m_hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_hRC);
    }
    if (m_pDC) delete m_pDC;
    if (m_surfaceDL) glDeleteLists(m_surfaceDL, 1);
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
    GenerateSurfaceData(50, 50);

    m_surfaceDL = glGenLists(1);
    glNewList(m_surfaceDL, GL_COMPILE);
    DrawSurface();
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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glFrontFace(GL_CCW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
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

    if (m_surfaceDL) glCallList(m_surfaceDL);

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

void COpenGLControl::GenerateSurfaceData(int width, int height) {
    m_surfacePoints.resize(height);
    for (int i = 0; i < height; ++i) {
        m_surfacePoints[i].resize(width);
        for (int j = 0; j < width; ++j) {
            double x = (j - width / 2.0) / 10.0;
            double y = (i - height / 2.0) / 10.0;
            m_surfacePoints[i][j] = Point3D(x, y, sin(sqrt(x * x + y * y)) * 2.0);
        }
    }
}

void COpenGLControl::DrawSurface() {
    int height = m_surfacePoints.size();
    if (height < 2) return;
    int width = m_surfacePoints[0].size();
    if (width < 2) return;

    GLfloat matAmbient[] = { 0.2f, 0.2f, 0.5f, 1.0f };
    GLfloat matDiffuse[] = { 0.5f, 0.5f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);

    for (int i = 0; i < height - 1; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < width; ++j) {
            Point3D p1 = m_surfacePoints[i][j];
            Point3D p2 = m_surfacePoints[i + 1][j];
            Point3D p3 = (j < width - 1) ? m_surfacePoints[i][j + 1] : p1;

            Point3D u(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
            Point3D v(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

            Point3D normal;
            normal.x = u.y * v.z - u.z * v.y;
            normal.y = u.z * v.x - u.x * v.z;
            normal.z = u.x * v.y - u.y * v.x;

            double len = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
            if (len > 0) {
                normal.x /= len;
                normal.y /= len;
                normal.z /= len;
            }

            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
        }
        glEnd();
    }
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
            // CTRL+拖动：平移
            m_transX += offset.cx * 0.01f;
            m_transY -= offset.cy * 0.01f;
        }
        else {
            // 普通拖动：旋转
            m_rotY += offset.cx * 0.5f;
            m_rotX += offset.cy * 0.5f;
        }

        m_ptLastPos = point;
        Invalidate(FALSE);
    }
    CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    // 鼠标滚轮缩放
    m_zoom += zDelta * 0.001f;
    m_zoom = max(0.1f, min(5.0f, m_zoom));
    Invalidate(FALSE);

    CWnd::OnMouseWheel(nFlags, zDelta, pt);
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