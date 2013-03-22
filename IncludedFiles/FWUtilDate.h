#ifndef FWUtilDate_H
#define FWUtilDate_H

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
#define FWDATE_SECOND		((__int64)10000000)
#define FWDATE_MINUTE		(60 * FWDATE_SECOND)
#define FWDATE_HOUR			(60 * FWDATE_MINUTE)
#define FWDATE_DAY			(24 * FWDATE_HOUR)
#define FWDATE_WEEK			( 7 * FWDATE_DAY)

#define FWDATE_BASE_YEAR	1970
#define FWDATE_BASE_MONTH	1
#define FWDATE_BASE_DAY		1
//
// End of modification

enum FWUtilDateStatus_Constants
{
	FWUtilDateStatus_Valid,
	FWUtilDateStatus_Invalid,
	FWUtilDateStatus_Null
};

enum FWUtilDateMonth_Constants
{
	FWUtilDateMonth_January = 1,
	FWUtilDateMonth_February = 2,
	FWUtilDateMonth_March = 3,
	FWUtilDateMonth_April = 4,
	FWUtilDateMonth_May = 5,
	FWUtilDateMonth_June = 6,
	FWUtilDateMonth_July = 7,
	FWUtilDateMonth_August = 8,
	FWUtilDateMonth_September = 9,
	FWUtilDateMonth_October = 10,
	FWUtilDateMonth_November = 11,
	FWUtilDateMonth_December = 12,
	FWUtilDateMonth_Count
};

enum FWUtilDateDay_Constants
{
	FWUtilDateDay_Sunday = 0,
	FWUtilDateDay_Monday = 1,
	FWUtilDateDay_Tuesday = 2,
	FWUtilDateDay_Wednesday = 3,
	FWUtilDateDay_Thursday = 4,
	FWUtilDateDay_Friday = 5,
	FWUtilDateDay_Saturday = 6,
	FWUtilDateDay_Count
};

// Modified by DJW 20020724
//
enum FWUtilDateAdd_Constants
{
	FWUtilDateAdd_Year,
	FWUtilDateAdd_Month,
	FWUtilDateAdd_Week,
	FWUtilDateAdd_Day
};
//
// End of modification

enum FWUtilDateDiff_Constants
{
	FWUtilDateDiff_Seconds,
	FWUtilDateDiff_Minutes,
	FWUtilDateDiff_Hours,
	FWUtilDateDiff_Days,
	FWUtilDateDiff_Weeks,

	// Added by MV 20040216
	//
	FWUtilDateDiff_Months,
	FWUtilDateDiff_Years
	//
	// End Addition
};

/*
Add the following line to the end of AUTOEXP.DAT so that tooltips will work for this class
CFWUtilDate =<m_DateTime.wDay>/<m_DateTime.wMonth>/<m_DateTime.wYear> <m_DateTime.wHour>:<m_DateTime.wMinute>:<m_DateTime.wSecond>:<m_DateTime.wMilliseconds>
*/

class CFWUtilDate
{
private:
	SYSTEMTIME		m_DateTime;
	//CString		m_cstrNull;

	static CString	UserLocaleInfo(LCTYPE LCType);
	static CString  LocaleInfo(LCID alcid, LCTYPE LCType);

public:

#ifdef __AFX_H__
	CFWUtilDate(const COleDateTime &aodtDate);
	FWUtilDateStatus_Constants SetDate(const COleDateTime &aodtDate);
	operator COleDateTime() const;
	CFWUtilDate &operator = (const COleDateTime &aodtDate);
	operator DATE() const;
	CFWUtilDate &operator = (DATE aDate);
#endif

	// Construction / Destruction
	CFWUtilDate();
	CFWUtilDate(const CFWUtilDate &asdDate);
	CFWUtilDate(int aiFWUtilDateIndex);
	CFWUtilDate(int aiYear, int aiMonth, int aiDay, int aiHours = 0, int aiMinutes = 0, int aiSeconds = 0, int aiMilliseconds = 0);
	CFWUtilDate(long alJulianDate);
	CFWUtilDate(SYSTEMTIME astDate);
	CFWUtilDate(ULONGLONG aui64Val);
	virtual ~CFWUtilDate();

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
	int GetDayOfYear(FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January);
	FWUtilDateDay_Constants GetDayOfWeek();
	FWUtilDateStatus_Constants GetStatus() const;

