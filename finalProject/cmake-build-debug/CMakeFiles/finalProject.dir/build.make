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
CMAKE_SOURCE_DIR = C:\Computer_Graphics_Git\Computer_Graphics\finalProject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\finalProject.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\finalProject.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\finalProject.dir\flags.make

CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.obj: CMakeFiles\finalProject.dir\flags.make
CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.obj: ..\finalProject\fp_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/finalProject.dir/finalProject/fp_main.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\finalProject.dir\finalProject\fp_main.cpp.obj /FdCMakeFiles\finalProject.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\fp_main.cpp
<<

CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/finalProject.dir/finalProject/fp_main.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\fp_main.cpp
<<

CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/finalProject.dir/finalProject/fp_main.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\finalProject.dir\finalProject\fp_main.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\fp_main.cpp
<<

CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.obj: CMakeFiles\finalProject.dir\flags.make
CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.obj: ..\finalProject\render\shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/finalProject.dir/finalProject/render/shader.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\finalProject.dir\finalProject\render\shader.cpp.obj /FdCMakeFiles\finalProject.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\render\shader.cpp
<<

CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/finalProject.dir/finalProject/render/shader.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\render\shader.cpp
<<

CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/finalProject.dir/finalProject/render/shader.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\finalProject.dir\finalProject\render\shader.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\finalProject\render\shader.cpp
<<

CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.obj: CMakeFiles\finalProject.dir\flags.make
CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.obj: ..\external\glad-3.3\src\gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/finalProject.dir/external/glad-3.3/src/gl.c.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.obj /FdCMakeFiles\finalProject.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\external\glad-3.3\src\gl.c
<<

CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/finalProject.dir/external/glad-3.3/src/gl.c.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\finalProject\external\glad-3.3\src\gl.c
<<

CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/finalProject.dir/external/glad-3.3/src/gl.c.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.s /c C:\Computer_Graphics_Git\Computer_Graphics\finalProject\external\glad-3.3\src\gl.c
<<

# Object files for target finalProject
finalProject_OBJECTS = \
"CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.obj" \
"CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.obj" \
"CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.obj"

# External object files for target finalProject
finalProject_EXTERNAL_OBJECTS =

finalProject.exe: CMakeFiles\finalProject.dir\finalProject\fp_main.cpp.obj
finalProject.exe: CMakeFiles\finalProject.dir\finalProject\render\shader.cpp.obj
finalProject.exe: CMakeFiles\finalProject.dir\external\glad-3.3\src\gl.c.obj
finalProject.exe: CMakeFiles\finalProject.dir\build.make
finalProject.exe: glfw3dll.lib
finalProject.exe: CMakeFiles\finalProject.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable finalProject.exe"
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\finalProject.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\finalProject.dir\objects1.rsp @<<
 /out:finalProject.exe /implib:finalProject.lib /pdb:C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\finalProject.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  opengl32.lib glu32.lib glfw3dll.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\finalProject.dir\build: finalProject.exe

.PHONY : CMakeFiles\finalProject.dir\build

CMakeFiles\finalProject.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\finalProject.dir\cmake_clean.cmake
.PHONY : CMakeFiles\finalProject.dir\clean

CMakeFiles\finalProject.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Computer_Graphics_Git\Computer_Graphics\finalProject C:\Computer_Graphics_Git\Computer_Graphics\finalProject C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\finalProject\cmake-build-debug\CMakeFiles\finalProject.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\finalProject.dir\depend
