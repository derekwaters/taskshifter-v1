#include "stdafx.h"
#include "TaskListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaskListDlg dialog


CTaskListDlg::CTaskListDlg(int &aiCurrentTask, TaskVector &avecTasks, CWnd* pParent /*=NULL*/)
	: CDialog(CTaskListDlg::IDD, pParent), m_vecTasks(avecTasks), m_iCurrentTask(aiCurrentTask)
{
}


void CTaskListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TASKS_LIST, m_ctrlTasks);
	DDX_Control(pDX, IDC_DELETE_TASK_BTN, m_ctrlDeleteTaskBtn);
	DDX_Control(pDX, IDC_NEW_TASK_BTN, m_ctrlNewTaskBtn);
}


BEGIN_MESSAGE_MAP(CTaskListDlg, CDialog)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_TASKS_LIST, OnListEndEdit)
	ON_COMMAND(IDC_DELETE_TASK_BTN, OnDeleteTask)
	ON_COMMAND(IDC_NEW_TASK_BTN, OnNewTask)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CTaskListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	m_ctrlTasks.InsertColumn(0, _T("Label"), LVCFMT_LEFT, 300);
	m_ctrlTasks.SetExtendedStyle(m_ctrlTasks.GetExtendedStyle() | LVS_EX_CHECKBOXES);

	PopulateTasks();

	m_hbmpNew = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NEW_BMP));
	m_hbmpDelete = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DELETE_BMP));
	m_ctrlNewTaskBtn.SetBitmap(m_hbmpNew);
	m_ctrlDeleteTaskBtn.SetBitmap(m_hbmpDelete);

	ResizeControls();

	return FALSE;
}

void CTaskListDlg::PopulateTasks()
{
	TaskVector::iterator	vtIter;
	int						viPos = 0;

	m_ctrlTasks.DeleteAllItems();

	for (vtIter = (m_vecTasks.begin() + 1); vtIter != m_vecTasks.end(); ++vtIter)
	{
		m_ctrlTasks.InsertItem(viPos, (*vtIter).strDescription);
#ifdef TASKSHIFTER_PROFESSIONAL
		m_ctrlTasks.SetItemData(viPos, (*vtIter).id);
#else
		m_ctrlTasks.SetItemData(viPos, (viPos == m_iCurrentTask));
#endif
		m_ctrlTasks.SetCheck(viPos, (*vtIter).iIconOffset != 0);
		viPos++;
	}
}

void CTaskListDlg::OnListEndEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = TRUE;
}

void CTaskListDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CTaskListDlg::OnNewTask()
{
	int	viInserted;
	viInserted = m_ctrlTasks.InsertItem(m_ctrlTasks.GetItemCount(), _T("Unnamed Task"));
	m_ctrlTasks.SetFocus();
	m_ctrlTasks.EditLabel(viInserted);
}

void CTaskListDlg::OnDeleteTask()
{
	POSITION	vposTask;
	int			viSelected;
	
	vposTask = m_ctrlTasks.GetFirstSelectedItemPosition();
	while (vposTask != NULL)
	{
		viSelected = m_ctrlTasks.GetNextSelectedItem(vposTask);
		m_ctrlTasks.DeleteItem(viSelected);
	}
}

void CTaskListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	ResizeControls();
}

void CTaskListDlg::ResizeControls()
{
	CRect	vrOKBtn;
	CRect	vrCancelBtn;
	CRect	vrList;
	CRect	vrClient;
	CRect	vrNewBtn;
	CRect	vrDeleteBtn;

	if (m_ctrlDeleteTaskBtn.GetSafeHwnd() != NULL)
	{
		GetClientRect(vrClient);

		GetDlgItem(IDCANCEL)->GetWindowRect(vrCancelBtn);
		vrCancelBtn -= vrCancelBtn.TopLeft();

		vrCancelBtn.OffsetRect(vrClient.Width() - TS_UI_GAP - vrCancelBtn.Width(), vrClient.Height() - TS_UI_GAP - vrCancelBtn.Height());
		vrOKBtn = vrCancelBtn;
		vrOKBtn.OffsetRect(-vrCancelBtn.Width() - TS_UI_GAP, 0);

		vrDeleteBtn.right = vrCancelBtn.right;
		vrDeleteBtn.left = vrDeleteBtn.right - TS_UI_SMALL_BTN;
		vrDeleteBtn.top = vrClient.top + TS_UI_GAP;
		vrDeleteBtn.bottom = vrDeleteBtn.top + TS_UI_SMALL_BTN;
		vrNewBtn = vrDeleteBtn;
		vrNewBtn.OffsetRect(-vrDeleteBtn.Width(), 0);

		vrList.left = vrClient.left + TS_UI_GAP;
		vrList.right = vrClient.right - TS_UI_GAP;
		vrList.top = vrNewBtn.bottom;
		vrList.bottom = vrCancelBtn.top - TS_UI_GAP;

		GetDlgItem(IDOK)->MoveWindow(vrOKBtn);
		GetDlgItem(IDCANCEL)->MoveWindow(vrCancelBtn);
		m_ctrlNewTaskBtn.MoveWindow(vrNewBtn);
		m_ctrlDeleteTaskBtn.MoveWindow(vrDeleteBtn);
		m_ctrlTasks.MoveWindow(vrList);
	}
}

void CTaskListDlg::OnOK()
{
	m_vecTasks.Clear();
	for (int i = 0; i < m_ctrlTasks.GetItemCount(); ++i)
	{
#ifdef TASKSHIFTER_PROFESSIONAL
		m_vecTasks.PushBack(m_ctrlTasks.GetItemData(i), m_ctrlTasks.GetItemText(i, 0), m_ctrlTasks.GetCheck(i));
		if (m_ctrlTasks.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			m_iCurrentTask = i + 1;
		}
#else
		m_vecTasks.PushBack(m_ctrlTasks.GetItemText(i, 0), m_ctrlTasks.GetCheck(i));
		if (m_ctrlTasks.GetItemData(i) != 0)
		{
			m_iCurrentTask = i + 1;
		}
#endif
	}
	CDialog::OnOK();
}
