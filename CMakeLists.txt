cmake_minimum_required(VERSION 3.5)

project(Sudoku VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

include_directories(include)


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

# Add resources to the project
qt_add_resources(RESOURCES ${RESOURCE_FILE})

# Combine source and header files
set(PROJECT_SOURCES
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Sudoku
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCES} 
    )
else()
    if(ANDROID)
        add_library(Sudoku SHARED
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
    else()
        add_executable(Sudoku
            ${PROJECT_SOURCES}
            ${RESOURCES}
        )
    endif()
endif()

target_link_libraries(Sudoku PRIVATE Qt${QT_VERSION_MAJOR}::Core 
                                     Qt${QT_VERSION_MAJOR}::Widgets 
                                     Qt${QT_VERSION_MAJOR}::Network)


target_compile_options(Sudoku PRIVATE
    -Wall                 # Enable all basic warnings
    -Wextra               # Enable extra warning flags not covered by -Wall
    -Wshadow              # Warn when variable declarations shadow others
    -Wswitch              # Warn about missing cases in switch statements
    -pedantic             # Enforce strict ISO C++ compliance
    -Wformat=2            # Warn about security issues in printf/scanf format strings
    -Wconversion          # Warn for implicit type conversions that may alter a value
    -Wnull-dereference    # Warn if a null pointer dereference is detected
    -Wunused-parameter    # Warn if function parameters are unused
    -Wunreachable-code    # Warn if there is code that will never be executed
    -Wimplicit-fallthrough # Warn when switch case falls through without a break
)

# Add additional strict checks in Debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Sudoku PRIVATE
        -Werror              # Treat all warnings as errors
        -Werror=return-type  # Treat return-type warnings as errors
        -Werror=uninitialized # Treat uninitialized variable warnings as errors
        -Werror=unused-result # Treat unused result warnings as errors (e.g., ignoring return value)
        -Werror=strict-overflow # Treat potential overflow warnings as errors
    )
endif()


# Add sanitizers for Debug builds
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     target_compile_options(Sudoku PRIVATE -fsanitize=address -fsanitize=undefined)
#     target_link_options(Sudoku PRIVATE -fsanitize=address -fsanitize=undefined)
# endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Sudoku)
endif()
set_target_properties(Sudoku PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Valgrind target with suppressions and logging
add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --track-origins=yes --gen-suppressions=all 
                     --suppressions=${CMAKE_SOURCE_DIR}/.valgrind-suppressions 
                     --log-file=${CMAKE_BINARY_DIR}/memcheck.log
    ./${CMAKE_PROJECT_NAME}
    DEPENDS ${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Valgrind with suppressions and logging to memcheck.log..."
)

include(GNUInstallDirs)

install(TARGETS Sudoku
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS Sudoku
    RUNTIME DESTINATION bin
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Sudoku)
endif()
