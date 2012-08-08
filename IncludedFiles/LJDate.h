#ifndef LJDATE_H
#define LJDATE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////
// This Class was created using the Date Algorithms found at //
// http://www.capecod.net/~pbaum/date/date0.htm              //
///////////////////////////////////////////////////////////////

// Modified by DJW 20020724
//
// Note: These definitions were placed in the header to allow
// date utility functions to access them without redefinition.
//
#define LJ_SECOND		((__int64)10000000)
#define LJ_MINUTE		(60 * LJ_SECOND)
#define LJ_HOUR			(60 * LJ_MINUTE)
#define LJ_DAY			(24 * LJ_HOUR)
#define LJ_WEEK			( 7 * LJ_DAY)

#define LJ_BASE_YEAR	1970
#define LJ_BASE_MONTH	1
#define LJ_BASE_DAY		1
//
// End of modification

enum LJDateStatus_Constants
{
	LJDateStatus_Valid,
	LJDateStatus_Invalid,
	LJDateStatus_Null
};

enum LJDateMonth_Constants
{
	LJDateMonth_January = 1,
	LJDateMonth_February = 2,
	LJDateMonth_March = 3,
	LJDateMonth_April = 4,
	LJDateMonth_May = 5,
	LJDateMonth_June = 6,
	LJDateMonth_July = 7,
	LJDateMonth_August = 8,
	LJDateMonth_September = 9,
	LJDateMonth_October = 10,
	LJDateMonth_November = 11,
	LJDateMonth_December = 12,
	LJDateMonth_Count
};

enum LJDateDay_Constants
{
	LJDateDay_Sunday = 0,
	LJDateDay_Monday = 1,
	LJDateDay_Tuesday = 2,
	LJDateDay_Wednesday = 3,
	LJDateDay_Thursday = 4,
	LJDateDay_Friday = 5,
	LJDateDay_Saturday = 6,
	LJDateDay_Count
};

// Modified by DJW 20020724
//
enum LJDateAdd_Constants
{
	LJDateAdd_Year,
	LJDateAdd_Month,
	LJDateAdd_Week,
	LJDateAdd_Day
};
//
// End of modification

enum LJDateDiff_Constants
{
	LJDateDiff_Seconds,
	LJDateDiff_Minutes,
	LJDateDiff_Hours,
	LJDateDiff_Days,
	LJDateDiff_Weeks,

	// Added by MV 20040216
	//
	LJDateDiff_Months,
	LJDateDiff_Years
	//
	// End Addition
};

/*
Add the following line to the end of AUTOEXP.DAT so that tooltips will work for this class
CLJDate =<m_DateTime.wDay>/<m_DateTime.wMonth>/<m_DateTime.wYear> <m_DateTime.wHour>:<m_DateTime.wMinute>:<m_DateTime.wSecond>:<m_DateTime.wMilliseconds>
*/

class CLJDate
{
private:
	SYSTEMTIME		m_DateTime;
	//CString		m_cstrNull;

	static CString	UserLocaleInfo(LCTYPE LCType);
	static CString  LocaleInfo(LCID alcid, LCTYPE LCType);

public:

#ifdef __AFX_H__
	CLJDate(const COleDateTime &aodtDate);
	LJDateStatus_Constants SetDate(const COleDateTime &aodtDate);
	operator COleDateTime() const;
	CLJDate &operator = (const COleDateTime &aodtDate);
	operator DATE() const;
	CLJDate &operator = (DATE aDate);
#endif

	// Construction / Destruction
	CLJDate();
	CLJDate(const CLJDate &asdDate);
	CLJDate(int aiLJDateIndex);
	CLJDate(int aiYear, int aiMonth, int aiDay, int aiHours = 0, int aiMinutes = 0, int aiSeconds = 0, int aiMilliseconds = 0);
	CLJDate(long alJulianDate);
	CLJDate(SYSTEMTIME astDate);
	CLJDate(ULONGLONG aui64Val);
	virtual ~CLJDate();

