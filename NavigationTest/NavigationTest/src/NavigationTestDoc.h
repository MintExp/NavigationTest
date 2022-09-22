// NavigationTestDoc.h : interface of the CNavigationTestDoc class
//

#pragma once

#include "SmartPathExport.h"

class CNavigationTestDoc : public CDocument
{
protected: // create from serialization only
	CNavigationTestDoc();
	DECLARE_DYNCREATE(CNavigationTestDoc)

// Attributes
private:
	TRIANGLE3D_LIST	m_lstTriangles;
	REGION2D		m_stMeshRegion;

	POINT3D_LIST	m_lstPathPoints[VISIBLE_PATH_COUNT];

// Operations
public:
	const TRIANGLE3D_LIST& GetTriangles()const { return m_lstTriangles; }
	const REGION2D& GetNavMeshRegion()const { return m_stMeshRegion; }
	const POINT3D_LIST& GetPathPointList(DWORD dwIndex)const { return m_lstPathPoints[dwIndex]; }

	BOOL IsPointInMeshRegion(float x, float y)const;
	BOOL IsPointOnNavMesh(const POINT3D& point)const;

	BOOL SearchPath(const POINT3D& ptStart, const POINT3D& ptEnd);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CNavigationTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

private:
	SMART_HANDLE	m_hSmartHandle;
};


