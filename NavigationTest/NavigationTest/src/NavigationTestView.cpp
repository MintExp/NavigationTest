// NavigationTestView.cpp : implementation of the CNavigationTestView class
//

#include "stdafx.h"
#include "NavigationTest.h"

#include "NavigationTestDoc.h"
#include "NavigationTestView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const static DWORD POLY_LIST_TRIANGLES = 1024;

static POINT s_ptTriangleVertices[POLY_LIST_TRIANGLES * 3];
static int s_nTrianglePolyList[POLY_LIST_TRIANGLES] = { 0 };

static COLORREF	s_clrNavPath[4] =
{
    RGB(0x80, 0x80, 0x80),
    RGB(0x50, 0x50, 0xF0),
    RGB(0x00, 0xC0, 0x00),
    RGB(0xE0, 0x00, 0x00),
};

// CNavigationTestView
IMPLEMENT_DYNCREATE(CNavigationTestView, CScrollView)

BEGIN_MESSAGE_MAP(CNavigationTestView, CScrollView)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONUP()
    ON_COMMAND_RANGE(ID_VIEW_PATH_0, ID_VIEW_PATH_3, OnShowNavPath)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_PATH_0, ID_VIEW_PATH_3, OnUpdateShowNavPath)
    ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateSaveFile)
END_MESSAGE_MAP()

// CNavigationTestView construction/destruction

CNavigationTestView::CNavigationTestView()
	: m_fMeshScale(1.0f)
    , m_bPathStartOK(FALSE)
    , m_bShowPath(TRUE)
{
	// TODO: add construction code here
    m_nBorderSpaceLeft = 35;
    m_nBorderSpaceTop = 50;
    m_nBorderSpaceRight = 35;
    m_nBorderSpaceBottom = 35;

    m_ptPathStart.x = 0.0f;
    m_ptPathStart.y = 0.0f;
    m_ptPathStart.z = 0.0f;

    for (DWORD i = 0; i < VISIBLE_PATH_COUNT; ++i)
    {
        m_bPathVisible[i] = TRUE;
    }

    if (!s_nTrianglePolyList[0])
    {
        for (int i = 0; i < POLY_LIST_TRIANGLES; ++i)
        {
            s_nTrianglePolyList[i] = 3;
        }
    }
}

CNavigationTestView::~CNavigationTestView()
{
}

BOOL CNavigationTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

void CNavigationTestView::GetNavMeshViewPoint(float x, float y, POINT& pt)
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    const REGION2D& stMeshRegion = pDoc->GetNavMeshRegion();

    // NOTICE: OnMouseMove(UINT nFlags, CPoint point)
    ASSERT(x >= stMeshRegion.xmin && x <= stMeshRegion.xmax);
	pt.x = (LONG)((x - stMeshRegion.xmin) * m_fMeshScale + 0.5f) + m_nBorderSpaceLeft;

    ASSERT(y >= stMeshRegion.ymin && y <= stMeshRegion.ymax);
    pt.y = (LONG)((stMeshRegion.ymax - y) * m_fMeshScale + 0.5f) + m_nBorderSpaceTop;
}

// CNavigationTestView drawing
void CNavigationTestView::OnDraw(CDC* pDC)
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    DrawNavMesh(pDC);

    if (m_bShowPath)
    {
        for (DWORD i = 0; i < VISIBLE_PATH_COUNT; ++i)
        {
            if (m_bPathVisible[i])
            {
                DrawNavPath(pDC, pDoc->GetPathPointList(i), s_clrNavPath[i]);
            }
        }
    }
    
    if (m_bPathStartOK)
    {
        DrawPickedPathStartPoint(pDC);
    }
}

