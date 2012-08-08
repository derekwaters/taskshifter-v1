#ifndef TASKSHIFTDEFS_H
#define	TASKSHIFTDEFS_H

#include <vector>


#define	TS_UI_GAP			5
#define	TS_UI_SMALL_BTN		14



struct	tTask
{
#ifdef TASKSHIFTER_PROFESSIONAL
	int		id;
#endif
	CString	strDescription;
	int		iIconOffset;

#ifdef TASKSHIFTER_PROFESSIONAL
	tTask(int taskid, LPCTSTR szDesc, int aiIcon = 0) { id = taskid; strDescription = szDesc; iIconOffset = aiIcon; }
#else
	tTask(LPCTSTR szDesc, int aiIcon = 0) { strDescription = szDesc; iIconOffset = aiIcon; }
#endif
};

struct	tTimeBlock
{
	int		iDate;
	int		iStartTime;
	int		iEndTime;
	int		iBreaks;
#ifdef TASKSHIFTER_PROFESSIONAL
	int		taskId;
#else
	CString	strDescription;
#endif

#ifdef TASKSHIFTER_PROFESSIONAL
	tTimeBlock(int date, int start, int end, int task) : iDate(date), iStartTime(start), iEndTime(end), taskId(task), iBreaks(0) { ; }
#else
	tTimeBlock(int date, int start, int end, LPCTSTR desc) : iDate(date), iStartTime(start), iEndTime(end), strDescription(desc), iBreaks(0) { ; }
#endif
};






class TaskVector : public std::vector<tTask>
{
public:
	TaskVector();	// This will automatically include the "--Away From Desk--" task.
	virtual ~TaskVector();

	bool	Load();
	bool	Save();
	bool	Clear();
#ifdef TASKSHIFTER_PROFESSIONAL
	bool	PushBack(int taskId, LPCTSTR apszDescription, int aiIconOffset = 0);
	int		FindTask(int taskId);
#else
	bool	PushBack(LPCTSTR apszDescription, int aiIconOffset = 0);
	int		FindTask(LPCTSTR apszDescription);
#endif
};


class TimeBlockVector : public std::vector<tTimeBlock>
{
public:
	bool	Load();
	bool	LoadFrom(LPCTSTR apszFilename);
	bool	Save();
	CString	GetXml();
};




class CTaskShiftSettings
{
public:
	bool	GetShowHoursWorkedInTask();
	bool	GetShowHoursWorkedInDay();
	bool	GetShowHoursWorkedInWeek();
#ifdef TASKSHIFTER_PROFESSIONAL
	bool	GetSelectTaskByDefaultAtStartup(int &taskId);
	CString	GetServerUrl();
#else
	bool	GetSelectTaskByDefaultAtStartup(CString &acstrTaskName);
#endif
	bool	GetShowWarningOnUserInput();
	bool	GetShowWarningOnNoUserInput(int &aiAfterSeconds);
	bool	GetShowWarningOnWeekMinutesLimit(int &aiLimitMinutes);
	bool	GetShowWarningOnDayMinutesLimit(int &aiLimitMinutes);
	bool	GetUseCrashProtection(int &aiMinutesTolerance);
	int		GetAwayFromDeskGracePeriod();	// In seconds

	bool	Init();
	bool	Close();
	bool	Save();
	bool	GetShowTimes() { return GetShowHoursWorkedInDay() || GetShowHoursWorkedInTask() || GetShowHoursWorkedInWeek(); }

	void	SetShowHoursWorkedInTask(bool abShow);
	void	SetShowHoursWorkedInDay(bool abShow);
	void	SetShowHoursWorkedInWeek(bool abShow);
#ifdef TASKSHIFTER_PROFESSIONAL
	void	SetSelectTaskByDefaultAtStartup(bool abSelect, int taskId);
	void	SetServerUrl(LPCTSTR newUrl);
#else
	void	SetSelectTaskByDefaultAtStartup(bool abSelect, LPCTSTR apszTaskName);
#endif
	void	SetShowWarningOnUserInput(bool abShow);
	void	SetShowWarningOnNoUserInput(bool abShow, int aiAfterSeconds);
	void	SetShowWarningOnWeekMinutesLimit(bool abShow, int aiLimitMinutes);
	void	SetShowWarningOnDayMinutesLimit(bool abShow, int aiLimitMinutes);
	void	SetUseCrashProtection(bool abUseProtection, int aiMinutesTolerance);
	void	SetAwayFromDeskGracePeriod(int aiSeconds);
};





struct	tTaskShiftPriorWeekRecord
{
	CString	strFilename;
	int		iWeekCommencingDate;
};

typedef std::vector<tTaskShiftPriorWeekRecord>	CTaskShiftPriorWeekVector;

#endif