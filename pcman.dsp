# Microsoft Developer Studio Project File - Name="pcman" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=pcman - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pcman.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pcman.mak" CFG="pcman - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pcman - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "pcman - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pcman - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /O1 /Ob2 /I "code" /I "contrib/oggvorbis/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib vorbisfile_static.lib ogg_static.lib vorbis_static.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib Winmm.lib /nologo /subsystem:windows /machine:I386 /out:"pcman.exe" /libpath:"contrib/oggvorbis/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "pcman - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pcman___Win32_Debug"
# PROP BASE Intermediate_Dir "pcman___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "pcman___Win32_Debug"
# PROP Intermediate_Dir "pcman___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "code" /I "contrib/oggvorbis/include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glu32.lib opengl32.lib vorbisfile_static.lib ogg_static.lib vorbis_static.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"pcman.exe" /pdbtype:sept /libpath:"contrib/oggvorbis/lib"

!ENDIF 

# Begin Target

# Name "pcman - Win32 Release"
# Name "pcman - Win32 Debug"
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Group "containers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\code\lib\darray.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\hash_table.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\key_tree.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\llist.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\sorting_hash_table.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\code\lib\binary_stream_readers.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\binary_stream_readers.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\buffered_file_reader.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\buffered_file_reader.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\debug_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\debug_utils.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\dir_walker.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\dir_walker.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\drawing_primitives.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\drawing_primitives_gdi.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\drawing_primitives_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\file_update_watcher.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\file_update_watcher.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\filesys_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\filesys_utils.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\fps_meter.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\fps_meter.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\hi_res_time.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\hi_res_time.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\input.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\input.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\math_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\math_utils.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\message_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\message_dialog.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\profiler.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\profiler.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\resource_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\resource_manager.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\rgb_colour.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\rgb_colour.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\sound_library_2d.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\sound_library_2d.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\sprite.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\string_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\string_utils.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\system_info.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\system_info.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\text_renderer.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\text_renderer_gdi.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\text_renderer_gdi.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\text_stream_readers.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\text_stream_readers.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\tga.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\tga.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\threading.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\threading.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\universal_include.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\universal_include.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\window_manager.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\window_manager_gdi.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\window_manager_gdi.h
# End Source File
# Begin Source File

SOURCE=.\code\lib\window_manager_opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\code\lib\window_manager_opengl.h
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\code\sound\sound_stream_decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\code\sound\sound_stream_decoder.h
# End Source File
# Begin Source File

SOURCE=.\code\sound\sound_system.cpp
# End Source File
# Begin Source File

SOURCE=.\code\sound\sound_system.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\code\app.cpp
# End Source File
# Begin Source File

SOURCE=.\code\app.h
# End Source File
# Begin Source File

SOURCE=.\code\creature.cpp
# End Source File
# Begin Source File

SOURCE=.\code\creature.h
# End Source File
# Begin Source File

SOURCE=.\code\ghost.cpp
# End Source File
# Begin Source File

SOURCE=.\code\ghost.h
# End Source File
# Begin Source File

SOURCE=.\code\maze.cpp
# End Source File
# Begin Source File

SOURCE=.\code\maze.h
# End Source File
# Begin Source File

SOURCE=.\code\pcman.cpp
# End Source File
# Begin Source File

SOURCE=.\code\pcman.h
# End Source File
# End Target
# End Project