void CNavigationTestView::DrawNavMesh(CDC* pDC)
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    const TRIANGLE3D_LIST& lstTriangles = pDoc->GetTriangles();

    // Draw information
    CFont fontInfo;
    fontInfo.CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "Times New Roman");

    CFont* pOldFont = pDC->SelectObject(&fontInfo);

    pDC->SetTextAlign(TA_LEFT);
    pDC->SetTextColor(RGB(0x00, 0x00, 0x00));

    CString strTriangleCount;
    strTriangleCount.Format("Triangle Count: %u", (DWORD)lstTriangles.size());

    pDC->TextOut(m_nBorderSpaceLeft, 15, strTriangleCount);

    pDC->SelectObject(pOldFont);

    const REGION2D& stMeshRegion = pDoc->GetNavMeshRegion();

    // Draw frame
    const int frame_width = 3;
    CPen penFrame(PS_SOLID, frame_width, RGB(0x00, 0x00, 0x00));
    CPen* pOldPen = pDC->SelectObject(&penFrame);

    const int x1 = m_nBorderSpaceLeft - frame_width - 1;
    const int y1 = m_nBorderSpaceTop - frame_width - 1;
    const int x2 = x1 + (int)((stMeshRegion.xmax - stMeshRegion.xmin) * m_fMeshScale + 0.5f) + frame_width * 2 + 2;
    const int y2 = y1 + (int)((stMeshRegion.ymax - stMeshRegion.ymin) * m_fMeshScale + 0.5f) + frame_width * 2 + 2;

    pDC->Rectangle(x1, y1, x2, y2);

    // Draw triangles
    CPen penTriangle(PS_SOLID, 1, RGB(0xC0, 0xC0, 0xC0));
    //CPen penTriangle(PS_SOLID, 1, RGB(0x60, 0x60, 0xC0));
    pDC->SelectObject(&penTriangle);

    CBrush brushTriangle;
    brushTriangle.CreateSolidBrush(RGB(0xE0, 0xE0, 0xE0));
    //brushTriangle.CreateSolidBrush(RGB(0xCC, 0xCC, 0xCC));
    CBrush* pOldBrush = pDC->SelectObject(&brushTriangle);

    unsigned int uiTriCount = 0;

    for (auto it = lstTriangles.begin(); it != lstTriangles.end(); ++it)
    {
        for (int i = 0; i < 3; ++i)
        {
            GetNavMeshViewPoint(it->vertex[i].x, it->vertex[i].y, s_ptTriangleVertices[3 * uiTriCount + i]);
        }

        if (++uiTriCount == POLY_LIST_TRIANGLES)
        {
            pDC->PolyPolygon(s_ptTriangleVertices, s_nTrianglePolyList, uiTriCount);
            uiTriCount = 0;
        }
    }

    if (uiTriCount > 0)
    {
        pDC->PolyPolygon(s_ptTriangleVertices, s_nTrianglePolyList, uiTriCount);
        uiTriCount = 0;
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CNavigationTestView::DrawPickedPathStartPoint(CDC* pDC)
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    ASSERT(pDoc->IsPointInMeshRegion(m_ptPathStart.x, m_ptPathStart.y));

    // TODO: add draw code for native data here
    const COLORREF clr = RGB(250, 0, 0);
    const int PEN_WIDTH = 1;
    CPen pen(PS_SOLID, PEN_WIDTH, clr);
    CPen* pOldPen = pDC->SelectObject(&pen);

    POINT pt;
    GetNavMeshViewPoint(m_ptPathStart.x, m_ptPathStart.y, pt);

    const LONG P_SIZE = 5;

    pDC->MoveTo(pt.x - P_SIZE, pt.y);
    pDC->LineTo(pt.x + P_SIZE + PEN_WIDTH, pt.y);

    pDC->MoveTo(pt.x, pt.y - P_SIZE);
    pDC->LineTo(pt.x, pt.y + P_SIZE + PEN_WIDTH);

    pDC->SelectObject(pOldPen);
}

void CNavigationTestView::DrawNavPath(CDC* pDC, const POINT3D_LIST& lstPathPoints, COLORREF clr)
{
    if (lstPathPoints.empty())
    {
        return;
    }

    // TODO: add draw code for native data here
    CPen pen(PS_SOLID, 1, clr);
    CPen* pOldPen = pDC->SelectObject(&pen);

    CBrush brush;
    brush.CreateSolidBrush(clr);
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    const LONG P_SIZE = 3;

    POINT pt;

    auto it = lstPathPoints.begin();
    GetNavMeshViewPoint(it->x, it->y, pt);

    // Draw start point of path
    pDC->Ellipse(pt.x - P_SIZE, pt.y - P_SIZE, pt.x + P_SIZE, pt.y + P_SIZE);
    pDC->MoveTo(pt);

    for (++it; it != lstPathPoints.end(); ++it)
    {
        GetNavMeshViewPoint(it->x, it->y, pt);
        pDC->LineTo(pt);
    }

    // Draw end point of path
    pDC->Ellipse(pt.x - P_SIZE, pt.y - P_SIZE, pt.x + P_SIZE, pt.y + P_SIZE);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}

void CNavigationTestView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    const REGION2D& stMeshRegion = pDoc->GetNavMeshRegion();
    const float w = stMeshRegion.xmax - stMeshRegion.xmin;
    const float h = stMeshRegion.ymax - stMeshRegion.ymin;

    const float fComfortableSize = 800.0f;

    if (w > 0 && h > 0 && w < fComfortableSize && h < fComfortableSize)
    {
        m_fMeshScale = MaxVal2(fComfortableSize / w, fComfortableSize / h);

        unsigned int uiScale = (unsigned int)(m_fMeshScale * 100.0f) + 24;
        uiScale /= 25;
        uiScale *= 25;
        m_fMeshScale = (float)uiScale * 0.01f;
    }
    else
    {
        m_fMeshScale = 1.0f;
    }

    UpdateScrollRange();
}

