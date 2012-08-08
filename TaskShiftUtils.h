#ifndef TASKSHIFTUTILS_H
#define	TASKSHIFTUTILS_H

class CTaskShiftUtils
{
public:
	static	CString	ValidatePath(LPCTSTR apszInputPath);
	static	CString	GetFilePath(LPCTSTR apszFullPath);
	static	CString	GetAppPath();
	static	CString	GetCurrentPath();
	static	CString	BrowseForFile(CWnd* apParentWnd, bool abOpenFileDialog, LPCTSTR aszFileTypeExt = NULL, LPCTSTR aszFileTypeName = NULL, LPCTSTR aszInitialDir = NULL, LPCTSTR aszName = NULL, bool abIncludeAllFilesInFilter = true);
	static	CString	BrowseForFolder(CWnd* apParentWnd, LPCTSTR apszTitle, LPCTSTR apszInitialPath, bool abNewStyle, bool abEditBox);
	static	int		SplitStringIntoArray(CStringArray &aarrResults, LPCTSTR apszInput, LPCTSTR apszDelimiter);
	static	CString	GetComputerName();
	static	CString	GetUserName();
};

#endif