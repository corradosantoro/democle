add_executable (sieve_remote sieve_remote.cpp)

include_directories (../lib)
link_directories (${CMAKE_SOURCE_DIR}/lib)

target_link_libraries (sieve_remote democle)
if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options( sieve PUBLIC "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries( sieve "${CMAKE_THREAD_LIBS_INIT}")
endif()

#install (TARGETS sieve RUNTIME DESTINATION bin)
