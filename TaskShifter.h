// TaskShifter.h : main header file for the TASKSHIFTER application
//

#if !defined(AFX_TASKSHIFTER_H__2E837FBD_52F8_4A2F_966B_67AD9F92D73C__INCLUDED_)
#define AFX_TASKSHIFTER_H__2E837FBD_52F8_4A2F_966B_67AD9F92D73C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CTaskShiftManager;

/////////////////////////////////////////////////////////////////////////////
// CTaskShifterApp:
// See TaskShifter.cpp for the implementation of this class
//

class CTaskShifterApp : public CWinApp
{
public:
	CTaskShifterApp();
	virtual ~CTaskShifterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaskShifterApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTaskShifterApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CTaskShiftManager	*m_ptManager;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TASKSHIFTER_H__2E837FBD_52F8_4A2F_966B_67AD9F92D73C__INCLUDED_)
