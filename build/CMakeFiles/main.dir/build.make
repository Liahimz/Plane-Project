# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.12.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.12.4/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/mihail/Desktop/OpenGL/Plane Project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/mihail/Desktop/OpenGL/Plane Project/build"

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/glad.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/glad.c.o: ../glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.dir/glad.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/main.dir/glad.c.o   -c "/Users/mihail/Desktop/OpenGL/Plane Project/glad.c"

CMakeFiles/main.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/glad.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/glad.c" > CMakeFiles/main.dir/glad.c.i

CMakeFiles/main.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/glad.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/glad.c" -o CMakeFiles/main.dir/glad.c.s

CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/main.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/main.cpp"

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/main.cpp" > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/main.cpp" -o CMakeFiles/main.dir/main.cpp.s

CMakeFiles/main.dir/Camera.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/Camera.cpp.o: ../Camera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/Camera.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/Camera.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/Camera.cpp"

CMakeFiles/main.dir/Camera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/Camera.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/Camera.cpp" > CMakeFiles/main.dir/Camera.cpp.i

CMakeFiles/main.dir/Camera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/Camera.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/Camera.cpp" -o CMakeFiles/main.dir/Camera.cpp.s

CMakeFiles/main.dir/ShaderProgram.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ShaderProgram.cpp.o: ../ShaderProgram.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/main.dir/ShaderProgram.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/ShaderProgram.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/ShaderProgram.cpp"

CMakeFiles/main.dir/ShaderProgram.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/ShaderProgram.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/ShaderProgram.cpp" > CMakeFiles/main.dir/ShaderProgram.cpp.i

CMakeFiles/main.dir/ShaderProgram.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/ShaderProgram.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/ShaderProgram.cpp" -o CMakeFiles/main.dir/ShaderProgram.cpp.s

CMakeFiles/main.dir/mesh.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/mesh.cpp.o: ../mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/main.dir/mesh.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/mesh.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/mesh.cpp"

CMakeFiles/main.dir/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/mesh.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/mesh.cpp" > CMakeFiles/main.dir/mesh.cpp.i

CMakeFiles/main.dir/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/mesh.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/mesh.cpp" -o CMakeFiles/main.dir/mesh.cpp.s

CMakeFiles/main.dir/drawcall.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/drawcall.cpp.o: ../drawcall.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/main.dir/drawcall.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/drawcall.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/drawcall.cpp"

CMakeFiles/main.dir/drawcall.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/drawcall.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/drawcall.cpp" > CMakeFiles/main.dir/drawcall.cpp.i

CMakeFiles/main.dir/drawcall.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/drawcall.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/drawcall.cpp" -o CMakeFiles/main.dir/drawcall.cpp.s

CMakeFiles/main.dir/ImportSceneObject.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/ImportSceneObject.cpp.o: ../ImportSceneObject.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/main.dir/ImportSceneObject.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/ImportSceneObject.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/ImportSceneObject.cpp"

CMakeFiles/main.dir/ImportSceneObject.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/ImportSceneObject.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/ImportSceneObject.cpp" > CMakeFiles/main.dir/ImportSceneObject.cpp.i

CMakeFiles/main.dir/ImportSceneObject.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/ImportSceneObject.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/ImportSceneObject.cpp" -o CMakeFiles/main.dir/ImportSceneObject.cpp.s

CMakeFiles/main.dir/rain_mesh.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/rain_mesh.cpp.o: ../rain_mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/main.dir/rain_mesh.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/rain_mesh.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/rain_mesh.cpp"

CMakeFiles/main.dir/rain_mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/rain_mesh.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/rain_mesh.cpp" > CMakeFiles/main.dir/rain_mesh.cpp.i

CMakeFiles/main.dir/rain_mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/rain_mesh.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/rain_mesh.cpp" -o CMakeFiles/main.dir/rain_mesh.cpp.s

CMakeFiles/main.dir/cloud_mesh.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/cloud_mesh.cpp.o: ../cloud_mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/main.dir/cloud_mesh.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/cloud_mesh.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/cloud_mesh.cpp"

CMakeFiles/main.dir/cloud_mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/cloud_mesh.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/cloud_mesh.cpp" > CMakeFiles/main.dir/cloud_mesh.cpp.i

CMakeFiles/main.dir/cloud_mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/cloud_mesh.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/cloud_mesh.cpp" -o CMakeFiles/main.dir/cloud_mesh.cpp.s

CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o: ../dependencies/include/Hydra/HydraVSGFExport.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/Hydra/HydraVSGFExport.cpp"

CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/Hydra/HydraVSGFExport.cpp" > CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.i

CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/Hydra/HydraVSGFExport.cpp" -o CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.s

CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o: ../dependencies/include/TinyXML2/tinyxml2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o -c "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/TinyXML2/tinyxml2.cpp"

CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/TinyXML2/tinyxml2.cpp" > CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.i

CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/mihail/Desktop/OpenGL/Plane Project/dependencies/include/TinyXML2/tinyxml2.cpp" -o CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/glad.c.o" \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/Camera.cpp.o" \
"CMakeFiles/main.dir/ShaderProgram.cpp.o" \
"CMakeFiles/main.dir/mesh.cpp.o" \
"CMakeFiles/main.dir/drawcall.cpp.o" \
"CMakeFiles/main.dir/ImportSceneObject.cpp.o" \
"CMakeFiles/main.dir/rain_mesh.cpp.o" \
"CMakeFiles/main.dir/cloud_mesh.cpp.o" \
"CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o" \
"CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/glad.c.o
main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/Camera.cpp.o
main: CMakeFiles/main.dir/ShaderProgram.cpp.o
main: CMakeFiles/main.dir/mesh.cpp.o
main: CMakeFiles/main.dir/drawcall.cpp.o
main: CMakeFiles/main.dir/ImportSceneObject.cpp.o
main: CMakeFiles/main.dir/rain_mesh.cpp.o
main: CMakeFiles/main.dir/cloud_mesh.cpp.o
main: CMakeFiles/main.dir/dependencies/include/Hydra/HydraVSGFExport.cpp.o
main: CMakeFiles/main.dir/dependencies/include/TinyXML2/tinyxml2.cpp.o
main: CMakeFiles/main.dir/build.make
main: /usr/local/lib/libglfw.3.2.dylib
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main

.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd "/Users/mihail/Desktop/OpenGL/Plane Project/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/mihail/Desktop/OpenGL/Plane Project" "/Users/mihail/Desktop/OpenGL/Plane Project" "/Users/mihail/Desktop/OpenGL/Plane Project/build" "/Users/mihail/Desktop/OpenGL/Plane Project/build" "/Users/mihail/Desktop/OpenGL/Plane Project/build/CMakeFiles/main.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend
