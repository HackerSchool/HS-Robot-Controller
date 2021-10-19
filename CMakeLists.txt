cmake_minimum_required(VERSION 3.16)
project(HSRobot LANGUAGES CXX)

option(USE_WEBOTS ON)
option(USE_BLUETOOTH OFF)

set(SOURCES
	"src/main.cpp"
	"src/robot/robot.hpp"
    "src/robot/webots_robot.hpp"
    "src/robot/webots_robot.cpp"
    "src/robot/hs_robot.hpp"
    "src/robot/hs_robot.cpp"
)

add_executable(controller ${SOURCES})

# Link OpenCV
find_package(OpenCV REQUIRED)
target_link_libraries(controller PUBLIC ${OpenCV_LIBS})

# Link Webots
if(USE_WEBOTS)
	target_include_directories(controller PUBLIC
		$ENV{WEBOTS_HOME}/include/controller/c
		$ENV{WEBOTS_HOME}/include/controller/cpp
		${OpenCV_INCLUDE_DIRS}
	)
	target_link_libraries(controller PUBLIC ${CMAKE_SHARED_LIBRARY_PREFIX}Controller${CMAKE_SHARED_LIBRARY_SUFFIX})
	target_link_directories(controller PUBLIC $ENV{WEBOTS_HOME}/lib/controller)
	target_compile_definitions(controller PUBLIC USE_WEBOTS)
endif()

# Link bluetooth libraries
if(USE_BLUETOOTH)
	target_link_libraries(controller PUBLIC bluetooth)
	target_compile_definitions(controller PUBLIC USE_BLUETOOTH)
endif()
