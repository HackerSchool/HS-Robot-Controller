cmake_minimum_required(VERSION 3.16)
project(HSRobot LANGUAGES CXX)

option(USE_WEBOTS ON)
option(USE_BLUETOOTH OFF)
option(SHOW_OPENCV_WINDOWS ON)
option(BUILD_ONLY_DOCS OFF)

if(NOT BUILD_ONLY_DOCS)
    set(SOURCES
        "src/main.cpp"

        "src/robot/robot.hpp"
        "src/robot/webots_robot.hpp"
        "src/robot/webots_robot.cpp"
        "src/robot/hs_robot.hpp"
        "src/robot/hs_robot.cpp"

        "src/behaviour/behaviour.hpp"
        "src/behaviour/behaviour.cpp"
        "src/behaviour/manager.hpp"
        "src/behaviour/manager.cpp"
        "src/behaviour/manual.hpp"
        "src/behaviour/manual.cpp"
        "src/behaviour/line_follower.hpp"
        "src/behaviour/line_follower.cpp"

        "src/app/app.hpp"
        "src/app/app.cpp"
        "src/app/webots_app.hpp"
        "src/app/webots_app.cpp"
    )

    add_executable(controller ${SOURCES})
    set_property(TARGET controller PROPERTY CXX_STANDARD 17)
    if(SHOW_OPENCV_WINDOWS)
        target_compile_definitions(controller PUBLIC SHOW_OPENCV_WINDOWS)
    endif()

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
endif()

# Doxygen documentation
find_package(Doxygen COMPONENTS dot)

if (Doxygen_FOUND)
    set(DOXYGEN_SHOW_NAMESPACES YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    doxygen_add_docs(
        doxygen
        "src"
        COMMENT "Generate documentation"
    )
endif()