void CNavigationTestView::UpdateScrollRange()
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    const REGION2D& stMeshRegion = pDoc->GetNavMeshRegion();

    CSize sizeTotal;
    sizeTotal.cx = (LONG)((stMeshRegion.xmax - stMeshRegion.xmin) * m_fMeshScale) + m_nBorderSpaceLeft + m_nBorderSpaceRight;
    sizeTotal.cy = (LONG)((stMeshRegion.ymax - stMeshRegion.ymin) * m_fMeshScale) + m_nBorderSpaceRight + m_nBorderSpaceBottom;
    SetScrollSizes(MM_TEXT, sizeTotal);

    CString str;
    str.Format("%u%%", (int)(m_fMeshScale * 100.0f));
    ((CMainFrame*)AfxGetApp()->GetMainWnd())->UpdateScaleInfo(str);
}

// CNavigationTestView diagnostics

#ifdef _DEBUG
void CNavigationTestView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CNavigationTestView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CNavigationTestDoc* CNavigationTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNavigationTestDoc)));
	return (CNavigationTestDoc*)m_pDocument;
}
#endif //_DEBUG

// Get matched mesh point from mouse position
POINT3D CNavigationTestView::GetMeshPointByMousePos(const CPoint& point)const
{
    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    const REGION2D& stMeshRegion = pDoc->GetNavMeshRegion();

    const CPoint scroll_pos = GetScrollPosition();

    POINT3D pt;
    pt.x = ((float)(point.x + scroll_pos.x - m_nBorderSpaceLeft)) / m_fMeshScale + stMeshRegion.xmin;
    pt.y = stMeshRegion.ymax - ((float)(point.y + scroll_pos.y - m_nBorderSpaceTop)) / m_fMeshScale;
    pt.z = 0.0f; // Omit z

    return pt;
}

// CNavigationTestView message handlers
void CNavigationTestView::OnMouseMove(UINT nFlags, CPoint point)
{
    // NOTICE: GetNavMeshViewPoint(float x, float y, POINT& pt)
    const POINT3D ptMesh = GetMeshPointByMousePos(point);

    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    CString str;

    if (pDoc->IsPointInMeshRegion(ptMesh.x, ptMesh.y))
    {
        str.Format("%.3f, %.3f", ptMesh.x, ptMesh.y);
    }

    ((CMainFrame*)AfxGetApp()->GetMainWnd())->UpdateCursorPosInfo(str);

    CScrollView::OnMouseMove(nFlags, point);
}

BOOL CNavigationTestView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
    if (nFlags & MK_CONTROL)
    {
        m_fMeshScale += ((float)(zDelta / 120)) * 0.25f;
        if (m_fMeshScale < 0.25f)
        {
            m_fMeshScale = 0.25f;
        }
        else if (m_fMeshScale > 100.0f)
        {
            m_fMeshScale = 100.0f;
        }

        UpdateScrollRange();

        InvalidateRect(NULL);
        UpdateWindow();

        return TRUE;
    }

    return CScrollView::OnMouseWheel(nFlags, zDelta, point);
}

void CNavigationTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
    const POINT3D pt = GetMeshPointByMousePos(point);

    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (pDoc->IsPointOnNavMesh(pt))
    {
        m_ptPathStart = pt;
        m_bPathStartOK = TRUE;

        InvalidateRect(NULL);
        UpdateWindow();
    }
}

void CNavigationTestView::OnRButtonUp(UINT nFlags, CPoint point)
{
    if (!m_bPathStartOK)
    {
        return;
    }

    const POINT3D pt = GetMeshPointByMousePos(point);

    CNavigationTestDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    if (pDoc->IsPointOnNavMesh(pt))
    {
        if (pDoc->SearchPath(m_ptPathStart, pt))
        {
            m_bShowPath = TRUE;

            InvalidateRect(NULL);
            UpdateWindow();
        }
    }
}

void CNavigationTestView::OnShowNavPath(UINT uICmdID)
{
    const int nIndex = uICmdID - ID_VIEW_PATH_0;
    m_bPathVisible[nIndex] = !m_bPathVisible[nIndex];

    InvalidateRect(NULL);
    UpdateWindow();
}

void CNavigationTestView::OnUpdateShowNavPath(CCmdUI* pCmdUI)
{
    const int nIndex = pCmdUI->m_nID - ID_VIEW_PATH_0;
    pCmdUI->SetCheck(m_bPathVisible[nIndex]);
}

void CNavigationTestView::OnUpdateSaveFile(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(GetDocument()->IsModified());
}