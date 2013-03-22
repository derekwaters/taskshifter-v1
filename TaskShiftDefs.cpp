#include "stdafx.h"
#include "TaskShiftDefs.h"
#include "TaskShiftUtils.h"
#include "TinyXml\TinyXml.h"
#include "IncludedFiles\FWUtilDate.h"

TaskVector::TaskVector()
{
	Clear();
}

TaskVector::~TaskVector()
{
}

bool TaskVector::Clear()
{
	clear();
	PushBack(-1, _T("--Away From Desk--"));
	return true;
}

#ifdef TASKSHIFTER_PROFESSIONAL
bool TaskVector::PushBack(int taskId, LPCTSTR apszDescription, int aiIconOffset)
{
	push_back(tTask(taskId, apszDescription, aiIconOffset));
	return true;
}

int TaskVector::FindTask(int withId)
{
	int			viRet = -1;
	int			viPos;
	iterator	vtIter;

	for (vtIter = begin(), viPos = 0; (vtIter != end()) && (viRet == -1); ++vtIter, ++viPos)
	{
		if ((*vtIter).id == withId)
		{
			viRet = viPos;
		}
	}
	return viRet;
}
#else
bool TaskVector::PushBack(LPCTSTR apszDescription, int aiIconOffset)
{
	push_back(tTask(apszDescription, aiIconOffset));
	return true;
}

int TaskVector::FindTask(LPCTSTR apszDescription)
{
	int			viRet = -1;
	int			viPos;
	iterator	vtIter;

	for (vtIter = begin(), viPos = 0; (vtIter != end()) && (viRet == -1); ++vtIter, ++viPos)
	{
		if ((*vtIter).strDescription.CompareNoCase(apszDescription) == 0)
		{
			viRet = viPos;
		}
	}
	return viRet;
}
#endif

bool TaskVector::Load()
{
	TiXmlDocument		vtDocument;
	TiXmlElement		*vptRoot;
	TiXmlElement		*vptItem;
	TiXmlNode			*vptObj;
	CString				vcstrTemp;
	int					viIconOffset;
	LPCTSTR				vszAttributeValue;

	Clear();

	vcstrTemp = CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()) + _T("tasks.xml");

	if (vtDocument.LoadFile(vcstrTemp))
	{
		vptRoot = vtDocument.RootElement();
		if (vptRoot != NULL)
		{
			for (vptItem = vptRoot->FirstChildElement(); vptItem != NULL; vptItem = vptItem->NextSiblingElement())
			{
				viIconOffset = 0;
				vszAttributeValue = vptItem->Attribute(_T("IconOffset"));
				if (vszAttributeValue != NULL)
				{
					viIconOffset = _ttoi(vszAttributeValue);
				}

				vptObj = vptItem->FirstChild();
				if (vptObj->Type() == TiXmlNode::TEXT)
				{
#ifndef TASKSHIFTER_PROFESSIONAL
					push_back(tTask(vptObj->ToText()->Value(), viIconOffset));
#endif
				}
			}
		}
	}
	return true;
}

bool TaskVector::Save()
{
	TiXmlDocument		vtDocument;
	TiXmlNode			*vptRoot;
	TiXmlElement		*vptItem;
	CString				vcstrTemp;
	iterator			vtIter;

	vcstrTemp = CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()) + _T("tasks.xml");

	vptRoot = vtDocument.InsertEndChild(TiXmlElement(_T("Tasks")));
	for (vtIter = begin() + 1; vtIter != end(); ++vtIter)
	{
		vptItem = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("Item")));
		vptItem->SetAttribute(_T("IconOffset"), (*vtIter).iIconOffset);
		vptItem->InsertEndChild(TiXmlText((*vtIter).strDescription));
	}

	vtDocument.SaveFile(vcstrTemp);

	return true;
}

bool TimeBlockVector::Load()
{
	// Load this weeks timings.
	//
	CString						vcstrTemp;
	CFWUtilDate					vdtCurrent;

	vdtCurrent.AddDays(-vdtCurrent.GetDayOfWeek());
	vcstrTemp.Format(_T("%sTimesheet%04i%02i%02i.xml"), CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()), vdtCurrent.GetYear(), vdtCurrent.GetMonth(), vdtCurrent.GetDay());

	return LoadFrom(vcstrTemp);
}

