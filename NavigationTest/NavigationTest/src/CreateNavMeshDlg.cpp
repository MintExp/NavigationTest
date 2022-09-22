// CreateNavMeshDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavigationTest.h"
#include "CreateNavMeshDlg.h"
#include "afxdialogex.h"
#include <vector>


// CCreateNavMeshDlg dialog

IMPLEMENT_DYNAMIC(CCreateNavMeshDlg, CDialog)

CCreateNavMeshDlg::CCreateNavMeshDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CREATENAVMESH_DIALOG, pParent)
	, m_dwGridRows(10)
    , m_dwGridCols(10)
	, m_dwRemoveTriangles(30)
{

}

CCreateNavMeshDlg::~CCreateNavMeshDlg()
{
}

void CCreateNavMeshDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GRID_ROWS, m_dwGridRows);
	DDV_MinMaxUInt(pDX, m_dwGridRows, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_GRID_COLS, m_dwGridCols);
	DDV_MinMaxUInt(pDX, m_dwGridCols, 1, 1000);
	DDX_Text(pDX, IDC_EDIT_REMOVE_TRIANGLES, m_dwRemoveTriangles);
}


BEGIN_MESSAGE_MAP(CCreateNavMeshDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_GRID_ROWS, &CCreateNavMeshDlg::OnEnChangeEditGridRows)
	ON_EN_CHANGE(IDC_EDIT_GRID_COLS, &CCreateNavMeshDlg::OnEnChangeEditGridCols)
	ON_BN_CLICKED(IDOK, &CCreateNavMeshDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateNavMeshDlg message handlers


void CCreateNavMeshDlg::OnEnChangeEditGridRows()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateTotalTriangles();
}

void CCreateNavMeshDlg::OnEnChangeEditGridCols()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
	UpdateTotalTriangles();
}

DWORD CCreateNavMeshDlg::GetTotalTriangles()const
{
    const DWORD dwRowTriangles = m_dwGridCols * 2;

    return dwRowTriangles * m_dwGridRows;
}

void CCreateNavMeshDlg::UpdateTotalTriangles()
{
    UpdateData(TRUE);

	CString strTriangles;
	strTriangles.Format(_T("Total Triangles: %u"), GetTotalTriangles());

	GetDlgItem(IDC_TOTAL_TRIANGLES_STATIC)->SetWindowText(strTriangles);
}

void CCreateNavMeshDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	
	if (m_dwRemoveTriangles >= GetTotalTriangles())
	{
		MessageBox("Remove too many triangles!", "Create Navigation Mesh", MB_ICONSTOP);
		return;
	}

    TCHAR szTempPath[MAX_PATH], szTempFile[MAX_PATH];
    if (GetTempPath(MAX_PATH, szTempPath) && GetTempFileName(szTempPath, "Nav", 0, szTempFile))
    {
		//m_strTempNavFile = CString("E:\\Temp.txt");
		m_strTempNavFile = szTempFile;

		if (CreateNavMesh(m_strTempNavFile))
		{
            CDialog::OnOK();
			return;
		}
    }

	CDialog::OnCancel();
}