	// Attributes
	bool IsLeapYear() const;
	bool IsNull() const;
	bool IsValid() const;
	bool IsEmpty() const;
	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;
	int GetHours() const;
	int GetMinutes() const;
	int GetSeconds() const;
	int GetMilliseconds() const;
	int	GetMinuteOfDay() const;
	int GetDayOfYear(LJDateMonth_Constants aeStartOfYear = LJDateMonth_January);
	LJDateDay_Constants GetDayOfWeek();
	LJDateStatus_Constants GetStatus() const;

	// Modified by DJW 20020724
	//
	// SetTotalMonths / GetTotalMonths
	// These functions set or get a number respresenting a whole number
	// of months since the base date (defined above).
	//
	int GetTotalMonths() const;
	LJDateStatus_Constants SetTotalMonths(int aiMonths);
	//
	// End of modification

	// Modified by MV 20040730
	//
	// These functions use a set format string to represent the containing SYSTEMTIME member.
	// The set format is: YYYYMMDDHHMMSSIII
	//
	CString		ToLocalIndependantFormat();
	bool		FromLocalIndependantFormat(LPCTSTR aszString);
	//
	// End of modification

	void	Empty();

	bool	SetToWCDate(LJDateDay_Constants aeStartOfWeek);
	bool	IsLastDayOfWeek(LJDateDay_Constants aeStartOfWeek);

	bool	SetToFirstDayOfWeek(int aiStartOfWeek);
	bool	SetToLastDayOfWeek(int aiStartOfWeek);
	bool	IsFirstDayOfWeek(int aiStartOfWeek);
	bool	IsLastDayOfWeek(int aiStartOfWeek);

	bool	SetToFirstDayOfMonth();
	bool	SetToLastDayOfMonth();
	bool	IsLastDayOfMonth();

	bool	SetToFirstDayOfQuarter();
	bool	SetToLastDayOfQuarter();
	bool	IsLastDayOfQuarter();

	bool	SetToFirstDayOfYear();
	bool	SetToLastDayOfYear();
	bool	IsFirstDayOfYear();
	bool	IsLastDayOfYear();

	// Date Math
	CLJDate &AddYears(int aiYears);
	CLJDate &AddMonths(int aiMonths);
	CLJDate &AddWeeks(int aiWeeks);
	CLJDate &AddDays(int aiDays);
	CLJDate &AddMinutes(int aiMinutes);
	CLJDate &AddSeconds(int aiSeconds);
	__int64	Difference(LJDateDiff_Constants aeType, const CLJDate& aLJDate);
	__int64	DifferenceEx(LJDateDiff_Constants aeType, const CLJDate& aLJDate);
	double	Subtract(LJDateDiff_Constants aeType, const CLJDate& aLJDate);

	// String Conversions
	CString GetDateTimeString(bool abUseLongDate = true, bool abUseLongTime = true) const;
	CString GetDateString(bool abUseLongDescription = true) const;
	CString GetTimeString(bool abUseLongDescription = true) const;
	CString GetTimeString(bool ab24Hour, bool abIncludeSeconds) const;
	CString GetTimeString(LPCTSTR aszDisplayFormat) const;
	CString GetDayName(bool abUseLongDescription = true);
	CString GetMonthName(bool abUseLongDescription = true);
	CString GetDayOfWeekName(bool abUseLongDescription);
	CString GetOracleDateString(bool abIncludeEqualSigns = true, bool abAssignment = false);
	CString GetOracleDateTimeString(bool abIncludeEqualSigns = true, bool abAssignment = false);
	CString	GetLJDateString() const;
	CString	GetTimeStamp() const;

