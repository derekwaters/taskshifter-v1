#if !defined(AFX_TASKLISTDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_)
#define AFX_TASKLISTDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_

#include "resource.h"
#include "TaskShiftDefs.h"

class CTaskListDlg : public CDialog
{
public:
	CTaskListDlg(int &aiCurrentTask, TaskVector &avecTasks, CWnd* pParent = NULL);

	enum { IDD = IDD_TASK_LIST_DLG };

protected:
	CListCtrl	m_ctrlTasks;
	TaskVector	&m_vecTasks;
	CButton		m_ctrlNewTaskBtn;
	CButton		m_ctrlDeleteTaskBtn;
	int			&m_iCurrentTask;
	HBITMAP		m_hbmpNew;
	HBITMAP		m_hbmpDelete;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	void			PopulateTasks();
	void			ResizeControls();

	afx_msg BOOL	OnInitDialog();
	afx_msg void	OnOK();
	afx_msg void	OnCancel();
	afx_msg void	OnNewTask();
	afx_msg void	OnDeleteTask();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnListEndEdit(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TASKLISTDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_)
