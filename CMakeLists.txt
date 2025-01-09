cmake_minimum_required (VERSION 2.6)
set (VERSION 0.1)
set (PACKAGE democle)
project (${PACKAGE})

find_package(Threads REQUIRED)

add_subdirectory (lib)
add_subdirectory (tests)

# cmake_minimum_required(VERSION 2.8)
# project( test_max )
# find_package(Threads REQUIRED)

# add_executable( test_max
#   st_camera.cpp
#   reader_420.cpp
#   streamer.cpp
#   config.cpp
#   pipeline.cpp
#   wb_matrix.cpp
#   threshold.cpp
#   undistort.cpp
#   arucomapper.cpp
#   cropper.cpp
#   ball_finder.cpp
#   utils.cpp
#   filter.cpp
#   net_utils.cpp
#   main.cpp )

add_compile_options(-g)

# target_link_libraries( detector ${OpenCV_LIBS} ${YAML_CPP_LIBRARIES} )
# if(THREADS_HAVE_PTHREAD_ARG)
#   target_compile_options( detector PUBLIC "-pthread")
# endif()
# if(CMAKE_THREAD_LIBS_INIT)
#   target_link_libraries( detector "${CMAKE_THREAD_LIBS_INIT}")
# endif()

