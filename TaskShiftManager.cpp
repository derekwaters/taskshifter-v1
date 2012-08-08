// TaskShiftManager.cpp: implementation of the CTaskShiftManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskShifter.h"
#include "TaskShiftManager.h"
#include "NewTaskDlg.h"
#include "TaskListDlg.h"
#include "TimeSheetDlg.h"
#include "TaskShiftOptionsDlg.h"
#include "TaskShiftUtils.h"
#include "TaskShiftExportOptionsDlg.h"
#include "IncludedFiles\LJDate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	ID_SHIFTER_NEW_TASK				12001
#define	ID_SHIFTER_TASK_FIRST			12050
#define	ID_SHIFTER_TASK_LAST			13000
#define	ID_SHIFTER_EXPORT_THIS_WEEK		13001
#define	ID_SHIFTER_EDIT_TASKS			13002
#define	ID_SHIFTER_EDIT_TIMESHEET		13003
#define	ID_SHIFTER_OPTIONS				13004
#define	ID_SHIFTER_EXPORT_OPTIONS		13005
#define	ID_SHIFTER_SUBMIT_TIMESHEET		13006
#define	ID_SHIFTER_EXPORT_PRIOR_FIRST	13050
#define	ID_SHIFTER_EXPORT_PRIOR_LAST	13099



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskShiftManager::CTaskShiftManager()
{
	// Initialise the tasks vector
	//
	m_iCurrentTask = -1;	// No task.

#ifdef TASKSHIFTER_PROFESSIONAL
	soapServer.Connect(m_tSettings.GetServerUrl(), m_vecTasks, NULL);
#else
	m_vecTasks.Load();
#endif
	m_vecTimeBlocks.Load();

	m_iMinuteUpdaterEvent = -1;
	m_iInputCheckEvent = -1;

	m_iAnimationState = 0;

	m_dwSetToAwayFromDeskTime = GetTickCount();

	TRACE("TASKSHIFTER -> 0x%08X\n", this);
}

CTaskShiftManager::~CTaskShiftManager()
{
	TRACE("    TASKSHIFTER ENDING -> 0x%08X\n", this);

	// Firstly, let's add a new time block record to the list.
	//
	SetCurrentTask(0);	// Away

	GetTimeBlocksIncludingCurrent().Save();
	
	m_vecTasks.Save();
}

