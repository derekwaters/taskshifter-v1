# Microsoft Developer Studio Generated NMAKE File, Based on CheckInput.dsp
!IF "$(CFG)" == ""
CFG=CheckInput - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CheckInput - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CheckInput - Win32 Debug" && "$(CFG)" != "CheckInput - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CheckInput.mak" CFG="CheckInput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CheckInput - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CheckInput - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "CheckInput - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll"


CLEAN :
	-@erase "$(INTDIR)\CheckInput.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\tschinp.exp"
	-@erase "$(OUTDIR)\tschinp.lib"
	-@erase "$(OUTDIR)\tschinp.pdb"
	-@erase "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll"
	-@erase "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CheckInput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\tschinp.pdb" /debug /machine:I386 /def:".\CheckInput.def" /out:"\Program Files\Frisbeeworld\TaskShifter\tschinp.dll" /implib:"$(OUTDIR)\tschinp.lib" 
DEF_FILE= \
	".\CheckInput.def"
LINK32_OBJS= \
	"$(INTDIR)\CheckInput.obj"

"..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CheckInput - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll"


CLEAN :
	-@erase "$(INTDIR)\CheckInput.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tschinp.exp"
	-@erase "$(OUTDIR)\tschinp.lib"
	-@erase "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll"
	-@erase "..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CheckInput.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\tschinp.pdb" /machine:I386 /def:".\CheckInput.def" /out:"\Program Files\Frisbeeworld\TaskShifter\tschinp.dll" /implib:"$(OUTDIR)\tschinp.lib" 
DEF_FILE= \
	".\CheckInput.def"
LINK32_OBJS= \
	"$(INTDIR)\CheckInput.obj"

"..\..\..\..\Program Files\Frisbeeworld\TaskShifter\tschinp.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("CheckInput.dep")
!INCLUDE "CheckInput.dep"
!ELSE 
!MESSAGE Warning: cannot find "CheckInput.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "CheckInput - Win32 Debug" || "$(CFG)" == "CheckInput - Win32 Release"
SOURCE=.\CheckInput.cpp

"$(INTDIR)\CheckInput.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

