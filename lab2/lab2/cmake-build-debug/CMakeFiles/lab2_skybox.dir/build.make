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
CMAKE_SOURCE_DIR = C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\lab2_skybox.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\lab2_skybox.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\lab2_skybox.dir\flags.make

CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.obj: CMakeFiles\lab2_skybox.dir\flags.make
CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.obj: ..\lab2\lab2_skybox.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab2_skybox.dir/lab2/lab2_skybox.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.obj /FdCMakeFiles\lab2_skybox.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\lab2_skybox.cpp
<<

CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab2_skybox.dir/lab2/lab2_skybox.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\lab2_skybox.cpp
<<

CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab2_skybox.dir/lab2/lab2_skybox.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\lab2_skybox.cpp
<<

CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.obj: CMakeFiles\lab2_skybox.dir\flags.make
CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.obj: ..\lab2\render\shader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/lab2_skybox.dir/lab2/render/shader.cpp.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.obj /FdCMakeFiles\lab2_skybox.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\render\shader.cpp
<<

CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab2_skybox.dir/lab2/render/shader.cpp.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\render\shader.cpp
<<

CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab2_skybox.dir/lab2/render/shader.cpp.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\lab2\render\shader.cpp
<<

CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.obj: CMakeFiles\lab2_skybox.dir\flags.make
CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.obj: ..\external\glad-3.3\src\gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/lab2_skybox.dir/external/glad-3.3/src/gl.c.obj"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.obj /FdCMakeFiles\lab2_skybox.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<

CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab2_skybox.dir/external/glad-3.3/src/gl.c.i"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<

CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab2_skybox.dir/external/glad-3.3/src/gl.c.s"
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<

# Object files for target lab2_skybox
lab2_skybox_OBJECTS = \
"CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.obj" \
"CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.obj" \
"CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.obj"

# External object files for target lab2_skybox
lab2_skybox_EXTERNAL_OBJECTS =

lab2_skybox.exe: CMakeFiles\lab2_skybox.dir\lab2\lab2_skybox.cpp.obj
lab2_skybox.exe: CMakeFiles\lab2_skybox.dir\lab2\render\shader.cpp.obj
lab2_skybox.exe: CMakeFiles\lab2_skybox.dir\external\glad-3.3\src\gl.c.obj
lab2_skybox.exe: CMakeFiles\lab2_skybox.dir\build.make
lab2_skybox.exe: glfw3dll.lib
lab2_skybox.exe: CMakeFiles\lab2_skybox.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable lab2_skybox.exe"
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\lab2_skybox.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\lab2_skybox.dir\objects1.rsp @<<
 /out:lab2_skybox.exe /implib:lab2_skybox.lib /pdb:C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\lab2_skybox.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  opengl32.lib glu32.lib glfw3dll.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\lab2_skybox.dir\build: lab2_skybox.exe

.PHONY : CMakeFiles\lab2_skybox.dir\build

CMakeFiles\lab2_skybox.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\lab2_skybox.dir\cmake_clean.cmake
.PHONY : CMakeFiles\lab2_skybox.dir\clean

CMakeFiles\lab2_skybox.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2 C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2 C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles\lab2_skybox.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\lab2_skybox.dir\depend

