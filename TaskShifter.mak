# Microsoft Developer Studio Generated NMAKE File, Based on TaskShifter.dsp
!IF "$(CFG)" == ""
CFG=TaskShifter - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TaskShifter - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TaskShifter - Win32 Release" && "$(CFG)" != "TaskShifter - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaskShifter.mak" CFG="TaskShifter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaskShifter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TaskShifter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaskShifter - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\Program Files\FrisbeeWorld\TaskShifter\TaskShifter.exe"


CLEAN :
	-@erase "$(INTDIR)\excel8.obj"
	-@erase "$(INTDIR)\LJDate.obj"
	-@erase "$(INTDIR)\NewTaskDlg.obj"
	-@erase "$(INTDIR)\OrderedListCtrl.obj"
	-@erase "$(INTDIR)\ShellIcon.obj"
	-@erase "$(INTDIR)\SimpleSendMail.obj"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TaskListDlg.obj"
	-@erase "$(INTDIR)\TaskShiftDefs.obj"
	-@erase "$(INTDIR)\TaskShifter.obj"
	-@erase "$(INTDIR)\TaskShifter.pch"
	-@erase "$(INTDIR)\TaskShifter.res"
	-@erase "$(INTDIR)\TaskShiftExportOptionsDlg.obj"
	-@erase "$(INTDIR)\TaskShiftIcon.obj"
	-@erase "$(INTDIR)\TaskShiftManager.obj"
	-@erase "$(INTDIR)\TaskShiftOptionsDlg.obj"
	-@erase "$(INTDIR)\TaskShiftOutput.obj"
	-@erase "$(INTDIR)\taskshiftschedulewnd.obj"
	-@erase "$(INTDIR)\TaskShiftUserInput.obj"
	-@erase "$(INTDIR)\TaskShiftUtils.obj"
	-@erase "$(INTDIR)\TimeSheetDlg.obj"
	-@erase "$(INTDIR)\tinyxml.obj"
	-@erase "$(INTDIR)\tinyxmlerror.obj"
	-@erase "$(INTDIR)\tinyxmlparser.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TaskShifter.pdb"
	-@erase "..\Program Files\FrisbeeWorld\TaskShifter\TaskShifter.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /Zi /O2 /I "\lj\common\dundas\utb" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TIXML_USE_STL" /Fp"$(INTDIR)\TaskShifter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\TaskShifter.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TaskShifter.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TaskShifter.pdb" /debug /machine:I386 /out:"\Program Files\FrisbeeWorld\TaskShifter\TaskShifter.exe" /libpath:".\CheckInput\Release\\" 
LINK32_OBJS= \
	"$(INTDIR)\NewTaskDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TaskListDlg.obj" \
	"$(INTDIR)\TaskShiftDefs.obj" \
	"$(INTDIR)\TaskShifter.obj" \
	"$(INTDIR)\TaskShiftExportOptionsDlg.obj" \
	"$(INTDIR)\TaskShiftIcon.obj" \
	"$(INTDIR)\TaskShiftManager.obj" \
	"$(INTDIR)\TaskShiftOptionsDlg.obj" \
	"$(INTDIR)\TaskShiftOutput.obj" \
	"$(INTDIR)\taskshiftschedulewnd.obj" \
	"$(INTDIR)\TaskShiftUserInput.obj" \
	"$(INTDIR)\TaskShiftUtils.obj" \
	"$(INTDIR)\TimeSheetDlg.obj" \
	"$(INTDIR)\tinyxml.obj" \
	"$(INTDIR)\tinyxmlerror.obj" \
	"$(INTDIR)\tinyxmlparser.obj" \
	"$(INTDIR)\LJDate.obj" \
	"$(INTDIR)\OrderedListCtrl.obj" \
	"$(INTDIR)\ShellIcon.obj" \
	"$(INTDIR)\SimpleSendMail.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\TaskShifter.res" \
	"$(INTDIR)\excel8.obj"

"..\Program Files\FrisbeeWorld\TaskShifter\TaskShifter.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TaskShifter.exe" "$(OUTDIR)\TaskShifter.bsc"


