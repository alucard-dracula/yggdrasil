# Microsoft Developer Studio Project File - Name="example" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=example - Win32 DebugSTLPort
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tests_build_sh.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tests_build_sh.mak" CFG="example - Win32 DebugSTLPort"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "example - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "example - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "example - Win32 BoundsChecker" (based on "Win32 (x86) External Target")
!MESSAGE "example - Win32 Profile" (based on "Win32 (x86) External Target")
!MESSAGE "example - Win32 DebugSTLPort" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/DTL34/example", IPABAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "example - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f example.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "example.exe"
# PROP BASE Bsc_Name "example.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "release"
# PROP Intermediate_Dir "release"
# PROP Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b release'"
# PROP Rebuild_Opt "real-clean all"
# PROP Target_File "release\example.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "example - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f example.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "example.exe"
# PROP BASE Bsc_Name "example.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debug"
# PROP Intermediate_Dir "debug"
# PROP Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug'"
# PROP Rebuild_Opt "real-clean all"
# PROP Target_File "debug\example.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "example - Win32 BoundsChecker"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "example___Win32_BoundsChecker"
# PROP BASE Intermediate_Dir "example___Win32_BoundsChecker"
# PROP BASE Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug'"
# PROP BASE Rebuild_Opt "real-clean all"
# PROP BASE Target_File "example.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "example___Win32_BoundsChecker"
# PROP Intermediate_Dir "example___Win32_BoundsChecker"
# PROP Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug-boundschecker'"
# PROP Rebuild_Opt ""
# PROP Target_File "debug-boundschecker\example.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "example - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "example___Win32_Profile"
# PROP BASE Intermediate_Dir "example___Win32_Profile"
# PROP BASE Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug'"
# PROP BASE Rebuild_Opt "real-clean all"
# PROP BASE Target_File "debug\example.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "example___Win32_Profile"
# PROP Intermediate_Dir "example___Win32_Profile"
# PROP Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b profile -r "uid=example;pwd=example;dsn=neo;" '"
# PROP Rebuild_Opt "real-clean all"
# PROP Target_File "debug\example.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "example - Win32 DebugSTLPort"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "example___Win32_DebugSTLPort"
# PROP BASE Intermediate_Dir "example___Win32_DebugSTLPort"
# PROP BASE Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug'"
# PROP BASE Rebuild_Opt "real-clean all"
# PROP BASE Target_File "debug\example.exe"
# PROP BASE Bsc_Name ""
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "example___Win32_DebugSTLPort"
# PROP Intermediate_Dir "example___Win32_DebugSTLPort"
# PROP Cmd_Line "sh -c 'exec env PATH="/usr/bin:$PATH" build.sh -p cygwin-i686-msvc -b debug_stlport'"
# PROP Rebuild_Opt "real-clean all"
# PROP Target_File "debug_stlport\example.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "example - Win32 Release"
# Name "example - Win32 Debug"
# Name "example - Win32 BoundsChecker"
# Name "example - Win32 Profile"
# Name "example - Win32 DebugSTLPort"

!IF  "$(CFG)" == "example - Win32 Release"

!ELSEIF  "$(CFG)" == "example - Win32 Debug"

!ELSEIF  "$(CFG)" == "example - Win32 BoundsChecker"

!ELSEIF  "$(CFG)" == "example - Win32 Profile"

!ELSEIF  "$(CFG)" == "example - Win32 DebugSTLPort"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lib\bind_basics.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\BoundIO.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\clib_fwd.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\CountedPtr.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\date_util.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\DB_Base.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\DBConnection.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\DBException.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\DBStmt.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\DBView.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\dtl_base_types.cpp
# End Source File
# Begin Source File

SOURCE=.\Example.cpp
# End Source File
# Begin Source File

SOURCE=.\IterSemanticsTest.cpp
# End Source File
# Begin Source File

SOURCE=.\JoinExample.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\LocalBCA.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\print_util.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\RootException.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\string_util.cpp
# End Source File
# Begin Source File

SOURCE=.\TestExceptions.cpp
# End Source File
# Begin Source File

SOURCE=.\tests.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\validate.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\variant_row.cpp
# End Source File
# Begin Source File

SOURCE=..\lib\VariantException.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\lib\array_string.h
# End Source File
# Begin Source File

SOURCE=..\lib\bind_basics.h
# End Source File
# Begin Source File

SOURCE=..\lib\BoundIO.h
# End Source File
# Begin Source File

SOURCE=..\lib\Callback.h
# End Source File
# Begin Source File

SOURCE=..\lib\clib_fwd.h
# End Source File
# Begin Source File

SOURCE=..\lib\CountedPtr.h
# End Source File
# Begin Source File

SOURCE=..\lib\date_util.h
# End Source File
# Begin Source File

SOURCE=..\lib\DB_Base.h
# End Source File
# Begin Source File

SOURCE=..\lib\DB_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBConnection.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBDefaults.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBException.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBIndex.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBStmt.h
# End Source File
# Begin Source File

SOURCE=..\lib\DBView.h
# End Source File
# Begin Source File

SOURCE=..\lib\delete_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\DTL.h
# End Source File
# Begin Source File

SOURCE=..\lib\dtl_algo.h
# End Source File
# Begin Source File

SOURCE=..\lib\dtl_base_types.h
# End Source File
# Begin Source File

SOURCE=..\lib\dtl_config.h
# End Source File
# Begin Source File

SOURCE=..\lib\DynaDBView.h
# End Source File
# Begin Source File

SOURCE=.\Example.h
# End Source File
# Begin Source File

SOURCE=..\lib\field_types.h
# End Source File
# Begin Source File

SOURCE=..\lib\IndexedDBView.h
# End Source File
# Begin Source File

SOURCE=..\lib\insert_iterator.h
# End Source File
# Begin Source File

SOURCE=.\IterSemanticsTest.h
# End Source File
# Begin Source File

SOURCE=.\JoinExample.h
# End Source File
# Begin Source File

SOURCE=..\lib\LocalBCA.h
# End Source File
# Begin Source File

SOURCE=..\lib\merant.h
# End Source File
# Begin Source File

SOURCE=..\lib\minimax.h
# End Source File
# Begin Source File

SOURCE=.\print_util.h
# End Source File
# Begin Source File

SOURCE=..\lib\random_select_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\random_select_update_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\RandomDBView.h
# End Source File
# Begin Source File

SOURCE=..\lib\RootException.h
# End Source File
# Begin Source File

SOURCE=..\lib\select_insert_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\select_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\select_update_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\sql_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\starit.h
# End Source File
# Begin Source File

SOURCE=..\lib\std_warn_off.h
# End Source File
# Begin Source File

SOURCE=..\lib\std_warn_on.h
# End Source File
# Begin Source File

SOURCE=..\lib\string_util.h
# End Source File
# Begin Source File

SOURCE=..\lib\table.h
# End Source File
# Begin Source File

SOURCE=.\TestExceptions.h
# End Source File
# Begin Source File

SOURCE=..\lib\unix_tchar.h
# End Source File
# Begin Source File

SOURCE=..\lib\update_iterator.h
# End Source File
# Begin Source File

SOURCE=..\lib\validate.h
# End Source File
# Begin Source File

SOURCE=..\lib\variant_row.h
# End Source File
# Begin Source File

SOURCE=..\lib\VariantException.h
# End Source File
# Begin Source File

SOURCE=..\lib\vec_multiset.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
