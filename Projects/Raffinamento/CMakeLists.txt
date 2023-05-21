cmake_minimum_required(VERSION 3.9)

project(raffinamento LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../externals/Main_Install/eigen3)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../externals/Main_Install/googletest)

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND raffinamento_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND raffinamento_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND teste_raffinamento_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND teste_raffinamento_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND teste_raffinamento_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND teste_raffinamento_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

list(APPEND raffinamento_SOURCES ${raffinamento_sources})
list(APPEND raffinamento_HEADERS ${raffinamento_headers})
list(APPEND raffinamento_INCLUDE ${raffinamento_includes})

# Insert Tests
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

list(APPEND teste_raffinamento_HEADERS ${teste_raffinamento_headers})
list(APPEND teste_raffinamento_INCLUDE ${teste_raffinamento_includes})

# Create executable
################################################################################

add_executable(teste_raffinamento
	main_T.cpp
	${teste_raffinamento_HEADERS}
	${teste_raffinamento_INCLUDE}
	${raffinamento_SOURCES}
    ${raffinamento_HEADERS})

target_link_libraries(teste_raffinamento ${raffinamento_LINKED_LIBRARIES})
target_link_libraries(teste_raffinamento ${teste_raffinamento_LINKED_LIBRARIES})
target_include_directories(teste_raffinamento PRIVATE ${raffinamento_INCLUDE} ${teste_raffinamento_INCLUDE})
target_compile_options(teste_raffinamento PUBLIC -fPIC)

gtest_discover_tests(teste_raffinamento)

add_executable(programma_raffinamento
	main_P.cpp
	${raffinamento_SOURCES}
    ${raffinamento_HEADERS})

target_link_libraries(programma_raffinamento ${raffinamento_LINKED_LIBRARIES})
target_include_directories(programma_raffinamento PRIVATE ${raffinamento_INCLUDE})
target_compile_options(programma_raffinamento PUBLIC -fPIC)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Maglia\ Raffinata)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Maglia\ Raffinata/Test1)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Maglia\ Raffinata/Test2)
