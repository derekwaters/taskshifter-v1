#include "stdafx.h"
#include "ShellIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CShellIcon, CWnd)


BEGIN_MESSAGE_MAP(CShellIcon, CWnd)
	ON_MESSAGE(WM_NOTIFY_SHELL_ICON, OnShellIconNotify)
END_MESSAGE_MAP()

CShellIcon::CShellIcon()
{
	memset(&m_tData, 0, sizeof(m_tData));
}

CShellIcon::~CShellIcon()
{
	Hide();
}

bool CShellIcon::Create(UINT nIdentifier)
{
	ASSERT(!::IsWindow(m_hWnd));
	m_nIdentifier = nIdentifier;

	return (CWnd::CreateEx(0, AfxRegisterWndClass(CS_CLASSDC | CS_GLOBALCLASS), NULL, 0, 0, 0, 0, 0, NULL, NULL) != FALSE);
}

void CShellIcon::SetTooltipText(LPCTSTR apszText, bool abRefresh)
{
	_tcsncpy_s(m_tData.szTip, sizeof(m_tData.szTip), apszText, _tcslen(apszText));
	if (abRefresh)
	{
		Refresh();
	}
}

void CShellIcon::SetIcon(HICON ahIcon, bool abRefresh)
{
	m_tData.hIcon = ahIcon;
	if (ahIcon == NULL)
		m_tData.uFlags = m_tData.uFlags & (~NIF_ICON);
	else
		m_tData.uFlags = m_tData.uFlags | NIF_ICON;
	if (abRefresh)
	{
		Refresh();
	}
}

void CShellIcon::SetIcon(UINT auiResourceId, bool abRefresh)
{
	HICON	vhTemp = LoadIcon(AfxFindResourceHandle(MAKEINTRESOURCE(auiResourceId), RT_GROUP_ICON), MAKEINTRESOURCE(auiResourceId));
	SetIcon(vhTemp, abRefresh);
}

void CShellIcon::ShowBalloon(LPCTSTR apszInfoMessage, UINT auiTimeout, LPCTSTR apszTitle, DWORD adwFlags)
{
	m_tData.uFlags |= NIF_INFO;
	_tcscpy(m_tData.szInfo, apszInfoMessage);
	_tcscpy(m_tData.szInfoTitle, apszTitle);
	// _tcsncpy_s(m_tData.szInfo, sizeof(m_tData.szInfo), apszInfoMessage, _tcslen(apszInfoMessage));
	// _tcsncpy_s(m_tData.szInfoTitle, sizeof(m_tData.szInfo), apszTitle, _tcslen(apszTitle));
	m_tData.uTimeout = auiTimeout;
	m_tData.dwInfoFlags = adwFlags;
	ShellNotify(NIM_MODIFY);
}

bool CShellIcon::Show()
{
	return ShellNotify(NIM_ADD);
}

bool CShellIcon::Hide()
{
	return ShellNotify(NIM_DELETE);
}

bool CShellIcon::Refresh()
{
	// Ensure that we don't show the balloon help again!
	//
	m_tData.uFlags &= (~NIF_INFO);
	return ShellNotify(NIM_MODIFY);
}

bool CShellIcon::ShellNotify(DWORD adwMessage)
{
	m_tData.cbSize = sizeof(NOTIFYICONDATA);
	m_tData.hWnd = m_hWnd;
	m_tData.uID = m_nIdentifier;
	if (adwMessage != NIM_DELETE)
	{
		m_tData.uFlags |= NIF_MESSAGE | NIF_TIP;
		m_tData.uCallbackMessage = WM_NOTIFY_SHELL_ICON;
	}
    return (Shell_NotifyIcon(adwMessage, &m_tData) != FALSE);
}

LRESULT CShellIcon::OnShellIconNotify(WPARAM wParam, LPARAM lParam)
{
	CWnd	*vpwndOwner = GetOwner();
	if (vpwndOwner != NULL)
	{
		vpwndOwner->PostMessage(WM_NOTIFY_SHELL_ICON, wParam, lParam);
	}
	else
	{
		PostMessage((UINT)lParam);
	}
	return -1;
}


