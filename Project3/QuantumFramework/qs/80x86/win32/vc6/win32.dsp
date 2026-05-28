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
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Release/qs.lib"

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
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Debug/qs.lib"

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
# ADD CPP /nologo /MT /W4 /Zi /Od /I "." /I "../../../source" /I "../../../../include" /I "../../../../ports/80x86/win32/vc6" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Debug/qep.lib"
# ADD LIB32 /nologo /out:"../../../../ports/80x86/win32/vc6/Spy/qs.lib"

!ENDIF 

# Begin Target

# Name "Win32 - Win32 Release"
# Name "Win32 - Win32 Debug"
# Name "Win32 - Win32 Spy"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\source\qs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_blk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_byte.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_f32.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_f64.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_mem.cpp
# End Source File
# Begin Source File

SOURCE=.\qs_port.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_str.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\include\qs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\qs_pkg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\ports\80x86\win32\vc6\qs_port.h
# End Source File
# End Group
# End Target
# End Project
