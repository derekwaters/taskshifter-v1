#include "stdafx.h"
#include "FWUtilDate.h"

#include <vector>

// LMJ #ifdef _DEBUG
// LMJ #undef THIS_FILE
// LMJ static char THIS_FILE[]=__FILE__;
// LMJ #define new DEBUG_NEW
// LMJ #endif

#define CFWUtilDate_NULL		_T("Null")
/////////////
// CFWUtilDate //
/////////////

CFWUtilDate::CFWUtilDate()
{
	SetCurrent();
	//m_cstrNull = _T("Null");
}

CFWUtilDate::CFWUtilDate(const CFWUtilDate &asdDate)
{
	SetDate(asdDate);
}

#ifdef __AFX_H__

CFWUtilDate::CFWUtilDate(const COleDateTime &aodtDate)
{
	SetDate(aodtDate);
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(const COleDateTime &aodtDate)
{
	m_DateTime.wYear = aodtDate.GetYear();
	m_DateTime.wMonth = aodtDate.GetMonth();
	m_DateTime.wDay = aodtDate.GetDay();
	m_DateTime.wHour = aodtDate.GetHour();
	m_DateTime.wMinute = aodtDate.GetMinute();
	m_DateTime.wSecond = aodtDate.GetSecond();
	m_DateTime.wMilliseconds = 0;
	m_DateTime.wDayOfWeek = 0;
	return GetStatus();
}

CFWUtilDate &CFWUtilDate::operator = (const COleDateTime &aodtDate)
{
	SetDate(aodtDate);
	return *this;
}

CFWUtilDate::operator DATE() const
{
	DATE vDate;
	if (IsValid())
	{
		vDate = (COleDateTime)*this;
	}
	return vDate;
}

CFWUtilDate &CFWUtilDate::operator = (DATE aDate)
{
	SetDate(COleDateTime(aDate));
	return *this;
}

CFWUtilDate::operator COleDateTime() const
{
	COleDateTime vodtRet;
	vodtRet.SetStatus(COleDateTime::null);
	if (IsValid())
		vodtRet.SetDateTime(m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay, m_DateTime.wHour, m_DateTime.wMinute, m_DateTime.wSecond);
	return vodtRet;
}

#endif

CFWUtilDate::CFWUtilDate(int aiFWUtilDateIndex)
{
	SetDate(aiFWUtilDateIndex);
}

CFWUtilDate::CFWUtilDate(int aiYear, int aiMonth, int aiDay, int aiHours, int aiMinutes, int aiSeconds, int aiMilliseconds)
{
	SetDateTime(aiYear, aiMonth, aiDay, aiHours, aiMinutes, aiSeconds, aiMilliseconds);
}

CFWUtilDate::CFWUtilDate(long alJulianDate)
{
	SetDate(alJulianDate);
}

CFWUtilDate::CFWUtilDate(SYSTEMTIME astDate)
{
	SetDate(astDate);
}

CFWUtilDate::CFWUtilDate(ULONGLONG aui64Val)
{
	SetDate(aui64Val);
}

CFWUtilDate::~CFWUtilDate()
{
}

bool CFWUtilDate::IsLeapYear() const
{
	bool vbRet = false;
	if (IsValid())
		vbRet = ((m_DateTime.wYear & 3) == 0) && ((m_DateTime.wYear % 100) != 0 || (m_DateTime.wYear % 400) == 0);
	return vbRet;
}

bool CFWUtilDate::IsNull() const
{
	return (GetStatus() == FWUtilDateStatus_Null);
}

bool CFWUtilDate::IsEmpty() const
{
	return IsNull();
}

void CFWUtilDate::Empty()
{
	memset(&m_DateTime, NULL, sizeof(m_DateTime));
}

bool CFWUtilDate::IsValid() const
{
	return (GetStatus() == FWUtilDateStatus_Valid);
}

int CFWUtilDate::GetYear() const
{
	return m_DateTime.wYear;
}

int CFWUtilDate::GetMonth() const
{
	return m_DateTime.wMonth;
}

int CFWUtilDate::GetDay() const
{
	return m_DateTime.wDay;
}

int CFWUtilDate::GetHours() const
{
	return m_DateTime.wHour;
}

int CFWUtilDate::GetMinutes() const
{
	return m_DateTime.wMinute;
}

int CFWUtilDate::GetSeconds() const
{
	return m_DateTime.wSecond;
}

int CFWUtilDate::GetMilliseconds() const
{
	return m_DateTime.wMilliseconds;
}

int CFWUtilDate::GetMinuteOfDay() const
{
	return (m_DateTime.wHour * 60) + m_DateTime.wMinute;
}

int CFWUtilDate::GetDayOfYear(FWUtilDateMonth_Constants aeStartOfYear)
{
	int viRet = 0;
	if (IsValid())
	{
		ULONGLONG vui64SOY = CFWUtilDate(m_DateTime.wYear, aeStartOfYear, 1);
		ULONGLONG vui64Date = CFWUtilDate(m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay);
		viRet = (int)((vui64Date - vui64SOY) / FWDATE_DAY) + 1;
	}
	return viRet;
}

FWUtilDateDay_Constants CFWUtilDate::GetDayOfWeek()
{
	FWUtilDateDay_Constants veRet = FWUtilDateDay_Sunday;
	if (IsValid())
	{
		SYSTEMTIME vstDate;
		FILETIME vftDate;
		if (SystemTimeToFileTime(&m_DateTime, &vftDate))
		{
			if (FileTimeToSystemTime(&vftDate, &vstDate))
			{
				veRet = (FWUtilDateDay_Constants)vstDate.wDayOfWeek;
			}
		}
	}
	return veRet;
}

FWUtilDateStatus_Constants CFWUtilDate::GetStatus() const
{
	FWUtilDateStatus_Constants veRet = FWUtilDateStatus_Invalid;
	if (m_DateTime.wYear == 0 && m_DateTime.wMonth == 0 && m_DateTime.wDay == 0 && m_DateTime.wHour == 0 && m_DateTime.wMinute == 0 && m_DateTime.wSecond == 0 && m_DateTime.wMilliseconds == 0)
	{
		veRet = FWUtilDateStatus_Null;
	}
	else
	{
		FILETIME vftDate;
		if (SystemTimeToFileTime(&m_DateTime, &vftDate) != 0)
		{
			veRet = FWUtilDateStatus_Valid;
		}
	}
	return veRet;
}

// Modified by DJW 20020724
//
int CFWUtilDate::GetTotalMonths() const
{
	return (((m_DateTime.wYear - FWDATE_BASE_YEAR) * 12) + m_DateTime.wMonth - 1);
}

FWUtilDateStatus_Constants CFWUtilDate::SetTotalMonths(int aiMonths)
{
	m_DateTime.wYear = FWDATE_BASE_YEAR + ((aiMonths) / 12);
	m_DateTime.wMonth = ((aiMonths) % 12) + 1;
	m_DateTime.wDay = 15;
	return GetStatus();
}
//
// End of modification

CFWUtilDate &CFWUtilDate::AddYears(int aiYears)
{
	if (IsValid())
	{
		m_DateTime.wYear += aiYears;
	}
	return *this;
}

CFWUtilDate &CFWUtilDate::AddMonths(int aiMonths)
{
	if (IsValid())
	{
		LONGLONG vi64Date = ((m_DateTime.wYear * 12) + m_DateTime.wMonth);
		vi64Date += aiMonths;
		m_DateTime.wYear = (int)((vi64Date - 1) / 12);
		m_DateTime.wMonth = (int)(((vi64Date - 1) % 12) + 1);

		// SJSG 11-01-05
		// OK - year and month have been updated, but the day may now be invalid!
		// eg. if date is 31/1/2005 and we add 1 month, we'll now have 31/2/2005
		// (day-of-month hasn't been touched) which is not a valid date...
		// So now we check if the data is valid - if not and day is > (days-in-Feb-in-leap-year)
		// decrement day-of-month until we hit a valid date. Note the <= 31 is to
		// make sure we ignore ridiculously large values.
		while(!IsValid() && (m_DateTime.wDay > 28) && (m_DateTime.wDay <= 31))
			m_DateTime.wDay--;

	}
	return *this;
}

CFWUtilDate &CFWUtilDate::AddWeeks(int aiWeeks)
{
	if (IsValid())
	{
		ULONGLONG vui64Date = *this;
		LONGLONG vi64Date = vui64Date + (aiWeeks * FWDATE_WEEK);
		if (vi64Date >= 0)
		{
			vui64Date = vi64Date;
			*this = vui64Date;
		}
	}
	return *this;
}

CFWUtilDate &CFWUtilDate::AddDays(int aiDays)
{
	if (IsValid())
	{
		ULONGLONG vui64Date = *this;
		LONGLONG vi64Date = vui64Date + (aiDays * FWDATE_DAY);
		if (vi64Date >= 0)
		{
			vui64Date = vi64Date;
			*this = vui64Date;
		}
	}
	return *this;
}

CFWUtilDate &CFWUtilDate::AddMinutes(int aiMinutes)
{
	if (IsValid())
	{
		ULONGLONG vui64Date = *this;
		LONGLONG vi64Date = vui64Date + (aiMinutes * FWDATE_MINUTE);
		if (vi64Date >= 0)
		{
			vui64Date = vi64Date;
			*this = vui64Date;
		}
	}
	return *this;
}

CFWUtilDate &CFWUtilDate::AddSeconds(int aiSeconds)
{
	if (IsValid())
	{
		ULONGLONG vui64Date = *this;
		LONGLONG vi64Date = vui64Date + (aiSeconds * FWDATE_SECOND);
		if (vi64Date >= 0)
		{
			vui64Date = vi64Date;
			*this = vui64Date;
		}
	}
	return *this;
}

__int64	 CFWUtilDate::Difference(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate)
{
	__int64 vi64Ret = 0;
	__int64 vi64Date1 = *this;
	__int64 vi64Date2 = aFWUtilDate;
	CFWUtilDate	vFWUtilDate1;
	CFWUtilDate vFWUtilDate2;
	CFWUtilDate vFWUtilDate1Year;
	CFWUtilDate vFWUtilDate2Year;
	__int64	viMonths = 0;

	CFWUtilDate	vdtTemp(aFWUtilDate);

	vi64Ret = (vi64Date1 - vi64Date2);
	if (vi64Ret < 0)
	{
		vi64Ret *= -1;
	}
	switch (aeType)
	{
	case FWUtilDateDiff_Seconds:
		return vi64Ret / FWDATE_SECOND;
		break;
	case FWUtilDateDiff_Minutes:
		return vi64Ret / FWDATE_MINUTE;
		break;
	case FWUtilDateDiff_Hours:
		return vi64Ret / FWDATE_HOUR;
		break;
	case FWUtilDateDiff_Days:
		return vi64Ret / FWDATE_DAY;
		break;
	case FWUtilDateDiff_Weeks:
		return vi64Ret / FWDATE_WEEK;
		break;
	// Added by MV 20040216
	//
	case FWUtilDateDiff_Months:

		if(*this < vdtTemp)
		{
			vFWUtilDate1 = *this;
			vFWUtilDate2 = aFWUtilDate;
		}
		else
		{
			vFWUtilDate1 = aFWUtilDate;
			vFWUtilDate2 = *this;
		}

		// Offset the dates to the first complete month
		if(vFWUtilDate1.GetDay() != 1)
		{
			vFWUtilDate1.SetDateTime(vFWUtilDate1.GetYear(), vFWUtilDate1.GetMonth(), 1);
			vFWUtilDate1.AddMonths(1);
		}
		if(vFWUtilDate2.GetDay() != 1)
		{
			vFWUtilDate2.SetDateTime(vFWUtilDate2.GetYear(), vFWUtilDate2.GetMonth(), 1);
		}

		// As long as the dates are still in order, we proceed
		if(vFWUtilDate1 < vFWUtilDate2)
		{
			vFWUtilDate1Year.SetDateTime(vFWUtilDate1.GetYear()+1, 1, 1);
			vFWUtilDate2Year.SetDateTime(vFWUtilDate2.GetYear(), 1, 1);

			if(vFWUtilDate1Year < vFWUtilDate2Year)
			{
				viMonths += vFWUtilDate1Year.Difference(FWUtilDateDiff_Years, vFWUtilDate2Year) * 12;
				viMonths += 12 - vFWUtilDate1.GetMonth();
				viMonths += vFWUtilDate2.GetMonth();
			}
			else
			{
				if(vFWUtilDate1.GetYear() == vFWUtilDate2.GetYear())
					viMonths = vFWUtilDate2.GetMonth() - vFWUtilDate1.GetMonth();
				else
					viMonths = (12 - vFWUtilDate1.GetMonth()) + (vFWUtilDate2.GetMonth());
			}
			return viMonths;
		}

		break;
	case FWUtilDateDiff_Years:

		if(*this < vdtTemp)
		{
			vFWUtilDate1 = *this;
			vFWUtilDate2 = aFWUtilDate;
		}
		else
		{
			vFWUtilDate1 = aFWUtilDate;
			vFWUtilDate2 = *this;
		}

		// Offset the dates to the complete year (next for 1st date, previous for 2nd date)
		if(vFWUtilDate1.GetDay() != 1 || vFWUtilDate1.GetMonth() != 1)
		{
			vFWUtilDate1.SetDate(vFWUtilDate1.GetYear()+1, 1, 1);
		}
		if(vFWUtilDate2.GetDay() != 1 || vFWUtilDate2.GetMonth() != 1)
		{
			vFWUtilDate2.SetDate(vFWUtilDate2.GetYear(), 1, 1);
		}

		// As long as the dates are still in order, we proceed
		if(vFWUtilDate1 < vFWUtilDate2)
		{
			return (vFWUtilDate2.GetYear() - vFWUtilDate1.GetYear());
		}

		break;
	//
	// End Addition
	}
	return 0;
}

__int64 CFWUtilDate::DifferenceEx(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate)
{
	__int64 vi64Ret = 0;
	__int64 vi64Date1 = *this;
	__int64 vi64Date2 = aFWUtilDate;

	vi64Ret = (vi64Date1 - vi64Date2);

	switch (aeType)
	{
	case FWUtilDateDiff_Seconds:
		return vi64Ret / FWDATE_SECOND;
		break;
	case FWUtilDateDiff_Minutes:
		return vi64Ret / FWDATE_MINUTE;
		break;
	case FWUtilDateDiff_Hours:
		return vi64Ret / FWDATE_HOUR;
		break;
	case FWUtilDateDiff_Days:
		return vi64Ret / FWDATE_DAY;
		break;
	case FWUtilDateDiff_Weeks:
		return vi64Ret / FWDATE_WEEK;
		break;
	}
	return 0;
}

double CFWUtilDate::Subtract(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate)
{
	__int64	vi64Ret = 0;
	__int64	vi64Date1 = *this;
	__int64	vi64Date2 = aFWUtilDate;

	vi64Ret = (vi64Date1 - vi64Date2);
	switch (aeType)
	{
	case FWUtilDateDiff_Seconds:
		return (double)vi64Ret / (double)FWDATE_SECOND;
		break;
	case FWUtilDateDiff_Minutes:
		return (double)vi64Ret / (double)FWDATE_MINUTE;
		break;
	case FWUtilDateDiff_Hours:
		return (double)vi64Ret / (double)FWDATE_HOUR;
		break;
	case FWUtilDateDiff_Days:
		return (double)vi64Ret / (double)FWDATE_DAY;
		break;
	case FWUtilDateDiff_Weeks:
		return (double)vi64Ret / (double)FWDATE_WEEK;
		break;
	}
	return 0.0;
}

CString CFWUtilDate::GetDateTimeString(bool abUseLongDate, bool abUseLongTime) const
{
	return GetLocaleDateTimeString(LOCALE_USER_DEFAULT, abUseLongDate, abUseLongTime);
//SJSG	CString vcstrRet;
//SJSG	vcstrRet.Format(_T("%s %s"), GetDateString(abUseLongDate), GetTimeString(abUseLongTime));
//SJSG	return vcstrRet;
}

CString CFWUtilDate::GetLocaleDateTimeString(LCID alcid, bool abUseLongDate, bool abUseLongTime) const
{
	CString vcstrRet;
	vcstrRet.Format(_T("%s %s"), GetLocaleDateString(alcid, abUseLongDate), GetLocaleTimeString(alcid, abUseLongTime));
	return vcstrRet;
}

CString CFWUtilDate::GetDateString(bool abUseLongDescription) const
{
	return GetLocaleDateString(LOCALE_USER_DEFAULT, abUseLongDescription);

//SJSG 	CString vcstrRet;
//SJSG 	DWORD vdwFlags = 0;
//SJSG 	TCHAR vtszDate[2000];
//SJSG 	if (IsValid())
//SJSG 	{
//SJSG 		if (abUseLongDescription)
//SJSG 		{
//SJSG 			vdwFlags = DATE_LONGDATE;
//SJSG 		}
//SJSG 		else
//SJSG 		{
//SJSG 			vdwFlags = DATE_SHORTDATE;
//SJSG 		}
//SJSG 		if (GetDateFormat(LOCALE_USER_DEFAULT, vdwFlags, &m_DateTime, NULL, vtszDate, sizeof(vtszDate)))
//SJSG 		{
//SJSG 			vcstrRet = vtszDate;
//SJSG 		}
//SJSG 	}
//SJSG 	else if (IsNull())
//SJSG 	{
//SJSG 		vcstrRet = _T("Null"); //m_cstrNull;
//SJSG 	}
//SJSG 	return vcstrRet;
}

CString CFWUtilDate::GetLocaleDateString(LCID alcid, bool abUseLongDescription) const
{
	CString vcstrRet;
	DWORD vdwFlags = 0;
	TCHAR vtszDate[2000];
	if (IsValid())
	{
		if (abUseLongDescription)
		{
			vdwFlags = DATE_LONGDATE;
		}
		else
		{
			vdwFlags = DATE_SHORTDATE;
		}
		if (GetDateFormat(alcid, vdwFlags, &m_DateTime, NULL, vtszDate, sizeof(vtszDate)))
		{
			vcstrRet = vtszDate;
		}
	}
	else if (IsNull())
	{
		vcstrRet = _T("Null"); //m_cstrNull;
	}
	return vcstrRet;
}

CString CFWUtilDate::GetTimeString(bool abUseLongDescription) const
{
	return GetLocaleTimeString(LOCALE_USER_DEFAULT, abUseLongDescription);
}

/**
 Will format times so AM/PM etc are correct given locale
 */
CString CFWUtilDate::GetLocaleTimeString(LCID alcid, bool abUseLongDescription) const
{
	CString vcstrRet;
	DWORD vdwFlags = 0;
	TCHAR vtszDate[2000];
	if (IsValid())
	{
		if (abUseLongDescription)
		{
			vdwFlags = 0;
		}
		else
		{
			vdwFlags = TIME_NOSECONDS;
		}
		if (GetTimeFormat(alcid, vdwFlags, &m_DateTime, NULL, vtszDate, sizeof(vtszDate)))
		{
			vcstrRet = vtszDate;
		}
	}
	else if (IsNull())
	{
		vcstrRet = _T("Null"); //m_cstrNull;
	}
	return vcstrRet;
}

CString CFWUtilDate::GetTimeString(bool ab24Hour, bool abIncludeSeconds) const
{
	return GetLocaleTimeString(LOCALE_USER_DEFAULT, ab24Hour, abIncludeSeconds);
}

/**
 Will format times so AM/PM etc are correct given locale
 */
CString CFWUtilDate::GetLocaleTimeString(LCID alcid, bool ab24Hour, bool abIncludeSeconds) const
{
	CString vstrFormat;
	CString	vstrRet;

	if (abIncludeSeconds)
	{
		if (ab24Hour)
		{
			vstrFormat = _T("H':'mm':'ss");
		}
		else
		{
			vstrFormat = _T("h':'mm':'ss tt");
		}
	}
	else
	{
		if (ab24Hour)
		{
			vstrFormat = _T("H':'mm");
		}
		else
		{
			vstrFormat = _T("h':'mm tt");
		}
	}
	::GetTimeFormat(alcid, 0, &m_DateTime, vstrFormat, vstrRet.GetBuffer(256), 256);
	vstrRet.ReleaseBuffer();
	return vstrRet;
}

// Added by MV 20030807
//
CString CFWUtilDate::GetTimeString(LPCTSTR aszDisplayFormat) const
{
	CString vcstrRet;
	DWORD vdwFlags = 0;
	TCHAR vtszDate[2000];
	if (IsValid() && aszDisplayFormat != NULL)
	{
		if (GetTimeFormat(LOCALE_USER_DEFAULT, vdwFlags, &m_DateTime, aszDisplayFormat, vtszDate, sizeof(vtszDate)))
		{
			vcstrRet = vtszDate;
		}
	}
	else if (IsNull())
	{
		vcstrRet = _T("Null");
	}
	return vcstrRet;
}
//
// End of Addition

// Added by MV 20030807
//
CString CFWUtilDate::GetLocaleTimeString(LCID alcid, LPCTSTR aszDisplayFormat) const
{
	CString vcstrRet;
	DWORD vdwFlags = 0;
	TCHAR vtszDate[2000];
	if (IsValid() && aszDisplayFormat != NULL)
	{
		if (GetTimeFormat(alcid, vdwFlags, &m_DateTime, aszDisplayFormat, vtszDate, sizeof(vtszDate)))
		{
			vcstrRet = vtszDate;
		}
	}
	else if (IsNull())
	{
		vcstrRet = _T("Null");
	}
	return vcstrRet;
}
//
// End of Addition

CString CFWUtilDate::GetFWUtilDateString() const
{
	CString vcstrDateString;
	vcstrDateString.Format(_T("%04i%02i%02i"), m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay);
	return vcstrDateString;
}

CString CFWUtilDate::GetTimeStamp() const
{
	CString vstrTimeStamp;
	vstrTimeStamp.Format(_T("%04i%02i%02i %02i%02i%02i"), m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay, m_DateTime.wHour, m_DateTime.wMinute, m_DateTime.wSecond);
	return vstrTimeStamp;
}

FWUtilDateStatus_Constants CFWUtilDate::SetFWUtilDateString(LPCTSTR aszFWUtilDate)
{
	FWUtilDateStatus_Constants	veRet = FWUtilDateStatus_Invalid;
	bool					vbOK = true;
	int						viYear;
	int						viMonth;
	int						viDay;
	TCHAR					vszTemp[5];

	if (aszFWUtilDate && _tcslen(aszFWUtilDate) == 8)
	{
		for (int i = 0; i < 8; i++)
		{
			if (!isdigit(aszFWUtilDate[i]))
			{
				vbOK = false;
				break;
			}
		}
		if (vbOK)
		{
			memset(vszTemp, NULL, sizeof(vszTemp));
			_tcsncpy_s(vszTemp, sizeof(vszTemp), aszFWUtilDate, 4);
			viYear = atoi(vszTemp);
			memset(vszTemp, NULL, sizeof(vszTemp));
			_tcsncpy_s(vszTemp, sizeof(vszTemp), aszFWUtilDate + 4, 2);
			viMonth = atoi(vszTemp);
			memset(vszTemp, NULL, sizeof(vszTemp));
			_tcsncpy_s(vszTemp, sizeof(vszTemp), aszFWUtilDate + 6, 2);
			viDay = atoi(vszTemp);
			veRet = SetDateTime(viYear, viMonth, viDay);
		}
	}
	return veRet;
}

CString CFWUtilDate::GetOracleDateString(bool abIncludeEqualSigns, bool abAssignment)
{
	CString vcstrDateString;
	CString vcstrTemp;
	if (IsValid())
	{
		vcstrTemp.Format(_T("%04i%02i%02i"), m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay);
		if (abIncludeEqualSigns)
		{
			vcstrDateString.Format(_T("= TO_DATE('%s', 'YYYYMMDD')"), vcstrTemp);
		}
		else
		{
			vcstrDateString.Format(_T("TO_DATE('%s', 'YYYYMMDD')"), vcstrTemp);
		}
	}
	else if (IsNull())
	{
		if (abAssignment)
		{
			if (abIncludeEqualSigns)
			{
				vcstrDateString = _T("= NULL");
			}
			else
			{
				vcstrDateString = _T("NULL");
			}
		}
		else
		{
			vcstrDateString = _T("IS NULL");
		}
	}
	return vcstrDateString;
}

CString CFWUtilDate::GetOracleDateTimeString(bool abIncludeEqualSigns, bool abAssignment)
{
	CString vcstrDateString;
	CString vcstrTemp;
	if (IsValid())
	{
		vcstrTemp.Format(_T("%04i%02i%02i %02i:%02i:%02i"), m_DateTime.wYear, m_DateTime.wMonth, m_DateTime.wDay, m_DateTime.wHour, m_DateTime.wMinute, m_DateTime.wSecond);
		if (abIncludeEqualSigns)
		{
			vcstrDateString.Format(_T("= TO_DATE('%s', 'YYYYMMDD HH24:MI:SS')"), vcstrTemp);
		}
		else
		{
			vcstrDateString.Format(_T("TO_DATE('%s', 'YYYYMMDD HH24:MI:SS')"), vcstrTemp);
		}
	}
	else if (IsNull())
	{
		if (abAssignment)
		{
			if (abIncludeEqualSigns)
			{
				vcstrDateString = _T("= NULL");
			}
			else
			{
				vcstrDateString = _T("NULL");
			}
		}
		else
		{
			vcstrDateString = _T("IS NULL");
		}
	}
	return vcstrDateString;
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(int aiYear, int aiMonth, int aiDay)
{
	m_DateTime.wYear = aiYear;
	m_DateTime.wMonth = aiMonth;
	m_DateTime.wDay = aiDay;
	m_DateTime.wDayOfWeek = 0;
	m_DateTime.wHour = 0;
	m_DateTime.wMinute = 0;
	m_DateTime.wSecond = 0;
	m_DateTime.wMilliseconds = 0;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetTime(int aiHours, int aiMinutes, int aiSeconds, int aiMilliseconds)
{
	m_DateTime.wHour = aiHours;
	m_DateTime.wMinute = aiMinutes;
	m_DateTime.wSecond = aiSeconds;
	m_DateTime.wMilliseconds = aiMilliseconds;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetTime(int aiMinuteOfDay)
{
	if (aiMinuteOfDay >= 0 && aiMinuteOfDay < 1440 && IsValid())
	{
		m_DateTime.wHour = 0;
		m_DateTime.wMinute = 0;
		m_DateTime.wSecond = 0;
		m_DateTime.wMilliseconds = 0;
		AddMinutes(aiMinuteOfDay);
	}
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetDateTime(int aiYear, int aiMonth, int aiDay, int aiHours, int aiMinutes, int aiSeconds, int aiMilliseconds)
{
	m_DateTime.wYear = aiYear;
	m_DateTime.wMonth = aiMonth;
	m_DateTime.wDay = aiDay;
	m_DateTime.wHour = aiHours;
	m_DateTime.wMinute = aiMinutes;
	m_DateTime.wSecond = aiSeconds;
	m_DateTime.wMilliseconds = aiMilliseconds;
	m_DateTime.wDayOfWeek = 0;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(const CFWUtilDate &asdDate)
{
	m_DateTime.wYear = asdDate.GetYear();
	m_DateTime.wMonth = asdDate.GetMonth();
	m_DateTime.wDay = asdDate.GetDay();
	m_DateTime.wHour = asdDate.GetHours();
	m_DateTime.wMinute = asdDate.GetMinutes();
	m_DateTime.wSecond = asdDate.GetSeconds();
	m_DateTime.wMilliseconds = 0;
	m_DateTime.wDayOfWeek = 0;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(int aiFWUtilDateIndex)
{
	ULONGLONG vui64BaseDate = CFWUtilDate(FWDATE_BASE_YEAR, FWDATE_BASE_MONTH, FWDATE_BASE_DAY);
	LONGLONG vi64Difference = aiFWUtilDateIndex * FWDATE_DAY;
	ULONGLONG vui64Date = vi64Difference + vui64BaseDate;
	Clear();
	return SetDate(vui64Date);
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(long alJulianDate)
{
	long L;
	long N;
	long I;
	long J;
	L = alJulianDate + 68569;
	N = (long)((4*L)/146097) ;
	L = L - ((long)((146097 * N + 3)/4) );
	I = (long) ((4000 *(L + 1)/1461001)) ;
	L = L - (long)((1461*I)/4) + 31; 
	J = (long)((80*L)/2447); 
	m_DateTime.wDay = (WORD)(L - (long)((2447*J)/80));
	L = (long)(J/11) ;
	m_DateTime.wMonth = (WORD)(J + 2 - 12*L);
	m_DateTime.wYear = (WORD)(100*(N-49) + I + L);
	m_DateTime.wHour = 0;
	m_DateTime.wMinute = 0;
	m_DateTime.wSecond = 0;
	m_DateTime.wMilliseconds = 0;
	m_DateTime.wDayOfWeek = 0;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(SYSTEMTIME astDate)
{
	m_DateTime.wYear = astDate.wYear;
	m_DateTime.wMonth = astDate.wMonth;
	m_DateTime.wDay = astDate.wDay;
	m_DateTime.wHour = astDate.wHour;
	m_DateTime.wMinute = astDate.wMinute;
	m_DateTime.wSecond = astDate.wSecond;
	m_DateTime.wMilliseconds = astDate.wMilliseconds;
	m_DateTime.wDayOfWeek = 0;
	return GetStatus();
}

FWUtilDateStatus_Constants CFWUtilDate::SetDate(ULONGLONG aui64Val)
{
	FWUtilDateStatus_Constants veRet = FWUtilDateStatus_Invalid;
	SYSTEMTIME vstDate;
	FILETIME vftDate;
	vftDate = *(FILETIME*)&aui64Val;
	if (FileTimeToSystemTime(&vftDate, &vstDate))
	{
		veRet = SetDate(vstDate);
	}
	return veRet;
}

void CFWUtilDate::SetCurrent()
{
	SYSTEMTIME vstCurrent;
	memset(&vstCurrent, 0, sizeof(vstCurrent));
	GetLocalTime(&vstCurrent);
	SetDate(vstCurrent);
}

void CFWUtilDate::Clear()
{
	m_DateTime.wYear = 0;
	m_DateTime.wMonth = 0;
	m_DateTime.wDay = 0;
	m_DateTime.wHour = 0;
	m_DateTime.wMinute = 0;
	m_DateTime.wSecond = 0;
	m_DateTime.wMilliseconds = 0;
	m_DateTime.wDayOfWeek = 0;
}

bool CFWUtilDate::operator != (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 != vui64Date2);
/*
	bool vbRet = true; // Assume dates are not equal
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 != vui64Date2);
	}
	else if (IsNull() && asdDate.IsNull())
	{
		vbRet = false;
	}
	return vbRet;
*/
}

bool CFWUtilDate::operator < (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 < vui64Date2);
/*
	bool vbRet = false;
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 < vui64Date2);
	}
	return vbRet;
*/
}

bool CFWUtilDate::operator <= (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 <= vui64Date2);
/*
	bool vbRet = false;
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 <= vui64Date2);
	}
	return vbRet;
*/
}

bool CFWUtilDate::operator == (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 == vui64Date2);
/*
	bool vbRet = false;
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 == vui64Date2);
	}
	else
	{
		vbRet = (IsNull() && asdDate.IsNull());
	}
	return vbRet;
*/
}

bool CFWUtilDate::operator > (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 > vui64Date2);
/*
	bool vbRet = false;
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 > vui64Date2);
	}
	return vbRet;
*/
}

bool CFWUtilDate::operator >= (const CFWUtilDate& asdDate) const
{
	ULONGLONG vui64Date1 = *this;
	ULONGLONG vui64Date2 = asdDate;

	if (!IsValid())
	{
		vui64Date1 = -1;	
	}
	
	if (!asdDate.IsValid())
	{
		vui64Date2 = -1;	
	}

	return (vui64Date1 >= vui64Date2);
/*
	bool vbRet = false;
	if (IsValid() && asdDate.IsValid())
	{
		ULONGLONG vui64Date1 = *this;
		ULONGLONG vui64Date2 = asdDate;
		vbRet = (vui64Date1 >= vui64Date2);
	}
	return vbRet;
*/
}

CFWUtilDate &CFWUtilDate::operator = (const CFWUtilDate& asdDate)
{
	SetDate(asdDate);
	return *this;
}

CFWUtilDate &CFWUtilDate::operator = (const SYSTEMTIME &astDate)
{
	SetDate(astDate);
	return *this;
}

CFWUtilDate &CFWUtilDate::operator = (int aiFWUtilDateIndex)
{
	SetDate(aiFWUtilDateIndex);
	return *this;
}

CFWUtilDate &CFWUtilDate::operator = (long alJulianDate)
{
	SetDate(alJulianDate);
	return *this;
}

CFWUtilDate &CFWUtilDate::operator = (ULONGLONG aui64Val)
{
	SetDate(aui64Val);
	return *this;
}

CFWUtilDate::operator int() const
{
	int viRet = 0;
	if (IsValid())
	{
		ULONGLONG vui64BaseDate = CFWUtilDate(FWDATE_BASE_YEAR, FWDATE_BASE_MONTH, FWDATE_BASE_DAY);
		ULONGLONG vui64Date = *this;
		if (vui64Date >= vui64BaseDate)
		{
			LONGLONG vi64Difference = vui64Date - vui64BaseDate;

			viRet = (int)(vi64Difference / FWDATE_DAY);
		}
		else
		{
			LONGLONG vi64Difference = vui64BaseDate - vui64Date;

			viRet = -(int)(vi64Difference / FWDATE_DAY);
		}
	}
	return viRet;
}

CFWUtilDate::operator long() const
{
	long vlRet = 0;
	if (IsValid())
	{
		int y = m_DateTime.wYear;
		int m = m_DateTime.wMonth;
		int d = m_DateTime.wDay;
		if (m < 3)
		{
			m = m + 12;
			y = y - 1;
		}
		vlRet = d + (153 * m - 457) / 5 + 365 * y + (y / 4) - (y / 100) + (y / 400) + 1721119;
	}
	return vlRet;
}

CFWUtilDate::operator LPCTSTR() const
{
	return GetDateString();
}

CFWUtilDate::operator SYSTEMTIME() const
{
	SYSTEMTIME vstRet;
	memset(&vstRet, 0, sizeof(vstRet));
	if (IsValid())
	{
		FILETIME vftTemp;
		memset(&vftTemp, 0, sizeof(vftTemp));
		vstRet.wYear = m_DateTime.wYear;
		vstRet.wMonth = m_DateTime.wMonth;
		vstRet.wDay = m_DateTime.wDay;
		vstRet.wHour = m_DateTime.wHour;
		vstRet.wMinute = m_DateTime.wMinute;
		vstRet.wSecond = m_DateTime.wSecond;
		vstRet.wMilliseconds = m_DateTime.wMilliseconds;
		if (SystemTimeToFileTime(&vstRet, &vftTemp))
		{
			FileTimeToSystemTime(&vftTemp, &vstRet);
		}
	}
	return vstRet;
}

CFWUtilDate::operator ULONGLONG() const
{
	ULONGLONG vui64Ret = 0;
	if (IsValid())
	{
		FILETIME vftDateTime;
		if (SystemTimeToFileTime(&m_DateTime, &vftDateTime))
		{
			vui64Ret = *(ULONGLONG*)&vftDateTime;
		}
	}
	return vui64Ret;
}

CFWUtilDate::operator __int64() const
{
	return (ULONGLONG)*this;
// LMJ 	ULONGLONG vui64Ret = 0;
// LMJ 	if (IsValid())
// LMJ 	{
// LMJ 		FILETIME vftDateTime;
// LMJ 		if (SystemTimeToFileTime(&m_DateTime, &vftDateTime))
// LMJ 		{
// LMJ 			vui64Ret = *(ULONGLONG*)&vftDateTime;
// LMJ 		}
// LMJ 	}
// LMJ 	return vui64Ret;
}

CString CFWUtilDate::GetNameOfDay(int aiDay, bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfDay(LOCALE_USER_DEFAULT, aiDay, abUseLongDescription);

//SJSG 	CString vcstrRet;
//SJSG 
//SJSG 	if (aiDay >= 0 && aiDay < 7)
//SJSG 	{
//SJSG 		if (abUseLongDescription)
//SJSG 		{
//SJSG 			switch (aiDay)
//SJSG 			{
//SJSG 			case FWUtilDateDay_Sunday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Monday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Tuesday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Wednesday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Thursday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Friday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Saturday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME6);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 		else
//SJSG 		{
//SJSG 			switch (aiDay)
//SJSG 			{
//SJSG 			case FWUtilDateDay_Sunday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Monday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Tuesday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Wednesday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Thursday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Friday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateDay_Saturday:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME6);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 	}
//SJSG 	return vcstrRet;
}

CString CFWUtilDate::GetLocaleNameOfDay(LCID alcid, int aiDay, bool abUseLongDescription)
{
	CString vcstrRet;

	if (aiDay >= 0 && aiDay < 7)
	{
		if (abUseLongDescription)
		{
			switch (aiDay)
			{
			case FWUtilDateDay_Sunday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME7);
				break;
			case FWUtilDateDay_Monday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME1);
				break;
			case FWUtilDateDay_Tuesday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME2);
				break;
			case FWUtilDateDay_Wednesday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME3);
				break;
			case FWUtilDateDay_Thursday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME4);
				break;
			case FWUtilDateDay_Friday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME5);
				break;
			case FWUtilDateDay_Saturday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SDAYNAME6);
				break;
			}
		}
		else
		{
			switch (aiDay)
			{
			case FWUtilDateDay_Sunday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME7);
				break;
			case FWUtilDateDay_Monday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME1);
				break;
			case FWUtilDateDay_Tuesday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME2);
				break;
			case FWUtilDateDay_Wednesday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME3);
				break;
			case FWUtilDateDay_Thursday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME4);
				break;
			case FWUtilDateDay_Friday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME5);
				break;
			case FWUtilDateDay_Saturday:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVDAYNAME6);
				break;
			}
		}
	}
	return vcstrRet;
}

