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
include external\glfw-3.1.2\src\CMakeFiles\glfw.dir\depend.make

# Include the progress variables for this target.
include external\glfw-3.1.2\src\CMakeFiles\glfw.dir\progress.make

# Include the compile flags for this target's objects.
include external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\context.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\context.c.obj: ..\external\glfw-3.1.2\src\context.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/context.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\context.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\context.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/context.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\context.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\context.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/context.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\context.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\init.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\init.c.obj: ..\external\glfw-3.1.2\src\init.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/init.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\init.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/init.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\init.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/init.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\init.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\input.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\input.c.obj: ..\external\glfw-3.1.2\src\input.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/input.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\input.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\input.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\input.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/input.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\input.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\input.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\input.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/input.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\input.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\input.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\monitor.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\monitor.c.obj: ..\external\glfw-3.1.2\src\monitor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/monitor.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\monitor.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\monitor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/monitor.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\monitor.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\monitor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/monitor.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\monitor.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\window.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\window.c.obj: ..\external\glfw-3.1.2\src\window.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/window.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\window.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\window.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/window.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\window.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\window.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/window.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\window.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_init.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_init.c.obj: ..\external\glfw-3.1.2\src\win32_init.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/win32_init.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\win32_init.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\win32_init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/win32_init.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\win32_init.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\win32_init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/win32_init.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\win32_init.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\win32_init.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_monitor.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_monitor.c.obj: ..\external\glfw-3.1.2\src\win32_monitor.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/win32_monitor.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\win32_monitor.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\win32_monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_monitor.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/win32_monitor.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\win32_monitor.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\win32_monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_monitor.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/win32_monitor.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\win32_monitor.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\win32_monitor.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_time.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_time.c.obj: ..\external\glfw-3.1.2\src\win32_time.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/win32_time.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\win32_time.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\win32_time.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_time.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/win32_time.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\win32_time.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\win32_time.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_time.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/win32_time.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\win32_time.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\win32_time.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_tls.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_tls.c.obj: ..\external\glfw-3.1.2\src\win32_tls.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/win32_tls.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\win32_tls.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\win32_tls.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_tls.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/win32_tls.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\win32_tls.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\win32_tls.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_tls.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/win32_tls.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\win32_tls.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\win32_tls.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_window.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_window.c.obj: ..\external\glfw-3.1.2\src\win32_window.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/win32_window.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\win32_window.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\win32_window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_window.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/win32_window.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\win32_window.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\win32_window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_window.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/win32_window.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\win32_window.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\win32_window.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\winmm_joystick.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\winmm_joystick.c.obj: ..\external\glfw-3.1.2\src\winmm_joystick.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/winmm_joystick.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\winmm_joystick.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\winmm_joystick.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\winmm_joystick.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/winmm_joystick.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\winmm_joystick.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\winmm_joystick.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\winmm_joystick.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/winmm_joystick.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\winmm_joystick.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\winmm_joystick.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\wgl_context.c.obj: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\flags.make
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\wgl_context.c.obj: ..\external\glfw-3.1.2\src\wgl_context.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building C object external/glfw-3.1.2/src/CMakeFiles/glfw.dir/wgl_context.c.obj"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\glfw.dir\wgl_context.c.obj /FdCMakeFiles\glfw.dir\ /FS -c C:\graphics\lab1\external\glfw-3.1.2\src\wgl_context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\wgl_context.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glfw.dir/wgl_context.c.i"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe > CMakeFiles\glfw.dir\wgl_context.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\graphics\lab1\external\glfw-3.1.2\src\wgl_context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\wgl_context.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glfw.dir/wgl_context.c.s"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\glfw.dir\wgl_context.c.s /c C:\graphics\lab1\external\glfw-3.1.2\src\wgl_context.c
<<
	cd C:\graphics\lab1\cmake-build-debug

# Object files for target glfw
glfw_OBJECTS = \
"CMakeFiles\glfw.dir\context.c.obj" \
"CMakeFiles\glfw.dir\init.c.obj" \
"CMakeFiles\glfw.dir\input.c.obj" \
"CMakeFiles\glfw.dir\monitor.c.obj" \
"CMakeFiles\glfw.dir\window.c.obj" \
"CMakeFiles\glfw.dir\win32_init.c.obj" \
"CMakeFiles\glfw.dir\win32_monitor.c.obj" \
"CMakeFiles\glfw.dir\win32_time.c.obj" \
"CMakeFiles\glfw.dir\win32_tls.c.obj" \
"CMakeFiles\glfw.dir\win32_window.c.obj" \
"CMakeFiles\glfw.dir\winmm_joystick.c.obj" \
"CMakeFiles\glfw.dir\wgl_context.c.obj"

# External object files for target glfw
glfw_EXTERNAL_OBJECTS =

glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\context.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\init.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\input.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\monitor.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\window.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_init.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_monitor.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_time.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_tls.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\win32_window.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\winmm_joystick.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\wgl_context.c.obj
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\build.make
glfw3.dll: external\glfw-3.1.2\src\CMakeFiles\glfw.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\graphics\lab1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking C shared library ..\..\..\glfw3.dll"
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	"C:\Program Files\JetBrains\CLion 2020.3.2\bin\cmake\win\bin\cmake.exe" -E vs_link_dll --intdir=CMakeFiles\glfw.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100220~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~2\2022\BUILDT~1\VC\Tools\MSVC\1434~1.319\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\glfw.dir\objects1.rsp @<<
 /out:..\..\..\glfw3.dll /implib:..\..\..\glfw3dll.lib /pdb:C:\graphics\lab1\cmake-build-debug\glfw3.pdb /dll /version:3.1 /machine:X86 /debug /INCREMENTAL  opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd C:\graphics\lab1\cmake-build-debug

# Rule to build all files generated by this target.
external\glfw-3.1.2\src\CMakeFiles\glfw.dir\build: glfw3.dll

.PHONY : external\glfw-3.1.2\src\CMakeFiles\glfw.dir\build

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\clean:
	cd C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src
	$(CMAKE_COMMAND) -P CMakeFiles\glfw.dir\cmake_clean.cmake
	cd C:\graphics\lab1\cmake-build-debug
.PHONY : external\glfw-3.1.2\src\CMakeFiles\glfw.dir\clean

external\glfw-3.1.2\src\CMakeFiles\glfw.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\graphics\lab1 C:\graphics\lab1\external\glfw-3.1.2\src C:\graphics\lab1\cmake-build-debug C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src C:\graphics\lab1\cmake-build-debug\external\glfw-3.1.2\src\CMakeFiles\glfw.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : external\glfw-3.1.2\src\CMakeFiles\glfw.dir\depend

