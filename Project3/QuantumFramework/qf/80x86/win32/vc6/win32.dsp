# Microsoft Developer Studio Project File - Name="Win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Win32 - Win32 Spy
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "win32.mak" CFG="Win32 - Win32 Spy"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Win32 - Win32 Spy" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release"
# PROP Intermediate_Dir "release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W4 /O2 /Ob2 /I "." /I "../../../source" /I "../../../../include" /I "../../../../ports/80x86/win32/vc6" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Release/qf.lib"

!ELSEIF  "$(CFG)" == "Win32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W4 /Zi /Od /I "." /I "../../../source" /I "../../../../include" /I "../../../../ports/80x86/win32/vc6" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Debug/qf.lib"

!ELSEIF  "$(CFG)" == "Win32 - Win32 Spy"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Win32___Win32_Spy"
# PROP BASE Intermediate_Dir "Win32___Win32_Spy"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Spy"
# PROP Intermediate_Dir "Spy"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /Zi /Od /I "." /I "../../../source" /I "../../../../include" /I "../../../../ports/80x86/win32/vc6" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MT /W4 /Zi /Od /I "." /I "../../../source" /I "../../../../include" /I "../../../../ports/80x86/win32/vc6" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "Q_SPY" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Debug/qep.lib"
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Spy/qf.lib"

!ENDIF 

# Begin Target

# Name "Win32 - Win32 Release"
# Name "Win32 - Win32 Debug"
# Name "Win32 - Win32 Spy"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\qa_fifo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_fifo_.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_get_.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_lifo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_lifo_.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_run.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_sub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_usub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qa_usuba.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qeq_fifo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qeq_get.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qeq_init.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qeq_lifo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_act.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_gc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_log2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_new.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_pool.cpp
# End Source File
# Begin Source File

SOURCE=.\qf_port.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_psini.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_pspub.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_pwr2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_tick.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_time.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qmp_get.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qmp_init.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qmp_put.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qte_arm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qte_ctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qte_darm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qte_rarm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\qassert.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\qep.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ports\80x86\win32\vc6\qep_port.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\qequeue.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\qf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qf_pkg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ports\80x86\win32\vc6\qf_port.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\qmpool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\include\qs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ports\80x86\win32\vc6\qs_port.h
# End Source File
# End Group
# End Target
# End Project
