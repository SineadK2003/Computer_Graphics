# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\lab3_cornellbox.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\lab3_cornellbox.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\lab3_cornellbox.dir\flags.make

CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.obj: CMakeFiles\lab3_cornellbox.dir\flags.make
CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.obj: ..\lab3\lab3_cornellbox.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab3_cornellbox.dir/lab3/lab3_cornellbox.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.obj /FdCMakeFiles\lab3_cornellbox.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\lab3_cornellbox.cpp
<<

CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab3_cornellbox.dir/lab3/lab3_cornellbox.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\lab3_cornellbox.cpp
<<

CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab3_cornellbox.dir/lab3/lab3_cornellbox.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\lab3_cornellbox.cpp
<<

CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.obj: CMakeFiles\lab3_cornellbox.dir\flags.make
CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.obj: ..\lab3\render\shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lab3_cornellbox.dir/lab3/render/shader.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.obj /FdCMakeFiles\lab3_cornellbox.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\render\shader.cpp
<<

CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab3_cornellbox.dir/lab3/render/shader.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\render\shader.cpp
<<

CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab3_cornellbox.dir/lab3/render/shader.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\lab3\render\shader.cpp
<<

# Object files for target lab3_cornellbox
lab3_cornellbox_OBJECTS = \
"CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.obj" \
"CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.obj"

# External object files for target lab3_cornellbox
lab3_cornellbox_EXTERNAL_OBJECTS =

lab3_cornellbox.exe: CMakeFiles\lab3_cornellbox.dir\lab3\lab3_cornellbox.cpp.obj
lab3_cornellbox.exe: CMakeFiles\lab3_cornellbox.dir\lab3\render\shader.cpp.obj
lab3_cornellbox.exe: CMakeFiles\lab3_cornellbox.dir\build.make
lab3_cornellbox.exe: glfw3dll.lib
lab3_cornellbox.exe: glad.lib
lab3_cornellbox.exe: CMakeFiles\lab3_cornellbox.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable lab3_cornellbox.exe"
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\lab3_cornellbox.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\lab3_cornellbox.dir\objects1.rsp @<<
 /out:lab3_cornellbox.exe /implib:lab3_cornellbox.lib /pdb:C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug\lab3_cornellbox.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  opengl32.lib glu32.lib glfw3dll.lib glad.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\lab3_cornellbox.dir\build: lab3_cornellbox.exe

.PHONY : CMakeFiles\lab3_cornellbox.dir\build

CMakeFiles\lab3_cornellbox.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab3_cornellbox.dir\cmake_clean.cmake
.PHONY : CMakeFiles\lab3_cornellbox.dir\clean

CMakeFiles\lab3_cornellbox.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3 C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3 C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\lab3\new_lab3\lab3\cmake-build-debug\CMakeFiles\lab3_cornellbox.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\lab3_cornellbox.dir\depend

