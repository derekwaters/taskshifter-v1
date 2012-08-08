#include "stdafx.h"
#include "EditListCtrl.h"

#define	IDC_EDIT_CELL			1000



CEditListEditCtrl::CEditListEditCtrl(int aiRow, int aiColumn)
{
}

protected:
	DECLARE_MESSAGE_MAP()

protected:
	int				m_iRow;
	int				m_iColumn;
};





CEditListCtrl::CEditListCtrl()
{
}

CEditListCtrl::~CEditListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CEdit *CEditListCtrl::EditSubItem(int aiRow, int aiColumn)
{
	CEdit			*vpRet = NULL;
	CRect			vrSubItem;
	CRect			vrClient;
	int				viColOffset = 0;
	int				viCol;
	CSize			vszScroll;
	DWORD			vdwStyle = 0;
	LV_COLUMN		vtColumn = { 0 };

	if (EnsureVisible(aiRow, TRUE))
	{
		if (aiColumn >= 0 && aiColumn < GetHeaderCtrl()->GetItemCount())
		{
			GetClientRect(vrClient);
			GetSubItemRect(aiRow, aiColumn, LVIR_BOUNDS, vrSubItem);

			for (viCol = 0; viCol < aiColumn; ++viCol)
			{
				viColOffset += GetColumnWidth(viCol);
			}

			if (vrSubItem.left > vrClient.right)
			{
				vszScroll.cy = 0;
				vszScroll.cx = vrClient.right - vrSubItem.left - vrSubItem.Width();
				Scroll(vszScroll);
			}

			vtColumn.mask = LVCF_FMT;
			GetColumn(aiColumn, &vtColumn);

			if (vtColumn.fmt & LVCFMT_LEFT) 
			{
				vdwStyle |= ES_LEFT;
			}
			else if (vtColumn.fmt & LVCFMT_RIGHT)
			{
				vdwStyle |= ES_RIGHT;
			}
			else
			{
				vdwStyle |= ES_CENTER;
			}

			vdwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;

			vpRet = CreateEditControl(aiRow, aiColumn);
			vpRet->Create(vdwStyle, vrSubItem, this, IDC_EDIT_CELL);
			vpRet->SetWindowText(GetItemText(aiRow, aiColumn));
			vpRet->SetSel(0, -1);
		}
	}

    return vpRet;
}

void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this)
		SetFocus();

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this)
		SetFocus();

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CEditListCtrl::OnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LV_DISPINFO		*vpDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM			*vpItem = &vpDispInfo->item;

    if (vpItem->pszText != NULL)
    {
		SetItemText(vpItem->iItem, vpItem->iSubItem, vpItem->pszText);
    }
    *pResult = FALSE;
}

void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint aptPoint)
{
	int				viIndex;
	LVHITTESTINFO	vtInfo = { 0 };

    CListCtrl::OnLButtonDown(nFlags, aptPoint);

	vtInfo.pt = aptPoint;

	viIndex = SubItemHitTest(&vtInfo);
	if (viIndex != -1 &&
		vtInfo.iSubItem >= 0)
	{
		if (GetStyle() & LVS_EDITLABELS)
		{
			EditSubItem(viIndex, vtInfo.iSubItem);
		}
    }
}
