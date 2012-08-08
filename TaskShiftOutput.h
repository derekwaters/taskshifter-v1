// TaskShiftOutput.h: interface for the CTaskShiftOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSHIFTOUTPUT_H__E099EDA7_7DD3_4028_B929_EC9A680E0911__INCLUDED_)
#define AFX_TASKSHIFTOUTPUT_H__E099EDA7_7DD3_4028_B929_EC9A680E0911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <vector>
#include "TaskShiftDefs.h"
#include "IncludedFiles\excel8.h"

enum	eTaskShiftOutputType
{
	OutputToExcel,
	OutputToCSV,
	OutputTypeCount
};

enum	eTaskShiftOutputItem
{
	TaskDescription,
	TaskStartTime,
	TaskEndTime,
	TaskDuration,
	TaskDate,
	TaskDay,
	Blank,
	TaskDurationFloat,
	TaskBreaks,
	OutputItemCount
};

struct	tTSOutputColumn
{
	CString					strLabel;
	eTaskShiftOutputItem	eItem;

	tTSOutputColumn(LPCTSTR label, eTaskShiftOutputItem item) : strLabel(label), eItem(item) { ; }
};

typedef std::vector<tTSOutputColumn>	TSOutputColumnVector;


class CTaskShiftOutput  
{
public:
	CTaskShiftOutput();
	virtual ~CTaskShiftOutput();

	bool	ResetToDefaultOptions();
	bool	Output(TimeBlockVector &avecTimeBlocks, TaskVector &avecTasks);

	bool	SaveOutputFormat();
	bool	LoadOutputFormat();

	void					SetOutputFolder(LPCTSTR apszOutputFolder) { m_cstrOutputFolder = apszOutputFolder; }
	LPCTSTR					GetOutputFolder() { return m_cstrOutputFolder; }
	void					SetOutputFilenameFormat(LPCTSTR apszFormat) { m_cstrOutputFilenameFormat = apszFormat; }
	LPCTSTR					GetOutputFilenameFormat() { return m_cstrOutputFilenameFormat; }
	void					SetOutputType(eTaskShiftOutputType aeType) { m_eOutputType = aeType; }
	eTaskShiftOutputType	GetOutputType() { return m_eOutputType; }
	void					SetOpenAfterOutput(bool abOpen) { m_bOpenAfterOutput = abOpen; }
	bool					GetOpenAfterOutput() { return m_bOpenAfterOutput; }

	void					SetEmailOptions(bool abAttach,
											LPCTSTR apszSendTo = NULL,
											LPCTSTR apszFrom = NULL,
											LPCTSTR apszSMTPServer = NULL,
											LPCTSTR apszSubject = NULL,
											LPCTSTR apszBody = NULL);
	bool					GetAttachToEmail() { return m_bAttachToEmail; }
	LPCTSTR					GetEmailTo() { return m_cstrEmailTo; }
	LPCTSTR					GetEmailFrom() { return m_cstrEmailFrom; }
	LPCTSTR					GetEmailSMTPServer() { return m_cstrSMTPServer; }
	LPCTSTR					GetEmailSubject() { return m_cstrEmailSubject; }
	LPCTSTR					GetEmailBody() { return m_cstrEmailBody; }

	TSOutputColumnVector	&GetOutputColumns() { return m_vecColumns; }

	static CString			m_acstrOutputItemLabel[OutputItemCount];

	void					SetTemplateFile(LPCTSTR apszTemplateFile) { m_cstrUseTemplateFile = apszTemplateFile; }
	LPCTSTR					GetTemplateFile() { return m_cstrUseTemplateFile; }

protected:
	bool					OutputToCSVFile(TimeBlockVector &avecTimeBlocks, LPCTSTR apszFilename, TaskVector &avecTasks);
	bool					OutputToExcelFile(TimeBlockVector &avecTimeBlocks, LPCTSTR apszFilename, TaskVector &avecTasks);
	bool					AttachExcelApplication(MSExcel::_Application &atApplication);

protected:
	eTaskShiftOutputType	m_eOutputType;
	bool					m_bOpenAfterOutput;
	bool					m_bAttachToEmail;
	bool					m_bMergeAdjoiningTasks;
	CString					m_cstrEmailTo;
	CString					m_cstrEmailFrom;
	CString					m_cstrSMTPServer;
	CString					m_cstrEmailSubject;
	CString					m_cstrEmailBody;
	TSOutputColumnVector	m_vecColumns;

	CString					m_cstrOutputFolder;
	CString					m_cstrOutputFilenameFormat;

	CString					m_cstrUseTemplateFile;

	bool					m_bOutputOnlyChangedColumns;

	std::vector<CString>	m_vecOutputFormat;
	std::vector<CString>	m_vecOutputItem;
};

#endif // !defined(AFX_TASKSHIFTOUTPUT_H__E099EDA7_7DD3_4028_B929_EC9A680E0911__INCLUDED_)
