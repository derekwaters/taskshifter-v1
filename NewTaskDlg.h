#if !defined(AFX_NEWTASKDLG_H__22E4F590_6F5D_47E5_A747_F7141ED6245E__INCLUDED_)
#define AFX_NEWTASKDLG_H__22E4F590_6F5D_47E5_A747_F7141ED6245E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewTaskDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewTaskDlg dialog

class CNewTaskDlg : public CDialog
{
// Construction
public:
	CNewTaskDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewTaskDlg)
	enum { IDD = IDD_NEW_TASK_DLG };
	CEdit	m_ctrlNewTaskEdit;
	CButton	m_ctrlAnimateIconTbtn;
	//}}AFX_DATA

	CString	GetNewTaskName() { return m_cstrNewTaskName; }
	int		GetNewTaskIconOffset() { return m_iNewTaskIconOffset; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewTaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	m_cstrNewTaskName;
	int		m_iNewTaskIconOffset;

	// Generated message map functions
	//{{AFX_MSG(CNewTaskDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWTASKDLG_H__22E4F590_6F5D_47E5_A747_F7141ED6245E__INCLUDED_)
