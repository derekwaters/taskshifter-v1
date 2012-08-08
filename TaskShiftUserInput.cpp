// TaskShiftUserInput.cpp: implementation of the CTaskShiftUserInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskShifter.h"
#include "TaskShiftUserInput.h"
#include "CheckInput\CheckInput.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
class CTaskShiftUserInputSingleton
{
public:
	CTaskShiftUserInputSingleton()
	{
		CheckInputInit();
	}
	virtual ~CTaskShiftUserInputSingleton()
	{
		CheckInputTerm();
	}
	DWORD	GetLastUserInputTickCount()
	{
		return CheckInputGetLastInputTime();
	}
};

CTaskShiftUserInputSingleton	g_tUserInput;

#endif



CTaskShiftUserInput::CTaskShiftUserInput()
{

}

CTaskShiftUserInput::~CTaskShiftUserInput()
{

}

DWORD CTaskShiftUserInput::GetLastUserInputTickCount()
{
#ifdef _DEBUG
	return GetTickCount();
#else
	return g_tUserInput.GetLastUserInputTickCount();
#endif
}