bool TimeBlockVector::LoadFrom(LPCTSTR apszFilename)
{
	TiXmlDocument				vtParser;
	TiXmlElement				*vptRoot;
	TiXmlElement				*vptItem;
	TiXmlNode					*vptDesc;
	int							viDate;
	int							viStartTime;
	int							viEndTime;
	int							viTaskId;

	if (vtParser.LoadFile(apszFilename))
	{
		vptRoot = vtParser.RootElement();
		for (vptItem = vptRoot->FirstChildElement(_T("Item")); vptItem != NULL; vptItem = vptItem->NextSiblingElement(_T("Item")))
		{
			viDate = _ttoi(vptItem->Attribute(_T("Date")));
			viStartTime = _ttoi(vptItem->Attribute(_T("StartTime")));
			viEndTime = _ttoi(vptItem->Attribute(_T("EndTime")));

#ifdef TASKSHIFTER_PROFESSIONAL
			viTaskId = _ttoi(vptItem->Attribute(_T("TaskId")));
			push_back(tTimeBlock(viDate, viStartTime, viEndTime, viTaskId));
#else
			vptDesc = vptItem->FirstChild();
			ASSERT(vptDesc->Type() == TiXmlNode::TEXT);

			push_back(tTimeBlock(viDate, viStartTime, viEndTime, vptDesc->ToText()->Value()));
#endif
		}
	}
	return true;
}

bool TimeBlockVector::Save()
{
	CFWUtilDate				vdtCurrent;
	TiXmlDocument			vtParser;
	TiXmlElement			*vptRoot;
	TiXmlElement			*vptItem;
	CString					vcstrTemp;
	iterator				vtIter;

	vdtCurrent.AddDays(-vdtCurrent.GetDayOfWeek());

	vptRoot = (TiXmlElement *)vtParser.InsertEndChild(TiXmlElement(_T("TimeBlocks")));
	for (vtIter = begin(); vtIter != end(); ++vtIter)
	{
		vptItem = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("Item")));
		vptItem->SetAttribute(_T("Date"), (*vtIter).iDate);
		vptItem->SetAttribute(_T("StartTime"), (*vtIter).iStartTime);
		vptItem->SetAttribute(_T("EndTime"), (*vtIter).iEndTime);
#ifdef TASKSHIFTER_PROFESSIONAL
		vptItem->SetAttribute(_T("TaskId"), (*vtIter).taskId);
#else
		vptItem->InsertEndChild(TiXmlText((*vtIter).strDescription));
#endif
	}

	vcstrTemp.Format(_T("%sTimesheet%04i%02i%02i.xml"), CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()), vdtCurrent.GetYear(), vdtCurrent.GetMonth(), vdtCurrent.GetDay());

	vtParser.SaveFile(vcstrTemp);
	return true;
}

CString TimeBlockVector::GetXml()
{
	CFWUtilDate				vdtCurrent;
	TiXmlDocument			vtParser;
	TiXmlElement			*vptRoot;
	TiXmlElement			*vptItem;
	CString					vcstrTemp;
	iterator				vtIter;
	std::string				result;

	vdtCurrent.AddDays(-vdtCurrent.GetDayOfWeek());

	vptRoot = (TiXmlElement *)vtParser.InsertEndChild(TiXmlElement(_T("TimeBlocks")));
	for (vtIter = begin(); vtIter != end(); ++vtIter)
	{
		vptItem = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("Item")));
		vdtCurrent.SetDate((*vtIter).iDate);
		vcstrTemp.Format(_T("%04d-%02d-%02d %02d:%02d"), vdtCurrent.GetYear(), vdtCurrent.GetMonth(), vdtCurrent.GetDay(), (*vtIter).iStartTime / 60,  (*vtIter).iStartTime % 60);
		vptItem->SetAttribute(_T("Start"), vcstrTemp);
		vcstrTemp.Format(_T("%04d-%02d-%02d %02d:%02d"), vdtCurrent.GetYear(), vdtCurrent.GetMonth(), vdtCurrent.GetDay(), (*vtIter).iEndTime / 60,  (*vtIter).iEndTime % 60);
		vptItem->SetAttribute(_T("End"), vcstrTemp);
#ifdef TASKSHIFTER_PROFESSIONAL
		vptItem->SetAttribute(_T("TaskId"), (*vtIter).taskId);
#else
		vptItem->InsertEndChild(TiXmlText((*vtIter).strDescription));
#endif
	}
	result << vtParser;
	return result.c_str();
}

class CTaskShiftSettingsSingleton
{
public:
	CTaskShiftSettingsSingleton()
	{
	}

