// NavigationTestDoc.cpp : implementation of the CNavigationTestDoc class
//

#include "stdafx.h"
#include "NavigationTest.h"

#include "NavigationTestDoc.h"
#include "CreateNavMeshDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNavigationTestDoc

IMPLEMENT_DYNCREATE(CNavigationTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CNavigationTestDoc, CDocument)
END_MESSAGE_MAP()


// CNavigationTestDoc construction/destruction

CNavigationTestDoc::CNavigationTestDoc()
	: m_hSmartHandle(NULL)
{
	// TODO: add one-time construction code here
	ZeroMemory(&m_stMeshRegion, sizeof(m_stMeshRegion));
}

CNavigationTestDoc::~CNavigationTestDoc()
{
}

BOOL CNavigationTestDoc::OnNewDocument()
{
    CCreateNavMeshDlg CreateNavMeshDlg;
    if (CreateNavMeshDlg.DoModal() != IDOK)
    {
        return FALSE;
    }

    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

    return OnOpenDocument(CreateNavMeshDlg.m_strTempNavFile);
}

BOOL CNavigationTestDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!m_hSmartHandle)
    {
        m_hSmartHandle = Smart_Create("Smart");

        if (!m_hSmartHandle)
        {
            ASSERT(FALSE);
            return FALSE;
        }
    }

    if (!Smart_LoadNavMesh(m_hSmartHandle, lpszPathName))
    {
        ASSERT(FALSE);
        return FALSE;
    }

    Smart_GetMeshRegion(m_hSmartHandle, m_stMeshRegion);

    Smart_GetTriangles(m_hSmartHandle, m_lstTriangles);

    m_bModified = FALSE;

	return TRUE;
}

BOOL CNavigationTestDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    //return CDocument::OnSaveDocument(lpszPathName);

    const unsigned int uiTotalTriangles = (unsigned int)m_lstTriangles.size();
    if (uiTotalTriangles == 0)
    {
        return FALSE;
    }

    // NOTE: WE DO NOT CONSIDER THE DISK SPACE!

    FILE* fp = NULL;
    if (fopen_s(&fp, (LPCSTR)lpszPathName, "w") != 0)
    {
        return FALSE;
    }

    if (m_lstTriangles.front().index[0] < 0) // Just write triangle vertices
    {
        fprintf_s(fp, "NMFT_1.0\n"); // Navigation Mesh File Type
        fprintf_s(fp, "%u\n", m_lstTriangles.size());

        for (auto it = m_lstTriangles.begin(); it != m_lstTriangles.end(); ++it)
        {
            for (int i = 0; i < 3; ++i)
            {
                const POINT3D& pt = it->vertex[i];
                fprintf_s(fp, "%.3f,%.3f,%.3f;\n", pt.x, pt.y, pt.z);
            }
        }

        fprintf_s(fp, "END OF FILE.\n");
    }
    else // Indexed vertices
    {
        fprintf_s(fp, "NMFT_2.0\n");

        int nVertexCount = -1;
        for (auto it = m_lstTriangles.begin(); it != m_lstTriangles.end(); ++it)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (nVertexCount < it->index[i])
                {
                    nVertexCount = it->index[i];
                }
            }
        }
        ++nVertexCount;

        if (nVertexCount > 0)
        {
            POINT3D* pVertices = new POINT3D[nVertexCount];
            ZeroMemory(pVertices, sizeof(POINT3D) * nVertexCount);

            for (auto it = m_lstTriangles.begin(); it != m_lstTriangles.end(); ++it)
            {
                for (int i = 0; i < 3; ++i)
                {
                    pVertices[it->index[i]] = it->vertex[i];
                }
            }

            fprintf_s(fp, "%d\n", nVertexCount);

            for (int i=0; i<nVertexCount; ++i)
            {
                const POINT3D& pt = pVertices[i];
                fprintf_s(fp, "%.3f,%.3f,%.3f;\n", pt.x, pt.y, pt.z);
            }

            fprintf_s(fp, "%u\n", uiTotalTriangles);

            for (auto it = m_lstTriangles.begin(); it != m_lstTriangles.end(); ++it)
            {
                fprintf_s(fp, "%d,%d,%d;\n", it->index[0], it->index[1], it->index[2]);
            }

            fprintf_s(fp, "END OF FILE.\n");

            delete[]pVertices;
            pVertices = NULL;
        }
    }

    fclose(fp);
    fp = NULL;

    return TRUE;
}

void CNavigationTestDoc::OnCloseDocument()
{
	if (m_hSmartHandle)
	{
		Smart_Destroy(m_hSmartHandle);
		m_hSmartHandle = NULL;
	}

	CDocument::OnCloseDocument();
}

BOOL CNavigationTestDoc::IsPointInMeshRegion(float x, float y)const
{
	return (x >= m_stMeshRegion.xmin && x <= m_stMeshRegion.xmax
		&& y >= m_stMeshRegion.ymin && y <= m_stMeshRegion.ymax);
}

BOOL CNavigationTestDoc::IsPointOnNavMesh(const POINT3D& point)const
{
	if (!IsPointInMeshRegion(point.x, point.y))
	{
		return FALSE;
	}

	if (m_hSmartHandle)
	{
        return Smart_IsPointOnNavMesh(m_hSmartHandle, point);
	}

    return FALSE;
}

BOOL CNavigationTestDoc::SearchPath(const POINT3D& ptStart, const POINT3D& ptEnd)
{
	if (m_hSmartHandle)
	{
		for (DWORD i = 0; i < 4; ++i)
		{
			m_lstPathPoints[i].clear();
		}

        //return Smart_SearchPath(m_hSmartHandle, ptStart, ptEnd, m_lstPathPoints[3]);

        return Smart_SearchPathEx(m_hSmartHandle, ptStart, ptEnd, m_lstPathPoints[0],
			m_lstPathPoints[1], m_lstPathPoints[2], m_lstPathPoints[3]);
	}

	return FALSE;
}

// CNavigationTestDoc serialization

void CNavigationTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CNavigationTestDoc diagnostics

#ifdef _DEBUG
void CNavigationTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNavigationTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNavigationTestDoc commands
