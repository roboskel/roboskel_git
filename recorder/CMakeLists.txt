cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
#rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
#target_link_libraries(${PROJECT_NAME} another_library)
#rosbuild_add_boost_directories()
#rosbuild_link_boost(${PROJECT_NAME} thread)
#rosbuild_add_executable(example examples/example.cpp)
#target_link_libraries(example ${PROJECT_NAME})
include_directories ("/usr/include/openni/") 
rosbuild_add_executable(recorder src/recorder.cpp)
rosbuild_add_executable(record_skel src/record_skel.cpp)
rosbuild_add_executable(record_hok src/record_hok.cpp)
rosbuild_add_executable(gp2 src/gp2.cpp)
rosbuild_add_executable(tf_test src/tf_test.cpp)

#rosbuild_add_executable(gp_2 src/gp_2.cpp)
#rosbuild_add_executable(rec src/rec.cpp)
#rosbuild_add_executable(rec2 src/rec2.cpp)
rosbuild_add_executable(rec3 src/rec3.cpp)
rosbuild_add_compile_flags(rec3 -std=gnu++0x)
rosbuild_add_compile_flags(record_hok -std=gnu++0x)
rosbuild_add_compile_flags(tf_test -std=gnu++0x)

rosbuild_add_compile_flags(record_skel -std=gnu++0x)


rosbuild_add_boost_directories()
rosbuild_link_boost(recorder  filesystem system)
rosbuild_link_boost(record_hok  filesystem system)
rosbuild_link_boost(record_skel  filesystem system)
rosbuild_link_boost(tf_test  filesystem system)
#rosbuild_link_boost(rec  filesystem system)
#rosbuild_link_boost(rec2  filesystem system)
rosbuild_link_boost(rec3  filesystem system)
