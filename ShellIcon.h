#ifndef __SHELLICON_H__
#define __SHELLICON_H__


#define WM_NOTIFY_SHELL_ICON	(WM_APP + 0x03a)

/////////////////////////////////////////////////////////////////////////////
// COXTaskbarIcon window

class CShellIcon : public CWnd
{
	DECLARE_DYNAMIC(CShellIcon);
public:
	CShellIcon();
	virtual ~CShellIcon();

	virtual bool	Create(UINT nIdentifier = 0);
	
	void	SetTooltipText(LPCTSTR apszText, bool abRefresh = true);
	void	SetIcon(HICON ahIcon, bool abRefresh = true);
	void	SetIcon(UINT auiResourceId, bool abRefresh = true);
	void	ShowBalloon(LPCTSTR apszInfoMessage, 
						UINT auiTimeout, 
						LPCTSTR apszTitle, 
						DWORD adwFlags);
	
	bool	Show();
	bool	Hide();
	bool	Refresh();

protected:
	bool	ShellNotify(DWORD adwMessage);

	afx_msg LRESULT OnShellIconNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	NOTIFYICONDATA	m_tData;
	UINT			m_nIdentifier;
};

#endif