	bool Init()
	{
		m_bShowHoursWorkedInTask = (AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowHoursWorkedInTask"), false) != 0);
		m_bShowHoursWorkedInDay = (AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowHoursWorkedInDay"), true) != 0);
		m_bShowHoursWorkedInWeek = (AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowHoursWorkedInWeek"), false) != 0);
#ifdef TASKSHIFTER_PROFESSIONAL
		m_iDefaultTaskId = AfxGetApp()->GetProfileInt(_T("Settings"), _T("DefaultTaskId"), -1);
		m_SoapServerUrl = AfxGetApp()->GetProfileString(_T("Settings"), _T("SoapServerUrl"), _T(""));
#else
		m_cstrDefaultTaskName = AfxGetApp()->GetProfileString(_T("Settings"), _T("DefaultTaskName"), _T(""));
#endif
		m_bSelectTaskByDefaultAtStartup = (AfxGetApp()->GetProfileInt(_T("Settings"), _T("SelectTaskByDefaultAtStartup"), false) != 0);
		m_bShowWarningOnUserInput = (AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowWarningOnUserInput"), false) != 0);
		m_iShowWarningOnNoUserInput = AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowWarningOnNoUserInput"), 120);
		m_iShowWarningOnWeekMinutesLimit = AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowWarningOnWeekMinutesLimit"), 0);
		m_iShowWarningOnDayMinutesLimit = AfxGetApp()->GetProfileInt(_T("Settings"), _T("ShowWarningOnDayMinutesLimit"), 0);
		m_iCrashProtectionTolerance = AfxGetApp()->GetProfileInt(_T("Settings"), _T("CrashProtectionTolerance"), 0);
		m_iAwayFromDeskGracePeriod = AfxGetApp()->GetProfileInt(_T("Settings"), _T("AwayFromDeskGracePeriod"), 30);
		return true;
	}

	bool Close()
	{
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowHoursWorkedInTask"), m_bShowHoursWorkedInTask);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowHoursWorkedInDay"), m_bShowHoursWorkedInDay);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowHoursWorkedInWeek"), m_bShowHoursWorkedInWeek);
#ifdef TASKSHIFTER_PROFESSIONAL
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("DefaultTaskId"), m_iDefaultTaskId);
		AfxGetApp()->WriteProfileString(_T("Settings"), _T("SoapServerUrl"), m_SoapServerUrl);
#else
		AfxGetApp()->WriteProfileString(_T("Settings"), _T("DefaultTaskName"), m_cstrDefaultTaskName);
#endif
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("SelectTaskByDefaultAtStartup"), m_bSelectTaskByDefaultAtStartup);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowWarningOnUserInput"), m_bShowWarningOnUserInput);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowWarningOnNoUserInput"), m_iShowWarningOnNoUserInput);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowWarningOnWeekMinutesLimit"), m_iShowWarningOnWeekMinutesLimit);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("ShowWarningOnDayMinutesLimit"), m_iShowWarningOnDayMinutesLimit);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("CrashProtectionTolerance"), m_iCrashProtectionTolerance);
		AfxGetApp()->WriteProfileInt(_T("Settings"), _T("AwayFromDeskGracePeriod"), m_iAwayFromDeskGracePeriod);
		return true;
	}

	bool	m_bShowHoursWorkedInTask;
	bool	m_bShowHoursWorkedInDay;
	bool	m_bShowHoursWorkedInWeek;
	bool	m_bSelectTaskByDefaultAtStartup;
#ifdef TASKSHIFTER_PROFESSIONAL
	int		m_iDefaultTaskId;
	CString	m_SoapServerUrl;
#else
	CString	m_cstrDefaultTaskName;
#endif
	bool	m_bShowWarningOnUserInput;
	int		m_iShowWarningOnNoUserInput;	// In seconds
	int		m_iShowWarningOnWeekMinutesLimit;
	int		m_iShowWarningOnDayMinutesLimit;
	int		m_iCrashProtectionTolerance;
	int		m_iAwayFromDeskGracePeriod;	// In seconds
};

CTaskShiftSettingsSingleton	g_tSettings;


bool CTaskShiftSettings::GetShowHoursWorkedInTask()
{
	return g_tSettings.m_bShowHoursWorkedInTask;
}

bool CTaskShiftSettings::GetShowHoursWorkedInDay()
{
	return g_tSettings.m_bShowHoursWorkedInDay;
}

bool CTaskShiftSettings::GetShowHoursWorkedInWeek()
{
	return g_tSettings.m_bShowHoursWorkedInWeek;
}

#ifdef TASKSHIFTER_PROFESSIONAL
bool CTaskShiftSettings::GetSelectTaskByDefaultAtStartup(int &taskId)
{
	taskId = g_tSettings.m_iDefaultTaskId;
	return g_tSettings.m_bSelectTaskByDefaultAtStartup;
}

