// TaskShiftOutput.cpp: implementation of the CTaskShiftOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskShifter.h"
#include "TaskShiftOutput.h"
#include "TaskShiftUtils.h"
#include "IncludedFiles\FWUtilDate.h"
#include "IncludedFiles\SimpleSendMail.h"
#include "IncludedFiles\MSExcel.h"
#include "TinyXml\TinyXml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString CTaskShiftOutput::m_acstrOutputItemLabel[OutputItemCount] = { _T("Task Description"),
																	  _T("Task Start Time"),
																	  _T("Task End Time"),
																	  _T("Task Duration"),
																	  _T("Task Date"),
																	  _T("Task Day Of Week"),
																	  _T("Blank Column"), 
																	  _T("Task Duration Float"), 
																	  _T("Task Breaks") };

CTaskShiftOutput::CTaskShiftOutput()
{
	m_vecOutputFormat.push_back(_T("Excel"));
	m_vecOutputFormat.push_back(_T("CSV"));
	ASSERT(m_vecOutputFormat.size() == OutputTypeCount);

	m_vecOutputItem.push_back(_T("Description"));
	m_vecOutputItem.push_back(_T("StartTime"));
	m_vecOutputItem.push_back(_T("EndTime"));
	m_vecOutputItem.push_back(_T("Duration"));
	m_vecOutputItem.push_back(_T("Date"));
	m_vecOutputItem.push_back(_T("Day"));
	m_vecOutputItem.push_back(_T("Blank"));
	m_vecOutputItem.push_back(_T("Duration"));
	m_vecOutputItem.push_back(_T("Breaks"));
	ASSERT(m_vecOutputItem.size() == OutputItemCount);

	ResetToDefaultOptions();
	LoadOutputFormat();
}

CTaskShiftOutput::~CTaskShiftOutput()
{
	SaveOutputFormat();
}

bool CTaskShiftOutput::ResetToDefaultOptions()
{
	m_eOutputType = OutputToCSV;
	m_bOpenAfterOutput = true;
	m_bAttachToEmail = false;
	m_cstrEmailTo.Empty();
	m_cstrEmailFrom.Empty();
	m_cstrSMTPServer.Empty();
	m_cstrEmailSubject.Empty();
	m_cstrEmailBody.Empty();
	m_cstrUseTemplateFile.Empty();
	m_bMergeAdjoiningTasks = false;

	m_bOutputOnlyChangedColumns = true;

	m_vecColumns.clear();
	m_vecColumns.push_back(tTSOutputColumn(_T("Day"), TaskDay));
	m_vecColumns.push_back(tTSOutputColumn(_T("Date"), TaskDate));
	m_vecColumns.push_back(tTSOutputColumn(_T("Start Time"), TaskStartTime));
	m_vecColumns.push_back(tTSOutputColumn(_T("End Time"), TaskEndTime));
	m_vecColumns.push_back(tTSOutputColumn(_T("Hours"), TaskDuration));
	m_vecColumns.push_back(tTSOutputColumn(_T("Details"), TaskDescription));

	m_cstrOutputFolder = CTaskShiftUtils::GetAppPath();
	m_cstrOutputFilenameFormat = _T("DJW_Timesheet_[Year][Month][Day].[Extension]");

	return true;
}

void CTaskShiftOutput::SetEmailOptions(bool abAttach, LPCTSTR apszSendTo, LPCTSTR apszFrom, LPCTSTR apszSMTPServer, LPCTSTR apszSubject, LPCTSTR apszBody)
{
	m_bAttachToEmail = abAttach;
	m_cstrEmailTo = apszSendTo;
	m_cstrEmailFrom = apszFrom;
	m_cstrSMTPServer = apszSMTPServer;
	m_cstrEmailSubject = apszSubject;
	m_cstrEmailBody = apszBody;
}