BEGIN_MESSAGE_MAP(CTaskShiftManager, CWnd)
	ON_COMMAND(ID_SHIFTER_NEW_TASK, OnShifterNewTask)
	ON_COMMAND_RANGE(ID_SHIFTER_TASK_FIRST, ID_SHIFTER_TASK_LAST, OnShifterSwitchTask)
	// ON_MESSAGE(WM_NOTIFY_TASKBAR_ICON, OnNotifyIcon)
	ON_MESSAGE(WM_NOTIFY_SHELL_ICON, OnNotifyIcon)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_SHIFTER_EXPORT_THIS_WEEK, OnExportThisWeek)
	ON_COMMAND(ID_SHIFTER_EDIT_TASKS, OnEditTasks)
	ON_COMMAND(ID_SHIFTER_EDIT_TIMESHEET, OnEditTimesheet)
	ON_COMMAND(ID_SHIFTER_OPTIONS, OnOptions)
	ON_COMMAND(ID_SHIFTER_EXPORT_OPTIONS, OnExportOptions)
	ON_COMMAND(ID_SHIFTER_SUBMIT_TIMESHEET, OnSubmitTimesheet)
	ON_COMMAND_RANGE(ID_SHIFTER_EXPORT_PRIOR_FIRST, ID_SHIFTER_EXPORT_PRIOR_LAST, OnExportPriorWeek)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CTaskShiftManager::Create(CWnd *apwndParent, UINT nID)
{
#ifdef TASKSHIFTER_PROFESSIONAL
	int		viDefaultTask;
#else
	CString	vcstrDefaultTask;
#endif
	int		viTask;
	int		viMinutes;
	CLJDate	vdtCurrent;

	if (CWnd::Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), apwndParent, nID))
	{
		m_tIcon.Create();
		m_tIcon.SetIcon(IDR_TASKSHTYPE);
		m_tIcon.SetOwner(this);
		m_tIcon.Show();

		viTask = 0;

#ifdef TASKSHIFTER_PROFESSIONAL
		if (m_tSettings.GetSelectTaskByDefaultAtStartup(viDefaultTask))
		{
			if (viDefaultTask <= 0)
			{
#else
		if (m_tSettings.GetSelectTaskByDefaultAtStartup(vcstrDefaultTask))
		{
			if (vcstrDefaultTask.IsEmpty())
			{
#endif
				// Select the last task.
				if (m_vecTimeBlocks.size() > 0)
				{
#ifdef TASKSHIFTER_PROFESSIONAL
					viTask = m_vecTasks.FindTask(m_vecTimeBlocks[m_vecTimeBlocks.size() - 1].taskId);
#else
					viTask = m_vecTasks.FindTask(m_vecTimeBlocks[m_vecTimeBlocks.size() - 1].strDescription);
#endif
					if (viTask == -1)
					{
						viTask = 0;	// Select the away from desk task by default.
					}
				}
				else
				{
					viTask = 0;	// Select the away from desk task by default.
				}
			}
			else
			{
#ifdef TASKSHIFTER_PROFESSIONAL
				viTask = m_vecTasks.FindTask(viDefaultTask);
#else
				viTask = m_vecTasks.FindTask(vcstrDefaultTask);
#endif
				if (viTask == -1)
				{
					viTask = 0;	// Select the away from desk task by default.
				}
			}
		}

		// Crash Protection
		//
		if (m_tSettings.GetUseCrashProtection(viMinutes))
		{
			if (m_vecTimeBlocks.size() > 0)
			{
				int	viDifference = (((int)vdtCurrent - m_vecTimeBlocks[m_vecTimeBlocks.size() - 1].iDate) * 24 * 60) + (vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes()) - m_vecTimeBlocks[m_vecTimeBlocks.size() - 1].iEndTime;

				if (viDifference > 0 &&
					viDifference <= viMinutes)
				{
					m_vecTimeBlocks[m_vecTimeBlocks.size() - 1].iEndTime = (vdtCurrent.GetHours() * 60) + vdtCurrent.GetMinutes();
				}
			}
		}

		SetCurrentTask(viTask);

		// NOTE: We always set up this timer, and persist information to disk once a minute.
		// This fixes the problem where the system shuts down unexpectedly.
		//
		m_iMinuteUpdaterEvent = SetTimer(1034, 60000, NULL);
		m_iInputCheckEvent = SetTimer(1035, 1000, NULL);
		m_iAnimateIconEvent = SetTimer(1036, 500, NULL);

		return TRUE;
	}
	return FALSE;
}
		
bool CTaskShiftManager::SetCurrentTask(int aiNewTask)
{
	// Firstly, let's add a new time block record to the list.
	//
	CLJDate	vdtCurrent;

	if (aiNewTask != m_iCurrentTask)
	{
		if (m_iCurrentTask >= 1 && m_iCurrentTask < (int)m_vecTasks.size())	// Task 0 is "away"
		{
#ifdef TASKSHIFTER_PROFESSIONAL
			m_vecTimeBlocks.push_back(tTimeBlock(m_iCurrentDate, m_iCurrentStartTime, vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes(), m_vecTasks[m_iCurrentTask].id));
#else
			m_vecTimeBlocks.push_back(tTimeBlock(m_iCurrentDate, m_iCurrentStartTime, vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes(), m_vecTasks[m_iCurrentTask].strDescription));
#endif
		}

		m_iCurrentDate = (int)vdtCurrent;
		m_iCurrentStartTime = vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes();

		m_dwSetToAwayFromDeskTime = GetTickCount();

		m_iCurrentTask = aiNewTask;
	}

	if (m_iCurrentTask == 0)	// Away from desk
	{
		m_bShownAwayFromDeskWarning = false;
	}

	RefreshTooltip();

	return true;
}

LRESULT CTaskShiftManager::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	return ProcessShellNotify(lParam);
}

