# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/skel/roboskel_workspace/sandbox/rn_face

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/skel/roboskel_workspace/sandbox/rn_face/build

# Include any dependencies generated for this target.
include CMakeFiles/fdati.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fdati.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fdati.dir/flags.make

CMakeFiles/fdati.dir/src/fdati.o: CMakeFiles/fdati.dir/flags.make
CMakeFiles/fdati.dir/src/fdati.o: ../src/fdati.cpp
CMakeFiles/fdati.dir/src/fdati.o: ../manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/geometry_msgs/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/sensor_msgs/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/vision_opencv/opencv2/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/roslang/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/roscpp/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/vision_opencv/cv_bridge/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/std_msgs/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/ros/core/rosbuild/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/roslib/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/rosconsole/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/pluginlib/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/message_filters/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/image_common/image_transport/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/std_srvs/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/bullet/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/geometry/angles/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/rospy/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/rostest/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/share/roswtf/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/geometry/tf/manifest.xml
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/geometry/tf/msg_gen/generated
CMakeFiles/fdati.dir/src/fdati.o: /opt/ros/fuerte/stacks/geometry/tf/srv_gen/generated
	$(CMAKE_COMMAND) -E cmake_progress_report /home/skel/roboskel_workspace/sandbox/rn_face/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/fdati.dir/src/fdati.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -DBT_USE_DOUBLE_PRECISION -DBT_EULER_DEFAULT_ZYX -W -Wall -Wno-unused-parameter -fno-strict-aliasing -pthread -std=gnu++0x -o CMakeFiles/fdati.dir/src/fdati.o -c /home/skel/roboskel_workspace/sandbox/rn_face/src/fdati.cpp

CMakeFiles/fdati.dir/src/fdati.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fdati.dir/src/fdati.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -DBT_USE_DOUBLE_PRECISION -DBT_EULER_DEFAULT_ZYX -W -Wall -Wno-unused-parameter -fno-strict-aliasing -pthread -std=gnu++0x -E /home/skel/roboskel_workspace/sandbox/rn_face/src/fdati.cpp > CMakeFiles/fdati.dir/src/fdati.i

CMakeFiles/fdati.dir/src/fdati.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fdati.dir/src/fdati.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -DBT_USE_DOUBLE_PRECISION -DBT_EULER_DEFAULT_ZYX -W -Wall -Wno-unused-parameter -fno-strict-aliasing -pthread -std=gnu++0x -S /home/skel/roboskel_workspace/sandbox/rn_face/src/fdati.cpp -o CMakeFiles/fdati.dir/src/fdati.s

CMakeFiles/fdati.dir/src/fdati.o.requires:
.PHONY : CMakeFiles/fdati.dir/src/fdati.o.requires

CMakeFiles/fdati.dir/src/fdati.o.provides: CMakeFiles/fdati.dir/src/fdati.o.requires
	$(MAKE) -f CMakeFiles/fdati.dir/build.make CMakeFiles/fdati.dir/src/fdati.o.provides.build
.PHONY : CMakeFiles/fdati.dir/src/fdati.o.provides

CMakeFiles/fdati.dir/src/fdati.o.provides.build: CMakeFiles/fdati.dir/src/fdati.o

# Object files for target fdati
fdati_OBJECTS = \
"CMakeFiles/fdati.dir/src/fdati.o"

# External object files for target fdati
fdati_EXTERNAL_OBJECTS =

../bin/fdati: CMakeFiles/fdati.dir/src/fdati.o
../bin/fdati: CMakeFiles/fdati.dir/build.make
../bin/fdati: CMakeFiles/fdati.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../bin/fdati"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fdati.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fdati.dir/build: ../bin/fdati
.PHONY : CMakeFiles/fdati.dir/build

CMakeFiles/fdati.dir/requires: CMakeFiles/fdati.dir/src/fdati.o.requires
.PHONY : CMakeFiles/fdati.dir/requires

CMakeFiles/fdati.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fdati.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fdati.dir/clean

CMakeFiles/fdati.dir/depend:
	cd /home/skel/roboskel_workspace/sandbox/rn_face/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/skel/roboskel_workspace/sandbox/rn_face /home/skel/roboskel_workspace/sandbox/rn_face /home/skel/roboskel_workspace/sandbox/rn_face/build /home/skel/roboskel_workspace/sandbox/rn_face/build /home/skel/roboskel_workspace/sandbox/rn_face/build/CMakeFiles/fdati.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fdati.dir/depend