bool CTaskShiftOutput::Output(TimeBlockVector &avecTimeBlocks, TaskVector &avecTasks)
{
	bool							vbRet = false;
	CString							vcstrOutputFilename;
	CFWUtilDate						vdtCurrent;
	CFWUtilDate						vdtLastDate;
	CString							vcstrTemp;

	if (avecTimeBlocks.size() > 0)
	{
		vdtLastDate.SetDate(avecTimeBlocks[avecTimeBlocks.size() - 1].iDate);
	}

	vcstrOutputFilename = CTaskShiftUtils::ValidatePath(m_cstrOutputFolder) + m_cstrOutputFilenameFormat;
	vcstrTemp.Format(_T("%04i"), vdtLastDate.GetYear());
	vcstrOutputFilename.Replace(_T("[Year]"), vcstrTemp);
	vcstrTemp.Format(_T("%02i"), vdtLastDate.GetMonth());
	vcstrOutputFilename.Replace(_T("[Month]"), vcstrTemp);
	vcstrTemp.Format(_T("%02i"), vdtLastDate.GetDay());
	vcstrOutputFilename.Replace(_T("[Day]"), vcstrTemp);

	vcstrTemp.Format(_T("%04i"), vdtCurrent.GetYear());
	vcstrOutputFilename.Replace(_T("[CurrentYear]"), vcstrTemp);
	vcstrTemp.Format(_T("%02i"), vdtCurrent.GetMonth());
	vcstrOutputFilename.Replace(_T("[CurrentMonth]"), vcstrTemp);
	vcstrTemp.Format(_T("%02i"), vdtCurrent.GetDay());
	vcstrOutputFilename.Replace(_T("[CurrentDay]"), vcstrTemp);

	switch (m_eOutputType)
	{
	case OutputToExcel:
		vcstrTemp = _T("xls");
		break;
	case OutputToCSV:
	default:
		vcstrTemp = _T("csv");
		break;
	}
	vcstrOutputFilename.Replace(_T("[Extension]"), vcstrTemp);

	switch (m_eOutputType)
	{
	case OutputToCSV:
		vbRet = OutputToCSVFile(avecTimeBlocks, vcstrOutputFilename, avecTasks);
		break;
	case OutputToExcel:
		vbRet = OutputToExcelFile(avecTimeBlocks, vcstrOutputFilename, avecTasks);
		break;
	}

	if (vbRet)
	{
		if (m_bOpenAfterOutput)
		{
			ShellExecute(::GetDesktopWindow(), _T("open"), vcstrOutputFilename, NULL, NULL, SW_SHOW);
		}
		if (m_bAttachToEmail)
		{
			CSimpleSendMail	vtMailer;
			CStringArray	varrTo;
			CStringArray	varrAttachments;

			varrAttachments.Add(vcstrOutputFilename);

			CTaskShiftUtils::SplitStringIntoArray(varrTo, m_cstrEmailTo, _T(";"));
			
			if (!vtMailer.SendMail(m_cstrSMTPServer,
								  varrTo,
								  varrTo,
								  m_cstrEmailFrom,
								  m_cstrEmailFrom,
								  m_cstrEmailSubject,
								  m_cstrEmailBody,
								  false,
								  varrAttachments))
			{
				vbRet = false;

				CString	vcstrErrorMessage;

				vcstrErrorMessage.Format(_T("Sending your timesheet using SMTP failed (%i). Please check your email settings and try again."), GetLastError());
				AfxMessageBox(vcstrErrorMessage);
			}
		}
	}
	return vbRet;
}