BOOL CTaskShiftManager::ProcessShellNotify(DWORD dwMessage)
{
	CPoint	vpntCursor;

	if (dwMessage == WM_LBUTTONDOWN ||
		dwMessage == WM_CONTEXTMENU ||
		dwMessage == WM_RBUTTONDOWN)
	{
		GetCursorPos(&vpntCursor);

		// The menu should look a little something like this:
		//
		// *------------------------*
		// | About...               |
		// *------------------------*
		// | <<New Task>>           |
		// | <<Not Working>>        |
		// | Task 1                 |
		// | Task 2                 |
		// *------------------------*

		CMenu								vmnuTemp;
		CMenu								vmnuSubMenu;
		CMenu								vmnuPreviousWeeks;
		CTaskShiftPriorWeekVector::iterator	vtIter;
		CLJDate								vdtTemp;
		int									viID;
		bool								simpleMenu;

		vmnuTemp.CreatePopupMenu();

		simpleMenu = (dwMessage == WM_LBUTTONDOWN || dwMessage == WM_CONTEXTMENU);
		if (!simpleMenu)
		{
			vmnuSubMenu.CreateMenu();
			vmnuSubMenu.AppendMenu(MF_STRING, ID_SHIFTER_EXPORT_THIS_WEEK, _T("This Week"));
			vmnuSubMenu.AppendMenu(MF_STRING, ID_SHIFTER_EXPORT_OPTIONS, _T("Options..."));

			vmnuPreviousWeeks.CreateMenu();
			RefreshPriorWeeks();
			for (vtIter = m_vecPriorWeeks.begin(), viID = ID_SHIFTER_EXPORT_PRIOR_FIRST; vtIter != m_vecPriorWeeks.end() && viID <= ID_SHIFTER_EXPORT_PRIOR_LAST; ++vtIter, ++viID)
			{
				vdtTemp.SetDate((*vtIter).iWeekCommencingDate);
				vmnuPreviousWeeks.AppendMenu(MF_STRING, viID, vdtTemp.GetDateString(false));
			}
			vmnuSubMenu.AppendMenu(MF_POPUP, (UINT)(HMENU)vmnuPreviousWeeks, _T("Previous Weeks"));

			vmnuTemp.AppendMenu(MF_POPUP, (UINT)(HMENU)vmnuSubMenu, _T("Export"));

#ifdef TASKSHIFTER_PROFESSIONAL
			if (soapServer.GetUserCanCreateTasks())
			{
				vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_EDIT_TASKS, _T("Edit Tasks..."));
			}
			if (!soapServer.GetTimesheetsAutoSubmitted())
			{
				vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_SUBMIT_TIMESHEET, _T("Submit Timesheet"));
			}
#else
			vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_EDIT_TASKS, _T("Edit Tasks..."));
#endif
			vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_EDIT_TIMESHEET, _T("Edit Time Sheet..."));
			vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_OPTIONS, _T("Options..."));
			vmnuTemp.AppendMenu(MF_STRING, ID_APP_ABOUT, _T("About..."));
			vmnuTemp.AppendMenu(MF_STRING, ID_APP_EXIT, _T("Exit"));
			vmnuTemp.AppendMenu(MF_SEPARATOR);
#ifdef TASKSHIFTER_PROFESSIONAL
			if (soapServer.GetUserCanCreateTasks())
			{
				vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_NEW_TASK, _T("Create New Task..."));
			}
#else
			vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_NEW_TASK, _T("Create New Task..."));
#endif
		}

		for (int i = 0; i < (int)m_vecTasks.size(); ++i)
		{
			vmnuTemp.AppendMenu(MF_STRING, ID_SHIFTER_TASK_FIRST + i, m_vecTasks[i].strDescription);
			if (i == m_iCurrentTask)
			{
				vmnuTemp.CheckMenuItem(ID_SHIFTER_TASK_FIRST + i, MF_BYCOMMAND | MF_CHECKED);
			}
		}

		m_tIcon.SetForegroundWindow();
		vmnuTemp.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, vpntCursor.x, vpntCursor.y, this);
		m_tIcon.PostMessage(WM_NULL);
		vmnuTemp.DestroyMenu();
	}
	return TRUE;
}

