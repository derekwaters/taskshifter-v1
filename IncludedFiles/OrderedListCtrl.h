#if !defined(ORDEREDLISTCTRL_H)
#define ORDEREDLISTCTRL_H

class COrderedListCtrl : public CListCtrl
{
public:
	COrderedListCtrl();
	virtual ~COrderedListCtrl();

public:
	void MoveItem(int aiFromPos, int aiToPos);

protected:
	int		m_iCurrentDragPos;
	int		m_iDraggedItem;
	bool	m_bDragging;

protected:
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCustomDrawList(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

#endif