bool CTaskShiftOutput::OutputToCSVFile(TimeBlockVector &avecTimeBlocks, LPCTSTR apszFilename, TaskVector &avecTasks)
{
	bool							vbRet = false;
	CString							vcstrTemp;
	TSOutputColumnVector::iterator	vtCol;
	CString							vcstrLine;
	TimeBlockVector::iterator		vtRow;
	CFWUtilDate						vdtTemp;
	int								viDuration;
	std::vector<CString>			vvecPrevRow;
	int								viCol;
	bool							vbCheckDuplicatesInColumn;
	int								viLength;
	CFile							vfTemplate;
	CString							vcstrTemplate;
	CFile							vfOutput;
	CString							vcstrBuffer;
	int								viWeekTotalTime = 0;
	TimeBlockVector					vvecMergedBlocks;
#ifdef TASKSHIFTER_PROFESSIONAL
	tTimeBlock						vtMerge(-1, 0, 0, -1);
#else
	tTimeBlock						vtMerge(-1, 0, 0, _T(""));
#endif

	vvecPrevRow.resize(m_vecColumns.size());

	// Write out the header row.
	//
	for (vtCol = m_vecColumns.begin(); vtCol != m_vecColumns.end(); ++vtCol)
	{
		vcstrTemp.Format(_T("\"%s\","), (*vtCol).strLabel);
		vcstrLine += vcstrTemp;
	}
	vcstrLine += _T("\n");
	vcstrBuffer += vcstrLine;

	if (m_bMergeAdjoiningTasks)
	{
		vtMerge.iDate = -1;
		for (vtRow = avecTimeBlocks.begin(); vtRow != avecTimeBlocks.end(); ++vtRow)
		{
			if (vtMerge.iDate != (*vtRow).iDate ||
#ifdef TASKSHIFTER_PROFESSIONAL
				vtMerge.taskId != (*vtRow).taskId)
#else
				vtMerge.strDescription != (*vtRow).strDescription)
#endif
			{
				// Create a new block and add the old one.
				//
				if (vtMerge.iDate != -1)
				{
					vvecMergedBlocks.push_back(vtMerge);
					vtMerge.iBreaks = 0;
					vtMerge.iDate = (*vtRow).iDate;
					vtMerge.iStartTime = (*vtRow).iStartTime;
					vtMerge.iEndTime = (*vtRow).iEndTime;
#ifdef TASKSHIFTER_PROFESSIONAL
					vtMerge.taskId = (*vtRow).taskId;
#else
					vtMerge.strDescription = (*vtRow).strDescription;
#endif
				}
			}
			else
			{
				vtMerge.iBreaks += (*vtRow).iStartTime - vtMerge.iEndTime;
				vtMerge.iEndTime = (*vtRow).iEndTime;
			}
		}
	}
	else
	{
		vvecMergedBlocks.insert(vvecMergedBlocks.begin(), avecTimeBlocks.begin(), avecTimeBlocks.end());
	}

	// Write out each detail row
	//
	for (vtRow = vvecMergedBlocks.begin(); vtRow != vvecMergedBlocks.end(); ++vtRow)
	{
		vcstrLine.Empty();

		viDuration = (*vtRow).iEndTime - (*vtRow).iStartTime;
		if (viDuration < 0)
			viDuration += (24 * 60);
		viWeekTotalTime += viDuration;

		for (vtCol = m_vecColumns.begin(), viCol = 0; vtCol != m_vecColumns.end(); ++vtCol, ++viCol)
		{
			vbCheckDuplicatesInColumn = false;

			switch ((*vtCol).eItem)
			{
			case TaskDescription:
#ifdef TASKSHIFTER_PROFESSIONAL
				vcstrTemp.Format(_T("\"%s\""), avecTasks[avecTasks.FindTask((*vtRow).taskId)].strDescription);;
#else
				vcstrTemp.Format(_T("\"%s\""), (*vtRow).strDescription);
#endif
				break;
			case TaskStartTime:
				vcstrTemp.Format(_T("\"%02i:%02i\""), (*vtRow).iStartTime / 60, (*vtRow).iStartTime % 60);
				break;
			case TaskEndTime:
				vcstrTemp.Format(_T("\"%02i:%02i\""), (*vtRow).iEndTime / 60, (*vtRow).iEndTime % 60);
				break;
			case TaskDuration:
				vcstrTemp.Format(_T("\"%02i:%02i\""), viDuration / 60, viDuration % 60);
				break;
			case TaskDurationFloat:
				vcstrTemp.Format(_T("\"%.1f\""), ((double)viDuration) / 60.0);
				break;
			case TaskBreaks:
				vcstrTemp.Format(_T("\"%02i:%02i\""), (*vtRow).iBreaks / 60, (*vtRow).iBreaks % 60);
				break;
			case TaskDate:
				vdtTemp.SetDate((*vtRow).iDate);
				vcstrTemp.Format(_T("\"%s\""), vdtTemp.GetDateString(false));
				vbCheckDuplicatesInColumn = true;
				break;
			case TaskDay:
				vdtTemp.SetDate((*vtRow).iDate);
				vcstrTemp.Format(_T("\"%s\""), vdtTemp.GetDayName(true));
				vbCheckDuplicatesInColumn = true;
				break;
			case Blank:
				vcstrTemp = _T("\"\"");
				break;
			}
			if (m_bOutputOnlyChangedColumns && 
				vtRow != avecTimeBlocks.begin() && 
				(vcstrTemp == vvecPrevRow[viCol]) &&
				vbCheckDuplicatesInColumn)
			{
				vcstrTemp = _T("\"\"");
			}
			else
			{
				vvecPrevRow[viCol] = vcstrTemp;
			}
			vcstrLine += vcstrTemp;
			vcstrLine += _T(",");
		}
		vcstrLine += _T("\n");

		vcstrBuffer += vcstrLine;
	}
	
	if (vfOutput.Open(apszFilename, CFile::modeCreate | CFile::modeWrite | CFile::shareExclusive))
	{
		if (!m_cstrUseTemplateFile.IsEmpty() && 
			GetFileAttributes(m_cstrUseTemplateFile) != NULL &&
			vfTemplate.Open(m_cstrUseTemplateFile, CFile::modeRead | CFile::shareDenyWrite))
		{
			viLength = (int)vfTemplate.GetLength() / sizeof(_TCHAR);
			vfTemplate.Read(vcstrTemplate.GetBuffer(viLength), viLength);
			vcstrTemplate.ReleaseBuffer(viLength);
			vfTemplate.Close();

			vdtTemp.SetDate(avecTimeBlocks[0].iDate);
			vdtTemp.SetToFirstDayOfWeek(LJDateDay_Sunday);
			vcstrTemplate.Replace(_T("[WeekSunday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekMonday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekTuesday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekWednesday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekThursday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekFriday]"), vdtTemp.GetDateString(false));
			vdtTemp.AddDays(1);
			vcstrTemplate.Replace(_T("[WeekSaturday]"), vdtTemp.GetDateString(false));

			vcstrTemp.Format(_T("\"%02i:%02i\""), viWeekTotalTime / 60, viWeekTotalTime % 60);
			vcstrTemplate.Replace(_T("[TotalWeekTime]"), vcstrTemp);

			vcstrTemplate.Replace(_T("[Timesheet]"), vcstrBuffer);

			vfOutput.Write(vcstrTemplate, vcstrTemplate.GetLength() * sizeof(_TCHAR));
		}
		else
		{
			vfOutput.Write(vcstrBuffer, vcstrBuffer.GetLength() * sizeof(_TCHAR));
		}
		vbRet = true;
		vfOutput.Close();
	}
		
	return vbRet;
}

