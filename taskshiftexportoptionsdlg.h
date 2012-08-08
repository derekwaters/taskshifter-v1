#if !defined(AFX_TASKSHIFTEXPORTOPTIONSDLG_H__C69CACE7_5799_463E_9CF1_342EAFFAC806__INCLUDED_)
#define AFX_TASKSHIFTEXPORTOPTIONSDLG_H__C69CACE7_5799_463E_9CF1_342EAFFAC806__INCLUDED_

#include "resource.h"
#include "IncludedFiles\OrderedListCtrl.h"

class CTaskShiftOutput;

class CTaskShiftExportOptionsDlg : public CDialog
{
public:
	CTaskShiftExportOptionsDlg(CTaskShiftOutput *aptOutput, CWnd* pParent = NULL);

	enum { IDD = IDD_EXPORT_OPTIONS_DLG };

protected:
	CTaskShiftOutput	*m_ptOutput;
	CButton				m_ctrlOpenAfterExportTbtn;
	CEdit				m_ctrlExportFolderEdit;
	CEdit				m_ctrlFilenameFormatEdit;
	CButton				m_ctrlExportFolderBtn;
	CEdit				m_ctrlEmailToEdit;
	CEdit				m_ctrlEmailSubjectEdit;
	CEdit				m_ctrlEmailServerEdit;
	CEdit				m_ctrlEmailFromEdit;
	CEdit				m_ctrlEmailBodyEdit;
	COrderedListCtrl	m_ctrlColumnsList;
	CButton				m_ctrlAttachEmailTbtn;
	CButton				m_ctrlExportCSVTbtn;
	CButton				m_ctrlExportExcelTbtn;
	CButton				m_ctrlUseTemplateFileTbtn;
	CEdit				m_ctrlTemplateFileEdit;
	CButton				m_ctrlTemplateFileBtn;

	int					m_iContextMenuItem;

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);
	afx_msg void	OnExportFolderBtn();
	afx_msg void	OnExportAttachEmail();
	afx_msg void	OnUseTemplateTbtn();
	afx_msg void	OnTemplateFileBtn();
	afx_msg BOOL	OnInitDialog();
	afx_msg void	OnOK();
	afx_msg void	OnContextMenu(CWnd *pWnd, CPoint pos);
	afx_msg	void	OnColumnsAddNew();
	afx_msg void	OnColumnsDelete();
	afx_msg void	OnColumnTypeRange(UINT nID);
	afx_msg void	OnEndColumnLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

#endif
