#include "stdafx.h"
#include "OrderedListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COrderedListCtrl::COrderedListCtrl()
{
	m_bDragging = false;
}

COrderedListCtrl::~COrderedListCtrl()
{
}


BEGIN_MESSAGE_MAP(COrderedListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT (NM_CUSTOMDRAW, OnCustomDrawList)
END_MESSAGE_MAP()

void COrderedListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_iCurrentDragPos = pNMListView->iItem;
	m_iDraggedItem = m_iCurrentDragPos;
	m_bDragging = true;

	// We set the dragged item state to CUT. This is just a way
	// to make the item being dragged visible to the user. This only
	// works if you have an imagelist, though, as the cut state
	// just dims the image.
	//
	SetItemState(m_iDraggedItem, LVIS_CUT, LVIS_CUT);
	SetCapture();
	Invalidate();

	*pResult = 0;
}

void COrderedListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		// We normalise the point so that it lies in the list control somewhere.
		//
		point.x = 10;

		CRect	vrHeader;
		GetHeaderCtrl()->GetClientRect(vrHeader);
		if (point.y < vrHeader.Height())
			point.y = vrHeader.Height();

		// Now find out which item the point is in.
		int	viTest = HitTest(point);

		// If the point does not lie on an item, then we use GetItemCount() as
		// the insert position. This allows us to insert an item at the end of
		// the list.
		//
		if (viTest == -1)
			viTest = GetItemCount();

		if (viTest != m_iCurrentDragPos)
		{
			// Only redraw if the drag position changes. This will prevent a
			// bit of flickering.
			//
			m_iCurrentDragPos = viTest;
			Invalidate();
		}
	}
	CListCtrl::OnMouseMove(nFlags, point);
}

void COrderedListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{
		m_bDragging = false;
		ReleaseCapture();
		// Clear the cut state.
		//
		SetItemState(m_iDraggedItem, 0, LVIS_CUT);
		MoveItem(m_iDraggedItem, m_iCurrentDragPos);
		Invalidate();
	}
	CListCtrl::OnLButtonUp(nFlags, point);
}

void COrderedListCtrl::OnCustomDrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

    *pResult = 0;

	if (m_bDragging)
	{

		// In the custom draw section, we need to handle the ITEMPOSTPAINT
		// customisation.

		if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		{
			*pResult = CDRF_NOTIFYPOSTPAINT;
		}
		else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT)
		{
			CRect	vtRect;
			CPen	vtPen(PS_SOLID, 2, GetSysColor(COLOR_BTNTEXT));
			int		viItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
			CDC		*vpDC;
			int		viDCState;
	
			vpDC = CDC::FromHandle(pLVCD->nmcd.hdc);
			viDCState = vpDC->SaveDC();
			GetItemRect(viItem, vtRect, LVIR_BOUNDS);

			if (viItem == m_iCurrentDragPos)
			{
				vpDC->SelectObject(&vtPen);
				vpDC->MoveTo(vtRect.left, vtRect.top - 3);
				vpDC->LineTo(vtRect.left + 2, vtRect.top);
				vpDC->LineTo(vtRect.left, vtRect.top + 3);
				vpDC->MoveTo(vtRect.left + 2, vtRect.top);
				vpDC->LineTo(vtRect.right - 1, vtRect.top);
				vpDC->LineTo(vtRect.right, vtRect.top - 3);
				vpDC->MoveTo(vtRect.right - 1, vtRect.top);
				vpDC->LineTo(vtRect.right, vtRect.top + 3);
			}
			else if (m_iCurrentDragPos == GetItemCount() && (viItem == GetItemCount() - 1))
			{
				vpDC->SelectObject(&vtPen);
				vpDC->MoveTo(vtRect.left, vtRect.bottom - 3);
				vpDC->LineTo(vtRect.left + 2, vtRect.bottom);
				vpDC->LineTo(vtRect.left, vtRect.bottom + 3);
				vpDC->MoveTo(vtRect.left + 2, vtRect.bottom);
				vpDC->LineTo(vtRect.right - 1, vtRect.bottom);
				vpDC->LineTo(vtRect.right, vtRect.bottom - 3);
				vpDC->MoveTo(vtRect.right - 1, vtRect.bottom);
				vpDC->LineTo(vtRect.right, vtRect.bottom + 3);
			}

			vpDC->RestoreDC(viDCState);
			*pResult = CDRF_SKIPDEFAULT;
		}
	}
}

void COrderedListCtrl::MoveItem(int aiFromPos, int aiToPos)
{
	if (aiToPos > aiFromPos)
	{
		aiToPos--;
	}

	if (aiToPos != aiFromPos)
	{
		LVITEM			vtItemData;
		BOOL			vbChecked;
		int				viInsertedAt;
		CStringArray	varrSubItems;
		int				viSubItem;

		memset(&vtItemData, 0, sizeof(LVITEM));
		vtItemData.mask = LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
		vtItemData.pszText = new _TCHAR[1001];
		memset(vtItemData.pszText, 0, 1001);
		vtItemData.cchTextMax = 1000;
		vtItemData.iItem = aiFromPos;
		vtItemData.stateMask = (UINT)-1;

		// NOTE: We use this because the check state seems to get lost between
		// GetItem/InsertItem.
		vbChecked = GetCheck(aiFromPos);

		GetItem(&vtItemData);

		for (viSubItem = 1; viSubItem < GetHeaderCtrl()->GetItemCount(); ++viSubItem)
		{
			varrSubItems.Add(GetItemText(aiFromPos, viSubItem));
		}
		DeleteItem(aiFromPos);

		vtItemData.iItem = aiToPos;
		viInsertedAt = InsertItem(&vtItemData);
		SetCheck(viInsertedAt, vbChecked);

		for (viSubItem = 1; viSubItem < GetHeaderCtrl()->GetItemCount(); ++viSubItem)
		{
			SetItemText(viInsertedAt, viSubItem, varrSubItems[viSubItem - 1]);
		}

		delete [] vtItemData.pszText;
	}

}