CLEAN :
	-@erase "$(INTDIR)\excel8.obj"
	-@erase "$(INTDIR)\excel8.sbr"
	-@erase "$(INTDIR)\LJDate.obj"
	-@erase "$(INTDIR)\LJDate.sbr"
	-@erase "$(INTDIR)\NewTaskDlg.obj"
	-@erase "$(INTDIR)\NewTaskDlg.sbr"
	-@erase "$(INTDIR)\OrderedListCtrl.obj"
	-@erase "$(INTDIR)\OrderedListCtrl.sbr"
	-@erase "$(INTDIR)\ShellIcon.obj"
	-@erase "$(INTDIR)\ShellIcon.sbr"
	-@erase "$(INTDIR)\SimpleSendMail.obj"
	-@erase "$(INTDIR)\SimpleSendMail.sbr"
	-@erase "$(INTDIR)\Smtp.obj"
	-@erase "$(INTDIR)\Smtp.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\TaskListDlg.obj"
	-@erase "$(INTDIR)\TaskListDlg.sbr"
	-@erase "$(INTDIR)\TaskShiftDefs.obj"
	-@erase "$(INTDIR)\TaskShiftDefs.sbr"
	-@erase "$(INTDIR)\TaskShifter.obj"
	-@erase "$(INTDIR)\TaskShifter.pch"
	-@erase "$(INTDIR)\TaskShifter.res"
	-@erase "$(INTDIR)\TaskShifter.sbr"
	-@erase "$(INTDIR)\TaskShiftExportOptionsDlg.obj"
	-@erase "$(INTDIR)\TaskShiftExportOptionsDlg.sbr"
	-@erase "$(INTDIR)\TaskShiftIcon.obj"
	-@erase "$(INTDIR)\TaskShiftIcon.sbr"
	-@erase "$(INTDIR)\TaskShiftManager.obj"
	-@erase "$(INTDIR)\TaskShiftManager.sbr"
	-@erase "$(INTDIR)\TaskShiftOptionsDlg.obj"
	-@erase "$(INTDIR)\TaskShiftOptionsDlg.sbr"
	-@erase "$(INTDIR)\TaskShiftOutput.obj"
	-@erase "$(INTDIR)\TaskShiftOutput.sbr"
	-@erase "$(INTDIR)\taskshiftschedulewnd.obj"
	-@erase "$(INTDIR)\taskshiftschedulewnd.sbr"
	-@erase "$(INTDIR)\TaskShiftUserInput.obj"
	-@erase "$(INTDIR)\TaskShiftUserInput.sbr"
	-@erase "$(INTDIR)\TaskShiftUtils.obj"
	-@erase "$(INTDIR)\TaskShiftUtils.sbr"
	-@erase "$(INTDIR)\TimeSheetDlg.obj"
	-@erase "$(INTDIR)\TimeSheetDlg.sbr"
	-@erase "$(INTDIR)\tinyxml.obj"
	-@erase "$(INTDIR)\tinyxml.sbr"
	-@erase "$(INTDIR)\tinyxmlerror.obj"
	-@erase "$(INTDIR)\tinyxmlerror.sbr"
	-@erase "$(INTDIR)\tinyxmlparser.obj"
	-@erase "$(INTDIR)\tinyxmlparser.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TaskShifter.bsc"
	-@erase "$(OUTDIR)\TaskShifter.exe"
	-@erase "$(OUTDIR)\TaskShifter.ilk"
	-@erase "$(OUTDIR)\TaskShifter.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\lj\common\dundas\utb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TIXML_USE_STL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TaskShifter.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0xc09 /fo"$(INTDIR)\TaskShifter.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TaskShifter.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\NewTaskDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\TaskListDlg.sbr" \
	"$(INTDIR)\TaskShiftDefs.sbr" \
	"$(INTDIR)\TaskShifter.sbr" \
	"$(INTDIR)\TaskShiftExportOptionsDlg.sbr" \
	"$(INTDIR)\TaskShiftIcon.sbr" \
	"$(INTDIR)\TaskShiftManager.sbr" \
	"$(INTDIR)\TaskShiftOptionsDlg.sbr" \
	"$(INTDIR)\TaskShiftOutput.sbr" \
	"$(INTDIR)\taskshiftschedulewnd.sbr" \
	"$(INTDIR)\TaskShiftUserInput.sbr" \
	"$(INTDIR)\TaskShiftUtils.sbr" \
	"$(INTDIR)\TimeSheetDlg.sbr" \
	"$(INTDIR)\tinyxml.sbr" \
	"$(INTDIR)\tinyxmlerror.sbr" \
	"$(INTDIR)\tinyxmlparser.sbr" \
	"$(INTDIR)\LJDate.sbr" \
	"$(INTDIR)\OrderedListCtrl.sbr" \
	"$(INTDIR)\ShellIcon.sbr" \
	"$(INTDIR)\SimpleSendMail.sbr" \
	"$(INTDIR)\Smtp.sbr" \
	"$(INTDIR)\excel8.sbr"

