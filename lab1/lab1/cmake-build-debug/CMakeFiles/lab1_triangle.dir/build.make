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
CMAKE_SOURCE_DIR = C:\graphics\lab1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\graphics\lab1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\lab1_triangle.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\lab1_triangle.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\lab1_triangle.dir\flags.make

CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.obj: CMakeFiles\lab1_triangle.dir\flags.make
CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.obj: ..\lab1\lab1_triangle.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab1_triangle.dir/lab1/lab1_triangle.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.obj /FdCMakeFiles\lab1_triangle.dir\ /FS -c C:\graphics\lab1\lab1\lab1_triangle.cpp
<<

CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab1_triangle.dir/lab1/lab1_triangle.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\graphics\lab1\lab1\lab1_triangle.cpp
<<

CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab1_triangle.dir/lab1/lab1_triangle.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.s /c C:\graphics\lab1\lab1\lab1_triangle.cpp
<<

# Object files for target lab1_triangle
lab1_triangle_OBJECTS = \
"CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.obj"

# External object files for target lab1_triangle
lab1_triangle_EXTERNAL_OBJECTS =

lab1_triangle.exe: CMakeFiles\lab1_triangle.dir\lab1\lab1_triangle.cpp.obj
lab1_triangle.exe: CMakeFiles\lab1_triangle.dir\build.make
lab1_triangle.exe: glfw3dll.lib
lab1_triangle.exe: glad.lib
lab1_triangle.exe: CMakeFiles\lab1_triangle.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lab1_triangle.exe"
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\lab1_triangle.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\lab1_triangle.dir\objects1.rsp @<<
 /out:lab1_triangle.exe /implib:lab1_triangle.lib /pdb:C:\graphics\lab1\cmake-build-debug\lab1_triangle.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  opengl32.lib glu32.lib glfw3dll.lib glad.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\lab1_triangle.dir\build: lab1_triangle.exe

.PHONY : CMakeFiles\lab1_triangle.dir\build

CMakeFiles\lab1_triangle.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab1_triangle.dir\cmake_clean.cmake
.PHONY : CMakeFiles\lab1_triangle.dir\clean

CMakeFiles\lab1_triangle.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\graphics\lab1 C:\graphics\lab1 C:\graphics\lab1\cmake-build-debug C:\graphics\lab1\cmake-build-debug C:\graphics\lab1\cmake-build-debug\CMakeFiles\lab1_triangle.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\lab1_triangle.dir\depend
