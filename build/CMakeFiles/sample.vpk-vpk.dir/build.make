# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.25.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.25.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/tylersand/sample

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/tylersand/sample/build

# Utility rule file for sample.vpk-vpk.

# Include any custom commands dependencies for this target.
include CMakeFiles/sample.vpk-vpk.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sample.vpk-vpk.dir/progress.make

CMakeFiles/sample.vpk-vpk: sample.vpk.out
	/usr/local/Cellar/cmake/3.25.0/bin/cmake -E copy /Users/tylersand/sample/build/sample.vpk.out sample.vpk

sample.vpk.out: sample.vpk_param.sfo
sample.vpk.out: sample.self
sample.vpk.out: /Users/tylersand/sample/sce_sys/icon0.png
sample.vpk.out: /Users/tylersand/sample/sce_sys/livearea/contents/bg.png
sample.vpk.out: /Users/tylersand/sample/sce_sys/livearea/contents/startup.png
sample.vpk.out: /Users/tylersand/sample/sce_sys/livearea/contents/template.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/tylersand/sample/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building vpk sample.vpk"
	/usr/local/vitasdk/bin/vita-pack-vpk -a /Users/tylersand/sample/sce_sys/icon0.png=sce_sys/icon0.png -a /Users/tylersand/sample/sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png -a /Users/tylersand/sample/sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png -a /Users/tylersand/sample/sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml -s /Users/tylersand/sample/build/sample.vpk_param.sfo -b /Users/tylersand/sample/build/sample.self /Users/tylersand/sample/build/sample.vpk.out

sample.vpk_param.sfo: sample.self
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/tylersand/sample/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating param.sfo for sample.vpk"
	/usr/local/vitasdk/bin/vita-mksfoex -d PARENTAL_LEVEL=1 -s APP_VER=01.00 -s TITLE_ID=VSDK00007 "Hello World" /Users/tylersand/sample/build/sample.vpk_param.sfo

sample.vpk-vpk: CMakeFiles/sample.vpk-vpk
sample.vpk-vpk: sample.vpk.out
sample.vpk-vpk: sample.vpk_param.sfo
sample.vpk-vpk: CMakeFiles/sample.vpk-vpk.dir/build.make
.PHONY : sample.vpk-vpk

# Rule to build all files generated by this target.
CMakeFiles/sample.vpk-vpk.dir/build: sample.vpk-vpk
.PHONY : CMakeFiles/sample.vpk-vpk.dir/build

CMakeFiles/sample.vpk-vpk.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sample.vpk-vpk.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sample.vpk-vpk.dir/clean

CMakeFiles/sample.vpk-vpk.dir/depend:
	cd /Users/tylersand/sample/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/tylersand/sample /Users/tylersand/sample /Users/tylersand/sample/build /Users/tylersand/sample/build /Users/tylersand/sample/build/CMakeFiles/sample.vpk-vpk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sample.vpk-vpk.dir/depend
