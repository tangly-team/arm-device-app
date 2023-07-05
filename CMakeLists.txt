cmake_minimum_required(VERSION 3.25)
project(vinci
        VERSION 0.0.1
        DESCRIPTION "vinci platform C++ library")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

option(BUILD_RELEASE "Build with static code checker and doxygen documentation" OFF)

if (BUILD_RELEASE)
    find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
    if (CMAKE_CXX_CPPCHECK)
        list(APPEND CMAKE_CXX_CPPCHECK
                "--enable=all"
                "--suppress=*:${CMAKE_CURRENT_SOURCE_DIR}/src/doctest/doctest.h")
    endif (CMAKE_CXX_CPPCHECK)
endif (BUILD_RELEASE)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_EXCLUDE_PATTERNS "doctest.h")
    doxygen_add_docs(docs
            ${CMAKE_SOURCE_DIR}
            COMMENT "Generate Doxygen Documentation")
else (DOXYGEN_FOUND)
    message("Doxygen need to be installed on your workstation to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

include_directories(src/vinci-cmsis-simulation/inc)

add_executable(Vinci
        src/main.cpp
        src/vinci/inc/Actor.h src/vinci/inc/ConcurrentQueue.h src/vinci/inc/TimerMgr.h
        src/vinci/inc/Message.h src/vinci/inc/MessagePool.h
        src/vinci-cpp/inc/ActorCpp.h src/vinci-cpp/inc/ConcurrentQueueCpp.h src/vinci-cpp/inc/MsgPoolCpp.h
        src/vinci-cmsis/inc/ActorCmsis.h src/vinci-cmsis/inc/ConcurrentQueueCmsis.h src/vinci-cmsis/inc/MsgPoolCmsis.h
        src/sal-cmsis/inc/Messages.h src/sal-cmsis/inc/TimerMgrSal.h
        src/sal-cmsis/inc/ValveOnOff.h src/sal-cmsis/inc/PressureSensor.h src/sal-cmsis/inc/Led.h
        src/vinci-cmsis-simulation/inc/cmsis_os2.h src/vinci-cmsis-simulation/inc/Simulator.h
        )

add_executable(VinciTest
        src/doctest/doctest.h
        src/vinci-tests/VinciTests.cpp
        )
