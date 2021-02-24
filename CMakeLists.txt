cmake_minimum_required(VERSION 3.16)
project(HSRobot)

set(SOURCES
	"src/main.c"
)

find_package(OpenCV REQUIRED)

link_directories($ENV{WEBOTS_HOME}/lib/controller)
set(LIBRARIES ${CMAKE_SHARED_LIBRARY_PREFIX}Controller${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories($ENV{WEBOTS_HOME}/include/controller/c $ENV{WEBOTS_HOME}/include/controller/cpp ${OpenCV_INCLUDE_DIRS})
add_executable(controller ${SOURCES})
target_link_libraries(controller PUBLIC ${LIBRARIES} ${OpenCV_LIBS})
