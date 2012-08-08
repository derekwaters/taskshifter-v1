#include "stdafx.h"
#include "TaskShiftUtils.h"

CString	CTaskShiftUtils::ValidatePath(LPCTSTR apszInputPath)
{
	CString vcstrRet = apszInputPath;
	if (vcstrRet.GetLength() > 0)
	{
		if (vcstrRet.Right(1) != _T('\\') &&
			vcstrRet.Right(1) != _T('/'))
		{
			vcstrRet += _T("\\");
		}
	}
	return vcstrRet;
}

CString CTaskShiftUtils::GetFilePath(LPCTSTR apszFullPath)
{
	CString vcstrRet;
	if (_tcslen(apszFullPath) > 0)
	{
		char szDrive[_MAX_DRIVE];
		char szDir[_MAX_DIR];
		char szFname[_MAX_FNAME];
		char szExt[_MAX_EXT];
		_splitpath_s(apszFullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);
		vcstrRet = ValidatePath(CString(szDrive) + CString(szDir));
	}
	return vcstrRet;
}

CString	CTaskShiftUtils::GetAppPath()
{
	CString vcstrRet;
	char szFullPath[_MAX_PATH];
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFname[_MAX_FNAME];
	char szExt[_MAX_EXT];
	GetModuleFileName(NULL, szFullPath, _MAX_PATH);
	_splitpath(szFullPath, szDrive, szDir, szFname, szExt);
	vcstrRet = szDrive;
	vcstrRet = vcstrRet + szDir;
	return vcstrRet;
}

CString	CTaskShiftUtils::GetCurrentPath()
{
	CString vstrRet;

	GetCurrentDirectory(_MAX_PATH, vstrRet.GetBuffer(_MAX_PATH));
	vstrRet.ReleaseBuffer();
	return CTaskShiftUtils::ValidatePath(vstrRet);
}

CString CTaskShiftUtils::BrowseForFile(CWnd* apParentWnd, bool abOpenFileDialog, LPCTSTR aszFileTypeExt, LPCTSTR aszFileTypeName, LPCTSTR aszInitialDir, LPCTSTR aszName, bool abIncludeAllFilesInFilter/*=true*/)
{
	DWORD	vdwFlags;
	CString	vcstrTemp;
	CString	vcstrFilter;
	CString	vcstrDefExt;
	CString vcstrFileName;

	if (aszFileTypeExt != NULL && aszFileTypeName != NULL)
	{
		if (_tcslen(aszFileTypeExt) > 0 && _tcslen(aszFileTypeName) > 0)
		{
			if (aszFileTypeExt[0] == _T('.'))
			{
				vcstrTemp.Format(_T("%s (*%s)|*%s|"), aszFileTypeName, aszFileTypeExt, aszFileTypeExt);
				vcstrDefExt.Format(_T("%s"), aszFileTypeExt);
			}
			else
			{
				vcstrTemp.Format(_T("%s (*.%s)|*.%s|"), aszFileTypeName, aszFileTypeExt, aszFileTypeExt);
				vcstrDefExt.Format(_T(".%s"), aszFileTypeExt);
			}
			vcstrFilter += vcstrTemp;
		}
		else
		{
			abIncludeAllFilesInFilter = true;
		}
	}
	else
	{
		abIncludeAllFilesInFilter = true;
	}

	if(abIncludeAllFilesInFilter)
	{
		vcstrTemp = _T("All Files (*.*)|*.*|");
		vcstrFilter += vcstrTemp;
		vcstrFilter += _T("|");
	}

	if (abOpenFileDialog)
	{
		vdwFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER;
	}
	else
	{
		vdwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_ENABLESIZING | OFN_EXPLORER;
	}
	CFileDialog vDlg(abOpenFileDialog, vcstrDefExt, aszName, vdwFlags, vcstrFilter, apParentWnd);
	if (aszInitialDir != NULL && _tcslen(aszInitialDir) > 0)
	{
		vDlg.m_ofn.lpstrInitialDir = aszInitialDir;
	}
	if (vDlg.DoModal() == IDOK)
	{
		vcstrFileName = vDlg.GetPathName();
	}
	return vcstrFileName;
}

