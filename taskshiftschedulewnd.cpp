// taskshiftschedulewnd.cpp : implementation file
//

#include "stdafx.h"
#include "taskshiftschedulewnd.h"
#include "IncludedFiles\FWUtilDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TaskShiftScheduleWnd

CTaskShiftScheduleWnd::CTaskShiftScheduleWnd()
{
	m_pvecTime = NULL;
}

CTaskShiftScheduleWnd::~CTaskShiftScheduleWnd()
{
}


BEGIN_MESSAGE_MAP(CTaskShiftScheduleWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CTaskShiftScheduleWnd::Create(TimeBlockVector *apvecTimeBlocks, TaskVector *apvecTasks, CWnd *pParentWnd, UINT nID)
{
	if (!CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pParentWnd, nID))
		return FALSE;

	m_pvecTime = apvecTimeBlocks;
	m_pvecTasks = apvecTasks;

	return TRUE;
}

void CTaskShiftScheduleWnd::OnPaint()
{
	RECT		vRect;
	PAINTSTRUCT vPS;
	HDC			vhMemDC = NULL;
	HBITMAP		vhBitmap = NULL;
	int			viSaved = 0;
	HBITMAP		vhOldBitmap = NULL;
	CDC			vdcAttach;

	if (::BeginPaint(GetSafeHwnd(), &vPS) != NULL)
	{
		// Get Size
		::GetClientRect(GetSafeHwnd(), &vRect);
		// Create a bitmap
		vhBitmap = ::CreateCompatibleBitmap(vPS.hdc, vRect.right - vRect.left, vRect.bottom - vRect.top);
		// Create a memory DC
		vhMemDC = ::CreateCompatibleDC(vPS.hdc);
		// Save DC state
		viSaved = ::SaveDC(vhMemDC);
		// Select the bitmap into memory DC so that we have a canvas to paint on
		vhOldBitmap = (HBITMAP)::SelectObject(vhMemDC, vhBitmap);
		vdcAttach.Attach(vhMemDC);
		// Draw Window
		DrawScheduleWnd(vdcAttach, vRect.right - vRect.left, vRect.bottom - vRect.top);
		// Blt the changes to the Screen DC
		::BitBlt(vPS.hdc, vRect.left, vRect.top, vRect.right - vRect.left, vRect.bottom - vRect.top, vhMemDC, 0, 0, SRCCOPY);
		// Restore DC state
		::SelectObject(vhMemDC, vhOldBitmap);
		::RestoreDC(vhMemDC, viSaved);
		// Clean up
		::DeleteObject(vhBitmap);
		::DeleteDC(vhMemDC);
	}
	::EndPaint(GetSafeHwnd(), &vPS);
}