bool CTaskShiftManager::RefreshPriorWeeks()
{
	CString						vcstrFind;
	CFileFind					vtFinder;
	BOOL						vbFound;
	tTaskShiftPriorWeekRecord	vtAdd;
	CLJDate						vdtTemp;
	CString						vcstrDateBit;

	m_vecPriorWeeks.clear();

	vcstrFind.Format(_T("%sTimesheet*.xml"), CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()));
	vbFound = vtFinder.FindFile(vcstrFind);
	while (vbFound)
	{
		vbFound = vtFinder.FindNextFile();

		if (vtFinder.GetFileTitle().GetLength() == 17)	// ie. Timesheet + yyyymmdd
		{
			vcstrDateBit = vtFinder.GetFileTitle().Right(8);

			vdtTemp.SetDate(_ttoi(vcstrDateBit.Mid(0, 4)), _ttoi(vcstrDateBit.Mid(4, 2)), _ttoi(vcstrDateBit.Mid(6, 2)));
			
			vtAdd.iWeekCommencingDate = (int)vdtTemp;
			vtAdd.strFilename = vtFinder.GetFilePath();
			m_vecPriorWeeks.push_back(vtAdd);
		}
	}
	
	return true;
}

void CTaskShiftManager::OnShifterNewTask()
{
	CNewTaskDlg	vdlgNewTask;
	int			newTaskId;

	if (vdlgNewTask.DoModal() == IDOK)
	{
#ifdef TASKSHIFTER_PROFESSIONAL
		if (soapServer.AddTask(vdlgNewTask.GetNewTaskName(), newTaskId))
		{
			m_vecTasks.PushBack(newTaskId, vdlgNewTask.GetNewTaskName(), vdlgNewTask.GetNewTaskIconOffset());
		}
		else
		{
			AfxMessageBox(_T("A problem occurred adding your new task. Please try again."), MB_ICONERROR | MB_OK);
		}
#else
		m_vecTasks.PushBack(vdlgNewTask.GetNewTaskName(), vdlgNewTask.GetNewTaskIconOffset());
#endif
		m_vecTasks.Save();
		SetCurrentTask(m_vecTasks.size() - 1);
	}
}

void CTaskShiftManager::OnEditTasks()
{
	int				viCurrentTask = m_iCurrentTask;
	CTaskListDlg	vdlgTasks(viCurrentTask, m_vecTasks, this);
	if (vdlgTasks.DoModal() == IDOK)
	{
		if (viCurrentTask != m_iCurrentTask)
		{
			SetCurrentTask(viCurrentTask);
		}
		RefreshTooltip();
	}
}

void CTaskShiftManager::OnEditTimesheet()
{
	TimeBlockVector	vvecTemp;

	vvecTemp = GetTimeBlocksIncludingCurrent();

	CTimeSheetDlg	vdlgSheet(vvecTemp, m_vecTasks, this);
	if (vdlgSheet.DoModal() == IDOK)
	{
		m_vecTimeBlocks.clear();
		m_vecTimeBlocks.insert(m_vecTimeBlocks.begin(), vvecTemp.begin(), vvecTemp.end() - 1);
		
		RefreshTooltip();
	}	
}

void CTaskShiftManager::OnSubmitTimesheet()
{
	soapServer.SubmitTimesheet(GetTimeBlocksIncludingCurrent());
}

void CTaskShiftManager::OnOptions()
{
	CTaskShiftOptionsDlg	vdlgOptions(m_vecTasks, this);

	if (vdlgOptions.DoModal() == IDOK)
	{
		RefreshTooltip();
	}
}

void CTaskShiftManager::OnExportOptions()
{
	CTaskShiftExportOptionsDlg	vdlgOptions(&m_tOutput, this);

	if (vdlgOptions.DoModal() == IDOK)
	{
		// Cool
	}
}

