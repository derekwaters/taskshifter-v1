#if !defined(AFX_TASKSHIFTSCHEDULEWND_H__EE5B5726_B97C_485F_8650_685B72540C99__INCLUDED_)
#define AFX_TASKSHIFTSCHEDULEWND_H__EE5B5726_B97C_485F_8650_685B72540C99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// taskshiftschedulewnd.h : header file
//

#include "TaskShiftDefs.h"

/////////////////////////////////////////////////////////////////////////////
// TaskShiftScheduleWnd window

class CTaskShiftScheduleWnd : public CWnd
{
// Construction
public:
	CTaskShiftScheduleWnd();
	virtual ~CTaskShiftScheduleWnd();
	
	virtual BOOL Create(TimeBlockVector *apvecTimeBlocks, TaskVector *apvecTasks, CWnd *pParentWnd, UINT nID);

protected:
	bool GetEarliestAndLatestTimes(int &aiMinTime, int &aiMaxTime);
	bool DrawScheduleWnd(CDC &adc, int aiWidth, int aiHeight);
	bool DrawBackground(CDC &adc, CRect &arArea);
	bool DrawTimeHeader(CDC &adc, int aiMinTime, int aiMaxTime, CRect &arArea);
	bool DrawDaysHeader(CDC &adc, CRect &arArea);
	bool DrawTasks(CDC &adc, int aiMinTime, int aiMaxTime, CRect &arArea);

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	TimeBlockVector	*m_pvecTime;
	TaskVector      *m_pvecTasks;
	int				m_iDayWidth;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSHIFTSCHEDULEWND_H__EE5B5726_B97C_485F_8650_685B72540C99__INCLUDED_)