bool CTaskShiftScheduleWnd::DrawScheduleWnd(CDC &adc, int aiWidth, int aiHeight)
{
	int							viMinTime;
	int							viMaxTime;
	CRect						vrClient;
	CRect						vrDaysHeader;
	CRect						vrTimeHeader;
	CRect						vrTaskArea;
	CFont						vfontDefault;

	if (m_pvecTime != NULL)
	{
		if (GetEarliestAndLatestTimes(viMinTime, viMaxTime))
		{
			vfontDefault.CreatePointFont(80, _T("MS Sans Serif"), &adc);
			adc.SelectObject(vfontDefault);

			viMinTime = 60 * (int)(viMinTime / 60);
			viMaxTime = 60 * (1 + (int)(viMaxTime / 60));

			vrClient.left = 0;
			vrClient.top = 0;
			vrClient.right = aiWidth;
			vrClient.bottom = aiHeight;

			vrDaysHeader = vrClient;
			vrDaysHeader.bottom = vrDaysHeader.top + 30;
			vrDaysHeader.left = vrDaysHeader.left + vrDaysHeader.Width() / 6;

			vrTimeHeader = vrClient;
			vrTimeHeader.top = vrDaysHeader.bottom;
			vrTimeHeader.right = vrDaysHeader.left;

			vrTaskArea = vrClient;
			vrTaskArea.left = vrTimeHeader.right;
			vrTaskArea.top = vrDaysHeader.bottom;

			m_iDayWidth = vrDaysHeader.Width() / 5;

			DrawBackground(adc, vrClient);
			DrawTimeHeader(adc, viMinTime, viMaxTime, vrTimeHeader);
			DrawDaysHeader(adc, vrDaysHeader);
			DrawTasks(adc, viMinTime, viMaxTime, vrTaskArea);
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// TaskShiftScheduleWnd message handlers

bool CTaskShiftScheduleWnd::GetEarliestAndLatestTimes(int &aiMinTime, int &aiMaxTime)
{
	bool						vbRet = false;
	TimeBlockVector::iterator	vtIter;

	aiMinTime = 99999;
	aiMaxTime = 0;

	if (m_pvecTime != NULL)
	{
		for (vtIter = m_pvecTime->begin(); vtIter != m_pvecTime->end(); ++vtIter)
		{
			vbRet = true;

			if ((*vtIter).iStartTime < aiMinTime)
			{
				aiMinTime = (*vtIter).iStartTime;
			}
			if ((*vtIter).iEndTime > aiMaxTime)
			{
				aiMaxTime = (*vtIter).iEndTime;
			}
		}
	}
	return vbRet;
}

bool CTaskShiftScheduleWnd::DrawBackground(CDC &adc, CRect &arArea)
{
	bool						vbRet = true;
	CPen						vpenBorder(PS_SOLID, 0, RGB(0, 0, 0));
	CBrush						vbrBack(RGB(255, 255, 255));

	adc.SelectObject(&vpenBorder);
	adc.SelectObject(&vbrBack);

	adc.Rectangle(arArea);

	return vbRet;
}

bool CTaskShiftScheduleWnd::DrawTimeHeader(CDC &adc, int aiMinTime, int aiMaxTime, CRect &arArea)
{
	bool		vbRet = true;
	CRect		vrArea = arArea;
	CPen		vpenHeader(PS_SOLID, 0, RGB(0, 0, 0));
	CBrush		vbrBack(RGB(0, 255, 0));
	CRect		vrHour;
	CString		vcstrLabel;
	
	adc.SelectObject(&vpenHeader);
	adc.SelectObject(&vbrBack);
	adc.SetTextColor(RGB(0, 60, 0));
	adc.SetBkMode(TRANSPARENT);

	vrArea.bottom--;
	adc.Rectangle(vrArea);
	vrArea.DeflateRect(1, 1, 1, 1);
	vrHour = vrArea;

	for (int viStartTime = aiMinTime, viCount = 0; viStartTime < aiMaxTime; viStartTime += 60, viCount++)
	{
		vrHour.top = vrArea.top + ((viStartTime - aiMinTime) * vrArea.Height() / (aiMaxTime - aiMinTime));
		vrHour.bottom = vrArea.top + (((viStartTime + 60) - aiMinTime) * vrArea.Height() / (aiMaxTime - aiMinTime)) + 1;
		vcstrLabel.Format(_T("%02i:%02i"), viStartTime / 60, viStartTime % 60);
		adc.FillSolidRect(vrHour, (viCount % 2 == 0) ? RGB(255, 240, 240) : RGB(240, 255, 240));
		adc.DrawText(vcstrLabel, vrHour, DT_TOP | DT_SINGLELINE | DT_CENTER);
	}
	return vbRet;
	
}

bool CTaskShiftScheduleWnd::DrawDaysHeader(CDC &adc, CRect &arArea)
{
	bool		vbRet = true;
	CRect		vrDayHeader;
	CPen		vpenHeader(PS_SOLID, 0, RGB(0, 0, 60));
	CBrush		vbrBack(RGB(0, 0, 255));
	int			viDay;
	CString		vcstrDays(_T("MTWTF"));
	
	adc.SelectObject(&vpenHeader);
	adc.SelectObject(&vbrBack);
	adc.SetTextColor(RGB(0, 0, 60));
	adc.SetBkMode(TRANSPARENT);

	adc.Rectangle(arArea);
	arArea.DeflateRect(1, 1, 1, 1);

	vrDayHeader = arArea;
	vrDayHeader.right = vrDayHeader.left + m_iDayWidth;

	for (viDay = 0; viDay < 5; ++viDay)
	{
		if (viDay == 4)
		{
			vrDayHeader.right = arArea.right;
		}

		adc.FillSolidRect(vrDayHeader, viDay % 2 == 0 ? RGB(255, 240, 255) : RGB(240, 240, 255));
		adc.DrawText(vcstrDays.Mid(viDay, 1), vrDayHeader, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		vrDayHeader.OffsetRect(m_iDayWidth, 0);
	}
	return vbRet;
}

bool CTaskShiftScheduleWnd::DrawTasks(CDC &adc, int aiMinTime, int aiMaxTime, CRect &arArea)
{
	bool						vbRet = true;
	CRect						vrDay;
	CRect						vrTask;
	CPen						vpenHeader(PS_SOLID, 0, RGB(0, 0, 0));
	CBrush						vbrBack(RGB(211, 211, 211));
	CBrush						vbrTask(RGB(240, 211, 215));
	int							viDay;
	TimeBlockVector::iterator	vtTask;
	CFWUtilDate					vdtTemp;
	
	vrDay = arArea;
	vrDay.right = vrDay.left + m_iDayWidth;

	adc.SelectObject(&vpenHeader);
	adc.SelectObject(&vbrBack);
	adc.SetTextColor(RGB(0, 0, 0));
	adc.SetBkMode(TRANSPARENT);

	for (viDay = 0; viDay < 5; ++viDay)
	{
		if (viDay == 4)
		{
			vrDay.right = arArea.right;
		}

		adc.Rectangle(vrDay);

		vrDay.OffsetRect(vrDay.Width() - 1, 0);
	}

	adc.SelectObject(vbrTask);

	for (vtTask = m_pvecTime->begin(); vtTask != m_pvecTime->end(); ++vtTask)
	{
		vdtTemp.SetDate((*vtTask).iDate);

		if (vdtTemp.GetDayOfWeek() >= LJDateDay_Monday && vdtTemp.GetDayOfWeek() <= LJDateDay_Friday)
		{
			vrTask.left = arArea.left + ((vdtTemp.GetDayOfWeek() - LJDateDay_Monday) * m_iDayWidth);
			vrTask.right = vrTask.left + m_iDayWidth;

			vrTask.top = arArea.top + (((*vtTask).iStartTime - aiMinTime) * arArea.Height() / (aiMaxTime - aiMinTime));
			vrTask.bottom = arArea.top + (((*vtTask).iEndTime - aiMinTime) * arArea.Height() / (aiMaxTime - aiMinTime)) + 1;

			adc.Rectangle(vrTask);
#ifdef TASKSHIFTER_PROFESSIONAL
			adc.DrawText(m_pvecTasks->at(m_pvecTasks->FindTask((*vtTask).taskId)).strDescription, vrTask, DT_TOP | DT_LEFT | DT_WORDBREAK);
#else
			adc.DrawText((*vtTask).strDescription, vrTask, DT_TOP | DT_LEFT | DT_WORDBREAK);
#endif
		}
	}


	return vbRet;
}