CString CTaskShiftSettings::GetServerUrl()
{
	return g_tSettings.m_SoapServerUrl;
}
#else
bool CTaskShiftSettings::GetSelectTaskByDefaultAtStartup(CString &acstrTaskName)
{
	acstrTaskName = g_tSettings.m_cstrDefaultTaskName;
	return g_tSettings.m_bSelectTaskByDefaultAtStartup;
}
#endif

bool CTaskShiftSettings::GetShowWarningOnUserInput()
{
	return g_tSettings.m_bShowWarningOnUserInput;
}

bool CTaskShiftSettings::Init()
{
	return g_tSettings.Init();
}

bool CTaskShiftSettings::Close()
{
	return g_tSettings.Close();
}

void CTaskShiftSettings::SetShowHoursWorkedInTask(bool abShow)
{
	g_tSettings.m_bShowHoursWorkedInTask = abShow;
}

void CTaskShiftSettings::SetShowHoursWorkedInDay(bool abShow)
{
	g_tSettings.m_bShowHoursWorkedInDay = abShow;
}

void CTaskShiftSettings::SetShowHoursWorkedInWeek(bool abShow)
{
	g_tSettings.m_bShowHoursWorkedInWeek = abShow;
}

#ifdef TASKSHIFTER_PROFESSIONAL
void CTaskShiftSettings::SetSelectTaskByDefaultAtStartup(bool abSelect, int taskId)
{
	g_tSettings.m_bSelectTaskByDefaultAtStartup = abSelect;
	g_tSettings.m_iDefaultTaskId = taskId;
}

void CTaskShiftSettings::SetServerUrl(LPCTSTR newUrl)
{
	g_tSettings.m_SoapServerUrl = newUrl;
}
#else
void CTaskShiftSettings::SetSelectTaskByDefaultAtStartup(bool abSelect, LPCTSTR apszTaskName)
{
	g_tSettings.m_bSelectTaskByDefaultAtStartup = abSelect;
	g_tSettings.m_cstrDefaultTaskName = apszTaskName;
}
#endif

void CTaskShiftSettings::SetShowWarningOnUserInput(bool abShow)
{
	g_tSettings.m_bShowWarningOnUserInput = abShow;
}

bool CTaskShiftSettings::GetShowWarningOnWeekMinutesLimit(int &aiLimitMinutes)
{
	aiLimitMinutes = g_tSettings.m_iShowWarningOnWeekMinutesLimit;
	return (aiLimitMinutes > 0);
}

bool CTaskShiftSettings::GetShowWarningOnDayMinutesLimit(int &aiLimitMinutes)
{
	aiLimitMinutes = g_tSettings.m_iShowWarningOnDayMinutesLimit;
	return (aiLimitMinutes > 0);
}

bool CTaskShiftSettings::GetUseCrashProtection(int &aiMinutesTolerance)
{
	aiMinutesTolerance = g_tSettings.m_iCrashProtectionTolerance;
	return (aiMinutesTolerance > 0);
}

void CTaskShiftSettings::SetShowWarningOnWeekMinutesLimit(bool abShow, int aiLimitMinutes)
{
	if (!abShow)
		aiLimitMinutes = 0;
	g_tSettings.m_iShowWarningOnWeekMinutesLimit = aiLimitMinutes;
}

void CTaskShiftSettings::SetShowWarningOnDayMinutesLimit(bool abShow, int aiLimitMinutes)
{
	if (!abShow)
		aiLimitMinutes = 0;
	g_tSettings.m_iShowWarningOnDayMinutesLimit = aiLimitMinutes;
}

void CTaskShiftSettings::SetUseCrashProtection(bool abUseProtection, int aiMinutesTolerance)
{
	if (!abUseProtection)
		aiMinutesTolerance = 0;
	g_tSettings.m_iCrashProtectionTolerance = aiMinutesTolerance;
}

int CTaskShiftSettings::GetAwayFromDeskGracePeriod()
{
	return g_tSettings.m_iAwayFromDeskGracePeriod;
}

void CTaskShiftSettings::SetAwayFromDeskGracePeriod(int aiSeconds)
{
	g_tSettings.m_iAwayFromDeskGracePeriod = aiSeconds;
}

bool CTaskShiftSettings::GetShowWarningOnNoUserInput(int &aiAfterSeconds)
{
	aiAfterSeconds = g_tSettings.m_iShowWarningOnNoUserInput;
	return (aiAfterSeconds > 0);
}

void CTaskShiftSettings::SetShowWarningOnNoUserInput(bool abShow, int aiAfterSeconds)
{
	if (!abShow)
		aiAfterSeconds = 0;
	g_tSettings.m_iShowWarningOnNoUserInput = aiAfterSeconds;
}

bool CTaskShiftSettings::Save()
{
	return g_tSettings.Close();
}