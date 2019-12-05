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

# Utility rule file for valgrind_test_tls_isap_2.

# Include the progress variables for this target.
include tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/progress.make

tls/tests/CMakeFiles/valgrind_test_tls_isap_2: tls/tests/test_tls_encrypted_record
	cd /home/its2019/its2019g25/tls/tests && env CK_FORK=no valgrind --track-origins=yes --tool=memcheck --leak-check=yes --leak-resolution=high --show-reachable=yes --xml=yes --xml-file=/home/its2019/its2019g25/tls/tests/valgrind-.xml /home/its2019/its2019g25/tls/tests/test_tls_encrypted_record isap /home/its2019/its2019g25/tls/tests/data/isap-testcase-2.chal /home/its2019/its2019g25/tls/tests/data/isap-testcase-2.sol

valgrind_test_tls_isap_2: tls/tests/CMakeFiles/valgrind_test_tls_isap_2
valgrind_test_tls_isap_2: tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/build.make

.PHONY : valgrind_test_tls_isap_2

# Rule to build all files generated by this target.
tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/build: valgrind_test_tls_isap_2

.PHONY : tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/build

tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/clean:
	cd /home/its2019/its2019g25/tls/tests && $(CMAKE_COMMAND) -P CMakeFiles/valgrind_test_tls_isap_2.dir/cmake_clean.cmake
.PHONY : tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/clean

tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/depend:
	cd /home/its2019/its2019g25 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/its2019/its2019g25 /home/its2019/its2019g25/tls/tests /home/its2019/its2019g25 /home/its2019/its2019g25/tls/tests /home/its2019/its2019g25/tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tls/tests/CMakeFiles/valgrind_test_tls_isap_2.dir/depend