CString CFWUtilDate::GetNameOfMonth(int aiMth, bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfMonth(LOCALE_USER_DEFAULT, aiMth, abUseLongDescription);
}

CString CFWUtilDate::GetLocaleNameOfMonth(LCID alcid, int aiMth, bool abUseLongDescription)
{
	CString vcstrRet;

	if (aiMth > 0 && aiMth <= 12)
	{
		if (abUseLongDescription)
		{
			switch (aiMth)
			{
			case FWUtilDateMonth_January:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME1);
				break;
			case FWUtilDateMonth_February:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME2);
				break;
			case FWUtilDateMonth_March:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME3);
				break;
			case FWUtilDateMonth_April:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME4);
				break;
			case FWUtilDateMonth_May:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME5);
				break;
			case FWUtilDateMonth_June:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME6);
				break;
			case FWUtilDateMonth_July:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME7);
				break;
			case FWUtilDateMonth_August:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME8);
				break;
			case FWUtilDateMonth_September:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME9);
				break;
			case FWUtilDateMonth_October:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME10);
				break;
			case FWUtilDateMonth_November:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME11);
				break;
			case FWUtilDateMonth_December:
				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME12);
				break;
			}
		}
		else
		{
			switch (aiMth)
			{
			case FWUtilDateMonth_January:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME1);
				break;
			case FWUtilDateMonth_February:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME2);
				break;
			case FWUtilDateMonth_March:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME3);
				break;
			case FWUtilDateMonth_April:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME4);
				break;
			case FWUtilDateMonth_May:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME5);
				break;
			case FWUtilDateMonth_June:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME6);
				break;
			case FWUtilDateMonth_July:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME7);
				break;
			case FWUtilDateMonth_August:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME8);
				break;
			case FWUtilDateMonth_September:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME9);
				break;
			case FWUtilDateMonth_October:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME10);
				break;
			case FWUtilDateMonth_November:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME11);
				break;
			case FWUtilDateMonth_December:
				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME12);
				break;
			}
		}
	}

	return vcstrRet;
}

