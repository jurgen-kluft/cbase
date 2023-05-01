cmake_minimum_required(VERSION 3.23)
project(cbase LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(cbase_test)
add_library(cbase STATIC)
add_library(cunittest STATIC)

# static library 'cbase'
file(GLOB CBASE_SOURCES
        source/main/include/*.h
        source/main/cpp/*.cpp
        )
# set compiler definitions for this library
target_compile_definitions(cbase PUBLIC TARGET_MAC TARGET_DEV TARGET_DEBUG)
# set include directories for this library
target_include_directories(cbase PUBLIC source/main/include)
target_sources(cbase PUBLIC ${CBASE_SOURCES})

# static library 'cunittest'
file(GLOB CUNITTEST_SOURCES
        ../cunittest/source/main/cpp/*.cpp
        )
# set compiler definitions for this library
target_compile_definitions(cunittest PUBLIC TARGET_MAC)
# set include directories for this library
target_include_directories(cunittest PUBLIC ../cunittest/source/main/include)
target_sources(cunittest PUBLIC ${CUNITTEST_SOURCES})

# executable 'cbase_test', depends on 'cbase' and 'cunittest'
file(GLOB CBASE_TEST_SOURCES
        source/test/cpp/*.cpp
        ../cunittest/source/main/cpp/entry/*.cpp
        )
# set compiler definitions for this executable
target_compile_definitions(cbase_test PUBLIC TARGET_MAC TARGET_DEV TARGET_DEBUG)
# set include directories for this executable
target_include_directories(cbase_test PUBLIC source/main/include)
target_include_directories(cbase_test PUBLIC source/test/include)
target_include_directories(cbase_test PUBLIC ../cunittest/source/main/include)
# link libraries to this executable
target_sources(cbase_test PUBLIC ${CBASE_TEST_SOURCES})
target_link_libraries(cbase_test cbase cunittest)


