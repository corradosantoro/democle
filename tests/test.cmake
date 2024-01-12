add_executable (test test.cpp)

include_directories (../lib)
link_directories (${CMAKE_SOURCE_DIR}/lib)

target_link_libraries (test democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( test PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( test "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS test RUNTIME DESTINATION bin)