void CTaskShiftManager::OnExportPriorWeek(UINT nID)
{
	int					viOffset = nID - ID_SHIFTER_EXPORT_PRIOR_FIRST;
	TimeBlockVector		vtPriorWeek;

	if (viOffset >= 0 && viOffset < (int)m_vecPriorWeeks.size())
	{
		if (vtPriorWeek.LoadFrom(m_vecPriorWeeks[viOffset].strFilename))
		{
			m_tOutput.Output(vtPriorWeek, m_vecTasks);
		}
	}
}

void CTaskShiftManager::OnShifterSwitchTask(UINT nID)
{
	SetCurrentTask(nID - ID_SHIFTER_TASK_FIRST);
}

void CTaskShiftManager::OnAppAbout()
{
	((CTaskShifterApp *)AfxGetApp())->OnAppAbout();
}

void CTaskShiftManager::OnAppExit()
{
	m_tIcon.Hide();
	SendMessage(WM_CLOSE);
}

void CTaskShiftManager::OnExportThisWeek()
{

	m_tOutput.Output(GetTimeBlocksIncludingCurrent(), m_vecTasks);
}

TimeBlockVector	CTaskShiftManager::GetTimeBlocksIncludingCurrent()
{
	TimeBlockVector	vvecRet;
	CLJDate			vdtCurrent;

	vvecRet.insert(vvecRet.begin(), m_vecTimeBlocks.begin(), m_vecTimeBlocks.end());
	if (m_iCurrentTask >= 1 && m_iCurrentTask < (int)m_vecTasks.size())	// Task 0 is "away"
	{
#ifdef TASKSHIFTER_PROFESSIONAL
		vvecRet.push_back(tTimeBlock(m_iCurrentDate, m_iCurrentStartTime, vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes(), m_vecTasks[m_iCurrentTask].id));
#else
		vvecRet.push_back(tTimeBlock(m_iCurrentDate, m_iCurrentStartTime, vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes(), m_vecTasks[m_iCurrentTask].strDescription));
#endif
	}
	return vvecRet;
}

void CTaskShiftManager::OnTimer(UINT nIDEvent)
{
	int	viSecondsToCheck;
	int	viSecSinceUserInput;
	int	viSecSinceAwayFromDesk;

	if (nIDEvent == m_iMinuteUpdaterEvent)
	{
		m_vecTasks.Save();

		GetTimeBlocksIncludingCurrent().Save();
		// m_vecTimeBlocks.Save();
#ifdef TASKSHIFTER_PROFESSIONAL
		if (soapServer.GetTimesheetsAutoSubmitted())
		{
			soapServer.SubmitTimesheet(GetTimeBlocksIncludingCurrent());
		}
#endif
		RefreshTooltip();
	}
	else if (nIDEvent == m_iInputCheckEvent)
	{
		if (m_tSettings.GetShowWarningOnUserInput() && m_iCurrentTask == 0 && !m_bShownAwayFromDeskWarning)
		{
			viSecSinceUserInput = (GetTickCount() - m_tUserInput.GetLastUserInputTickCount()) / 1000;
			viSecSinceAwayFromDesk = (GetTickCount() - m_dwSetToAwayFromDeskTime) / 1000;
			
			if (viSecSinceUserInput < 2 && viSecSinceAwayFromDesk > m_tSettings.GetAwayFromDeskGracePeriod())
			{
				m_tIcon.ShowBalloon(_T("You are currently shown as 'Away From Desk'"), 10000, _T("TaskShifter"), NIIF_WARNING);
				m_bShownAwayFromDeskWarning = true;
			}
		}
		if (m_tSettings.GetShowWarningOnNoUserInput(viSecondsToCheck) && (m_iCurrentTask != 0))
		{
			viSecSinceUserInput = (GetTickCount() - m_tUserInput.GetLastUserInputTickCount()) / 1000;
			
			if (viSecSinceUserInput > viSecondsToCheck)
			{
				m_tIcon.ShowBalloon(_T("No user input has been detected, and the current task is now being set to '--Away From Desk--'"), 10000, _T("TaskShifter"), NIIF_WARNING);
				SetCurrentTask(0);
			}
		}
	}
	else if (nIDEvent == m_iAnimateIconEvent)
	{
		// Animate the icon if required.
		//
		if (m_iCurrentTask != 0 && m_vecTasks[m_iCurrentTask].iIconOffset == 0)
		{
			m_iAnimationState = 0;
		}
		else
		{
			m_iAnimationState = 1 - m_iAnimationState;
		}

		if (m_iAnimationState == 0)
		{
			m_tIcon.SetIcon(IDR_TASKSHTYPE);
		}
		else
		{
			m_tIcon.SetIcon(IDR_TASKSHTYPE_ALT);
		}
	}
	else
	{
		CWnd::OnTimer(nIDEvent);
	}
}