	// Modified by DJW 20020724
	//
	// SetTotalMonths / GetTotalMonths
	// These functions set or get a number respresenting a whole number
	// of months since the base date (defined above).
	//
	int GetTotalMonths() const;
	FWUtilDateStatus_Constants SetTotalMonths(int aiMonths);
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

	bool	SetToWCDate(FWUtilDateDay_Constants aeStartOfWeek);
	bool	IsLastDayOfWeek(FWUtilDateDay_Constants aeStartOfWeek);

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
	CFWUtilDate &AddYears(int aiYears);
	CFWUtilDate &AddMonths(int aiMonths);
	CFWUtilDate &AddWeeks(int aiWeeks);
	CFWUtilDate &AddDays(int aiDays);
	CFWUtilDate &AddMinutes(int aiMinutes);
	CFWUtilDate &AddSeconds(int aiSeconds);
	__int64	Difference(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate);
	__int64	DifferenceEx(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate);
	double	Subtract(FWUtilDateDiff_Constants aeType, const CFWUtilDate& aFWUtilDate);

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
	CString	GetFWUtilDateString() const;
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
	FWUtilDateStatus_Constants SetTimeStamp(LPCTSTR aszTimeStamp);
	FWUtilDateStatus_Constants SetDate(const CFWUtilDate &asdDate);
	FWUtilDateStatus_Constants SetDate(int aiFWUtilDateIndex);
	FWUtilDateStatus_Constants SetDate(int aiYear, int aiMonth, int aiDay);
	FWUtilDateStatus_Constants SetTime(int aiHours, int aiMinutes, int aiSeconds, int aiMilliseconds);
	FWUtilDateStatus_Constants SetTime(int aiMinuteOfDay);
	FWUtilDateStatus_Constants SetDateTime(int aiYear, int aiMonth, int aiDay, int aiHours = 0, int aiMinutes = 0, int aiSeconds = 0, int aiMilliseconds = 0);
	FWUtilDateStatus_Constants SetDate(long alJulianDate);
	FWUtilDateStatus_Constants SetDate(SYSTEMTIME astDate);
	FWUtilDateStatus_Constants SetDate(ULONGLONG aui64Val);
	FWUtilDateStatus_Constants SetFWUtilDateString(LPCTSTR aszFWUtilDate); // Converts String Date in format YYYYMMDD
	SYSTEMTIME* GetPtrAccess() {return &m_DateTime;}
	void SetCurrent();
	void Clear();

