// TaskShiftUserInput.h: interface for the CTaskShiftUserInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKSHIFTUSERINPUT_H__933F7D79_B918_4DD3_8B0D_F06D4C8899A5__INCLUDED_)
#define AFX_TASKSHIFTUSERINPUT_H__933F7D79_B918_4DD3_8B0D_F06D4C8899A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaskShiftUserInput  
{
public:
	CTaskShiftUserInput();
	virtual ~CTaskShiftUserInput();

	DWORD	GetLastUserInputTickCount();
};

#endif // !defined(AFX_TASKSHIFTUSERINPUT_H__933F7D79_B918_4DD3_8B0D_F06D4C8899A5__INCLUDED_)