bool CTaskShiftManager::RefreshTooltip()
{
	CString						vcstrWorked;
	CString						vcstrTemp;
	CString						vcstrTask;
	CString						vcstrDay;
	CString						vcstrWeek;
	
	int							viTimeWorkedWeek = 0;
	int							viTimeWorkedDay = 0;
	int							viTimeWorkedTask = 0;

	int							viExceedHours;

	TimeBlockVector::iterator	vtIter;
	CLJDate						vdtCurrent;

	for (vtIter = m_vecTimeBlocks.begin(); vtIter != m_vecTimeBlocks.end(); ++vtIter)
	{
		viTimeWorkedWeek += ((*vtIter).iEndTime - (*vtIter).iStartTime);

		if ((*vtIter).iDate == (int)vdtCurrent)
		{
			viTimeWorkedDay += ((*vtIter).iEndTime - (*vtIter).iStartTime);
		}
	}

	if (m_iCurrentTask >= 1)
	{
		viTimeWorkedTask = (vdtCurrent.GetHours() * 60 + vdtCurrent.GetMinutes() - m_iCurrentStartTime);
		viTimeWorkedWeek += viTimeWorkedTask;
		viTimeWorkedDay += viTimeWorkedTask;
	}

	if (m_tSettings.GetShowTimes())
	{
		if (m_tSettings.GetShowHoursWorkedInTask())
		{
			vcstrTask.Format(_T("[%ih%im]"), viTimeWorkedTask / 60, viTimeWorkedTask % 60);
		}
		if (m_tSettings.GetShowHoursWorkedInDay())
		{
			vcstrDay.Format(_T("[Day:%ih%im]"), viTimeWorkedDay / 60, viTimeWorkedDay % 60);
		}
		if (m_tSettings.GetShowHoursWorkedInWeek())
		{
			vcstrWeek.Format(_T("[Week:%ih%im]"), viTimeWorkedWeek / 60, viTimeWorkedWeek % 60);
		}

		vcstrWorked.Format(_T("%s %s %s"), vcstrTask, vcstrDay, vcstrWeek);
	}

	if (m_tSettings.GetShowWarningOnWeekMinutesLimit(viExceedHours))
	{
		if (viTimeWorkedWeek == viExceedHours)
		{
			// Show the warning.
			//
			vcstrTemp.Format(_T("You have now worked more than %ih%im this week"), viExceedHours / 60, viExceedHours % 60);
			m_tIcon.ShowBalloon(vcstrTemp, 10000, _T("TaskShifter"), NIIF_WARNING);
		}
	}
	if (m_tSettings.GetShowWarningOnDayMinutesLimit(viExceedHours))
	{
		if (viTimeWorkedDay == viExceedHours)
		{
			// Show the warning.
			//
			vcstrTemp.Format(_T("You have now worked more than %ih%im today"), viExceedHours / 60, viExceedHours % 60);
			m_tIcon.ShowBalloon(vcstrTemp, 10000, _T("TaskShifter"), NIIF_WARNING);
		}
	}
	vcstrTemp.Format(_T("Currently: %s %s"), m_vecTasks[m_iCurrentTask].strDescription, vcstrWorked);

	m_tIcon.SetTooltipText(vcstrTemp);

	return true;
}

void CTaskShiftManager::OnDestroy()
{
	CWnd::OnDestroy();
}