	//Locale Strings
	CString GetLocaleDateTimeString(LCID alcid, bool abUseLongDate = true, bool abUseLongTime = true) const;
	CString GetLocaleDateString(LCID alcid, bool abUseLongDescription = true) const;
	CString GetLocaleTimeString(LCID alcid, bool abUseLongDescription = true) const;
	CString GetLocaleTimeString(LCID alcid, bool ab24Hour, bool abIncludeSeconds) const;
	CString GetLocaleTimeString(LCID alcid, LPCTSTR aszDisplayFormat) const;
	CString GetLocaleDayName(LCID alcid, bool abUseLongDescription = true);
	CString GetLocaleMonthName(LCID alcid, bool abUseLongDescription = true);
	CString GetLocaleDayOfWeekName(LCID alcid, bool abUseLongDescription = true);

	// Operations
	LJDateStatus_Constants SetTimeStamp(LPCTSTR aszTimeStamp);
	LJDateStatus_Constants SetDate(const CLJDate &asdDate);
	LJDateStatus_Constants SetDate(int aiLJDateIndex);
	LJDateStatus_Constants SetDate(int aiYear, int aiMonth, int aiDay);
	LJDateStatus_Constants SetTime(int aiHours, int aiMinutes, int aiSeconds, int aiMilliseconds);
	LJDateStatus_Constants SetTime(int aiMinuteOfDay);
	LJDateStatus_Constants SetDateTime(int aiYear, int aiMonth, int aiDay, int aiHours = 0, int aiMinutes = 0, int aiSeconds = 0, int aiMilliseconds = 0);
	LJDateStatus_Constants SetDate(long alJulianDate);
	LJDateStatus_Constants SetDate(SYSTEMTIME astDate);
	LJDateStatus_Constants SetDate(ULONGLONG aui64Val);
	LJDateStatus_Constants SetLJDateString(LPCTSTR aszLJDate); // Converts String Date in format YYYYMMDD
	SYSTEMTIME* GetPtrAccess() {return &m_DateTime;}
	void SetCurrent();
	void Clear();

	// Operators
	bool operator != (const CLJDate &asdDate) const;
	bool operator < (const CLJDate& asdDate) const;
	bool operator <= (const CLJDate &asdDate) const;
	bool operator == (const CLJDate &asdDate) const;
	bool operator > (const CLJDate &asdDate) const;
	bool operator >= (const CLJDate &asdDate) const;
	CLJDate &operator = (const CLJDate& asdDate);
	CLJDate &operator = (const SYSTEMTIME &astDate);
	CLJDate &operator = (int aiLJDateIndex);
	CLJDate &operator = (long alJulianDate);
	CLJDate &operator = (ULONGLONG aui64Val);

	// Conversion Operators
	operator int() const;
	operator long() const;
	operator LPCTSTR() const;
	operator SYSTEMTIME() const;
	operator ULONGLONG() const;
	operator __int64() const;

	void SetFractionalDate(double adDate);
	double GetFractionalDate() const;

	// This function returns true if the date falls within the specified day mask.
	// The day mask is a byte with one bit per day (and one bit left over).
	//
	bool CheckDayMask(BYTE abMask);

	static CString GetNameOfDay(int aiDay, bool abUseLongDescription);
	static CString GetLocaleNameOfDay(LCID alcid, int aiDay, bool abUseLongDescription);
	static CString GetNameOfMonth(int aiMth, bool abUseLongDescription);
	static CString GetLocaleNameOfMonth(LCID alcid, int aiMth, bool abUseLongDescription);
};

class CLJDateRange
{
	CLJDate m_LJDate1;
	CLJDate m_LJDate2;
public:
	// Construction / Destruction
	CLJDateRange();
	CLJDateRange(const CLJDateRange &aLJDateRange);
	CLJDateRange(CLJDate aLJDate1, CLJDate aLJDate2);
	virtual ~CLJDateRange();
	// Members
	CLJDate GetDate1();
	CLJDate GetDate2();
	void SetRange(CLJDate aLJDate1, CLJDate aLJDate2);
	void SetRange(const CLJDateRange &aLJDateRange);
	CLJDateRange &operator =(const CLJDateRange &aLJDateRange);
	bool IsNull();
	bool IsValid();
	void Clear();
	bool IsBetween(CLJDate aLJDate);
};

