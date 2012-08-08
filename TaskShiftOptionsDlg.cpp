#include "stdafx.h"
#include "TaskShiftOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTaskShiftOptionsDlg::CTaskShiftOptionsDlg(TaskVector &avecTasks, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskShiftOptionsDlg::IDD, pParent), m_vecTasks(avecTasks)
{
}


void CTaskShiftOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOURS_TASK_TBTN, m_ctrlShowTaskTimeTbtn);
	DDX_Control(pDX, IDC_HOURS_DAY_TBTN, m_ctrlShowDayTimeTbtn);
	DDX_Control(pDX, IDC_HOURS_WEEK_TBTN, m_ctrlShowWeekTimeTbtn);
	DDX_Control(pDX, IDC_SELECT_DEFAULT_TASK, m_ctrlSelectDefaultTaskTbtn);
	DDX_Control(pDX, IDC_DEFAULT_USE_LAST_TASK, m_ctrlUseLastDefaultTaskTbtn);
	DDX_Control(pDX, IDC_DEFAULT_USE_THIS_TASK, m_ctrlUseSelectedTaskTbtn);
	DDX_Control(pDX, IDC_DEFAULT_TASK_COMBO, m_ctrlDefaultTaskCombo);
	DDX_Control(pDX, IDC_USER_INPUT_WARNING_TBTN, m_ctrlUserInputWarningTbtn);
	DDX_Control(pDX, IDC_TOTAL_DAY_HOURS_WARNING, m_ctrlWarningDayTbtn);
	DDX_Control(pDX, IDC_WARNING_DAY_HOURS_HOURS_EDIT, m_ctrlWarningDayHoursEdit);
	DDX_Control(pDX, IDC_WARNING_DAY_HOURS_MINUTES_EDIT, m_ctrlWarningDayMinutesEdit);
	DDX_Control(pDX, IDC_TOTAL_WEEK_HOURS_WARNING, m_ctrlWarningWeekTbtn);
	DDX_Control(pDX, IDC_WARNING_WEEK_HOURS_HOURS_EDIT, m_ctrlWarningWeekHoursEdit);
	DDX_Control(pDX, IDC_WARNING_WEEK_HOURS_MINUTES_EDIT, m_ctrlWarningWeekMinutesEdit);
	DDX_Control(pDX, IDC_CRASH_PROTECT, m_ctrlCrashProtectionTbtn);
	DDX_Control(pDX, IDC_WARNING_GAP_HOURS_EDIT, m_ctrlCrashProtectionHoursEdit);
	DDX_Control(pDX, IDC_WARNING_GAP_MINUTES_EDIT, m_ctrlCrashProtectionMinutesEdit);
	DDX_Control(pDX, IDC_WARNING_SWITCH_AWAY_FROM_DESK, m_ctrlSwitchAwayFromDeskTbtn);
	DDX_Control(pDX, IDC_WARNING_SWITCH_AWAY_EDIT, m_ctrlSwitchAwayFromDeskEdit);
}

BEGIN_MESSAGE_MAP(CTaskShiftOptionsDlg, CDialog)
	ON_COMMAND(IDC_SELECT_DEFAULT_TASK, OnSelectDefaultTbtn)
	ON_COMMAND(IDC_DEFAULT_USE_LAST_TASK, OnUseLastOrSelectedTbtn)
	ON_COMMAND(IDC_DEFAULT_USE_THIS_TASK, OnUseLastOrSelectedTbtn)
	ON_COMMAND(IDC_TOTAL_DAY_HOURS_WARNING, OnWarningDayTbtn)
	ON_COMMAND(IDC_TOTAL_WEEK_HOURS_WARNING, OnWarningWeekTbtn)
	ON_COMMAND(IDC_CRASH_PROTECT, OnCrashProtectionTbtn)
	ON_COMMAND(IDC_WARNING_SWITCH_AWAY_FROM_DESK, OnSwitchAwayFromDeskTbtn)
END_MESSAGE_MAP()

BOOL CTaskShiftOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString					vcstrLabel;
	int						viTaskId;
	int						viSelected = -1;
	int						viPos;
	TaskVector::iterator	vtIter;
	int						viMinutes;
	int						viSeconds;


	m_ctrlShowTaskTimeTbtn.SetCheck(m_tSettings.GetShowHoursWorkedInTask() ? 1 : 0);
	m_ctrlShowDayTimeTbtn.SetCheck(m_tSettings.GetShowHoursWorkedInDay() ? 1 : 0);
	m_ctrlShowWeekTimeTbtn.SetCheck(m_tSettings.GetShowHoursWorkedInWeek() ? 1 : 0);
	m_ctrlUserInputWarningTbtn.SetCheck(m_tSettings.GetShowWarningOnUserInput() ? 1 : 0);

