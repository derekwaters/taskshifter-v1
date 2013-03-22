#include "StdAfx.h"
#include "TaskShifterSoapServer.h"
#include "../TinyXml/TinyXml.h"
#include "../TaskShiftUtils.h"
#include "../IncludedFiles/FWUtilDate.h"

#ifdef TASKSHIFTER_PROFESSIONAL

CTaskShifterSoapServer::CTaskShifterSoapServer()
{
	timesheetsAutoSubmitted = false;
	userCanCreateTasks = true;
}

CTaskShifterSoapServer::~CTaskShifterSoapServer()
{
}

bool CTaskShifterSoapServer::Connect(LPCTSTR server, TaskVector &taskList, LPCTSTR customUserName)
{
	_Struct_1		tempTask;
	_Struct_2		tempStruct;
	TiXmlDocument	xmlParser;
	TiXmlElement	*findElement;
	TiXmlElement	*findChild;
	TiXmlNode		*findNode;
	int				item;
	CString			key;
	CString			value;
	CString			tempName;
	int				tempId;

	serverUrl = server;
	if (!serverUrl.IsEmpty())
	{
		soapServer.soap_endpoint = serverUrl;
	}
	soapServer.Connect(customUserName ? customUserName : (LPCTSTR)CTaskShiftUtils::GetUserName(), (LPCTSTR)CTaskShiftUtils::GetComputerName(), customUserName ? UserType_Custom : UserType_Windows, userId);
	if (soapServer.error)
	{
		TRACE("SOAP Error (Connect): %s\n", soapServer.soap_fault_string());
		return false;
	}

	if (userId <= 0)
	{
		TRACE("SOAP Error (Connect): The user was not allowed to connect (userId = %d)\n", userId);
	}

	soapServer.GetPolicy(tempStruct);
	if (soapServer.error)
	{
		TRACE("SOAP Error (GetPolicy): %s\n", soapServer.soap_fault_string());
		return false;
	}
	for (item = 0; item < tempStruct.__size; ++item)
	{
		xmlParser.Clear();
		xmlParser.Parse(tempStruct.__ptr[item]);
		
		findElement = xmlParser.FirstChildElement(_T("key"));
		if (findElement)
		{
			findNode = findElement->FirstChild();
			ASSERT(findNode->Type() == TiXmlNode::TEXT);
			key = findNode->ToText()->Value();
			if (key.CompareNoCase(_T("timesheets_auto_submitted")) == 0)
			{
				findElement = xmlParser.FirstChildElement(_T("value"));
				if (findElement)
				{
					findNode = findElement->FirstChild();
					ASSERT(findNode->Type() == TiXmlNode::TEXT);
					value = findNode->ToText()->Value();
					
					timesheetsAutoSubmitted = _ttoi(value);
				}
			}
			else if (key.CompareNoCase(_T("user_can_create_tasks")) == 0)
			{
				findElement = xmlParser.FirstChildElement(_T("value"));
				if (findElement)
				{
					findNode = findElement->FirstChild();
					ASSERT(findNode->Type() == TiXmlNode::TEXT);
					value = findNode->ToText()->Value();
					
					userCanCreateTasks = _ttoi(value);
				}
			}
		}
	}

	soapServer.GetUserTasks(userId, tempTask);
	if (soapServer.error)
	{
		TRACE("SOAP Error (GetUserTasks): %s\n", soapServer.soap_fault_string());
		return false;
	}

	taskList.Clear();
	for (item = 0; item < tempTask.__size; ++item)
	{
		xmlParser.Clear();
		xmlParser.Parse(tempTask.__ptr[item]);
	
		tempId = -1;
		tempName.Empty();

		for (findElement = xmlParser.FirstChildElement(_T("item")); 
			 findElement != NULL;
			 findElement = findElement->NextSiblingElement(_T("item")))
		{
			findChild = findElement->FirstChildElement(_T("key"));
			if (findChild)
			{
				findNode = findChild->FirstChild();
				ASSERT(findNode->Type() == TiXmlNode::TEXT);
				key = findNode->ToText()->Value();

				findChild = findElement->FirstChildElement(_T("value"));
				if (findChild)
				{
					findNode = findChild->FirstChild();
					ASSERT(findNode->Type() == TiXmlNode::TEXT);

					if (key.Compare(_T("Id")) == 0)
					{
						tempId = _ttoi(findNode->ToText()->Value());
					}
					else if (key.Compare(_T("Name")) == 0)
					{
						tempName = findNode->ToText()->Value();
					}
				}
			}
		}
		if (tempId > 0 && !tempName.IsEmpty())
		{
			taskList.PushBack(tempId, tempName, 1);
		}
	}
	return true;
}

bool CTaskShifterSoapServer::AddTask(LPCTSTR taskName, int &taskId)
{
	soapServer.AddTask(userId, taskName, taskId);
	if (soapServer.error)
	{
		TRACE("SOAP Error (AddTask): %s\n", soapServer.soap_fault_string());
		return false;
	}
	return true;
}

bool CTaskShifterSoapServer::SubmitTimesheet(TimeBlockVector &timesheet, int *timesheetId)
{
	int								tsid;
	CString							xmlToSend;

	xmlToSend = timesheet.GetXml();

	soapServer.SubmitTimesheet(userId, (LPCTSTR)xmlToSend, tsid);
	if (soapServer.error)
	{
		TRACE("SOAP Error (AddTask): %s\n", soapServer.soap_fault_string());
		return false;
	}
	if (timesheetId != NULL)
	{
		*timesheetId = tsid;
	}
	return true;
}
#endif