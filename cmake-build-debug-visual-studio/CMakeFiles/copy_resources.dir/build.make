﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
CMAKE_COMMAND = C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\CLionProjects\programowanie_gier

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\CLionProjects\programowanie_gier\cmake-build-debug-visual-studio

# Utility rule file for copy_resources.

# Include any custom commands dependencies for this target.
include CMakeFiles\copy_resources.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\copy_resources.dir\progress.make

CMakeFiles\copy_resources:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=C:\Users\User\CLionProjects\programowanie_gier\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Copying resources into binary directory"
	C:\Users\User\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E copy_directory C:/Users/User/CLionProjects/programowanie_gier/resources C:/Users/User/CLionProjects/programowanie_gier/cmake-build-debug-visual-studio/resources

copy_resources: CMakeFiles\copy_resources
copy_resources: CMakeFiles\copy_resources.dir\build.make
.PHONY : copy_resources

# Rule to build all files generated by this target.
CMakeFiles\copy_resources.dir\build: copy_resources
.PHONY : CMakeFiles\copy_resources.dir\build

CMakeFiles\copy_resources.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\copy_resources.dir\cmake_clean.cmake
.PHONY : CMakeFiles\copy_resources.dir\clean

CMakeFiles\copy_resources.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\User\CLionProjects\programowanie_gier C:\Users\User\CLionProjects\programowanie_gier C:\Users\User\CLionProjects\programowanie_gier\cmake-build-debug-visual-studio C:\Users\User\CLionProjects\programowanie_gier\cmake-build-debug-visual-studio C:\Users\User\CLionProjects\programowanie_gier\cmake-build-debug-visual-studio\CMakeFiles\copy_resources.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles\copy_resources.dir\depend