bool CTaskShiftOutput::OutputToExcelFile(TimeBlockVector &avecTimeBlocks, LPCTSTR apszFilename, TaskVector &avecTasks)
{
	// Generate a temporary CSV file, then open it in Excel, and then save it under the new filename.
	//
	bool							vbRet = false;
	_TCHAR							vszTempFile[_MAX_PATH];
	_TCHAR							vszTempPath[_MAX_PATH];

	if (GetTempPath(_MAX_PATH, vszTempPath))
	{
		if (GetTempFileName(vszTempPath, _T("TSX"), 0, vszTempFile))
		{
			if (OutputToCSVFile(avecTimeBlocks, vszTempFile, avecTasks))
			{
				MSExcel::_Application		vtExcel;
				MSExcel::Workbooks			vtBooks;
				MSExcel::_Workbook			vtBook;
				COleVariant					vvtZero((long)0);
				COleVariant					vvtFalse((long)false);
				COleVariant					vvtFormat((long)2);
				COleVariant					vvtEmpty(_T(""));
				COleVariant					vvtOrigin((long)MSExcelEnum::xlWindows);
				COleVariant					vvtNull;
				COleVariant					vvtOutputFilename(apszFilename);
				
				if (AttachExcelApplication(vtExcel))
				{
					vtBooks = vtExcel.GetWorkbooks();

					vtBook = vtBooks.Open(vszTempFile, vvtZero, vvtFalse, vvtFormat, vvtEmpty, vvtEmpty, vvtFalse, vvtOrigin, vvtNull, vvtFalse, vvtFalse, vvtNull, vvtFalse);

					vtBook.SaveAs(vvtOutputFilename, COleVariant((long)MSExcelEnum::xlWorkbookNormal), vvtNull, vvtNull, vvtNull, vvtNull, 0, vvtNull, vvtFalse, vvtNull, vvtNull);

					vtBook.Close(vvtFalse, vvtNull, vvtNull);

					vbRet = true;
				}

				DeleteFile(vszTempFile);
			}
		}
	}
	return vbRet;
}