BOOL CCreateNavMeshDlg::CreateNavMesh(LPCTSTR lpszNavMeshFile)
{
	if (!lpszNavMeshFile || !lpszNavMeshFile[0])
	{
		return FALSE;
	}

	if (m_dwGridRows <= 0 || m_dwGridCols <= 0)
	{
		return FALSE;
	}

    const DWORD dwVertVertexCount = m_dwGridRows + 1;
    const DWORD dwHorzVertexCount = m_dwGridCols + 1;
    const DWORD dwVertextCount = dwVertVertexCount * dwHorzVertexCount;
    const DWORD dwRowTriangles = m_dwGridCols * 2;
    const DWORD dwTriangleCount = dwRowTriangles * m_dwGridRows;

	if (m_dwRemoveTriangles >= dwTriangleCount)
	{
		return FALSE;
	}

    std::vector<DWORD>	arrTriIndices;

	arrTriIndices.reserve(dwTriangleCount);

    for (DWORD i = 0; i < dwTriangleCount; ++i)
    {
        arrTriIndices.push_back(i);
    }

    for (DWORD i = 0; i < m_dwRemoveTriangles; ++i)
    {
        arrTriIndices.erase(arrTriIndices.begin() + (rand() % arrTriIndices.size()));
    }

	FILE* fp = NULL;

	if (fopen_s(&fp, lpszNavMeshFile, "w") != 0)
	{
		return FALSE;
	}

    fprintf_s(fp, "NMFT_2.0\n");

	/*
	* Mesh Vertex Table Example (Rows=5, Cols=4) :
	* 25 26 27 28 29
	* 20 21 22 23 24
	* 15 16 17 18 19
	* 10 11 12 13 14
	*  5  6  7  8  9
	*  0  1  2  3  4
	*/

	const float fGridSizeX = 1.0f;
	const float fGridSizeY = 1.0f;

	const float fOrgX = 0.0f;
	const float fOrgY = 0.0f;

	fprintf_s(fp, "%u\n", dwVertextCount);

	BOOL bRandomSize = IsDlgButtonChecked(IDC_RANDOM_GRID_SIZE_CHECK);

	if (bRandomSize)
	{
        const float fRandRange = 0.35f;

        for (DWORD i = 0; i <= m_dwGridRows; ++i)
        {
            for (DWORD j = 0; j <= m_dwGridCols; ++j)
            {
				const float x = fOrgX + (j + Ranged_Random(-fRandRange, fRandRange)) * fGridSizeX;
				const float y = fOrgY + (i + Ranged_Random(-fRandRange, fRandRange)) * fGridSizeY;

                fprintf_s(fp, "%.3f,%.3f,%.3f;\n", x, y, 0.0f);
            }
        }
	}
	else
	{
        for (DWORD i = 0; i <= m_dwGridRows; ++i)
        {
			const float y = fOrgY + (float)i * fGridSizeY;

            for (DWORD j = 0; j <= m_dwGridCols; ++j)
            {
                const float x = fOrgX + (float)j * fGridSizeX;
                fprintf_s(fp, "%.3f,%.3f,%.3f;\n", x, y, 0.0f);
            }
        }
	}

    fprintf_s(fp, "%u\n", (DWORD)arrTriIndices.size());

	DWORD dwIndex[3];

    for (auto it = arrTriIndices.begin(); it != arrTriIndices.end(); ++it)
    {
		const DWORD dwTriIndex = *it;

		// Row and column index of this triangle
        const DWORD dwRowIndex = dwTriIndex / dwRowTriangles;
        const DWORD dwColIndex = dwTriIndex % dwRowTriangles;

        /* Bottom Left Vertex Index Of Current Grid, every grid has two triangles,
		* 5--6
		* |\ |
		* | \|
		* 0--1
		* For example, grid above has two triangles: 501(Left Bottom) and 516(Right Top).
		*/
		const DWORD dwLBVtxIdx = dwHorzVertexCount * dwRowIndex + dwColIndex / 2;

        if (dwColIndex & 0x01) // The right side triangle of the grid
        {
            dwIndex[0] = dwLBVtxIdx + dwHorzVertexCount;
            dwIndex[1] = dwLBVtxIdx + 1;
            dwIndex[2] = dwIndex[0] + 1;
        }
        else
        {
			dwIndex[0] = dwLBVtxIdx + dwHorzVertexCount;
            dwIndex[1] = dwLBVtxIdx;
            dwIndex[2] = dwIndex[1] + 1;
        }

        fprintf_s(fp, "%u,%u,%u;\n", dwIndex[0], dwIndex[1], dwIndex[2]);
    }

	fclose(fp);
	fp = NULL;

	return TRUE;
}