#ifdef TASKSHIFTER_PROFESSIONAL
	m_ctrlSelectDefaultTaskTbtn.SetCheck(m_tSettings.GetSelectTaskByDefaultAtStartup(viTaskId) ? 1 : 0);

	for (vtIter = (m_vecTasks.begin() + 1); vtIter != m_vecTasks.end(); ++vtIter)
	{
		viPos = m_ctrlDefaultTaskCombo.AddString((*vtIter).strDescription);
		m_ctrlDefaultTaskCombo.SetItemData(viPos, (*vtIter).id);
		if ((*vtIter).id == viTaskId)
		{
			viSelected = viPos;
		}
	}
#else
	m_ctrlSelectDefaultTaskTbtn.SetCheck(m_tSettings.GetSelectTaskByDefaultAtStartup(vcstrLabel) ? 1 : 0);

	for (vtIter = (m_vecTasks.begin() + 1); vtIter != m_vecTasks.end(); ++vtIter)
	{
		viPos = m_ctrlDefaultTaskCombo.AddString((*vtIter).strDescription);
		if ((*vtIter).strDescription.CompareNoCase(vcstrLabel) == 0)
		{
			viSelected = viPos;
		}
	}
#endif

	if (viSelected != -1)
	{
		m_ctrlDefaultTaskCombo.SetCurSel(viSelected);
	}
	m_ctrlUseLastDefaultTaskTbtn.SetCheck(vcstrLabel.IsEmpty());
	m_ctrlUseSelectedTaskTbtn.SetCheck(!vcstrLabel.IsEmpty());

	// Day Time Warning
	viMinutes = 0;
	m_ctrlWarningDayTbtn.SetCheck(m_tSettings.GetShowWarningOnDayMinutesLimit(viMinutes) ? 1 : 0);
	vcstrLabel.Format(_T("%i"), viMinutes / 60);
	m_ctrlWarningDayHoursEdit.SetWindowText(vcstrLabel);
	vcstrLabel.Format(_T("%i"), viMinutes % 60);
	m_ctrlWarningDayMinutesEdit.SetWindowText(vcstrLabel);
		
	// Week Time Warning
	viMinutes = 0;
	m_ctrlWarningWeekTbtn.SetCheck(m_tSettings.GetShowWarningOnWeekMinutesLimit(viMinutes) ? 1 : 0);
	vcstrLabel.Format(_T("%i"), viMinutes / 60);
	m_ctrlWarningWeekHoursEdit.SetWindowText(vcstrLabel);
	vcstrLabel.Format(_T("%i"), viMinutes % 60);
	m_ctrlWarningWeekMinutesEdit.SetWindowText(vcstrLabel);
		
	// Crash Protection
	viMinutes = 0;
	m_ctrlCrashProtectionTbtn.SetCheck(m_tSettings.GetUseCrashProtection(viMinutes) ? 1 : 0);
	vcstrLabel.Format(_T("%i"), viMinutes / 60);
	m_ctrlCrashProtectionHoursEdit.SetWindowText(vcstrLabel);
	vcstrLabel.Format(_T("%i"), viMinutes % 60);
	m_ctrlCrashProtectionMinutesEdit.SetWindowText(vcstrLabel);

	// Switch to "Away From Desk"
	viSeconds = 0;
	m_ctrlSwitchAwayFromDeskTbtn.SetCheck(m_tSettings.GetShowWarningOnNoUserInput(viSeconds) ? 1 : 0);
	vcstrLabel.Format(_T("%i"), viSeconds);
	m_ctrlSwitchAwayFromDeskEdit.SetWindowText(vcstrLabel);

	m_ctrlCrashProtectionHoursEdit.SetLimitText(2);
	m_ctrlCrashProtectionMinutesEdit.SetLimitText(2);
	m_ctrlWarningDayHoursEdit.SetLimitText(2);
	m_ctrlWarningDayMinutesEdit.SetLimitText(2);
	m_ctrlWarningWeekHoursEdit.SetLimitText(2);
	m_ctrlWarningWeekMinutesEdit.SetLimitText(2);
	m_ctrlSwitchAwayFromDeskEdit.SetLimitText(3);

	OnUseLastOrSelectedTbtn();
	OnSelectDefaultTbtn();
	OnWarningDayTbtn();
	OnWarningWeekTbtn();
	OnCrashProtectionTbtn();
	OnSwitchAwayFromDeskTbtn();

	return FALSE;
}

void CTaskShiftOptionsDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CTaskShiftOptionsDlg::OnOK()
{
	CString	vcstrTemp;
	int		viTaskId;
	int		viMinutes;
	CString	vcstrHours;
	CString	vcstrMinutes;
	CString	vcstrSeconds;

	m_tSettings.SetShowHoursWorkedInTask(m_ctrlShowTaskTimeTbtn.GetCheck() != 0);
	m_tSettings.SetShowHoursWorkedInDay(m_ctrlShowDayTimeTbtn.GetCheck() != 0);
	m_tSettings.SetShowHoursWorkedInWeek(m_ctrlShowWeekTimeTbtn.GetCheck() != 0);
	m_tSettings.SetShowWarningOnUserInput(m_ctrlUserInputWarningTbtn.GetCheck() != 0);
	
#ifdef TASKSHIFTER_PROFESSIONAL
	if (m_ctrlSelectDefaultTaskTbtn.GetCheck())
	{
		if (m_ctrlUseLastDefaultTaskTbtn.GetCheck())
		{
			viTaskId = 0;
		}
		else
		{
			viTaskId = m_ctrlDefaultTaskCombo.GetItemData(m_ctrlDefaultTaskCombo.GetCurSel());
		}
		m_tSettings.SetSelectTaskByDefaultAtStartup(true, viTaskId);
	}
	else
	{
		m_tSettings.SetSelectTaskByDefaultAtStartup(false, -1);
	}
#else
	if (m_ctrlSelectDefaultTaskTbtn.GetCheck())
	{
		if (m_ctrlUseLastDefaultTaskTbtn.GetCheck())
		{
			vcstrTemp.Empty();
		}
		else
		{
			m_ctrlDefaultTaskCombo.GetLBText(m_ctrlDefaultTaskCombo.GetCurSel(), vcstrTemp);
		}
		m_tSettings.SetSelectTaskByDefaultAtStartup(true, vcstrTemp);
	}
	else
	{
		m_tSettings.SetSelectTaskByDefaultAtStartup(false, _T(""));
	}
#endif

	m_ctrlWarningDayHoursEdit.GetWindowText(vcstrHours);
	m_ctrlWarningDayMinutesEdit.GetWindowText(vcstrMinutes);
	viMinutes = (_ttoi(vcstrHours) * 60) + _ttoi(vcstrMinutes);
	m_tSettings.SetShowWarningOnDayMinutesLimit(m_ctrlWarningDayTbtn.GetCheck() != 0, viMinutes);

	m_ctrlWarningWeekHoursEdit.GetWindowText(vcstrHours);
	m_ctrlWarningWeekMinutesEdit.GetWindowText(vcstrMinutes);
	viMinutes = (_ttoi(vcstrHours) * 60) + _ttoi(vcstrMinutes);
	m_tSettings.SetShowWarningOnWeekMinutesLimit(m_ctrlWarningWeekTbtn.GetCheck() != 0, viMinutes);

	m_ctrlCrashProtectionHoursEdit.GetWindowText(vcstrHours);
	m_ctrlCrashProtectionMinutesEdit.GetWindowText(vcstrMinutes);
	viMinutes = (_ttoi(vcstrHours) * 60) + _ttoi(vcstrMinutes);
	m_tSettings.SetUseCrashProtection(m_ctrlCrashProtectionTbtn.GetCheck() != 0, viMinutes);

	m_ctrlSwitchAwayFromDeskEdit.GetWindowText(vcstrSeconds);
	m_tSettings.SetShowWarningOnNoUserInput(m_ctrlSwitchAwayFromDeskTbtn.GetCheck() != 0, _ttoi(vcstrSeconds));

	m_tSettings.Save();

	CDialog::OnOK();
}

void CTaskShiftOptionsDlg::OnSelectDefaultTbtn()
{
	if (m_ctrlSelectDefaultTaskTbtn.GetCheck() != 0)
	{
		m_ctrlUseLastDefaultTaskTbtn.EnableWindow();
		m_ctrlUseSelectedTaskTbtn.EnableWindow();
		OnUseLastOrSelectedTbtn();
	}
	else
	{
		m_ctrlUseLastDefaultTaskTbtn.EnableWindow(FALSE);
		m_ctrlUseSelectedTaskTbtn.EnableWindow(FALSE);
		m_ctrlDefaultTaskCombo.EnableWindow(FALSE);
	}
}

void CTaskShiftOptionsDlg::OnUseLastOrSelectedTbtn()
{
	m_ctrlDefaultTaskCombo.EnableWindow(m_ctrlUseLastDefaultTaskTbtn.GetCheck() == 0);
}

void CTaskShiftOptionsDlg::OnWarningDayTbtn()
{
	m_ctrlWarningDayHoursEdit.EnableWindow(m_ctrlWarningDayTbtn.GetCheck() != 0);
	m_ctrlWarningDayMinutesEdit.EnableWindow(m_ctrlWarningDayTbtn.GetCheck() != 0);
}

void CTaskShiftOptionsDlg::OnWarningWeekTbtn()
{
	m_ctrlWarningWeekHoursEdit.EnableWindow(m_ctrlWarningWeekTbtn.GetCheck() != 0);
	m_ctrlWarningWeekMinutesEdit.EnableWindow(m_ctrlWarningWeekTbtn.GetCheck() != 0);
}

void CTaskShiftOptionsDlg::OnCrashProtectionTbtn()
{
	m_ctrlCrashProtectionHoursEdit.EnableWindow(m_ctrlCrashProtectionTbtn.GetCheck() != 0);
	m_ctrlCrashProtectionMinutesEdit.EnableWindow(m_ctrlCrashProtectionTbtn.GetCheck() != 0);
}

void CTaskShiftOptionsDlg::OnSwitchAwayFromDeskTbtn()
{
	m_ctrlSwitchAwayFromDeskEdit.EnableWindow(m_ctrlSwitchAwayFromDeskTbtn.GetCheck() != 0);
}