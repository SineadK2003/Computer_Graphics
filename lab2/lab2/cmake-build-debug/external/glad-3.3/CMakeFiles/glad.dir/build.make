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
include external\glad-3.3\CMakeFiles\glad.dir\depend.make

# Include the progress variables for this target.
include external\glad-3.3\CMakeFiles\glad.dir\progress.make

# Include the compile flags for this target's objects.
include external\glad-3.3\CMakeFiles\glad.dir\flags.make

external\glad-3.3\CMakeFiles\glad.dir\src\gl.c.obj: external\glad-3.3\CMakeFiles\glad.dir\flags.make
external\glad-3.3\CMakeFiles\glad.dir\src\gl.c.obj: ..\external\glad-3.3\src\gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glad-3.3/CMakeFiles/glad.dir/src/gl.c.obj"
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glad.dir\src\gl.c.obj /FdCMakeFiles\glad.dir\ /FS -c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug

external\glad-3.3\CMakeFiles\glad.dir\src\gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glad.dir/src/gl.c.i"
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe > CMakeFiles\glad.dir\src\gl.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug

external\glad-3.3\CMakeFiles\glad.dir\src\gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glad.dir/src/gl.c.s"
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glad.dir\src\gl.c.s /c C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3\src\gl.c
<<
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug

# Object files for target glad
glad_OBJECTS = \
"CMakeFiles\glad.dir\src\gl.c.obj"

# External object files for target glad
glad_EXTERNAL_OBJECTS =

glad.dll: external\glad-3.3\CMakeFiles\glad.dir\src\gl.c.obj
glad.dll: external\glad-3.3\CMakeFiles\glad.dir\build.make
glad.dll: external\glad-3.3\CMakeFiles\glad.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library ..\..\glad.dll"
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_dll --intdir=CMakeFiles\glad.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1442~1.344\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\glad.dir\objects1.rsp @<<
 /out:..\..\glad.dll /implib:..\..\glad.lib /pdb:C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\glad.pdb /dll /version:0.0 /machine:X86 /debug /INCREMENTAL  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug

# Rule to build all files generated by this target.
external\glad-3.3\CMakeFiles\glad.dir\build: glad.dll

.PHONY : external\glad-3.3\CMakeFiles\glad.dir\build

external\glad-3.3\CMakeFiles\glad.dir\clean:
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3
	$(CMAKE_COMMAND) -P CMakeFiles\glad.dir\cmake_clean.cmake
	cd C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug
.PHONY : external\glad-3.3\CMakeFiles\glad.dir\clean

external\glad-3.3\CMakeFiles\glad.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2 C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\external\glad-3.3 C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3 C:\Computer_Graphics_Git\Computer_Graphics\lab2\lab2\cmake-build-debug\external\glad-3.3\CMakeFiles\glad.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : external\glad-3.3\CMakeFiles\glad.dir\depend

