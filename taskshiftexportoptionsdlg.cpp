#include "stdafx.h"
#include "taskshiftexportoptionsdlg.h"
#include "TaskShiftOutput.h"
#include "TaskShiftUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_COLUMNS_ADD_NEW		17004
#define	ID_COLUMNS_DELETE		17005
#define	ID_COLUMNS_MOVE_DOWN	17006
#define	ID_COLUMNS_MOVE_UP		17007
#define	ID_COLUMN_TYPE_FIRST	17050
#define	ID_COLUMN_TYPE_LAST		17099

CTaskShiftExportOptionsDlg::CTaskShiftExportOptionsDlg(CTaskShiftOutput *aptOutput, CWnd* pParent)
	: m_ptOutput(aptOutput), CDialog(CTaskShiftExportOptionsDlg::IDD, pParent)
{
}


void CTaskShiftExportOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPORT_OPEN_AFTER, m_ctrlOpenAfterExportTbtn);
	DDX_Control(pDX, IDC_EXPORT_FOLDER_EDIT, m_ctrlExportFolderEdit);
	DDX_Control(pDX, IDC_EXPORT_FILENAME_FORMAT_EDIT, m_ctrlFilenameFormatEdit);
	DDX_Control(pDX, IDC_EXPORT_FOLDER_BTN, m_ctrlExportFolderBtn);
	DDX_Control(pDX, IDC_EXPORT_EMAIL_TO_EDIT, m_ctrlEmailToEdit);
	DDX_Control(pDX, IDC_EXPORT_EMAIL_SUBJECT_EDIT, m_ctrlEmailSubjectEdit);
	DDX_Control(pDX, IDC_EXPORT_EMAIL_SERVER_EDIT, m_ctrlEmailServerEdit);
	DDX_Control(pDX, IDC_EXPORT_EMAIL_FROM_EDIT, m_ctrlEmailFromEdit);
	DDX_Control(pDX, IDC_EXPORT_EMAIL_BODY_EDIT, m_ctrlEmailBodyEdit);
	DDX_Control(pDX, IDC_EXPORT_COLUMNS_LIST, m_ctrlColumnsList);
	DDX_Control(pDX, IDC_EXPORT_ATTACH_EMAIL, m_ctrlAttachEmailTbtn);
	DDX_Control(pDX, IDC_EXPORT_AS_CSV_TBTN, m_ctrlExportCSVTbtn);
	DDX_Control(pDX, IDC_EXPORT_AS_EXCEL_TBTN, m_ctrlExportExcelTbtn);
	DDX_Control(pDX, IDC_EXPORT_USE_TEMPLATE_TBTN, m_ctrlUseTemplateFileTbtn);
	DDX_Control(pDX, IDC_EXPORT_TEMPLATE_FILE_EDIT, m_ctrlTemplateFileEdit);
	DDX_Control(pDX, IDC_EXPORT_TEMPLATE_FILE_BTN, m_ctrlTemplateFileBtn);
}


BEGIN_MESSAGE_MAP(CTaskShiftExportOptionsDlg, CDialog)
	ON_BN_CLICKED(IDC_EXPORT_FOLDER_BTN, OnExportFolderBtn)
	ON_BN_CLICKED(IDC_EXPORT_ATTACH_EMAIL, OnExportAttachEmail)
	ON_BN_CLICKED(IDC_EXPORT_USE_TEMPLATE_TBTN, OnUseTemplateTbtn)
	ON_BN_CLICKED(IDC_EXPORT_TEMPLATE_FILE_BTN, OnTemplateFileBtn)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COLUMNS_ADD_NEW, OnColumnsAddNew)
	ON_COMMAND(ID_COLUMNS_DELETE, OnColumnsDelete)
	ON_COMMAND_RANGE(ID_COLUMN_TYPE_FIRST, ID_COLUMN_TYPE_LAST, OnColumnTypeRange)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_EXPORT_COLUMNS_LIST, OnEndColumnLabelEdit)
END_MESSAGE_MAP()