bool CTaskShiftOutput::AttachExcelApplication(MSExcel::_Application &atApplication)
{
	bool				vbRet = false;
	CLSID				vclsExcel;
	HRESULT				vhResult;
	IUnknown			*vpUnknown = NULL;
	IDispatch			*vpDispatch = NULL;
	
	vhResult = CLSIDFromProgID(L"Excel.Application", &vclsExcel);
	if (SUCCEEDED(vhResult))
	{
		vhResult = GetActiveObject(vclsExcel, NULL, &vpUnknown);
		if (SUCCEEDED(vhResult) && (vpUnknown != NULL))
		{
			vhResult = vpUnknown->QueryInterface(IID_IDispatch, (void **)&vpDispatch);
			if (SUCCEEDED(vhResult ) && (vpDispatch != NULL))
			{
				atApplication.AttachDispatch(vpDispatch, TRUE);	// Because we specify auto-release, we do not
				// need to release the IDispatch pointer.
				vbRet = true;
			}
			
			vpUnknown->Release();	// We don't need this COM interface any more.
		}
	}
	if (!vbRet)
	{
		vbRet = (atApplication.CreateDispatch(_T("Excel.Application")) != FALSE);
	}
	return vbRet;
}

bool CTaskShiftOutput::SaveOutputFormat()
{
	TiXmlDocument					vtDocument;
	TiXmlElement					*vptRoot;
	TiXmlElement					*vptOption;
	TiXmlElement					*vptColumns;
	TiXmlElement					*vptColumn;
	CString							vcstrFilename;
	TSOutputColumnVector::iterator	vtCol;

	vcstrFilename = CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()) + _T("OutputFormat.xml");

	vptRoot = (TiXmlElement *)vtDocument.InsertEndChild(TiXmlElement(_T("OutputFormat")));
	vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("Type")));
	vptOption->InsertEndChild(TiXmlText(m_vecOutputFormat[m_eOutputType]));
	if (m_bOpenAfterOutput)
	{
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("OpenAfterOutput")));
	}
	
	if (m_bAttachToEmail)
	{
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("EmailTo")));
		vptOption->InsertEndChild(TiXmlText(m_cstrEmailTo));
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("EmailFrom")));
		vptOption->InsertEndChild(TiXmlText(m_cstrEmailFrom));
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("EmailServer")));
		vptOption->InsertEndChild(TiXmlText(m_cstrSMTPServer));
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("EmailSubject")));
		vptOption->InsertEndChild(TiXmlText(m_cstrEmailSubject));
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("EmailBody")));
		vptOption->InsertEndChild(TiXmlText(m_cstrEmailBody));
	}

	vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("OutputPath")));
	vptOption->InsertEndChild(TiXmlText(m_cstrOutputFolder));
	vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("OutputFilenameFormat")));
	vptOption->InsertEndChild(TiXmlText(m_cstrOutputFilenameFormat));

	if (!m_cstrUseTemplateFile.IsEmpty())
	{
		vptOption = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("UseTemplateFile")));
		vptOption->InsertEndChild(TiXmlText(m_cstrUseTemplateFile));
	}

	vptColumns = (TiXmlElement *)vptRoot->InsertEndChild(TiXmlElement(_T("Columns")));
	for (vtCol = m_vecColumns.begin(); vtCol != m_vecColumns.end(); ++vtCol)
	{
		vptColumn = (TiXmlElement *)vptColumns->InsertEndChild(TiXmlElement(_T("Column")));
		vptOption = (TiXmlElement *)vptColumn->InsertEndChild(TiXmlElement(_T("Label")));
		vptOption->InsertEndChild(TiXmlText((*vtCol).strLabel));
		vptOption = (TiXmlElement *)vptColumn->InsertEndChild(TiXmlElement(_T("Item")));
		vptOption->InsertEndChild(TiXmlText(m_vecOutputItem[(*vtCol).eItem]));
	}

	return vtDocument.SaveFile(vcstrFilename);
}

