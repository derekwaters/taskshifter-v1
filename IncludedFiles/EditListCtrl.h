#ifndef EDITLISTCTRL_H
#define	EDITLISTCTRL_H


class CEditListEditCtrl : public CEdit
{
public:
	CEditListEditCtrl(int aiRow, int aiColumn);

protected:
	DECLARE_MESSAGE_MAP()

protected:
	int				m_iRow;
	int				m_iColumn;
};



class CEditListCtrl : public CListCtrl
{
public:
	CEditListCtrl();
	virtual ~CEditListCtrl();

	virtual CEditListEditCtrl	*CreateEditControl(int aiRow, int aiColumn) { return new CEditListEditCtrl(aiRow, aiColumn); }

	
	CEdit				*EditSubItem(int aiRow, int aiColumn);

protected:
	void				OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void				OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void				OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	void				OnLButtonDown(UINT nFlags, CPoint aptPoint);

	DECLARE_MESSAGE_MAP()
};

#endif