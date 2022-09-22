// NavigationTestView.h : interface of the CNavigationTestView class
//


#pragma once


#include "SmartDefine.h"


class CNavigationTestView : public CScrollView
{
protected: // create from serialization only
	CNavigationTestView();
	DECLARE_DYNCREATE(CNavigationTestView)

// Attributes
public:
	CNavigationTestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

	void DrawNavMesh(CDC* pDC);
    void DrawPickedPathStartPoint(CDC* pDC);

	void DrawNavPath(CDC* pDC, const POINT3D_LIST& lstPathPoints, COLORREF clr);

// Implementation
public:
	virtual ~CNavigationTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int	    m_nBorderSpaceLeft;
	int	    m_nBorderSpaceTop;
	int	    m_nBorderSpaceRight;
    int	    m_nBorderSpaceBottom;
	float	m_fMeshScale;

	BOOL	m_bPathStartOK;
	POINT3D	m_ptPathStart;

	BOOL	m_bShowPath;
	BOOL	m_bPathVisible[4];

protected:
	void	UpdateScrollRange();
	void	GetNavMeshViewPoint(float x, float y, POINT& pt);

	POINT3D GetMeshPointByMousePos(const CPoint& point)const;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowNavPath(UINT uICmdID);
    afx_msg void OnUpdateShowNavPath(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveFile(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in NavigationTestView.cpp
inline CNavigationTestDoc* CNavigationTestView::GetDocument() const
   { return reinterpret_cast<CNavigationTestDoc*>(m_pDocument); }
#endif