// Modified by DJW 20020724
//
///////////////////////////////
// Date Utility Functions //
///////////////////////////////

bool		LJDateAdd(LJDateAdd_Constants aeAddWhat, int aiVal, int &aiYear, int &aiMonth, int &aiDay);
bool		LJDateCompare(int aiYear1, int aiMonth1, int aiDay1, int aiYear2, int aiMonth2, int aiDay2, int &aiCompare);
bool		LJDateToUi64(int aiYear, int aiMonth, int aiDay, ULONGLONG &aui64Val);
bool		LJUi64ToDate(ULONGLONG aui64Val, int &aiYear, int &aiMonth, int &aiDay);
bool		LJIsLeapYear(int aiYear);
bool		LJIsValidDate(int aiYear, int aiMonth, int aiDay);
bool		LJSplitDate(LPCTSTR apctszDate, int &aiYear, int &aiMonth, int &aiDay, LPCTSTR apctszSeperator = NULL);
CString		LJDateString(int aiYear = 0, int aiMonth = 0, int aiDay = 0, bool abUseLongDescription = false);
CString		LJGetDayName(int aiDay, bool abUseLongDescription = true);
CString		LJGetMonthName(int aiMonth, bool abUseLongDescription = true);
int			LJFindHalf(int &aiYear, int &aiMonth, int &aiDay, LJDateMonth_Constants aeStartOfYear = LJDateMonth_January);
int			LJFindQuarter(int &aiYear, int &aiMonth, int &aiDay, LJDateMonth_Constants aeStartOfYear = LJDateMonth_January);
// Added by MV 20030304
int			LJFindWeekEx(int &aiYear, int &aiMonth, int &aiDay);
int			LJFindWeekEx(int &aiYear, int &aiMonth, int &aiDay, CLJDate &atStartOfWeekDay, LJDateMonth_Constants aeStartOfYear = LJDateMonth_January, LJDateDay_Constants aeStartOfWeek = LJDateDay_Sunday);
bool		IsDateBeforeFirstStartOfWeekDayOfYear(SYSTEMTIME astDate, SYSTEMTIME astSOY, LJDateDay_Constants aeStartOfWeek);
// End of addition
int			LJFindWeek(int &aiYear, int &aiMonth, int &aiDay, LJDateMonth_Constants aeStartOfYear = LJDateMonth_January, LJDateDay_Constants aeStartOfWeek = LJDateDay_Sunday);
int			LJFindDay(int aiYear, int aiMonth, int aiDay, LJDateMonth_Constants aeStartOfYear = LJDateMonth_January);
int			LJGetDayOfWeek(int aiYear, int aiMonth, int aiDay);
int			LJLastDayOfMonth(int aiYear, int aiMonth);
int			LJStartOfWeekAdjuster(int aiDayOfWeek, int aiStartOfWeek);
bool		LJTimeToInt(int aiHours, int aiMinutes, int aiSeconds, int& aiInt);
bool		LJIntToTime(int aiInt, int& aiHours, int& aiMinutes, int& aiSeconds);
bool		LJDateTimeToString(LPCTSTR aszPicture, CString& astrResult, SYSTEMTIME* apstDateTime, LPCTSTR aszDayChars = NULL);
//
// End of modification

// SJSG 20050621 Added LCID to LJParseTime() and LJParseShortDate() functions
bool		LJParseTime(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, LCID aLocale = LOCALE_USER_DEFAULT);
bool		LJParseTimeEx(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, int& aiSeconds, int& aiMilliseconds, int aiStartOfDayInMinutes, bool& abNextDay, LCID aLocale = LOCALE_USER_DEFAULT);
bool		LJParseShortDate(LPCTSTR aszDateString, int& aiYear, int& aiMonth, int& aiDay, LCID aLocale = LOCALE_USER_DEFAULT);

#endif