"$(OUTDIR)\TaskShifter.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ws2_32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TaskShifter.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TaskShifter.exe" /pdbtype:sept /libpath:".\CheckInput\Debug\\" 
LINK32_OBJS= \
	"$(INTDIR)\NewTaskDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TaskListDlg.obj" \
	"$(INTDIR)\TaskShiftDefs.obj" \
	"$(INTDIR)\TaskShifter.obj" \
	"$(INTDIR)\TaskShiftExportOptionsDlg.obj" \
	"$(INTDIR)\TaskShiftIcon.obj" \
	"$(INTDIR)\TaskShiftManager.obj" \
	"$(INTDIR)\TaskShiftOptionsDlg.obj" \
	"$(INTDIR)\TaskShiftOutput.obj" \
	"$(INTDIR)\taskshiftschedulewnd.obj" \
	"$(INTDIR)\TaskShiftUserInput.obj" \
	"$(INTDIR)\TaskShiftUtils.obj" \
	"$(INTDIR)\TimeSheetDlg.obj" \
	"$(INTDIR)\tinyxml.obj" \
	"$(INTDIR)\tinyxmlerror.obj" \
	"$(INTDIR)\tinyxmlparser.obj" \
	"$(INTDIR)\LJDate.obj" \
	"$(INTDIR)\OrderedListCtrl.obj" \
	"$(INTDIR)\ShellIcon.obj" \
	"$(INTDIR)\SimpleSendMail.obj" \
	"$(INTDIR)\Smtp.obj" \
	"$(INTDIR)\TaskShifter.res" \
	"$(INTDIR)\excel8.obj"

"$(OUTDIR)\TaskShifter.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TaskShifter.dep")
!INCLUDE "TaskShifter.dep"
!ELSE 
!MESSAGE Warning: cannot find "TaskShifter.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TaskShifter - Win32 Release" || "$(CFG)" == "TaskShifter - Win32 Debug"
SOURCE=.\NewTaskDlg.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\NewTaskDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\NewTaskDlg.obj"	"$(INTDIR)\NewTaskDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /Zi /O2 /I "\lj\common\dundas\utb" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TIXML_USE_STL" /Fp"$(INTDIR)\TaskShifter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TaskShifter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /I "\lj\common\dundas\utb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "TIXML_USE_STL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\TaskShifter.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\TaskShifter.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TaskListDlg.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskListDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskListDlg.obj"	"$(INTDIR)\TaskListDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftDefs.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftDefs.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftDefs.obj"	"$(INTDIR)\TaskShiftDefs.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShifter.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShifter.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShifter.obj"	"$(INTDIR)\TaskShifter.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShifter.rc

"$(INTDIR)\TaskShifter.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TaskShiftExportOptionsDlg.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftExportOptionsDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftExportOptionsDlg.obj"	"$(INTDIR)\TaskShiftExportOptionsDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftIcon.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftIcon.obj"	"$(INTDIR)\TaskShiftIcon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftManager.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftManager.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftManager.obj"	"$(INTDIR)\TaskShiftManager.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftOptionsDlg.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftOptionsDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftOptionsDlg.obj"	"$(INTDIR)\TaskShiftOptionsDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftOutput.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftOutput.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftOutput.obj"	"$(INTDIR)\TaskShiftOutput.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\taskshiftschedulewnd.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\taskshiftschedulewnd.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\taskshiftschedulewnd.obj"	"$(INTDIR)\taskshiftschedulewnd.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftUserInput.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftUserInput.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftUserInput.obj"	"$(INTDIR)\TaskShiftUserInput.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TaskShiftUtils.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TaskShiftUtils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TaskShiftUtils.obj"	"$(INTDIR)\TaskShiftUtils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TimeSheetDlg.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\TimeSheetDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\TimeSheetDlg.obj"	"$(INTDIR)\TimeSheetDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\TinyXml\tinyxml.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\tinyxml.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\tinyxml.obj"	"$(INTDIR)\tinyxml.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\TinyXml\tinyxmlerror.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\tinyxmlerror.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\tinyxmlerror.obj"	"$(INTDIR)\tinyxmlerror.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\TinyXml\tinyxmlparser.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\tinyxmlparser.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\tinyxmlparser.obj"	"$(INTDIR)\tinyxmlparser.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\IncludedFiles\excel8.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\excel8.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\excel8.obj"	"$(INTDIR)\excel8.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\IncludedFiles\LJDate.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\LJDate.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\LJDate.obj"	"$(INTDIR)\LJDate.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\IncludedFiles\OrderedListCtrl.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\OrderedListCtrl.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\OrderedListCtrl.obj"	"$(INTDIR)\OrderedListCtrl.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ShellIcon.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\ShellIcon.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\ShellIcon.obj"	"$(INTDIR)\ShellIcon.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"


!ENDIF 

SOURCE=.\IncludedFiles\SimpleSendMail.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\SimpleSendMail.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\SimpleSendMail.obj"	"$(INTDIR)\SimpleSendMail.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\IncludedFiles\Smtp.cpp

!IF  "$(CFG)" == "TaskShifter - Win32 Release"


"$(INTDIR)\Smtp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "TaskShifter - Win32 Debug"


"$(INTDIR)\Smtp.obj"	"$(INTDIR)\Smtp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TaskShifter.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

