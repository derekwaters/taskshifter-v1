// NewTaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskShifter.h"
#include "NewTaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewTaskDlg dialog


CNewTaskDlg::CNewTaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewTaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewTaskDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iNewTaskIconOffset = 0;
}


void CNewTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewTaskDlg)
	DDX_Control(pDX, IDC_TASK_NAME_EDIT, m_ctrlNewTaskEdit);
	DDX_Control(pDX, IDC_ANIMATE_ICON, m_ctrlAnimateIconTbtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewTaskDlg, CDialog)
	//{{AFX_MSG_MAP(CNewTaskDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewTaskDlg message handlers

void CNewTaskDlg::OnOK() 
{
	m_ctrlNewTaskEdit.GetWindowText(m_cstrNewTaskName);
	m_iNewTaskIconOffset = m_ctrlAnimateIconTbtn.GetCheck();
	if (m_cstrNewTaskName.IsEmpty())
	{
		AfxMessageBox(_T("You need to enter a task name."));
	}
	else
	{
		CDialog::OnOK();
	}
}

BOOL CNewTaskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrlNewTaskEdit.SetFocus();
	
	return FALSE;
}