CString CFWUtilDate::GetDayName(bool abUseLongDescription)
{
	return GetLocaleNameOfDay(LOCALE_USER_DEFAULT, GetDayOfWeek(), abUseLongDescription);
}

CString CFWUtilDate::GetLocaleDayName(LCID alcid, bool abUseLongDescription)
{
	return GetLocaleNameOfDay(alcid, GetDayOfWeek(), abUseLongDescription);
}

CString CFWUtilDate::GetMonthName(bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfMonth(LOCALE_USER_DEFAULT, m_DateTime.wMonth, abUseLongDescription);

//SJSG 	CString vcstrRet;
//SJSG 	if (IsValid())
//SJSG 	{
//SJSG 		if (abUseLongDescription)
//SJSG 		{
//SJSG 			switch (m_DateTime.wMonth)
//SJSG 			{
//SJSG 			case FWUtilDateMonth_January:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_February:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_March:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_April:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_May:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_June:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME6);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_July:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_August:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME8);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_September:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME9);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_October:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME10);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_November:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME11);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_December:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SMONTHNAME12);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 		else
//SJSG 		{
//SJSG 			switch (m_DateTime.wMonth)
//SJSG 			{
//SJSG 			case FWUtilDateMonth_January:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_February:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_March:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_April:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_May:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_June:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME6);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_July:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_August:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME8);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_September:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME9);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_October:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME10);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_November:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME11);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_December:
//SJSG 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVMONTHNAME12);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 	}
//SJSG 	return vcstrRet;
}

