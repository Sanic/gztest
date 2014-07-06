cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(PROJECT_NAME_STR gztest)
project(${PROJECT_NAME_STR} C CXX)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  pkg_check_modules(OGRE OGRE)
endif()   
include_directories(include)

include_directories(${GAZEBO_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})

# Necessary BOOST packages ...
include(FindBoost)
find_package(Boost REQUIRED system filesystem regex thread)
#find_package(Protobuf REQUIRED)
#add_subdirectory(msgs)

# Include gazebo dirs and our msgs dirs in build/
include_directories(
  ${GAZEBO_INCLUDE_DIRS}
)

# Download and compile gtest on the fly ... Thanks to:
# https://github.com/snikulov/google-test-examples 

set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
add_subdirectory(${EXT_PROJECTS_DIR}/gtest)
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})
set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

add_subdirectory(${EXT_PROJECTS_DIR}/json-rpc-cpp)

add_library(TestPlugin SHARED src/TestPlugin.cpp)
target_link_libraries(TestPlugin ${GAZEBO_libraries} jsonrpc)

#add the gztest library
add_library(gztest src/test_helpers.cpp src/test_client.cpp)
target_link_libraries(gztest ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} jsonrpc)

# Build tests
# add_executable(delayed_tests tests/delayed_tests.cpp)
# Download and build googletest (from ext/gtest/ ) before building the test
# add_dependencies(delayed_tests googletest)
# target_link_libraries(delayed_tests pthread)
# 	target_link_libraries(delayed_tests 
# 		${GTEST_LIBS_DIR}/libgtest.a
# 		${GTEST_LIBS_DIR}/libgtest_main.a
# 	)

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
add_dependencies(${PROJECT_TEST_NAME} googletest)
target_link_libraries(${PROJECT_TEST_NAME} pthread ${Boost_LIBRARIES} gztest jsonrpc)
	target_link_libraries(${PROJECT_TEST_NAME} 
		${GTEST_LIBS_DIR}/libgtest.a
		${GTEST_LIBS_DIR}/libgtest_main.a
)
# Add a single test which is executable via "make test"
add_test(test1 ${PROJECT_TEST_NAME})