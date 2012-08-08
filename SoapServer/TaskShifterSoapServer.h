#pragma once

#ifdef TASKSHIFTER_PROFESSIONAL

#include "soapTSServerBindingProxy.h"
#include "../TaskShiftDefs.h"


class CTaskShifterSoapServer
{
public:
	enum ItemStatus
	{
		Status_Valid = 0,
		Status_Tentative = 1,
		Status_Rejected = 2
	};

	enum UserType
	{
		UserType_Windows = 0,
		UserType_Custom = 1
	};

public:
	CTaskShifterSoapServer();
	virtual ~CTaskShifterSoapServer();

	bool	Connect(LPCTSTR serverUrl, TaskVector &taskList, LPCTSTR customUserName = NULL);
	bool	AddTask(LPCTSTR taskName, int &taskId);
	bool	SubmitTimesheet(TimeBlockVector &timesheet, int *timesheetId = NULL);

	// System Policy
	bool	GetTimesheetsAutoSubmitted() { return timesheetsAutoSubmitted; }
	bool	GetUserCanCreateTasks() { return userCanCreateTasks; }

protected:
	CString					serverUrl;
	int						userId;
	TSServerBindingProxy	soapServer;
	bool					timesheetsAutoSubmitted;
	bool					userCanCreateTasks;
};

#endif