BOOL CTaskShiftExportOptionsDlg::OnInitDialog()
{
	int								viInsertPos;
	TSOutputColumnVector::iterator	vtIter;
	CString							vcstrTemp;

	CDialog::OnInitDialog();

	m_ctrlOpenAfterExportTbtn.SetCheck(m_ptOutput->GetOpenAfterOutput() ? 1 : 0);
	m_ctrlExportFolderEdit.SetWindowText(m_ptOutput->GetOutputFolder());
	m_ctrlFilenameFormatEdit.SetWindowText(m_ptOutput->GetOutputFilenameFormat());
	m_ctrlEmailToEdit.SetWindowText(m_ptOutput->GetEmailTo());
	m_ctrlEmailSubjectEdit.SetWindowText(m_ptOutput->GetEmailSubject());
	m_ctrlEmailServerEdit.SetWindowText(m_ptOutput->GetEmailSMTPServer());
	m_ctrlEmailFromEdit.SetWindowText(m_ptOutput->GetEmailFrom());
	m_ctrlEmailBodyEdit.SetWindowText(m_ptOutput->GetEmailBody());
	// COrderedListCtrl	m_ctrlColumnsList;
	m_ctrlAttachEmailTbtn.SetCheck(m_ptOutput->GetAttachToEmail() ? 1 : 0);

	if (m_ptOutput->GetOutputType() == OutputToCSV)
	{
		m_ctrlExportCSVTbtn.SetCheck(1);
	}
	else
	{
		m_ctrlExportExcelTbtn.SetCheck(1);
	}

	vcstrTemp = m_ptOutput->GetTemplateFile();
	m_ctrlUseTemplateFileTbtn.SetCheck(!vcstrTemp.IsEmpty() && GetFileAttributes(vcstrTemp) != -1);
	m_ctrlTemplateFileEdit.SetWindowText(vcstrTemp);

	m_ctrlColumnsList.InsertColumn(0, _T("Column Label"));
	m_ctrlColumnsList.InsertColumn(1, _T("Column Type"));
	for (vtIter = m_ptOutput->GetOutputColumns().begin(); vtIter < m_ptOutput->GetOutputColumns().end(); ++vtIter)
	{
		viInsertPos = m_ctrlColumnsList.InsertItem(m_ctrlColumnsList.GetItemCount(), (*vtIter).strLabel);
		m_ctrlColumnsList.SetItemData(viInsertPos, (*vtIter).eItem);
		m_ctrlColumnsList.SetItemText(viInsertPos, 1, CTaskShiftOutput::m_acstrOutputItemLabel[(*vtIter).eItem]);
	}
	m_ctrlColumnsList.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_ctrlColumnsList.SetColumnWidth(1, LVSCW_AUTOSIZE);

	OnExportAttachEmail();
	OnUseTemplateTbtn();
		
	return TRUE;
}

void CTaskShiftExportOptionsDlg::OnUseTemplateTbtn()
{
	BOOL	vbEnable = m_ctrlUseTemplateFileTbtn.GetCheck() != 0;

	m_ctrlTemplateFileEdit.EnableWindow(vbEnable);
	m_ctrlTemplateFileBtn.EnableWindow(vbEnable);
}

void CTaskShiftExportOptionsDlg::OnTemplateFileBtn()
{
	CString	vcstrPrevious;

	m_ctrlTemplateFileEdit.GetWindowText(vcstrPrevious);

	vcstrPrevious = CTaskShiftUtils::BrowseForFile(this, true, _T("csv"), _T("Comma-separated Template Files"), CTaskShiftUtils::GetAppPath(), _T("Select Template File"), true);
	if (!vcstrPrevious.IsEmpty())
	{
		m_ctrlTemplateFileEdit.SetWindowText(vcstrPrevious);
	}
}

void CTaskShiftExportOptionsDlg::OnExportFolderBtn() 
{
	CString	vcstrPrevFolder;

	m_ctrlExportFolderEdit.GetWindowText(vcstrPrevFolder);
	vcstrPrevFolder = CTaskShiftUtils::BrowseForFolder(this, _T("Select Export Folder"), vcstrPrevFolder, true, true);
	if (!vcstrPrevFolder.IsEmpty())
	{
		m_ctrlExportFolderEdit.SetWindowText(vcstrPrevFolder);
	}
}

void CTaskShiftExportOptionsDlg::OnExportAttachEmail() 
{
	BOOL	vbEnable;

	vbEnable = m_ctrlAttachEmailTbtn.GetCheck() != 0;
	m_ctrlEmailToEdit.EnableWindow(vbEnable);
	m_ctrlEmailSubjectEdit.EnableWindow(vbEnable);
	m_ctrlEmailServerEdit.EnableWindow(vbEnable);
	m_ctrlEmailFromEdit.EnableWindow(vbEnable);
	m_ctrlEmailBodyEdit.EnableWindow(vbEnable);
}

