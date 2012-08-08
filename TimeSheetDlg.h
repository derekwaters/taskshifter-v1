#if !defined(AFX_TIMESHEETDLG_H__C1FC14AF_ACDB_471B_B1FC_B8CAAECF4140__INCLUDED_)
#define AFX_TIMESHEETDLG_H__C1FC14AF_ACDB_471B_B1FC_B8CAAECF4140__INCLUDED_

#include "resource.h"
// #include "TaskShiftDefs.h"
#include "TaskShiftScheduleWnd.h"

class CTimeSheetDlg : public CDialog
{
public:
	CTimeSheetDlg(TimeBlockVector &avecTimeBlocks, TaskVector &avecTasks, CWnd* pParent = NULL);

	enum { IDD = IDD_TIME_SHEET_DLG };

protected:
	CTaskShiftScheduleWnd	m_ctrlTimeSchedule;
	TimeBlockVector			&m_vecTimeBlocks;
	TaskVector				&m_vecTasks;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
	bool			ResizeWnd();

	afx_msg BOOL	OnInitDialog();
	afx_msg void	OnOK();
	afx_msg void	OnCancel();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnListEndEdit(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TIMESHEETDLG_H__C1FC14AF_ACDB_471B_B1FC_B8CAAECF4140__INCLUDED_)
