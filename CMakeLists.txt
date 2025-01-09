# Define the project and Qt options
cmake_minimum_required(VERSION 3.12)
project(Sudoku VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Collect source and header files
file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/utils/*.cpp
    src/SudokuButton/*.cpp
    Network/src/*.cpp
)

file(GLOB_RECURSE HEADER_FILES
    include/*.h
    include/utils/*.h
    include/SudokuButton/*.h
    Network/include/*.h
)

# Add the Qt resource file
set(RESOURCE_FILE resources.qrc)
qt_add_resources(RESOURCES ${RESOURCE_FILE})

# Combine source and header files
set(PROJECT_SOURCES
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

# Create the target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            ${RESOURCES}
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
    endif()
endif()

# Add include directories AFTER creating the target
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
        include Network/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
)

# Add compile options (unchanged)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wshadow
    -Wswitch
    -pedantic
    -Wformat=2
    -Wconversion
    -Wnull-dereference
    -Wunused-parameter
    -Wunreachable-code
    -Wimplicit-fallthrough
)

# Debug-specific options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Werror
        -Werror=return-type
        -Werror=uninitialized
        -Werror=unused-result
        -Werror=strict-overflow
    )
endif()

# Finalize the executable for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

# Install and valgrind targets (unchanged)