void CTaskShiftExportOptionsDlg::OnOK()
{
	CString					vcstrTemp;
	CString					vcstrSubject;
	CString					vcstrServer;
	CString					vcstrFrom;
	CString					vcstrBody;
	int						viItem;
	TSOutputColumnVector	&vvecColumns = m_ptOutput->GetOutputColumns();

	m_ptOutput->SetOpenAfterOutput(m_ctrlOpenAfterExportTbtn.GetCheck() != 0);

	m_ctrlExportFolderEdit.GetWindowText(vcstrTemp);
	m_ptOutput->SetOutputFolder(vcstrTemp);

	m_ctrlFilenameFormatEdit.GetWindowText(vcstrTemp);
	m_ptOutput->SetOutputFilenameFormat(vcstrTemp);

	m_ctrlEmailToEdit.GetWindowText(vcstrTemp);
	m_ctrlEmailSubjectEdit.GetWindowText(vcstrSubject);
	m_ctrlEmailServerEdit.GetWindowText(vcstrServer);
	m_ctrlEmailFromEdit.GetWindowText(vcstrFrom);
	m_ctrlEmailBodyEdit.GetWindowText(vcstrBody);
	
	m_ptOutput->SetEmailOptions(m_ctrlAttachEmailTbtn.GetCheck() != 0,
								vcstrTemp,
								vcstrFrom,
								vcstrServer,
								vcstrSubject,
								vcstrBody);

	if (m_ctrlExportCSVTbtn.GetCheck() != 0)
	{
		m_ptOutput->SetOutputType(OutputToCSV);
	}
	else
	{
		m_ptOutput->SetOutputType(OutputToExcel);
	}

	m_ctrlTemplateFileEdit.GetWindowText(vcstrTemp);
	if (m_ctrlUseTemplateFileTbtn.GetCheck() == 0)
	{
		vcstrTemp.Empty();
	}
	m_ptOutput->SetTemplateFile(vcstrTemp);

	vvecColumns.clear();
	for (viItem = 0; viItem < m_ctrlColumnsList.GetItemCount(); ++viItem)
	{
		vvecColumns.push_back(tTSOutputColumn(m_ctrlColumnsList.GetItemText(viItem, 0), (eTaskShiftOutputItem)m_ctrlColumnsList.GetItemData(viItem)));
	}

	m_ptOutput->SaveOutputFormat();

	CDialog::OnOK();
}

void CTaskShiftExportOptionsDlg::OnContextMenu(CWnd *pWnd, CPoint pos)
{
	if (pWnd->GetSafeHwnd() == m_ctrlColumnsList.GetSafeHwnd())
	{
		CMenu			vmnuAdditionalItems;
		CPoint			vpntTemp(pos);
		LVHITTESTINFO	vtInfo = { 0 };

		vmnuAdditionalItems.CreatePopupMenu();

		m_ctrlColumnsList.ScreenToClient(&vpntTemp);
		vtInfo.pt = vpntTemp;
		m_iContextMenuItem = m_ctrlColumnsList.SubItemHitTest(&vtInfo);

		if (m_iContextMenuItem == -1 || vtInfo.iSubItem == 0)
		{
			vmnuAdditionalItems.AppendMenu(MF_STRING, ID_COLUMNS_ADD_NEW, _T("Add New Column"));
			if (m_iContextMenuItem >= 0)
			{
				vmnuAdditionalItems.AppendMenu(MF_STRING, ID_COLUMNS_DELETE, _T("Delete Column"));
			}
		}
		else
		{
			for (int i = 0; i < OutputItemCount; ++i)
			{
				vmnuAdditionalItems.AppendMenu(MF_STRING, ID_COLUMN_TYPE_FIRST + i, CTaskShiftOutput::m_acstrOutputItemLabel[i]);
			}
		}
		vmnuAdditionalItems.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pos.x, pos.y, this);
	}
	else
	{
		CDialog::OnContextMenu(pWnd, pos);
	}
}

void CTaskShiftExportOptionsDlg::OnColumnsAddNew()
{
	int	viInserted;
	viInserted = m_ctrlColumnsList.InsertItem(m_ctrlColumnsList.GetItemCount(), _T("<New Column>"), 0);
	m_ctrlColumnsList.SetItemData(viInserted, 0);
	m_ctrlColumnsList.SetItemText(viInserted, 1, CTaskShiftOutput::m_acstrOutputItemLabel[0]);
	m_ctrlColumnsList.EditLabel(viInserted);
}

void CTaskShiftExportOptionsDlg::OnColumnsDelete()
{
	m_ctrlColumnsList.DeleteItem(m_iContextMenuItem);
}

void CTaskShiftExportOptionsDlg::OnColumnTypeRange(UINT nID)
{
	int	viType = nID - ID_COLUMN_TYPE_FIRST;
	m_ctrlColumnsList.SetItemData(m_iContextMenuItem, viType);
	m_ctrlColumnsList.SetItemText(m_iContextMenuItem, 1, CTaskShiftOutput::m_acstrOutputItemLabel[viType]);
	m_ctrlColumnsList.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_ctrlColumnsList.SetColumnWidth(1, LVSCW_AUTOSIZE);
}

void CTaskShiftExportOptionsDlg::OnEndColumnLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 1;
	m_ctrlColumnsList.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_ctrlColumnsList.SetColumnWidth(1, LVSCW_AUTOSIZE);
}