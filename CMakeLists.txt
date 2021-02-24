cmake_minimum_required(VERSION 3.16)
project(HSRobot LANGUAGES CXX)

option(USE_WEBOTS ON)

set(SOURCES
	"src/main.cpp"
	"src/robot.hpp"
	"src/line.hpp"
	"src/line.cpp"
)

if(USE_WEBOTS)
	set(SOURCES ${SOURCES} "src/wb_robot.cpp")
else()
	set(SOURCES ${SOURCES} "src/hs_robot.cpp")
endif()

find_package(OpenCV REQUIRED)

add_executable(controller ${SOURCES})

target_link_libraries(controller PUBLIC ${OpenCV_LIBS})
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