INT CALLBACK BrowseForFolderStartFunction(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData) 
{
	_TCHAR	vszDir[MAX_PATH];
	CString	vstrPath;
	
	switch(uMsg) 
	{
	case BFFM_INITIALIZED:
		{
			if (pData == NULL)
			{
				vstrPath = CTaskShiftUtils::GetCurrentPath();
			}
			else
			{
				vstrPath = (LPCTSTR)pData;
			}
			// WParam is TRUE since you are passing a path.
			// It would be FALSE if you were passing a pidl.
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)vstrPath);
		}
		break;
	case BFFM_SELCHANGED: 
		{
			// Set the status window to the currently selected path.
			if (SHGetPathFromIDList((LPITEMIDLIST)lp, vszDir))
			{
				SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)vszDir);
			}
		}
		break;
	}
	return 0;
}

CString CTaskShiftUtils::BrowseForFolder(CWnd* apParentWnd, LPCTSTR apszTitle, LPCTSTR apszInitialPath, bool abNewStyle, bool abEditBox)
{
	CString			vstrRet;
	LPITEMIDLIST	vpidlRet = NULL;
	LPMALLOC		vpMalloc = NULL;
	BROWSEINFO		bi = {0};

	if (apParentWnd != NULL)
	{
		if (IsWindow(apParentWnd->GetSafeHwnd()))
		{
			SHGetMalloc(&vpMalloc);
			bi.hwndOwner = apParentWnd->GetSafeHwnd();
			bi.lpszTitle = apszTitle;
			bi.ulFlags = BIF_RETURNONLYFSDIRS;
			if (abNewStyle)
			{
				bi.ulFlags |= BIF_NEWDIALOGSTYLE;
			}
			if (abEditBox)
			{
				bi.ulFlags |= BIF_EDITBOX;
			}
			bi.lParam = (LPARAM)apszInitialPath;
			bi.lpfn = BrowseForFolderStartFunction;
			vpidlRet = SHBrowseForFolder(&bi);
			if (vpidlRet)
			{
				SHGetPathFromIDList(vpidlRet, vstrRet.GetBuffer(MAX_PATH + 1));
				vstrRet.ReleaseBuffer();
				vpMalloc->Free(vpidlRet);
				if (!vstrRet.IsEmpty())
				{
					if (vstrRet.Right(1).Compare(_T("\\")) != 0)
					{
						vstrRet += _T("\\");
					}
				}
			}
		}
	}
	return vstrRet;
}

int CTaskShiftUtils::SplitStringIntoArray(CStringArray &aarrStringArray, LPCTSTR apszInput, LPCTSTR apszDelimiter)
{
	int		viRet = 0;
	_TCHAR	*vpszToken;

	aarrStringArray.RemoveAll();

	vpszToken = NULL;
	vpszToken = _tcstok_s((_TCHAR *)apszInput, apszDelimiter, &vpszToken);
	while (vpszToken != NULL)
	{
		viRet++;
		aarrStringArray.Add(vpszToken);
		vpszToken = _tcstok_s(NULL, apszDelimiter, &vpszToken);
	}

	return viRet;
}

CString CTaskShiftUtils::GetComputerName()
{
	_TCHAR	compName[_MAX_PATH];
	DWORD	length = _MAX_PATH;
	CString	retVal;

	if (::GetComputerName(compName, &length))
	{
		retVal = compName;
	}
	return retVal;
}

CString CTaskShiftUtils::GetUserName()
{
	_TCHAR	userName[_MAX_PATH];
	DWORD	length = _MAX_PATH;
	CString	retVal;

	if (::GetUserName(userName, &length))
	{
		retVal = userName;
	}
	return retVal;
}
