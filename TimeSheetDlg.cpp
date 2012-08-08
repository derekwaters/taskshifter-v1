#include "stdafx.h"
#include "TimeSheetDlg.h"
#include "IncludedFiles\ljdate.h"

#define	IDC_TIME_SHEET_WND			13100

CTimeSheetDlg::CTimeSheetDlg(TimeBlockVector &avecTimeBlocks, TaskVector &avecTasks, CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSheetDlg::IDD, pParent), m_vecTimeBlocks(avecTimeBlocks), m_vecTasks(avecTasks)
{
}

void CTimeSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTimeSheetDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CTimeSheetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	TimeBlockVector::iterator	vtIter;
	int							viPos = 0;
	CLJDate						vdtTemp;
	CString						vcstrTemp;

	m_ctrlTimeSchedule.Create(&m_vecTimeBlocks, &m_vecTasks, this, IDC_TIME_SHEET_WND);

	ResizeWnd();

	return FALSE;
}

void CTimeSheetDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CTimeSheetDlg::OnOK()
{
	CDialog::OnOK();
}

void CTimeSheetDlg::OnSize(UINT nType, int cx, int cy)
{
	ResizeWnd();
	CDialog::OnSize(nType, cx, cy);
}

bool CTimeSheetDlg::ResizeWnd()
{
	int		viMargin = 5;
	CRect	vrList;
	CRect	vrButton;
	CRect	vrClient;

	if (m_ctrlTimeSchedule.GetSafeHwnd() != NULL)
	{
		GetClientRect(vrClient);
		GetDlgItem(IDOK)->GetWindowRect(vrButton);
		ScreenToClient(vrButton);
		vrButton -= vrButton.TopLeft();

		vrList.left = vrClient.left + viMargin;
		vrList.top = vrClient.top + viMargin;
		vrList.right = vrClient.right - viMargin;
		vrList.bottom = vrClient.bottom - viMargin - vrButton.Height() - viMargin;

		vrButton.OffsetRect(vrClient.Width() - viMargin - vrButton.Width(), vrClient.Height() - viMargin - vrButton.Height());

		m_ctrlTimeSchedule.MoveWindow(vrList);

		GetDlgItem(IDCANCEL)->MoveWindow(vrButton);
		vrButton.OffsetRect(-vrButton.Width() - viMargin, 0);
		GetDlgItem(IDOK)->MoveWindow(vrButton);

	}
	return true;
}