CString CFWUtilDate::GetLocaleMonthName(LCID alcid, bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfMonth(alcid, m_DateTime.wMonth, abUseLongDescription);

//SJSG 	CString vcstrRet;
//SJSG 	if (IsValid())
//SJSG 	{
//SJSG 		if (abUseLongDescription)
//SJSG 		{
//SJSG 			switch (m_DateTime.wMonth)
//SJSG 			{
//SJSG 			case FWUtilDateMonth_January:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_February:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_March:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_April:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_May:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_June:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME6);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_July:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_August:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME8);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_September:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME9);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_October:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME10);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_November:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME11);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_December:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SMONTHNAME12);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 		else
//SJSG 		{
//SJSG 			switch (m_DateTime.wMonth)
//SJSG 			{
//SJSG 			case FWUtilDateMonth_January:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME1);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_February:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME2);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_March:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME3);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_April:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME4);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_May:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME5);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_June:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME6);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_July:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME7);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_August:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME8);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_September:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME9);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_October:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME10);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_November:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME11);
//SJSG 				break;
//SJSG 			case FWUtilDateMonth_December:
//SJSG 				vcstrRet = LocaleInfo(alcid, LOCALE_SABBREVMONTHNAME12);
//SJSG 				break;
//SJSG 			}
//SJSG 		}
//SJSG 	}
//SJSG 	return vcstrRet;
}


CString CFWUtilDate::GetDayOfWeekName(bool abUseLongDescription)
{
// LMJ 	CString vcstrRet;
// LMJ 	
// LMJ 	if (IsValid())
// LMJ 	{
// LMJ 		if (abUseLongDescription)
// LMJ 		{
// LMJ 			switch (GetDayOfWeek())
// LMJ 			{
// LMJ 			case FWUtilDateDay_Monday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME1);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Tuesday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME2);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Wednesday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME3);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Thursday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME4);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Friday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME5);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Saturday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME6);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Sunday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SDAYNAME7);
// LMJ 				break;
// LMJ 			}
// LMJ 		}
// LMJ 		else
// LMJ 		{
// LMJ 			switch (GetDayOfWeek())
// LMJ 			{
// LMJ 			case FWUtilDateDay_Monday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME1);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Tuesday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME2);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Wednesday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME3);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Thursday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME4);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Friday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME5);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Saturday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME6);
// LMJ 				break;
// LMJ 			case FWUtilDateDay_Sunday:
// LMJ 				vcstrRet = UserLocaleInfo(LOCALE_SABBREVDAYNAME7);
// LMJ 				break;
// LMJ 			}
// LMJ 		}
// LMJ 	}
// LMJ 	return vcstrRet;
	return GetNameOfDay(GetDayOfWeek(), abUseLongDescription);
}

CString CFWUtilDate::GetLocaleDayOfWeekName(LCID alcid, bool abUseLongDescription)
{
	return GetLocaleNameOfDay(alcid, GetDayOfWeek(), abUseLongDescription);
}

CString CFWUtilDate::UserLocaleInfo(LCTYPE LCType)
{
	CString vcstrRet;
	int viNumChars = GetLocaleInfo(LOCALE_USER_DEFAULT, LCType, NULL, 0);
	if (viNumChars > 0)
	{
		TCHAR* aptszInfo = new TCHAR[viNumChars];
		if (aptszInfo)
		{
			GetLocaleInfo(LOCALE_USER_DEFAULT, LCType, aptszInfo, viNumChars);
			vcstrRet = aptszInfo;
			delete [] aptszInfo;
		}
	}
	return vcstrRet;
}

CString CFWUtilDate::LocaleInfo(LCID alcid, LCTYPE LCType)
{
	CString vcstrRet;
	int viNumChars = GetLocaleInfo(alcid, LCType, NULL, 0);
	if (viNumChars > 0)
	{
		TCHAR* aptszInfo = new TCHAR[viNumChars];
		if (aptszInfo)
		{
			GetLocaleInfo(alcid, LCType, aptszInfo, viNumChars);
			vcstrRet = aptszInfo;
			delete [] aptszInfo;
		}
	}
	return vcstrRet;
}

