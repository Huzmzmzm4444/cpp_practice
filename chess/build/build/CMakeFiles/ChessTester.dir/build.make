# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/petrfateev/CLionProjects/Семинары_С++/01

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/petrfateev/CLionProjects/Семинары_С++/01/build

# Include any dependencies generated for this target.
include CMakeFiles/ChessTester.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ChessTester.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ChessTester.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ChessTester.dir/flags.make

CMakeFiles/ChessTester.dir/frontend.cpp.o: CMakeFiles/ChessTester.dir/flags.make
CMakeFiles/ChessTester.dir/frontend.cpp.o: /Users/petrfateev/CLionProjects/Семинары_С++/01/frontend.cpp
CMakeFiles/ChessTester.dir/frontend.cpp.o: CMakeFiles/ChessTester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ChessTester.dir/frontend.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ChessTester.dir/frontend.cpp.o -MF CMakeFiles/ChessTester.dir/frontend.cpp.o.d -o CMakeFiles/ChessTester.dir/frontend.cpp.o -c /Users/petrfateev/CLionProjects/Семинары_С++/01/frontend.cpp

CMakeFiles/ChessTester.dir/frontend.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChessTester.dir/frontend.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/petrfateev/CLionProjects/Семинары_С++/01/frontend.cpp > CMakeFiles/ChessTester.dir/frontend.cpp.i

CMakeFiles/ChessTester.dir/frontend.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChessTester.dir/frontend.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/petrfateev/CLionProjects/Семинары_С++/01/frontend.cpp -o CMakeFiles/ChessTester.dir/frontend.cpp.s

CMakeFiles/ChessTester.dir/tester.cpp.o: CMakeFiles/ChessTester.dir/flags.make
CMakeFiles/ChessTester.dir/tester.cpp.o: /Users/petrfateev/CLionProjects/Семинары_С++/01/tester.cpp
CMakeFiles/ChessTester.dir/tester.cpp.o: CMakeFiles/ChessTester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ChessTester.dir/tester.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ChessTester.dir/tester.cpp.o -MF CMakeFiles/ChessTester.dir/tester.cpp.o.d -o CMakeFiles/ChessTester.dir/tester.cpp.o -c /Users/petrfateev/CLionProjects/Семинары_С++/01/tester.cpp

CMakeFiles/ChessTester.dir/tester.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChessTester.dir/tester.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/petrfateev/CLionProjects/Семинары_С++/01/tester.cpp > CMakeFiles/ChessTester.dir/tester.cpp.i

CMakeFiles/ChessTester.dir/tester.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChessTester.dir/tester.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/petrfateev/CLionProjects/Семинары_С++/01/tester.cpp -o CMakeFiles/ChessTester.dir/tester.cpp.s

CMakeFiles/ChessTester.dir/backend.cpp.o: CMakeFiles/ChessTester.dir/flags.make
CMakeFiles/ChessTester.dir/backend.cpp.o: /Users/petrfateev/CLionProjects/Семинары_С++/01/backend.cpp
CMakeFiles/ChessTester.dir/backend.cpp.o: CMakeFiles/ChessTester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ChessTester.dir/backend.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ChessTester.dir/backend.cpp.o -MF CMakeFiles/ChessTester.dir/backend.cpp.o.d -o CMakeFiles/ChessTester.dir/backend.cpp.o -c /Users/petrfateev/CLionProjects/Семинары_С++/01/backend.cpp

CMakeFiles/ChessTester.dir/backend.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChessTester.dir/backend.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/petrfateev/CLionProjects/Семинары_С++/01/backend.cpp > CMakeFiles/ChessTester.dir/backend.cpp.i

CMakeFiles/ChessTester.dir/backend.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChessTester.dir/backend.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/petrfateev/CLionProjects/Семинары_С++/01/backend.cpp -o CMakeFiles/ChessTester.dir/backend.cpp.s

CMakeFiles/ChessTester.dir/bot.cpp.o: CMakeFiles/ChessTester.dir/flags.make
CMakeFiles/ChessTester.dir/bot.cpp.o: /Users/petrfateev/CLionProjects/Семинары_С++/01/bot.cpp
CMakeFiles/ChessTester.dir/bot.cpp.o: CMakeFiles/ChessTester.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ChessTester.dir/bot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ChessTester.dir/bot.cpp.o -MF CMakeFiles/ChessTester.dir/bot.cpp.o.d -o CMakeFiles/ChessTester.dir/bot.cpp.o -c /Users/petrfateev/CLionProjects/Семинары_С++/01/bot.cpp

CMakeFiles/ChessTester.dir/bot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ChessTester.dir/bot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/petrfateev/CLionProjects/Семинары_С++/01/bot.cpp > CMakeFiles/ChessTester.dir/bot.cpp.i

CMakeFiles/ChessTester.dir/bot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ChessTester.dir/bot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/petrfateev/CLionProjects/Семинары_С++/01/bot.cpp -o CMakeFiles/ChessTester.dir/bot.cpp.s

# Object files for target ChessTester
ChessTester_OBJECTS = \
"CMakeFiles/ChessTester.dir/frontend.cpp.o" \
"CMakeFiles/ChessTester.dir/tester.cpp.o" \
"CMakeFiles/ChessTester.dir/backend.cpp.o" \
"CMakeFiles/ChessTester.dir/bot.cpp.o"

# External object files for target ChessTester
ChessTester_EXTERNAL_OBJECTS =

ChessTester: CMakeFiles/ChessTester.dir/frontend.cpp.o
ChessTester: CMakeFiles/ChessTester.dir/tester.cpp.o
ChessTester: CMakeFiles/ChessTester.dir/backend.cpp.o
ChessTester: CMakeFiles/ChessTester.dir/bot.cpp.o
ChessTester: CMakeFiles/ChessTester.dir/build.make
ChessTester: /opt/homebrew/lib/libfltk_images.dylib
ChessTester: /opt/homebrew/lib/libfltk_gl.dylib
ChessTester: /opt/homebrew/lib/libfltk.dylib
ChessTester: CMakeFiles/ChessTester.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ChessTester"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ChessTester.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ChessTester.dir/build: ChessTester
.PHONY : CMakeFiles/ChessTester.dir/build

CMakeFiles/ChessTester.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ChessTester.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ChessTester.dir/clean

CMakeFiles/ChessTester.dir/depend:
	cd /Users/petrfateev/CLionProjects/Семинары_С++/01/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/petrfateev/CLionProjects/Семинары_С++/01 /Users/petrfateev/CLionProjects/Семинары_С++/01 /Users/petrfateev/CLionProjects/Семинары_С++/01/build /Users/petrfateev/CLionProjects/Семинары_С++/01/build /Users/petrfateev/CLionProjects/Семинары_С++/01/build/CMakeFiles/ChessTester.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ChessTester.dir/depend

