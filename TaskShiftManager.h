// TaskShiftManager.h: interface for the CTaskShiftManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSHIFTMANAGER_H__D11233F3_881E_45CB_8F4A_4AED2C0E49BA__INCLUDED_)
#define AFX_TASKSHIFTMANAGER_H__D11233F3_881E_45CB_8F4A_4AED2C0E49BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TaskShiftDefs.h"
#include "TaskShiftUserInput.h"
#include "TaskShiftOutput.h"
#include "TaskShiftIcon.h"
#ifdef TASKSHIFTER_PROFESSIONAL
#include "SoapServer/TaskShifterSoapServer.h"
#endif


class CTaskShiftManager : public CWnd
{
public:
	CTaskShiftManager();
	virtual ~CTaskShiftManager();

	virtual BOOL Create(CWnd *apwndParent, UINT nID);
		
	bool	SetCurrentTask(int aiNewTask);

protected:

	bool			LoadTasks();
	bool			LoadTimeBlocks();
	bool			SaveTasks();
	bool			SaveTimeBlocks();

	bool			RefreshTooltip();
	TimeBlockVector	GetTimeBlocksIncludingCurrent();
	bool			RefreshPriorWeeks();

	CTaskShiftIcon	m_tIcon;

	TaskVector		m_vecTasks;
	int				m_iCurrentTask;

	TimeBlockVector	m_vecTimeBlocks;
	int				m_iCurrentDate;
	int				m_iCurrentStartTime;

	UINT			m_iMinuteUpdaterEvent;
	UINT			m_iInputCheckEvent;
	UINT			m_iAnimateIconEvent;

	DWORD			m_dwSetToAwayFromDeskTime;
	bool			m_bShownAwayFromDeskWarning;

	int				m_iAnimationState;
	
	CTaskShiftPriorWeekVector	m_vecPriorWeeks;

	CTaskShiftSettings	m_tSettings;
	CTaskShiftUserInput	m_tUserInput;
	CTaskShiftOutput	m_tOutput;
#ifdef TASKSHIFTER_PROFESSIONAL
	CTaskShifterSoapServer	soapServer;
#endif


	afx_msg void	OnShifterOpen();
	afx_msg void	OnShifterNewTask();
	afx_msg void	OnShifterSwitchTask(UINT nID);
	afx_msg void	OnAppAbout();
	afx_msg void	OnAppExit();
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	virtual BOOL	ProcessShellNotify(DWORD dwMessage);
	afx_msg void	OnExportThisWeek();
	afx_msg void	OnEditTasks();
	afx_msg void	OnEditTimesheet();
	afx_msg void	OnSubmitTimesheet();
	afx_msg void	OnOptions();
	afx_msg void	OnTimer(UINT nIDEvent);
	afx_msg void	OnExportOptions();
	afx_msg void	OnExportPriorWeek(UINT nID);
	afx_msg void	OnDestroy();

	afx_msg LRESULT	OnMyMouseMove(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP();
};

#endif // !defined(AFX_TASKSHIFTMANAGER_H__D11233F3_881E_45CB_8F4A_4AED2C0E49BA__INCLUDED_)
