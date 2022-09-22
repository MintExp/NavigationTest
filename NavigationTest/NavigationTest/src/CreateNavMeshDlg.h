#pragma once


// CCreateNavMeshDlg dialog

class CCreateNavMeshDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateNavMeshDlg)

public:
	CCreateNavMeshDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCreateNavMeshDlg();

    BOOL CreateNavMesh(LPCTSTR lpszNavMeshFile);


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATENAVMESH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DWORD GetTotalTriangles()const;
	void UpdateTotalTriangles();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditGridRows();
	afx_msg void OnEnChangeEditGridCols();
	afx_msg void OnBnClickedOk();
	DWORD m_dwGridRows;
	DWORD m_dwGridCols;
	DWORD m_dwRemoveTriangles;

	CString m_strTempNavFile;
};