	// Operators
	bool operator != (const CFWUtilDate &asdDate) const;
	bool operator < (const CFWUtilDate& asdDate) const;
	bool operator <= (const CFWUtilDate &asdDate) const;
	bool operator == (const CFWUtilDate &asdDate) const;
	bool operator > (const CFWUtilDate &asdDate) const;
	bool operator >= (const CFWUtilDate &asdDate) const;
	CFWUtilDate &operator = (const CFWUtilDate& asdDate);
	CFWUtilDate &operator = (const SYSTEMTIME &astDate);
	CFWUtilDate &operator = (int aiFWUtilDateIndex);
	CFWUtilDate &operator = (long alJulianDate);
	CFWUtilDate &operator = (ULONGLONG aui64Val);

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

class CFWUtilDateRange
{
	CFWUtilDate m_FWUtilDate1;
	CFWUtilDate m_FWUtilDate2;
public:
	// Construction / Destruction
	CFWUtilDateRange();
	CFWUtilDateRange(const CFWUtilDateRange &aFWUtilDateRange);
	CFWUtilDateRange(CFWUtilDate aFWUtilDate1, CFWUtilDate aFWUtilDate2);
	virtual ~CFWUtilDateRange();
	// Members
	CFWUtilDate GetDate1();
	CFWUtilDate GetDate2();
	void SetRange(CFWUtilDate aFWUtilDate1, CFWUtilDate aFWUtilDate2);
	void SetRange(const CFWUtilDateRange &aFWUtilDateRange);
	CFWUtilDateRange &operator =(const CFWUtilDateRange &aFWUtilDateRange);
	bool IsNull();
	bool IsValid();
	void Clear();
	bool IsBetween(CFWUtilDate aFWUtilDate);
};

// Modified by DJW 20020724
//
///////////////////////////////
// Date Utility Functions //
///////////////////////////////

bool		FWUtilDateAdd(FWUtilDateAdd_Constants aeAddWhat, int aiVal, int &aiYear, int &aiMonth, int &aiDay);
bool		FWUtilDateCompare(int aiYear1, int aiMonth1, int aiDay1, int aiYear2, int aiMonth2, int aiDay2, int &aiCompare);
bool		FWUtilDateToUi64(int aiYear, int aiMonth, int aiDay, ULONGLONG &aui64Val);
bool		FWUi64ToDate(ULONGLONG aui64Val, int &aiYear, int &aiMonth, int &aiDay);
bool		FWIsLeapYear(int aiYear);
bool		FWIsValidDate(int aiYear, int aiMonth, int aiDay);
bool		FWSplitDate(LPCTSTR apctszDate, int &aiYear, int &aiMonth, int &aiDay, LPCTSTR apctszSeperator = NULL);
CString		FWUtilDateString(int aiYear = 0, int aiMonth = 0, int aiDay = 0, bool abUseLongDescription = false);
CString		FWGetDayName(int aiDay, bool abUseLongDescription = true);
CString		FWGetMonthName(int aiMonth, bool abUseLongDescription = true);
int			FWFindHalf(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January);
int			FWFindQuarter(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January);
// Added by MV 20030304
int			FWFindWeekEx(int &aiYear, int &aiMonth, int &aiDay);
int			FWFindWeekEx(int &aiYear, int &aiMonth, int &aiDay, CFWUtilDate &atStartOfWeekDay, FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January, FWUtilDateDay_Constants aeStartOfWeek = FWUtilDateDay_Sunday);
bool		IsDateBeforeFirstStartOfWeekDayOfYear(SYSTEMTIME astDate, SYSTEMTIME astSOY, FWUtilDateDay_Constants aeStartOfWeek);
// End of addition
int			FWFindWeek(int &aiYear, int &aiMonth, int &aiDay, FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January, FWUtilDateDay_Constants aeStartOfWeek = FWUtilDateDay_Sunday);
int			FWFindDay(int aiYear, int aiMonth, int aiDay, FWUtilDateMonth_Constants aeStartOfYear = FWUtilDateMonth_January);
int			FWGetDayOfWeek(int aiYear, int aiMonth, int aiDay);
int			FWLastDayOfMonth(int aiYear, int aiMonth);
int			FWStartOfWeekAdjuster(int aiDayOfWeek, int aiStartOfWeek);
bool		FWTimeToInt(int aiHours, int aiMinutes, int aiSeconds, int& aiInt);
bool		FWIntToTime(int aiInt, int& aiHours, int& aiMinutes, int& aiSeconds);
bool		FWUtilDateTimeToString(LPCTSTR aszPicture, CString& astrResult, SYSTEMTIME* apstDateTime, LPCTSTR aszDayChars = NULL);
//
// End of modification

// SJSG 20050621 Added LCID to FWParseTime() and FWParseShortDate() functions
bool		FWParseTime(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, LCID aLocale = LOCALE_USER_DEFAULT);
bool		FWParseTimeEx(LPCTSTR aszTimeString, int& aiHours, int& aiMinutes, int& aiSeconds, int& aiMilliseconds, int aiStartOfDayInMinutes, bool& abNextDay, LCID aLocale = LOCALE_USER_DEFAULT);
bool		FWParseShortDate(LPCTSTR aszDateString, int& aiYear, int& aiMonth, int& aiDay, LCID aLocale = LOCALE_USER_DEFAULT);

#endif
