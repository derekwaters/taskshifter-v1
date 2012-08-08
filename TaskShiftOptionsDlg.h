#if !defined(AFX_OPTIONSDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_)
#define AFX_OPTIONSDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_

#include "resource.h"
#include "TaskShiftDefs.h"

class CTaskShiftOptionsDlg : public CDialog
{
public:
	CTaskShiftOptionsDlg(TaskVector &avecTasks, CWnd* pParent = NULL);

	enum { IDD = IDD_OPTIONS_DLG };

protected:
	TaskVector			&m_vecTasks;
	CButton				m_ctrlShowTaskTimeTbtn;
	CButton				m_ctrlShowDayTimeTbtn;
	CButton				m_ctrlShowWeekTimeTbtn;

	CButton				m_ctrlSelectDefaultTaskTbtn;
	CButton				m_ctrlUseLastDefaultTaskTbtn;
	CButton				m_ctrlUseSelectedTaskTbtn;
	CComboBox			m_ctrlDefaultTaskCombo;

	CButton				m_ctrlUserInputWarningTbtn;

	CButton				m_ctrlWarningDayTbtn;
	CEdit				m_ctrlWarningDayHoursEdit;
	CEdit				m_ctrlWarningDayMinutesEdit;

	CButton				m_ctrlWarningWeekTbtn;
	CEdit				m_ctrlWarningWeekHoursEdit;
	CEdit				m_ctrlWarningWeekMinutesEdit;

	CButton				m_ctrlCrashProtectionTbtn;
	CEdit				m_ctrlCrashProtectionHoursEdit;
	CEdit				m_ctrlCrashProtectionMinutesEdit;

	CButton				m_ctrlSwitchAwayFromDeskTbtn;
	CEdit				m_ctrlSwitchAwayFromDeskEdit;

	CTaskShiftSettings	m_tSettings;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	afx_msg BOOL	OnInitDialog();
	afx_msg void	OnOK();
	afx_msg void	OnCancel();
	afx_msg void	OnSelectDefaultTbtn();
	afx_msg void	OnUseLastOrSelectedTbtn();
	afx_msg void	OnWarningDayTbtn();
	afx_msg void	OnWarningWeekTbtn();
	afx_msg void	OnCrashProtectionTbtn();
	afx_msg void	OnSwitchAwayFromDeskTbtn();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TASKLISTDLG_H__01E8FDB4_3A2B_4AD5_AD69_02527CFD0066__INCLUDED_)