bool CTaskShiftOutput::LoadOutputFormat()
{
	bool							vbRet = false;
	TiXmlDocument					vtDocument;
	TiXmlElement					*vptRoot;
	TiXmlElement					*vptOption;
	TiXmlNode						*vptChild;

	TiXmlElement					*vptColumns;
	TiXmlElement					*vptColumn;
	CString							vcstrFilename;
	CString							vcstrTemp;
	CString							vcstrLabel;

	int								viPos;
	eTaskShiftOutputItem			veItem;

	vcstrFilename = CTaskShiftUtils::ValidatePath(CTaskShiftUtils::GetAppPath()) + _T("OutputFormat.xml");

	m_eOutputType = OutputToCSV;
	m_bOpenAfterOutput = false;
	m_bAttachToEmail = false;
	m_cstrEmailTo.Empty();
	m_cstrEmailFrom.Empty();
	m_cstrSMTPServer.Empty();
	m_cstrEmailSubject.Empty();
	m_cstrEmailBody.Empty();
	m_vecColumns.clear();
	m_cstrUseTemplateFile.Empty();

	if (vtDocument.LoadFile(vcstrFilename))
	{
		vptRoot = vtDocument.RootElement();
		if (vptRoot != NULL)
		{
			vptOption = vptRoot->FirstChildElement(_T("Type"));
			if (vptOption != NULL && !vptOption->NoChildren())
			{
				vptChild = vptOption->FirstChild();
				if (vptChild->Type() == TiXmlNode::TEXT)
				{
					vcstrTemp = vptChild->ToText()->Value();
					
					for (viPos = 0; viPos < OutputTypeCount; ++viPos)
					{
						if (vcstrTemp.CompareNoCase(m_vecOutputFormat[viPos]) == 0)
						{
							m_eOutputType = (eTaskShiftOutputType)viPos;
							break;
						}
					}
				}
			}
			vptOption = vptRoot->FirstChildElement(_T("OpenAfterOutput"));
			if (vptOption != NULL)
			{
				m_bOpenAfterOutput = true;
			}
		
			vptOption = vptRoot->FirstChildElement(_T("UseTemplateFile"));
			if (vptOption != NULL && !vptOption->NoChildren())
			{
				vptChild = vptOption->FirstChild();
				if (vptChild->Type() == TiXmlNode::TEXT)
				{
					m_cstrUseTemplateFile = vptChild->ToText()->Value();
				}
			}

			vptOption = vptRoot->FirstChildElement(_T("EmailTo"));
			if (vptOption != NULL && !vptOption->NoChildren())
			{
				m_bAttachToEmail = true;

				vptChild = vptOption->FirstChild();
				if (vptChild->Type() == TiXmlNode::TEXT)
				{
					m_cstrEmailTo = vptChild->ToText()->Value();
				}

				vptOption = vptRoot->FirstChildElement(_T("EmailFrom"));
				if (vptOption != NULL && !vptOption->NoChildren())
				{
					vptChild = vptOption->FirstChild();
					if (vptChild->Type() == TiXmlNode::TEXT)
					{
						m_cstrEmailFrom = vptChild->ToText()->Value();
					}
				}
				vptOption = vptRoot->FirstChildElement(_T("EmailServer"));
				if (vptOption != NULL && !vptOption->NoChildren())
				{
					vptChild = vptOption->FirstChild();
					if (vptChild->Type() == TiXmlNode::TEXT)
					{
						m_cstrSMTPServer = vptChild->ToText()->Value();
					}
				}
				vptOption = vptRoot->FirstChildElement(_T("EmailSubject"));
				if (vptOption != NULL && !vptOption->NoChildren())
				{
					vptChild = vptOption->FirstChild();
					if (vptChild->Type() == TiXmlNode::TEXT)
					{
						m_cstrEmailSubject = vptChild->ToText()->Value();
					}
				}
				vptOption = vptRoot->FirstChildElement(_T("EmailBody"));
				if (vptOption != NULL && !vptOption->NoChildren())
				{
					vptChild = vptOption->FirstChild();
					if (vptChild->Type() == TiXmlNode::TEXT)
					{
						m_cstrEmailBody = vptChild->ToText()->Value();
					}
				}
			}

			vptOption = vptRoot->FirstChildElement(_T("OutputPath"));
			if (vptOption != NULL && !vptOption->NoChildren())
			{
				vptChild = vptOption->FirstChild();
				if (vptChild->Type() == TiXmlNode::TEXT)
				{
					m_cstrOutputFolder = vptChild->ToText()->Value();
				}
			}
			vptOption = vptRoot->FirstChildElement(_T("OutputFilenameFormat"));
			if (vptOption != NULL && !vptOption->NoChildren())
			{
				vptChild = vptOption->FirstChild();
				if (vptChild->Type() == TiXmlNode::TEXT)
				{
					m_cstrOutputFilenameFormat = vptChild->ToText()->Value();
				}
			}

			vptColumns = vptRoot->FirstChildElement(_T("Columns"));
			if (vptColumns != NULL)
			{
				for (vptColumn = vptColumns->FirstChildElement(_T("Column")); vptColumn != NULL; vptColumn = vptColumn->NextSiblingElement(_T("Column")))
				{

					vptOption = vptColumn->FirstChildElement(_T("Label"));
					if (vptOption != NULL && !vptOption->NoChildren())
					{
						vptChild = vptOption->FirstChild();
						if (vptChild->Type() == TiXmlNode::TEXT)
						{
							vcstrLabel = vptChild->ToText()->Value();

							vptOption = vptColumn->FirstChildElement(_T("Item"));
							if (vptOption != NULL && !vptOption->NoChildren())
							{
								vptChild = vptOption->FirstChild();
								if (vptChild->Type() == TiXmlNode::TEXT)
								{
									vcstrTemp = vptChild->ToText()->Value();

									veItem = Blank;
									for (viPos = 0; viPos < OutputItemCount; ++viPos)
									{
										if (vcstrTemp.CompareNoCase(m_vecOutputItem[viPos]) == 0)
										{
											veItem = (eTaskShiftOutputItem)viPos;
											break;
										}
									}

									m_vecColumns.push_back(tTSOutputColumn(vcstrLabel, veItem));
									vbRet = true;
								}
							}
						}
					}
				}
			}
		}
	}
	return vbRet;
}