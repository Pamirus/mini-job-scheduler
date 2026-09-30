cmake_minimum_required(VERSION 3.16)

project(mini-job-scheduler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mini-job-scheduler main.cpp
    Job.h Job.cpp
    JobQueue.h JobQueue.cpp
    Worker.h Worker.cpp
    Scheduler.h Scheduler.cpp
    CommandLineInterface.h CommandLineInterface.cpp
    Commands.h Commands.cpp
)

target_compile_options(mini-job-scheduler PRIVATE -Wall -Wextra -Wpedantic)

include(GNUInstallDirs)
install(TARGETS mini-job-scheduler
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