bool CFWUtilDate::SetToWCDate(FWUtilDateDay_Constants aeStartOfWeek)
{
	if (IsValid() && aeStartOfWeek >= 0 && aeStartOfWeek < FWUtilDateDay_Count)
	{
		if (GetDayOfWeek() != aeStartOfWeek)
		{
			int viDays = (int)GetDayOfWeek() - (int)aeStartOfWeek;
			if (viDays < 0)
			{
				viDays += (int)FWUtilDateDay_Count;
			}
			AddDays(-viDays);
			return IsValid();
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool CFWUtilDate::IsLastDayOfWeek(FWUtilDateDay_Constants aeStartOfWeek)
{
	bool vbRet = false;

	if (IsValid())
	{
		AddDays(1);
		vbRet = (GetDayOfWeek() == aeStartOfWeek);
		AddDays(-1);
	}
	return vbRet;
}

bool CFWUtilDate::SetToFirstDayOfWeek(int aiStartOfWeek)
{
	if (IsValid() && aiStartOfWeek >= 0 && aiStartOfWeek < FWUtilDateDay_Count)
	{
		if ((int)GetDayOfWeek() != aiStartOfWeek)
		{
			int viDays = (int)GetDayOfWeek() - aiStartOfWeek;
			if (viDays < 0)
			{
				viDays += (int)FWUtilDateDay_Count;
			}
			AddDays(-viDays);
			return IsValid();
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool CFWUtilDate::SetToLastDayOfWeek(int aiStartOfWeek)
{
	if (SetToFirstDayOfWeek(aiStartOfWeek))
	{
		AddDays(FWUtilDateDay_Count - 1);
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::IsFirstDayOfWeek(int aiStartOfWeek)
{
	if (IsValid() && aiStartOfWeek >= 0 && aiStartOfWeek < FWUtilDateDay_Count)
	{
		return ((int)GetDayOfWeek() == aiStartOfWeek);
	}
	return false;
}

bool CFWUtilDate::IsLastDayOfWeek(int aiStartOfWeek)
{
	if (IsValid() && aiStartOfWeek >= 0 && aiStartOfWeek < FWUtilDateDay_Count)
	{
		return ((int)GetDayOfWeek() == (aiStartOfWeek + (FWUtilDateDay_Count - 1)));
	}
	return false;
}

bool CFWUtilDate::SetToFirstDayOfMonth()
{
	if (IsValid())
	{
		m_DateTime.wDay = 1;
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::SetToLastDayOfMonth()
{
	if (SetToFirstDayOfMonth())
	{
		AddMonths(1);
		AddDays(-1);
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::IsLastDayOfMonth()
{
	bool	vbRet = false;
	int		viMonth;

	if (IsValid())
	{
		viMonth = m_DateTime.wMonth;
		AddDays(1);
		if(viMonth == 12 && m_DateTime.wMonth == 1)
		{
			vbRet = true;
		}
		else
		{
			vbRet = (m_DateTime.wMonth - 1) == viMonth;
		}
		AddDays(-1);
	}
	return vbRet;
}

bool CFWUtilDate::IsLastDayOfQuarter()
{
	bool	vbRet = false;
	int		viQuarter;
	int		viCompareQuarter;

	if (IsValid())
	{
		viQuarter = (m_DateTime.wMonth - 1) / 4;
		AddDays(1);
		viCompareQuarter = (m_DateTime.wMonth - 1) / 4;

		if(viQuarter == 3 && viCompareQuarter == 0)
		{
			vbRet = true;
		}
		else
		{
			vbRet = (viCompareQuarter - 1) == viQuarter;
		}
		AddDays(-1);
	}
	return vbRet;
}

bool CFWUtilDate::SetToFirstDayOfQuarter()
{
	if (IsValid())
	{
		m_DateTime.wDay = 1;
		m_DateTime.wMonth = 1 + (3 * ((m_DateTime.wMonth - 1) / 3));
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::SetToLastDayOfQuarter()
{
	if (SetToFirstDayOfQuarter())
	{
		AddMonths(3);
		AddDays(-1);
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::SetToFirstDayOfYear()
{
	if (IsValid())
	{
		m_DateTime.wDay = 1;
		m_DateTime.wMonth = 1;
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::SetToLastDayOfYear()
{
	if (IsValid())
	{
		m_DateTime.wDay = 31;
		m_DateTime.wMonth = 12;
		return IsValid();
	}
	return false;
}

bool CFWUtilDate::IsFirstDayOfYear()
{
	bool vbRet = false;

	if (IsValid())
	{
		vbRet = (m_DateTime.wDay == 1 && m_DateTime.wMonth == 1);
	}
	return vbRet;
}

bool CFWUtilDate::IsLastDayOfYear()
{
	bool vbRet = false;

	if (IsValid())
	{
		vbRet = (m_DateTime.wDay == 31 && m_DateTime.wMonth == 12);
	}
	return vbRet;
}

CString CFWUtilDate::ToLocalIndependantFormat()
{
	CString vcstrRet;

	vcstrRet.Format(_T("%0.4d%0.2d%0.2d%0.2d%0.2d%0.2d%0.3d"),	GetYear(), 
																GetMonth(), 
																GetDay(), 
																GetHours(), 
																GetMinutes(), 
																GetSeconds(), 
																GetMilliseconds());

	return vcstrRet;
}

bool CFWUtilDate::FromLocalIndependantFormat(LPCTSTR aszString)
{
	bool vbRet = false;

	CString vcstrDateTime = aszString;
	int		viYear, viMonth, viDay, viHours, viMinutes, viSeconds, viMilliseconds;

	if(vcstrDateTime.GetLength() == 17)
	{
		viYear			= atoi(vcstrDateTime.Left(4));
		viMonth			= atoi(vcstrDateTime.Mid(4,2));
		viDay			= atoi(vcstrDateTime.Mid(6,2));
		viHours			= atoi(vcstrDateTime.Mid(8,2));
		viMinutes		= atoi(vcstrDateTime.Mid(10,2));
		viSeconds		= atoi(vcstrDateTime.Mid(12,2));
		viMilliseconds	= atoi(vcstrDateTime.Mid(14,3));

		vbRet = (FWUtilDateStatus_Valid == SetDateTime(viYear, viMonth, viDay, viHours, viMinutes, viSeconds, viMilliseconds));
	}

	return vbRet;
}

//////////////////
// CFWUtilDateRange //
//////////////////

CFWUtilDateRange::CFWUtilDateRange()
{
}

CFWUtilDateRange::CFWUtilDateRange(const CFWUtilDateRange &aFWUtilDateRange)
{
	SetRange(aFWUtilDateRange.m_FWUtilDate1, aFWUtilDateRange.m_FWUtilDate2);
}

CFWUtilDateRange::CFWUtilDateRange(CFWUtilDate aFWUtilDate1, CFWUtilDate aFWUtilDate2)
{
	SetRange(aFWUtilDate1, aFWUtilDate2);
}

CFWUtilDateRange::~CFWUtilDateRange()
{
}

CFWUtilDate CFWUtilDateRange::GetDate1()
{
	return m_FWUtilDate1;
}

CFWUtilDate CFWUtilDateRange::GetDate2()
{
	return m_FWUtilDate2;
}

void CFWUtilDateRange::SetRange(CFWUtilDate aFWUtilDate1, CFWUtilDate aFWUtilDate2)
{
	if (aFWUtilDate1 <= aFWUtilDate2)
	{
		m_FWUtilDate1 = aFWUtilDate1;
		m_FWUtilDate2 = aFWUtilDate2;
	}
	else
	{
		m_FWUtilDate1 = aFWUtilDate2;
		m_FWUtilDate2 = aFWUtilDate1;
	}
}

void CFWUtilDateRange::SetRange(const CFWUtilDateRange &aFWUtilDateRange)
{
	SetRange(aFWUtilDateRange.m_FWUtilDate1, aFWUtilDateRange.m_FWUtilDate2);
}

CFWUtilDateRange &CFWUtilDateRange::operator =(const CFWUtilDateRange &aFWUtilDateRange)
{
	SetRange(aFWUtilDateRange.m_FWUtilDate1, aFWUtilDateRange.m_FWUtilDate2);
	return *this;
}

bool CFWUtilDateRange::IsNull()
{
	return (m_FWUtilDate1.IsNull() || m_FWUtilDate2.IsNull());
}

bool CFWUtilDateRange::IsValid()
{
	return (m_FWUtilDate1.IsValid() && m_FWUtilDate2.IsValid());
}

void CFWUtilDateRange::Clear()
{
	m_FWUtilDate1.Clear();
	m_FWUtilDate2.Clear();
}

bool CFWUtilDateRange::IsBetween(CFWUtilDate aFWUtilDate)
{
	bool vbRet = false;
	if (IsValid() && aFWUtilDate.IsValid())
	{
		vbRet = ((m_FWUtilDate1 <= aFWUtilDate) && (aFWUtilDate <= m_FWUtilDate2));
	}
	return vbRet;
}



// Modified by DJW 20020724
//
///////////////////////////////
// Date Utility Functions    //
///////////////////////////////

bool FWUtilDateAdd(FWUtilDateAdd_Constants aeAddWhat, int aiVal, int &aiYear, int &aiMonth, int &aiDay)
{
	bool vbRet = false;
	switch (aeAddWhat)
	{
	case FWUtilDateAdd_Year:
		{
			if (FWIsValidDate(aiYear, aiMonth, aiDay))
			{
				aiYear += aiVal;
				vbRet = true;
			}
		}
		break;
	case FWUtilDateAdd_Month:
		{
			if (FWIsValidDate(aiYear, aiMonth, aiDay))
			{
				LONGLONG vi64Date = ((aiYear * 12) + aiMonth);
				vi64Date += aiVal;
				aiYear = (int)((vi64Date - 1) / 12);
				aiMonth = (int)(((vi64Date - 1) % 12) + 1);
				vbRet = true;
			}
		}
		break;
	case FWUtilDateAdd_Week:
		{
			ULONGLONG vui64Date;
			if (FWUtilDateToUi64(aiYear, aiMonth, aiDay, vui64Date))
			{
				LONGLONG vi64Date = vui64Date + (aiVal * FWDATE_WEEK);
				if (vi64Date >= 0)
				{
					vui64Date = vi64Date;
					vbRet = FWUi64ToDate(vui64Date, aiYear, aiMonth, aiDay);
				}
			}
		}
		break;
	case FWUtilDateAdd_Day:
		{
// LMJ 			unsigned __int64	vui64Date;
// LMJ 			SYSTEMTIME			vstDate;
// LMJ 			FILETIME			vftDate;
// LMJ 				
// LMJ 			memset(&vstDate, NULL, sizeof(vstDate));
// LMJ 			memset(&vftDate, NULL, sizeof(vstDate));
// LMJ 			vstDate.wYear = aiYear;
// LMJ 			vstDate.wMonth = aiMonth;
// LMJ 			vstDate.wDay = aiDay;
// LMJ 
// LMJ 			if (::SystemTimeToFileTime(&vstDate, &vftDate))
// LMJ 			{
// LMJ 				vui64Date = *(ULONGLONG*)&vftDate;
// LMJ 				if 
// LMJ 				vui64Date += (UINT)
// LMJ 			}

			ULONGLONG vui64Date;
			if (FWUtilDateToUi64(aiYear, aiMonth, aiDay, vui64Date))
			{
				LONGLONG vi64Date = vui64Date + (aiVal * FWDATE_DAY);
				if (vi64Date >= 0)
				{
					vui64Date = vi64Date;
					vbRet = FWUi64ToDate(vui64Date, aiYear, aiMonth, aiDay);
				}
			}
		}
		break;
	}
	return vbRet;
}

bool FWUtilDateCompare(int aiYear1, int aiMonth1, int aiDay1, int aiYear2, int aiMonth2, int aiDay2, int &aiCompare)
{
	bool		vbRet = false;
	SYSTEMTIME	vstDate1;
	SYSTEMTIME	vstDate2;
	FILETIME	vftDate1;
	FILETIME	vftDate2;
	ULONGLONG	vui64Date1;
	ULONGLONG	vui64Date2;

	memset(&vstDate1, 0, sizeof(vstDate1));
	vstDate1.wYear = aiYear1;
	vstDate1.wMonth = aiMonth1;
	vstDate1.wDay = aiDay1;

	memset(&vstDate2, 0, sizeof(vstDate2));
	vstDate2.wYear = aiYear2;
	vstDate2.wMonth = aiMonth2;
	vstDate2.wDay = aiDay2;

	if (SystemTimeToFileTime(&vstDate1, &vftDate1))
	{
		if (SystemTimeToFileTime(&vstDate2, &vftDate2))
		{
			vui64Date1 = *(ULONGLONG*)&vftDate1;
			vui64Date2 = *(ULONGLONG*)&vftDate2;
			if (vui64Date1 < vui64Date2)
				aiCompare = -1;
			else if (vui64Date1 == vui64Date2)
				aiCompare = 0;
			else if (vui64Date1 > vui64Date2)
				aiCompare = 1;
			vbRet = true;
		}
	}
	return vbRet;
}

bool FWUtilDateToUi64(int aiYear, int aiMonth, int aiDay, ULONGLONG &aui64Val)
{
	bool vbRet = false;
	SYSTEMTIME vstDate;
	FILETIME vftDate;
	memset(&vstDate, 0, sizeof(vstDate));
	vstDate.wYear = aiYear;
	vstDate.wMonth = aiMonth;
	vstDate.wDay = aiDay;
	if (SystemTimeToFileTime(&vstDate, &vftDate))
	{
		aui64Val = *(ULONGLONG*)&vftDate;
		vbRet = true;
	}
	return vbRet;
}

bool FWUi64ToDate(ULONGLONG aui64Val, int &aiYear, int &aiMonth, int &aiDay)
{
	bool vbRet = false;
	SYSTEMTIME vstDate;
	FILETIME vftDate;
	vftDate = *(FILETIME*)&aui64Val;
	if (FileTimeToSystemTime(&vftDate, &vstDate))
	{
		aiYear = vstDate.wYear;
		aiMonth = vstDate.wMonth;
		aiDay = vstDate.wDay;
		vbRet = true;
	}
	return vbRet;
}

bool FWIsLeapYear(int aiYear)
{
	return ((aiYear & 3) == 0) && ((aiYear % 100) != 0 || (aiYear % 400) == 0);
}

bool FWIsValidDate(int aiYear, int aiMonth, int aiDay)
{
	SYSTEMTIME	vstDate;
	FILETIME	vftDate;
	memset(&vstDate, 0, sizeof(vstDate));
	vstDate.wYear = aiYear;
	vstDate.wMonth = aiMonth;
	vstDate.wDay = aiDay;
	return (SystemTimeToFileTime(&vstDate, &vftDate) != 0);
}

bool FWSplitDate(LPCTSTR apctszDate, int &aiYear, int &aiMonth, int &aiDay, LPCTSTR apctszSeperator)
{
	enum	eState {eDAY, eMONTH, eYEAR};

	bool	vbRet = false;
	CString	vstrSep = apctszSeperator;
	TCHAR	vszYear[5];
	TCHAR	vszMonth[5];
	TCHAR	vszDay[5];
	LPCTSTR	vpChar;
	int		viState = eDAY;
	int		viIndex = 0;

	if (apctszDate != NULL && *apctszDate != NULL)
	{
		vbRet = true;
		memset(vszYear, NULL, sizeof(vszYear));
		memset(vszMonth, NULL, sizeof(vszMonth));
		memset(vszDay, NULL, sizeof(vszDay));
		if (vstrSep.IsEmpty())
		{
			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, vstrSep.GetBuffer(4), 4);
			vstrSep.ReleaseBuffer();
		}
		vpChar = &apctszDate[0];
		while (vpChar != NULL && vbRet)
		{
			if (isdigit(*vpChar))
			{
				if (viIndex < 4)
				{
					switch (viState)
					{
					case eYEAR:
						vszYear[viIndex] = *vpChar;
						break;
					case eMONTH:
						vszMonth[viIndex] = *vpChar;
						break;
					case eDAY:
						vszDay[viIndex] = *vpChar;
						break;
					default:
						vbRet = false;
						break;
					}
					viIndex++;
				}
				else
				{
					vbRet = false;
				}
				vpChar++;
			}
			else
			{
				if (_tcsnicmp(vstrSep, vpChar, vstrSep.GetLength()) == 0)
				{
					viState++;
					viIndex = 0;
					vpChar += vstrSep.GetLength();
				}
				else
				{
					vbRet = false;
				}
			}
		}
		if (vbRet)
		{
			aiYear = _ttoi(vszYear);
			aiMonth = _ttoi(vszMonth);
			aiDay = _ttoi(vszDay);
			vbRet = FWIsValidDate(aiYear, aiMonth, aiDay);
		}
	}
	return vbRet;
}

CString FWUtilDateString(int aiYear, int aiMonth, int aiDay, bool abUseLongDescription)
{
	bool	vbRet = false;
	CString	vcstrRet;
	DWORD	vdwFlags = 0;
	TCHAR	vtszDate[255];
	if (abUseLongDescription)
		vdwFlags = DATE_LONGDATE;
	else
		vdwFlags = DATE_SHORTDATE;
	if (FWIsValidDate(aiYear, aiMonth, aiDay))
	{
		SYSTEMTIME vstDate;
		vstDate.wYear = aiYear;
		vstDate.wMonth = aiMonth;
		vstDate.wDay = aiDay;
		vbRet = (GetDateFormat(LOCALE_USER_DEFAULT, vdwFlags, &vstDate, NULL, vtszDate, sizeof(vtszDate)) != 0);
	}
	else
		vbRet = (GetDateFormat(LOCALE_USER_DEFAULT, vdwFlags, NULL, NULL, vtszDate, sizeof(vtszDate)) != 0);
	if (vbRet)
		vcstrRet = vtszDate;
	return vcstrRet;
}

CString FWGetDayName(int aiDay, bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfDay(LOCALE_USER_DEFAULT, aiDay, abUseLongDescription);

//SJSG 	CString	vcstrRet;
//SJSG 	int		viLen;
//SJSG 
//SJSG 	if (abUseLongDescription)
//SJSG 	{
//SJSG 		switch (aiDay)
//SJSG 		{
//SJSG 		case FWUtilDateDay_Sunday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME7, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME7, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Monday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME1, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME1, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Tuesday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME2, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME2, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Wednesday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME3, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME3, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Thursday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME4, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME4, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Friday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME5, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME5, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Saturday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME6, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDAYNAME6, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		}
//SJSG 	}
//SJSG 	else
//SJSG 	{
//SJSG 		switch (aiDay)
//SJSG 		{
//SJSG 		case FWUtilDateDay_Sunday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME7, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME7, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Monday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME1, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME1, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Tuesday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME2, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME2, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Wednesday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME3, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME3, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Thursday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME4, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME4, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Friday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME5, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME5, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateDay_Saturday:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME6, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVDAYNAME6, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		}
//SJSG 	}
//SJSG 	return vcstrRet;
}

CString FWGetMonthName(int aiMonth, bool abUseLongDescription)
{
	return CFWUtilDate::GetLocaleNameOfMonth(LOCALE_USER_DEFAULT, aiMonth, abUseLongDescription);

	// NOTE: The fix below is now handled by the CFWUtilDate::LocaleInfo() function...

//SJSG 
//SJSG 	CString	vcstrRet;
//SJSG 	int		viLen;
//SJSG 
//SJSG 	// FIX: The strings returned from this function actually had a length 1 greater than they should have,
//SJSG 	// because the return value from GetLocaleInfo includes the terminating NULL, whilst the CString::ReleaseBuffer
//SJSG 	// function does not. By changing the ReleaseBuffer function to use the default parameter, it automatically
//SJSG 	// searches for the correct string length.
//SJSG 	//
//SJSG 	if (abUseLongDescription)
//SJSG 	{
//SJSG 		switch (aiMonth)
//SJSG 		{
//SJSG 		case FWUtilDateMonth_January:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME1, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME1, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_February:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME2, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME2, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_March:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME3, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME3, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_April:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME4, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME4, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_May:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME5, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME5, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_June:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME6, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME6, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_July:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME7, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME7, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_August:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME8, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME8, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_September:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME9, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME9, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_October:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME10, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME10, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_November:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME11, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME11, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_December:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME12, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHNAME12, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		}
//SJSG 	}
//SJSG 	else
//SJSG 	{
//SJSG 		switch (aiMonth)
//SJSG 		{
//SJSG 		case FWUtilDateMonth_January:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME1, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME1, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_February:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME2, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME2, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_March:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME3, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME3, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_April:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME4, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME4, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_May:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME5, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME5, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_June:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME6, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME6, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_July:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME7, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME7, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_August:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME8, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME8, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_September:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME9, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME9, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_October:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME10, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME10, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_November:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME11, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME11, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		case FWUtilDateMonth_December:
//SJSG 			viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME12, NULL, 0);
//SJSG 			::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SABBREVMONTHNAME12, vcstrRet.GetBuffer(viLen), viLen);
//SJSG 			vcstrRet.ReleaseBuffer();
//SJSG 			break;
//SJSG 		}
//SJSG 	}
//SJSG 	return vcstrRet;
}

int FWFindHalf(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear)
{
	int viRet = 0;
	if (FWIsValidDate(aiYear, aiMonth, aiDay))
	{
		//               Month  Start Of Year
		int viHalfYearTable[12][12] = 
		{
			{1,	2,	2,	2,	2,	2,	2,	1,	1,	1,	1,	1},
			{1,	1,	2,	2,	2,	2,	2,	2,	1,	1,	1,	1},
			{1,	1,	1,	2,	2,	2,	2,	2,	2,	1,	1,	1},
			{1,	1,	1,	1,	2,	2,	2,	2,	2,	2,	1,	1},
			{1,	1,	1,	1,	1,	2,	2,	2,	2,	2,	2,	1},
			{1,	1,	1,	1,	1,	1,	2,	2,	2,	2,	2,	2},
			{2,	1,	1,	1,	1,	1,	1,	2,	2,	2,	2,	2},
			{2,	2,	1,	1,	1,	1,	1,	1,	2,	2,	2,	2},
			{2,	2,	2,	1,	1,	1,	1,	1,	1,	2,	2,	2},
			{2,	2,	2,	2,	1,	1,	1,	1,	1,	1,	2,	2},
			{2,	2,	2,	2,	2,	1,	1,	1,	1,	1,	1,	2},
			{2,	2,	2,	2,	2,	2,	1,	1,	1,	1,	1,	1},
		};
		viRet = viHalfYearTable[aiMonth - 1][aeStartOfYear - 1];
		int vi1stHlf;
		int vi2ndHlf;
		int viNewMonth;
		vi1stHlf = aeStartOfYear;
		vi2ndHlf = (vi1stHlf + 6) % 12;
		if (vi2ndHlf == 0)
			vi2ndHlf = 12;
		switch (viRet)
		{
		case 1:
			viNewMonth = vi1stHlf;
			break;
		case 2:
			viNewMonth = vi2ndHlf;
			break;
		}
		if (aiMonth < viNewMonth)
			aiYear--;
		aiMonth = viNewMonth;
		aiDay = 1;
	}
	return viRet;
}

int FWFindQuarter(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear)
{
	int viRet = 0;
	if (FWIsValidDate(aiYear, aiMonth, aiDay))
	{
		//                  Month  Start Of Year
		int viQuarterYearTable[12][12] = 
		{
			{1,	4,	4,	4,	3,	3,	3,	2,	2,	2,	1,	1},
			{1,	1,	4,	4,	4,	3,	3,	3,	2,	2,	2,	1},
			{1,	1,	1,	4,	4,	4,	3,	3,	3,	2,	2,	2},
			{2,	1,	1,	1,	4,	4,	4,	3,	3,	3,	2,	2},
			{2,	2,	1,	1,	1,	4,	4,	4,	3,	3,	3,	2},
			{2,	2,	2,	1,	1,	1,	4,	4,	4,	3,	3,	3},
			{3,	2,	2,	2,	1,	1,	1,	4,	4,	4,	3,	3},
			{3,	3,	2,	2,	2,	1,	1,	1,	4,	4,	4,	3},
			{3,	3,	3,	2,	2,	2,	1,	1,	1,	4,	4,	4},
			{4,	3,	3,	3,	2,	2,	2,	1,	1,	1,	4,	4},
			{4,	4,	3,	3,	3,	2,	2,	2,	1,	1,	1,	4},
			{4,	4,	4,	3,	3,	3,	2,	2,	2,	1,	1,	1},
		};
		viRet = viQuarterYearTable[aiMonth - 1][aeStartOfYear - 1];
		int vi1stQtr;
		int vi2ndQtr;
		int vi3rdQtr;
		int vi4thQtr;
		int viNewMonth;
		vi1stQtr = aeStartOfYear;
		vi2ndQtr = (vi1stQtr + 3) % 12;
		if (vi2ndQtr == 0)
			vi2ndQtr = 12;
		vi3rdQtr = (vi2ndQtr + 3) % 12;
		if (vi3rdQtr == 0)
			vi3rdQtr = 12;
		vi4thQtr = (vi3rdQtr + 3) % 12;
		if (vi4thQtr == 0)
			vi4thQtr = 12;
		switch (viRet)
		{
		case 1:
			viNewMonth = vi1stQtr;
			break;
		case 2:
			viNewMonth = vi2ndQtr;
			break;
		case 3:
			viNewMonth = vi3rdQtr;
			break;
		case 4:
			viNewMonth = vi4thQtr;
			break;
		}
		if (aiMonth < viNewMonth)
			aiYear--;
		aiMonth = viNewMonth;
		aiDay = 1;
	}
	return viRet;
}

int	FWFindWeekEx(int &aiYear, int &aiMonth, int &aiDay)
{
	CFWUtilDate vtDate;

	return FWFindWeekEx(aiYear, aiMonth, aiDay, vtDate);
}

int FWFindWeekEx(int &aiYear, int &aiMonth, int &aiDay, CFWUtilDate &atStartOfWeekDay, FWUtilDateMonth_Constants aeStartOfYear, FWUtilDateDay_Constants aeStartOfWeek)
{
	int						viRet = -1;
	FILETIME				vftDate;
	FILETIME				vftSOY;
	int						viDayAdj;
	int						viWeekCount;
	SYSTEMTIME				vstDate;
	SYSTEMTIME				vstSOY;
	ULONGLONG				vui64Current;
	ULONGLONG				vui64Date;
	ULONGLONG				vui64SOY;
	bool					vbAdjustSOYBack1Year;

	memset(&vstDate, 0, sizeof(vstDate));
	memset(&vstSOY, 0, sizeof(vstSOY));
	vstDate.wDay = aiDay;
	vstDate.wMonth = aiMonth;
	vstDate.wYear = aiYear;
	vstSOY.wDay = 1;
	vstSOY.wMonth = aeStartOfYear;
	vstSOY.wYear = aiYear;
	if (SystemTimeToFileTime(&vstDate, &vftDate) && SystemTimeToFileTime(&vstSOY, &vftSOY))
	{
		vui64Date = *(ULONGLONG*)&vftDate;
		vui64SOY = *(ULONGLONG*)&vftSOY;

		if(FileTimeToSystemTime(&vftDate, &vstDate) && FileTimeToSystemTime(&vftSOY, &vstSOY))
		{
			vbAdjustSOYBack1Year = IsDateBeforeFirstStartOfWeekDayOfYear(vstDate, vstSOY, aeStartOfWeek);

			if (vui64Date < vui64SOY || (vbAdjustSOYBack1Year) )
			{
				vstSOY.wYear--;
				SystemTimeToFileTime(&vstSOY, &vftSOY);
				vui64SOY = *(ULONGLONG*)&vftSOY;
			}
			if (FileTimeToSystemTime(&vftDate, &vstDate) && FileTimeToSystemTime(&vftSOY, &vstSOY))
			{
				viDayAdj = FWStartOfWeekAdjuster(vstDate.wDayOfWeek, aeStartOfWeek);
				vui64Current = vui64Date - (viDayAdj * FWDATE_DAY);
				if (FWUi64ToDate(vui64Current, aiYear, aiMonth, aiDay))
				{
					if (vstSOY.wDayOfWeek == aeStartOfWeek)
					{
						vui64Current = vui64SOY;
						viWeekCount = 0;
					}
					else
					{
						viDayAdj = FWStartOfWeekAdjuster(vstSOY.wDayOfWeek, aeStartOfWeek);
						vui64Current = vui64SOY - (viDayAdj * FWDATE_DAY);
						viWeekCount = -1;
					}
					while (vui64Current <= vui64Date)
					{
						viWeekCount++;
						vui64Current += FWDATE_WEEK;
					}
					atStartOfWeekDay = vui64Current - FWDATE_WEEK;
					viRet = viWeekCount;
				}
			}
		}
	}

	return viRet;
}

bool IsDateBeforeFirstStartOfWeekDayOfYear(SYSTEMTIME astDate, SYSTEMTIME astSOY, FWUtilDateDay_Constants aeStartOfWeek)
{
	bool vbRet = false;

	int			viDayAdj;
	FILETIME	vftDate, vftSOY;
	ULONGLONG	vui64SOWSOY, vui64Date, vui64SOY;

	if(astSOY.wDayOfWeek != aeStartOfWeek/*SOY is NOT start of week day*/)
	{
		if(SystemTimeToFileTime(&astDate, &vftDate) && SystemTimeToFileTime(&astSOY, &vftSOY))
		{
			vui64Date = *(ULONGLONG*)&vftDate;
			vui64SOY = *(ULONGLONG*)&vftSOY;

			viDayAdj = FWStartOfWeekAdjuster(astSOY.wDayOfWeek, aeStartOfWeek);
			vui64SOWSOY = vui64SOY + ((7 - viDayAdj) * FWDATE_DAY);
			if(vui64Date < vui64SOWSOY)
				vbRet = true;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	return vbRet;
}

int FWFindWeek(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear, FWUtilDateDay_Constants aeStartOfWeek)
{
	int			viRet = -1;
	FILETIME	vftDate;
	FILETIME	vftSOY;
	int			viDayAdj;
	int			viWeekCount;
	SYSTEMTIME	vstDate;
	SYSTEMTIME	vstSOY;
	ULONGLONG	vui64Current;
	ULONGLONG	vui64Date;
	ULONGLONG	vui64SOY;
	memset(&vstDate, 0, sizeof(vstDate));
	memset(&vstSOY, 0, sizeof(vstSOY));
	vstDate.wDay = aiDay;
	vstDate.wMonth = aiMonth;
	vstDate.wYear = aiYear;
	vstSOY.wDay = 1;
	vstSOY.wMonth = aeStartOfYear;
	vstSOY.wYear = aiYear;
	if (SystemTimeToFileTime(&vstDate, &vftDate) && SystemTimeToFileTime(&vstSOY, &vftSOY))
	{
		vui64Date = *(ULONGLONG*)&vftDate;
		vui64SOY = *(ULONGLONG*)&vftSOY;
		if (vui64Date < vui64SOY)
		{
			vstSOY.wYear--;
			SystemTimeToFileTime(&vstSOY, &vftSOY);
			vui64SOY = *(ULONGLONG*)&vftSOY;
		}
		if (FileTimeToSystemTime(&vftDate, &vstDate) && FileTimeToSystemTime(&vftSOY, &vstSOY))
		{
			viDayAdj = FWStartOfWeekAdjuster(vstDate.wDayOfWeek, aeStartOfWeek);
			vui64Current = vui64Date - (viDayAdj * FWDATE_DAY);
			if (FWUi64ToDate(vui64Current, aiYear, aiMonth, aiDay))
			{
				if (vstSOY.wDayOfWeek == aeStartOfWeek)
				{
					vui64Current = vui64SOY;
					viWeekCount = 0;
				}
				else
				{
					viDayAdj = FWStartOfWeekAdjuster(vstSOY.wDayOfWeek, aeStartOfWeek);
					vui64Current = vui64SOY - (viDayAdj * FWDATE_DAY);
					viWeekCount = -1;
				}
				while (vui64Current <= vui64Date)
				{
					viWeekCount++;
					vui64Current += FWDATE_WEEK;
				}
				viRet = viWeekCount;
			}
		}
	}
	return viRet;
}

int FWFindDay(int aiYear, int aiMonth, int aiDay, FWUtilDateMonth_Constants aeStartOfYear)
{
	int			viRet = 0;
	ULONGLONG	vui64Date;
	ULONGLONG	vui64SOY;
	int			viYear = aiYear;
	if (aiMonth < aeStartOfYear)
		viYear--;
	if (FWUtilDateToUi64(viYear, aeStartOfYear, 1, vui64SOY))
	{
		if (FWUtilDateToUi64(aiYear, aiMonth, aiDay, vui64Date))
		{
			viRet = (int)((vui64Date - vui64SOY) / FWDATE_DAY) + 1;
		}
	}
	return viRet;
}

int FWGetDayOfWeek(int aiYear, int aiMonth, int aiDay)
{
	int			viDayOfWeek = -1;
	SYSTEMTIME	vstDate;
	FILETIME	vftDate;
	memset(&vstDate, 0, sizeof(vstDate));
	vstDate.wDay = aiDay;
	vstDate.wMonth = aiMonth;
	vstDate.wYear = aiYear;
	if (SystemTimeToFileTime(&vstDate, &vftDate))
	{
		if (FileTimeToSystemTime(&vftDate, &vstDate))
		{
			viDayOfWeek = vstDate.wDayOfWeek;
		}
	}
	return viDayOfWeek;
}

int FWLastDayOfMonth(int aiYear, int aiMonth)
{
	int viDay = 0;
	switch (aiMonth)
	{
	case FWUtilDateMonth_January:
		viDay = 31;
		break;
	case FWUtilDateMonth_February:
		if (FWIsLeapYear(aiYear))
			viDay = 29;
		else
			viDay = 28;
		break;
	case FWUtilDateMonth_March:
		viDay = 31;
		break;
	case FWUtilDateMonth_April:
		viDay = 30;
		break;
	case FWUtilDateMonth_May:
		viDay = 31;
		break;
	case FWUtilDateMonth_June:
		viDay = 30;
		break;
	case FWUtilDateMonth_July:
		viDay = 31;
		break;
	case FWUtilDateMonth_August:
		viDay = 31;
		break;
	case FWUtilDateMonth_September:
		viDay = 30;
		break;
	case FWUtilDateMonth_October:
		viDay = 31;
		break;
	case FWUtilDateMonth_November:
		viDay = 30;
		break;
	case FWUtilDateMonth_December:
		viDay = 31;
		break;
	}
	return viDay;
}

int FWStartOfWeekAdjuster(int aiDayOfWeek, int aiStartOfWeek)
{
	int viRet = 0;
	if (aiDayOfWeek >= aiStartOfWeek)
		viRet = aiDayOfWeek - aiStartOfWeek;
	else
		viRet = 7 - (aiStartOfWeek - aiDayOfWeek);
	return viRet;
}
//
// End of modification

bool FWTimeToInt(int aiHours, int aiMinutes, int aiSeconds, int& aiInt)
{
	if (aiHours >= 0 && aiHours < 24)
	{
		if (aiMinutes >= 0 && aiMinutes < 60)
		{
			if (aiSeconds >= 0 && aiSeconds < 60)
			{
				aiInt = ((aiHours * 60) * 60) + (aiMinutes * 60) + aiSeconds;
				return true;
			}
		}
	}
	return false;
}

bool FWIntToTime(int aiInt, int& aiHours, int& aiMinutes, int& aiSeconds)
{
	int viQuot;
	int	viRem;

	if (aiInt >= 0 && aiInt < 86400)
	{
		viQuot = aiInt / 60;
		viRem = aiInt % 60; 
		aiMinutes = viQuot;
		aiSeconds = viRem;
		viQuot = aiMinutes / 60;
		viRem = aiMinutes % 60; 
		aiHours = viQuot;
		aiMinutes = viRem;
		return true;
	}
	return false;
}

bool FWUtilDateTimeToString(LPCTSTR aszPicture, CString& astrResult, SYSTEMTIME* apstDateTime, LPCTSTR aszDayChars)
{
	bool		vbRet = false;
	bool		vbContinue = false;
	FILETIME	vftDateTime;
	TCHAR		vcPrev = NULL;
	CString		vstrSymbol;
	CString		vstrTemp;
	bool		vbLitFlag;
	int			viLen;
	CString		vstrDayChars = aszDayChars;

	if (vstrDayChars.GetLength() != 7)
	{
		vstrDayChars.Empty();
		for (int i = 0; i < 7; i++)
		{
			vstrDayChars += FWGetDayName(i, false).Left(1);
		}
	}
	if (aszPicture && apstDateTime)
	{
		viLen = _tcslen(aszPicture);
		if (viLen > 0)
		{
			if (::SystemTimeToFileTime(apstDateTime, &vftDateTime))
			{
				if (::FileTimeToSystemTime(&vftDateTime, apstDateTime))
				{
					vbLitFlag = false;
					for (int i = 0; i <= viLen; i++)
					{
						if (vbLitFlag)
						{
							if (aszPicture[i] == _T('\''))
							{
								if (aszPicture[i + 1] == _T('\''))
								{
									i++;
									vstrTemp += aszPicture[i];
								}
								else
								{
									vbLitFlag = false;
									astrResult += vstrTemp;
									vstrTemp.Empty();
								}
							}
							else
							{
								vstrTemp += aszPicture[i];
							}
						}
						else
						{
							if (vcPrev != aszPicture[i])
							{
								if (vstrSymbol.GetLength() > 0)
								{
									if (vstrSymbol.GetLength() > 4)
									{
										vstrSymbol = vstrSymbol.Left(4);
									}
									// Need to identify previous symbol
									if (_tcscmp(vstrSymbol, _T("d")) == 0)
									{
										vstrTemp.Format(_T("%i"), apstDateTime->wDay);
									}
									else if (_tcscmp(vstrSymbol, _T("dd")) == 0)
									{
										vstrTemp.Format(_T("%02i"), apstDateTime->wDay);
									}
									else if (_tcscmp(vstrSymbol, _T("D")) == 0)
									{
										vstrTemp = vstrDayChars.GetAt(apstDateTime->wDayOfWeek);
									}
									else if (_tcscmp(vstrSymbol, _T("ddd")) == 0)
									{
										vstrTemp = FWGetDayName(apstDateTime->wDayOfWeek, false);
									}
									else if (_tcscmp(vstrSymbol, _T("dddd")) == 0)
									{
										vstrTemp = FWGetDayName(apstDateTime->wDayOfWeek, true);
									}
									else if (_tcscmp(vstrSymbol, _T("M")) == 0)
									{
										vstrTemp.Format(_T("%i"), apstDateTime->wMonth);
									}
									else if (_tcscmp(vstrSymbol, _T("MM")) == 0)
									{
										vstrTemp.Format(_T("%02i"), apstDateTime->wMonth);
									}
									else if (_tcscmp(vstrSymbol, _T("MMM")) == 0)
									{
										vstrTemp = FWGetMonthName(apstDateTime->wMonth, false);
									}
									else if (_tcscmp(vstrSymbol, _T("MMMM")) == 0)
									{
										vstrTemp = FWGetMonthName(apstDateTime->wMonth, true);
									}
									else if (_tcscmp(vstrSymbol, _T("y")) == 0)
									{
										vstrTemp.Format(_T("%04i"), apstDateTime->wYear);
										vstrTemp = vstrTemp.Right(2);
										if (vstrTemp[0] == _T('0'))
										{
											vstrTemp = vstrTemp.Right(1);
										}
									}
									else if (_tcscmp(vstrSymbol, _T("yy")) == 0 || _tcscmp(vstrSymbol, _T("yyy")) == 0)
									{
										vstrTemp.Format(_T("%04i"), apstDateTime->wYear);
										vstrTemp = vstrTemp.Right(2);
									}
									else if (_tcscmp(vstrSymbol, _T("yyyy")) == 0)
									{
										vstrTemp.Format(_T("%04i"), apstDateTime->wYear);
									}
									else if (_tcscmp(vstrSymbol, _T("h")) == 0)
									{
										int viTemp = apstDateTime->wHour;
										if (viTemp > 12)
										{
											viTemp -= 12;
										}
										vstrTemp.Format(_T("%i"), viTemp);
									}
									else if (_tcscmp(vstrSymbol, _T("hh")) == 0)
									{
										int viTemp = apstDateTime->wHour;
										if (viTemp > 12)
										{
											viTemp -= 12;
										}
										vstrTemp.Format(_T("%02i"), viTemp);
									}
									else if (_tcscmp(vstrSymbol, _T("H")) == 0)
									{
										vstrTemp.Format(_T("%i"), apstDateTime->wHour);
									}
									else if (_tcscmp(vstrSymbol, _T("HH")) == 0)
									{
										vstrTemp.Format(_T("%02i"), apstDateTime->wHour);
									}
									else if (_tcscmp(vstrSymbol, _T("m")) == 0)
									{
										vstrTemp.Format(_T("%i"), apstDateTime->wMinute);
									}
									else if (_tcscmp(vstrSymbol, _T("mm")) == 0)
									{
										vstrTemp.Format(_T("%02i"), apstDateTime->wMinute);
									}
									else if (_tcscmp(vstrSymbol, _T("s")) == 0)
									{
										vstrTemp.Format(_T("%i"), apstDateTime->wSecond);
									}
									else if (_tcscmp(vstrSymbol, _T("ss")) == 0)
									{
										vstrTemp.Format(_T("%02i"), apstDateTime->wSecond);
									}
									else if (_tcscmp(vstrSymbol, _T("t")) == 0)
									{
										if (apstDateTime->wHour >= 12)
										{
											::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, vstrTemp.GetBuffer(9), 9);
											vstrTemp.ReleaseBuffer();
											if (vstrTemp.IsEmpty())
											{
												vstrTemp = _T("pm");
											}
										}
										else
										{
											::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, vstrTemp.GetBuffer(9), 9);
											vstrTemp.ReleaseBuffer();
											if (vstrTemp.IsEmpty())
											{
												vstrTemp = _T("am");
											}
										}
										vstrTemp = vstrTemp.Left(1);
									}
									else if (_tcscmp(vstrSymbol, _T("tt")) == 0)
									{
										if (apstDateTime->wHour >= 12)
										{
											::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, vstrTemp.GetBuffer(9), 9);
											vstrTemp.ReleaseBuffer();
											if (vstrTemp.IsEmpty())
											{
												vstrTemp = _T("pm");
											}
										}
										else
										{
											::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, vstrTemp.GetBuffer(9), 9);
											vstrTemp.ReleaseBuffer();
											if (vstrTemp.IsEmpty())
											{
												vstrTemp = _T("am");
											}
										}
									}
									else if (_tcscmp(vstrSymbol, _T(" ")) == 0)
									{
										vstrTemp = _T(" ");
									}
									astrResult += vstrTemp;
									vstrTemp.Empty();
								}
								if (aszPicture[i] == _T('\''))
								{
									vbLitFlag = true;
								}
								vstrSymbol = aszPicture[i];
								vcPrev = aszPicture[i];
							}
							else
							{
								vstrSymbol += aszPicture[i];
							}
						}
					}
					vbRet = true;
				}
			}
		}
	}
	return vbRet;
}

bool FWParseTime(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, LCID aLocale /*=LOCALE_USER_DEFAULT*/)
{
	int		viSeconds = -1;
	int		viMilliseconds = -1;
	bool	vbNextDay;

	return FWParseTimeEx(aszTimeString, aiHours, aiMinutes, viSeconds, viMilliseconds, 0, vbNextDay, aLocale);
}

bool FWParseTimeEx(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, int& aiSeconds, int& aiMilliseconds, int aiStartOfDayInMinutes, bool& abNextDay, LCID aLocale /*=LOCALE_USER_DEFAULT*/)
{
	bool	vbRet = false;
	int		viHours = -1;
	int		viMinutes = -1;
	int		viSeconds = -1;
	int		viMilliseconds = -1;
	bool	vb12hrTime = false;
	bool	vbPm = false;
	CString	vstrTime;
	CString	vstrAmSymbol;
	CString	vstrPmSymbol;
	LPCTSTR	vpTimeMarker;
	int		viTimeMarketLen;
	int		viLen;
	int		viFirstNonDigit;
	int		i;
	bool	vbContinue;
	bool	vbAmSymbolFound;
	bool	vbPmSymbolFound;
// LMJ 	CString	vstrTimeSep;
// LMJ 	int		viTimeSepLen;

	abNextDay = false;
	if (aszTimeString && _tcslen(aszTimeString) > 0)
	{
		// Get User AM Symbol
		viLen = ::GetLocaleInfo(aLocale, LOCALE_S1159, NULL, 0);
		::GetLocaleInfo(aLocale, LOCALE_S1159, vstrAmSymbol.GetBuffer(viLen), viLen);
		vstrAmSymbol.ReleaseBuffer();
		vstrAmSymbol.MakeUpper();
		if (vstrAmSymbol.IsEmpty())
		{
			vstrAmSymbol = _T("AM");
		}

		// Get User PM Symbol
		viLen = ::GetLocaleInfo(aLocale, LOCALE_S2359, NULL, 0);
		::GetLocaleInfo(aLocale, LOCALE_S2359, vstrPmSymbol.GetBuffer(viLen), viLen);
		vstrPmSymbol.ReleaseBuffer();
		vstrPmSymbol.MakeUpper();
		if (vstrPmSymbol.IsEmpty())
		{
			vstrPmSymbol = _T("PM");
		}

// LMJ 		// Get Time Separator
// LMJ 		viLen = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, NULL, 0);
// LMJ 		vstrTimeSep.ReleaseBuffer(::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, vstrTimeSep.GetBuffer(viLen), viLen));
// LMJ 		vstrTimeSep.TrimLeft();
// LMJ 		vstrTimeSep.TrimRight();
// LMJ 		viTimeSepLen = vstrTimeSep.GetLength();

		vbContinue = true;
		viLen = _tcslen(aszTimeString);
		for (i = _tcslen(aszTimeString) - 1; i >= 0; i--)
		{
			if (isdigit(aszTimeString[i]))
			{
				viFirstNonDigit = i + 1;
				break;
			}
			else if (aszTimeString[i] == _T(' '))
			{
				viFirstNonDigit = i + 1;
				break;
			}
		}
		if (viFirstNonDigit >= 1 && viFirstNonDigit < viLen)
		{
			vpTimeMarker = &aszTimeString[viFirstNonDigit];
			viTimeMarketLen = _tcslen(vpTimeMarker);

			// Check Am Symbol
			vbAmSymbolFound = false;
			if (vstrAmSymbol.CompareNoCase(vpTimeMarker) == 0)
			{
				vbAmSymbolFound = true;
				vb12hrTime = true;
				vbPm = false;
			}
			else
			{
				if (vstrAmSymbol.CompareNoCase(_T("AM")) == 0)
				{
					if (viTimeMarketLen == 1)
					{
						if (vstrAmSymbol.Left(1).CompareNoCase(vpTimeMarker) == 0)
						{
							vbAmSymbolFound = true;
							vb12hrTime = true;
							vbPm = false;
						}
					}
				}
			}

			// Check Pm Symbol
			vbPmSymbolFound = false;
			if (vstrPmSymbol.CompareNoCase(vpTimeMarker) == 0)
			{
				vbPmSymbolFound = true;
				vb12hrTime = true;
				vbPm = true;
			}
			else
			{
				if (vstrPmSymbol.CompareNoCase(_T("PM")) == 0)
				{
					if (viTimeMarketLen == 1)
					{
						if (vstrPmSymbol.Left(1).CompareNoCase(vpTimeMarker) == 0)
						{
							vbPmSymbolFound = true;
							vb12hrTime = true;
							vbPm = true;
						}
					}
				}
			}

			vbContinue = vbAmSymbolFound || vbPmSymbolFound;
		}
		else
		{
			vb12hrTime = false;
			vbPm = false;
		}

// LMJ 			// Determine Time Format
// LMJ 			vstrTime = aszTimeString;
// LMJ 			vstrTime.MakeUpper();
// LMJ 			vb12hrTime = (vstrTime.Find(vstrAmSymbol) >= 0 || vstrTime.Find(vstrPmSymbol) >= 0);
// LMJ 			if (vb12hrTime)
// LMJ 			{
// LMJ 				vbPm = vstrTime.Find(vstrPmSymbol) >= 0;
// LMJ 			}

		if (vbContinue)
		{
			// Strip non digit chars
			vstrTime.Empty();
			for (i = 0; i < viLen; i++)
			{
				if (isdigit(aszTimeString[i]))
				{
					vstrTime += aszTimeString[i];
				}
			}

			// Get Hours and Minutes
			if (vstrTime.GetLength() > 0 && vstrTime.GetLength() <= 8)
			{
				switch (vstrTime.GetLength())
				{
				case 8:
					viHours = _ttoi(vstrTime.Mid(0, 2));
					viMinutes = _ttoi(vstrTime.Mid(2, 2));
					viSeconds = _ttoi(vstrTime.Mid(4, 2));
					viMilliseconds = _ttoi(vstrTime.Mid(6, 2));
					break;
				case 7:
					viHours = _ttoi(vstrTime.Mid(0, 1));
					viMinutes = _ttoi(vstrTime.Mid(1, 2));
					viSeconds = _ttoi(vstrTime.Mid(3, 2));
					viMilliseconds = _ttoi(vstrTime.Mid(5, 2));
					break;
				case 6:
					viHours = _ttoi(vstrTime.Mid(0, 2));
					viMinutes = _ttoi(vstrTime.Mid(2, 2));
					viSeconds = _ttoi(vstrTime.Mid(4, 2));
					viMilliseconds = 0;
					break;
				case 5:
					viHours = _ttoi(vstrTime.Mid(0, 1));
					viMinutes = _ttoi(vstrTime.Mid(1, 2));
					viSeconds = _ttoi(vstrTime.Mid(3, 2));
					viMilliseconds = 0;
					break;
				case 4:
					viHours = _ttoi(vstrTime.Left(2));
					viMinutes = _ttoi(vstrTime.Right(2));
					viSeconds = 0;
					viMilliseconds = 0;
					break;
				case 3:
					viHours = _ttoi(vstrTime.Left(1));
					viMinutes = _ttoi(vstrTime.Right(2));
					viSeconds = 0;
					viMilliseconds = 0;
					break;
				case 2:
				case 1:
					viHours = _ttoi(vstrTime);
					viMinutes = 0;
					viSeconds = 0;
					viMilliseconds = 0;
					break;
				}
				if (vb12hrTime)
				{
					if (viHours >= 1 && viHours <= 12)
					{
						if (vbPm)
						{
							if (viHours < 12)
							{
								viHours += 12;
							}
						}
						else
						{
							if (viHours > 11)
							{
								viHours -= 12;
							}
						}
					}
					else
					{
						viHours = -1;
					}
				}
				if (viHours >= 0 && viHours < 24)
				{
					if (viMinutes >= 0 && viMinutes < 60)
					{
						if (viSeconds >= 0 && viSeconds < 60)
						{
							if (viMilliseconds >= 0 && viMilliseconds < 100)
							{
								aiHours = viHours;
								aiMinutes = viMinutes;
								aiSeconds = viSeconds;
								aiMilliseconds = viMilliseconds;
								vbRet = true;
							}
						}
					}
				}
				if (!vbRet && aiStartOfDayInMinutes > 0)
				{
					int viTimeInMinutes = ((viHours * 60) + viMinutes);
					if (viTimeInMinutes >= 1440 && viTimeInMinutes < (1440 + aiStartOfDayInMinutes))
					{
						if ((viTimeInMinutes - 1440) >= 0 && (viTimeInMinutes - 1440) < 1440)
						{
							abNextDay = true;
							viHours -= 24;
							aiHours = viHours;
							aiMinutes = viMinutes;
							aiSeconds = viSeconds;
							aiMilliseconds = viMilliseconds;
							vbRet = true;
						}
					}
				}
			}
		}
	}
	return vbRet;
}

static int _FWExtractLongYear(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[5];
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 4);
	vszTemp[4] = 0;
	apcPtr += 4;
	return _ttoi(vszTemp);
}

static int _FWExtractShortYear(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[3];
	int    viYear;
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 2);
	vszTemp[2] = 0;
	apcPtr += 2;
	viYear = _ttoi(vszTemp);
	if (viYear <= 50)
		return 2000 + viYear;
	else
		return 1900 + viYear;
}

static int _FWExtractTwoCharMonth(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[3];
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 2);
	vszTemp[2] = 0;
	apcPtr += 2;
	return _ttoi(vszTemp);
}

static int _FWExtractOneCharMonth(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[2];
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 1);
	vszTemp[1] = 0;
	apcPtr += 1;
	return _ttoi(vszTemp);
}

static int _FWExtractTwoCharDay(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[3];
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 2);
	vszTemp[2] = 0;
	apcPtr += 2;
	return _ttoi(vszTemp);
}

static int _FWExtractOneCharDay(TCHAR* &apcPtr)
{
	TCHAR  vszTemp[2];
	_tcsncpy_s(vszTemp, sizeof(vszTemp), apcPtr, 1);
	vszTemp[1] = 0;
	apcPtr += 1;
	return _ttoi(vszTemp);
}

static enum
{
	DateOrder_Year = 0,
	DateOrder_Month,
	DateOrder_Day
};


bool FWParseShortDate(LPCTSTR aszDateString, int& aiYear, int& aiMonth, int& aiDay, LCID aLocale /*=LOCALE_USER_DEFAULT*/)
{
	static const int viDateOrder_MinSize[3] = { 2, 1, 1 };
	static const int viDateOrder_MaxSize[3] = { 4, 2, 2 };
	static const int viMinTotalSize = viDateOrder_MinSize[0] + viDateOrder_MinSize[1] + viDateOrder_MinSize[2];
	static const int viMaxTotalSize = viDateOrder_MaxSize[0] + viDateOrder_MaxSize[1] + viDateOrder_MaxSize[2];

	char    vcNumber[3][32];
	int     viNumLen[3] = {0,0,0};
	TCHAR	vszSep[4];
	TCHAR	vszOrder[2];
	TCHAR*	vpcTemp1;
	TCHAR*	vpcTemp2;
	TCHAR*	vpcTemp3;
	int		viLen;
	TCHAR*	vszDateString = NULL;
	int		viYear = -1;
	int		viMonth = -1;
	int		viDay = -1;
	bool	vbRet = false;
	static const TCHAR vcSeps[] = _T("/\\.,-");
	int     viCount = 0;
	int     viDateOrder[3];
	int     viField[3][4];
	bool	vbContinue = false;

	::GetLocaleInfo(aLocale, LOCALE_SDATE, vszSep, 4);
	::GetLocaleInfo(aLocale, LOCALE_IDATE, vszOrder, 2);

	switch (vszOrder[0])
	{
	case _T('0'): // m / d / y
		viDateOrder[0] = DateOrder_Month;
		viDateOrder[1] = DateOrder_Day;
		viDateOrder[2] = DateOrder_Year;
		break;
	case _T('1'): // d / m / y
		viDateOrder[0] = DateOrder_Day;
		viDateOrder[1] = DateOrder_Month;
		viDateOrder[2] = DateOrder_Year;
		break;
	case _T('2'): // y / m / d
		viDateOrder[0] = DateOrder_Year;
		viDateOrder[1] = DateOrder_Month;
		viDateOrder[2] = DateOrder_Day;
		break;
	}

	if (aszDateString)
	{
		viLen = _tcslen(aszDateString);
		if (viLen > 0)
		{
			vszDateString = new TCHAR[viLen + 1];
			vszDateString[viLen] = NULL;
			_tcscpy_s(vszDateString, viLen, aszDateString);
		}
	}

	try
	{
		if (vszDateString)
		{
			//replace potentially complex vszSep with '/';
			vpcTemp1 = vpcTemp2 = vszDateString;
			vpcTemp3 = strstr(vpcTemp2, vszSep);
			while (vpcTemp3)
			{
				*(vpcTemp1 + (vpcTemp3 - vpcTemp2)) = '/';
				memmove(vpcTemp1, vpcTemp2, vpcTemp3 - vpcTemp2);
				vpcTemp1 = vpcTemp3 + 1;
				vpcTemp2 = vpcTemp3 + strlen(vszSep);
				vpcTemp3 = strstr(vpcTemp2, vszSep);
			}
			vpcTemp3 = vpcTemp2 + strlen(vpcTemp2);
			memmove(vpcTemp1, vpcTemp2, vpcTemp3 - vpcTemp2);

			//extract numbers (max 3)
			vpcTemp1 = vszDateString;
			for (viCount = 0; viCount < 3; viCount++)
			{
				viLen = _tcscspn(vpcTemp1, vcSeps);
				vpcTemp2 = vpcTemp1 + viLen;

				if (viLen > viMaxTotalSize) viLen = viMaxTotalSize;//max numeric characters is 8

				viNumLen[viCount] = viLen;
				_tcsncpy_s(vcNumber[viCount], 31, vpcTemp1, viLen);
				vcNumber[viCount][viLen] = 0;
				if (!*vpcTemp2)
				{
					break;
				}
				vpcTemp1 = vpcTemp2 + 1;
			}
			viCount++;//2 separators create 3 numbers
			switch (viCount)
			{
			case 1:
				{
					for (int i = 0; i < 3; i++)
					{
						viField[0][i] = viDateOrder[i];
					}
					viField[0][3] = -1;
					vbContinue = true;
				}
				break;
			case 2:
				{
					/*
					bool vbMiddleItemInFirstGroup;
					int  viMaxOneSize = viDateOrder_MaxSize[viDateOrder[0]];
					int  viMinTwoSize = viDateOrder_MinSize[viDateOrder[1]] + viDateOrder_MinSize[viDateOrder[2]];
					
					if (viNumLen[0] > viMaxOneSize)
					{
						vbMiddleItemInFirstGroup = true;
					}
					else if (viNumLen[0] < viMinTwoSize)
					{
						vbMiddleItemInFirstGroup = false;
					}
					else
					{
						vbMiddleItemInFirstGroup = viNumLen[1] > viDateOrder_MaxSize[viDateOrder[2]];
					}
					if (vbMiddleItemInFirstGroup)
					{
						for (int i = 0; i < 2; i++)
						{
							viField[0][i] = viDateOrder[i];
						}
						viField[0][2] = -1;
						viField[1][0] = viDateOrder[2];
						viField[1][1] = -1;
					}
					else
					{
						viField[0][0] = viDateOrder[0];
						viField[0][1] = -1;
						for (int i = 0; i < 2; i++)
						{
							viField[1][i] = viDateOrder[i+1];
						}
						viField[1][2] = -1;
					}
					*/
				}
				break;
			case 3:
				{
					for (int i = 0; i < 3; i++)
					{
						viField[i][0] = viDateOrder[i];
						viField[i][1] = -1;
					}
					vbContinue = true;
				}
				break;
			}
			if (vbContinue)
			{
				for (int i = 0; i < viCount; i++)
				{
					bool vbSectionHasYear = false;
					vpcTemp1 = vcNumber[i];
					viLen    = viNumLen[i];
					if (viCount == 2)
					{
						for (int j = 0; viField[i][j] != -1 && j < 3; j++)
						{
							if (viField[i][j] == DateOrder_Year)
							{
								vbSectionHasYear = true;
								break;
							}
						}
					}
					for (int j = 0; viField[i][j] != -1 && j < 3; j++)
					{
						switch (viField[i][j])
						{
						case DateOrder_Year:
							switch (viCount)
							{
							case 1:
								if (viLen > 6)
									viYear = _FWExtractLongYear(vpcTemp1);
								else
									viYear = _FWExtractShortYear(vpcTemp1);
								break;
							case 2:
								if (viLen > 4)
									viYear = _FWExtractLongYear(vpcTemp1);
								else
									viYear = _FWExtractShortYear(vpcTemp1);
								break;
							case 3:
								if (viLen > 2)
									viYear = _FWExtractLongYear(vpcTemp1);
								else
									viYear = _FWExtractShortYear(vpcTemp1);
								break;
							}
							break;
						case DateOrder_Month:
							switch (viCount)
							{
							case 1:
								if (viLen > 4)
									viMonth = _FWExtractTwoCharMonth(vpcTemp1);
								else
									viMonth = _FWExtractOneCharMonth(vpcTemp1);
								break;
							case 2:
								if (vbSectionHasYear)
								{
									if (viLen > 5)
										viMonth = _FWExtractTwoCharMonth(vpcTemp1);
									else
										viMonth = _FWExtractOneCharMonth(vpcTemp1);
								}
								else
								{
									if (viLen > 2)
										viMonth = _FWExtractTwoCharMonth(vpcTemp1);
									else
										viMonth = _FWExtractOneCharMonth(vpcTemp1);
								}
								break;
							case 3:
								if (viLen > 1)
									viMonth = _FWExtractTwoCharMonth(vpcTemp1);
								else
									viMonth = _FWExtractOneCharMonth(vpcTemp1);
								break;
							}
							break;
						case DateOrder_Day:
							switch (viCount)
							{
							case 1:
								if (viLen > 5 && viLen != 7)
									viDay = _FWExtractTwoCharDay(vpcTemp1);
								else
									viDay = _FWExtractOneCharDay(vpcTemp1);
								break;
							case 2:
								if (vbSectionHasYear)
								{
									if (viLen > 5)
										viDay = _FWExtractTwoCharDay(vpcTemp1);
									else
										viDay = _FWExtractOneCharDay(vpcTemp1);
								}
								else
								{
									if (viLen > 3)
										viDay = _FWExtractTwoCharDay(vpcTemp1);
									else
										viDay = _FWExtractOneCharDay(vpcTemp1);
								}
								break;
							case 3:
								if (viLen > 1)
									viDay = _FWExtractTwoCharDay(vpcTemp1);
								else
									viDay = _FWExtractOneCharDay(vpcTemp1);
								break;
							}
							break;
						}
					}
				}
				if (FWIsValidDate(viYear, viMonth, viDay))
				{
					aiYear = viYear;
					aiMonth = viMonth;
					aiDay = viDay;
					vbRet = true;
				}
			}
		}
	}
	catch (...)
	{
	}

	delete [] vszDateString;
	return vbRet;
}

/**
 Sets the date from a double. The fractional part of the
 value sets the time.
 */
void CFWUtilDate::SetFractionalDate(double adDate)
{
	// Day is integer value
	SetDate((int)adDate);

	// Treat time as fractional part of day...
	adDate -= (int)adDate;
	adDate *= 24;
	int iHours = (int)adDate;
	adDate -= iHours;

	adDate *= 60;
	int iMinutes = (int)adDate;
	adDate -= iMinutes;

	adDate *= 60;
	int	iSeconds = (int)adDate;
	adDate -= iSeconds;

	adDate *= 1000;
	int iMilliseconds = (int)adDate;

	SetTime(iHours, iMinutes, iSeconds, iMilliseconds);
}

/**
 Gets the date as a double.
 The time will set the fractional part of the return value.
 */
double CFWUtilDate::GetFractionalDate() const
{
	double vdRet = (int)(*this);
	vdRet += (double)GetHours() / 24;
	vdRet += (double)GetMinutes() / (24 * 60);
	vdRet += (double)GetSeconds() / (24 * 60 * 60);
	vdRet += (double)GetMilliseconds() / (24 * 60 * 60 * 1000);
	return vdRet;
}

/**
 Converts a string Time Stamp in the format "YYYYMMDD HHMMSS"
 into a Date and Time
 */
FWUtilDateStatus_Constants CFWUtilDate::SetTimeStamp(LPCTSTR aszTimeStamp)
{
	FWUtilDateStatus_Constants	veRet = FWUtilDateStatus_Invalid;
	TCHAR					vszTemp[5];
	int						viYear;
	int						viMonth;
	int						viDay;
	int						viHours;
	int						viMinutes;
	int						viSeconds;
	bool					vbOK;

	if (aszTimeStamp)
	{
		if (_tcslen(aszTimeStamp) == 15)
		{
			vbOK = true;
			for (int i = 0; i < 15 && vbOK; i++)
			{
				if (i == 8)
				{
					if (aszTimeStamp[i] != _T(' '))
					{
						vbOK = false;
					}
				}
				else
				{
					if (!isdigit(aszTimeStamp[i]))
					{
						vbOK = false;
					}
				}
			}
			if (vbOK)
			{
				// Year
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 0, 4);
				viYear = atoi(vszTemp);
				// Month
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 4, 2);
				viMonth = atoi(vszTemp);
				// Day
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 6, 2);
				viDay = atoi(vszTemp);
				// Hours
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 9, 2);
				viHours = atoi(vszTemp);
				// Minutes
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 11, 2);
				viMinutes = atoi(vszTemp);
				// Seconds
				memset(vszTemp, NULL, sizeof(vszTemp));
				_tcsncpy_s(vszTemp, sizeof(vszTemp), aszTimeStamp + 13, 2);
				viSeconds = atoi(vszTemp);

				veRet = SetDateTime(viYear, viMonth, viDay, viHours, viMinutes, viSeconds);
			}
		}
	}
	return veRet;
}

bool CFWUtilDate::CheckDayMask(BYTE abMask)
{
	FWUtilDateDay_Constants	veDayOfWeek = GetDayOfWeek();
	BYTE				vbCompare = (1 << (int)veDayOfWeek);
	return (vbCompare & abMask) != 0;
}
