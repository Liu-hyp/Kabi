# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/hgfs/Share/Kabi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/hgfs/Share/Kabi/build

# Include any dependencies generated for this target.
include kabisrc/common/CMakeFiles/common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include kabisrc/common/CMakeFiles/common.dir/compiler_depend.make

# Include the progress variables for this target.
include kabisrc/common/CMakeFiles/common.dir/progress.make

# Include the compile flags for this target's objects.
include kabisrc/common/CMakeFiles/common.dir/flags.make

kabisrc/common/CMakeFiles/common.dir/config.cpp.o: kabisrc/common/CMakeFiles/common.dir/flags.make
kabisrc/common/CMakeFiles/common.dir/config.cpp.o: ../kabisrc/common/config.cpp
kabisrc/common/CMakeFiles/common.dir/config.cpp.o: kabisrc/common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/Share/Kabi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object kabisrc/common/CMakeFiles/common.dir/config.cpp.o"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT kabisrc/common/CMakeFiles/common.dir/config.cpp.o -MF CMakeFiles/common.dir/config.cpp.o.d -o CMakeFiles/common.dir/config.cpp.o -c /mnt/hgfs/Share/Kabi/kabisrc/common/config.cpp

kabisrc/common/CMakeFiles/common.dir/config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common.dir/config.cpp.i"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/Share/Kabi/kabisrc/common/config.cpp > CMakeFiles/common.dir/config.cpp.i

kabisrc/common/CMakeFiles/common.dir/config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common.dir/config.cpp.s"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/Share/Kabi/kabisrc/common/config.cpp -o CMakeFiles/common.dir/config.cpp.s

kabisrc/common/CMakeFiles/common.dir/log.cpp.o: kabisrc/common/CMakeFiles/common.dir/flags.make
kabisrc/common/CMakeFiles/common.dir/log.cpp.o: ../kabisrc/common/log.cpp
kabisrc/common/CMakeFiles/common.dir/log.cpp.o: kabisrc/common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/Share/Kabi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object kabisrc/common/CMakeFiles/common.dir/log.cpp.o"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT kabisrc/common/CMakeFiles/common.dir/log.cpp.o -MF CMakeFiles/common.dir/log.cpp.o.d -o CMakeFiles/common.dir/log.cpp.o -c /mnt/hgfs/Share/Kabi/kabisrc/common/log.cpp

kabisrc/common/CMakeFiles/common.dir/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common.dir/log.cpp.i"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/Share/Kabi/kabisrc/common/log.cpp > CMakeFiles/common.dir/log.cpp.i

kabisrc/common/CMakeFiles/common.dir/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common.dir/log.cpp.s"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/Share/Kabi/kabisrc/common/log.cpp -o CMakeFiles/common.dir/log.cpp.s

kabisrc/common/CMakeFiles/common.dir/util.cpp.o: kabisrc/common/CMakeFiles/common.dir/flags.make
kabisrc/common/CMakeFiles/common.dir/util.cpp.o: ../kabisrc/common/util.cpp
kabisrc/common/CMakeFiles/common.dir/util.cpp.o: kabisrc/common/CMakeFiles/common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/hgfs/Share/Kabi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object kabisrc/common/CMakeFiles/common.dir/util.cpp.o"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT kabisrc/common/CMakeFiles/common.dir/util.cpp.o -MF CMakeFiles/common.dir/util.cpp.o.d -o CMakeFiles/common.dir/util.cpp.o -c /mnt/hgfs/Share/Kabi/kabisrc/common/util.cpp

kabisrc/common/CMakeFiles/common.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/common.dir/util.cpp.i"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/hgfs/Share/Kabi/kabisrc/common/util.cpp > CMakeFiles/common.dir/util.cpp.i

kabisrc/common/CMakeFiles/common.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/common.dir/util.cpp.s"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/hgfs/Share/Kabi/kabisrc/common/util.cpp -o CMakeFiles/common.dir/util.cpp.s

# Object files for target common
common_OBJECTS = \
"CMakeFiles/common.dir/config.cpp.o" \
"CMakeFiles/common.dir/log.cpp.o" \
"CMakeFiles/common.dir/util.cpp.o"

# External object files for target common
common_EXTERNAL_OBJECTS =

kabisrc/common/libcommon.a: kabisrc/common/CMakeFiles/common.dir/config.cpp.o
kabisrc/common/libcommon.a: kabisrc/common/CMakeFiles/common.dir/log.cpp.o
kabisrc/common/libcommon.a: kabisrc/common/CMakeFiles/common.dir/util.cpp.o
kabisrc/common/libcommon.a: kabisrc/common/CMakeFiles/common.dir/build.make
kabisrc/common/libcommon.a: kabisrc/common/CMakeFiles/common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/hgfs/Share/Kabi/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libcommon.a"
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean_target.cmake
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
kabisrc/common/CMakeFiles/common.dir/build: kabisrc/common/libcommon.a
.PHONY : kabisrc/common/CMakeFiles/common.dir/build

kabisrc/common/CMakeFiles/common.dir/clean:
	cd /mnt/hgfs/Share/Kabi/build/kabisrc/common && $(CMAKE_COMMAND) -P CMakeFiles/common.dir/cmake_clean.cmake
.PHONY : kabisrc/common/CMakeFiles/common.dir/clean

kabisrc/common/CMakeFiles/common.dir/depend:
	cd /mnt/hgfs/Share/Kabi/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/hgfs/Share/Kabi /mnt/hgfs/Share/Kabi/kabisrc/common /mnt/hgfs/Share/Kabi/build /mnt/hgfs/Share/Kabi/build/kabisrc/common /mnt/hgfs/Share/Kabi/build/kabisrc/common/CMakeFiles/common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : kabisrc/common/CMakeFiles/common.dir/depend
