# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/its2019/its2019g25

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/its2019/its2019g25

# Include any dependencies generated for this target.
include tls/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include tls/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include tls/CMakeFiles/server.dir/flags.make

tls/CMakeFiles/server.dir/server.cpp.o: tls/CMakeFiles/server.dir/flags.make
tls/CMakeFiles/server.dir/server.cpp.o: tls/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/its2019/its2019g25/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tls/CMakeFiles/server.dir/server.cpp.o"
	cd /home/its2019/its2019g25/tls && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server.cpp.o -c /home/its2019/its2019g25/tls/server.cpp

tls/CMakeFiles/server.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server.cpp.i"
	cd /home/its2019/its2019g25/tls && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/its2019/its2019g25/tls/server.cpp > CMakeFiles/server.dir/server.cpp.i

tls/CMakeFiles/server.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server.cpp.s"
	cd /home/its2019/its2019g25/tls && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/its2019/its2019g25/tls/server.cpp -o CMakeFiles/server.dir/server.cpp.s

tls/CMakeFiles/server.dir/server.cpp.o.requires:

.PHONY : tls/CMakeFiles/server.dir/server.cpp.o.requires

tls/CMakeFiles/server.dir/server.cpp.o.provides: tls/CMakeFiles/server.dir/server.cpp.o.requires
	$(MAKE) -f tls/CMakeFiles/server.dir/build.make tls/CMakeFiles/server.dir/server.cpp.o.provides.build
.PHONY : tls/CMakeFiles/server.dir/server.cpp.o.provides

tls/CMakeFiles/server.dir/server.cpp.o.provides.build: tls/CMakeFiles/server.dir/server.cpp.o


# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/server.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

tls/server: tls/CMakeFiles/server.dir/server.cpp.o
tls/server: tls/CMakeFiles/server.dir/build.make
tls/server: tls/libprimitives.so
tls/server: utils/libutils.so
tls/server: /usr/lib/x86_64-linux-gnu/libboost_system.so
tls/server: ecclib/libecc_lib.so
tls/server: ascon/libascon.so
tls/server: isap/libisap.so
tls/server: tls/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/its2019/its2019g25/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server"
	cd /home/its2019/its2019g25/tls && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tls/CMakeFiles/server.dir/build: tls/server

.PHONY : tls/CMakeFiles/server.dir/build

tls/CMakeFiles/server.dir/requires: tls/CMakeFiles/server.dir/server.cpp.o.requires

.PHONY : tls/CMakeFiles/server.dir/requires

tls/CMakeFiles/server.dir/clean:
	cd /home/its2019/its2019g25/tls && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : tls/CMakeFiles/server.dir/clean

tls/CMakeFiles/server.dir/depend:
	cd /home/its2019/its2019g25 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/its2019/its2019g25 /home/its2019/its2019g25/tls /home/its2019/its2019g25 /home/its2019/its2019g25/tls /home/its2019/its2019g25/tls/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tls/CMakeFiles/server.dir/depend

