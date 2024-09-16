cmake_minimum_required(VERSION 3.5)

project(Sudoku VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(include)

# List source files separately using GLOB to handle wildcard patterns
file(GLOB SOURCE_FILES
    src/*.cpp
    src/utils/*.cpp
    src/SudokuButton/*.cpp
)

# List header files separately using GLOB
file(GLOB HEADER_FILES
    include/*.h
    include/utils/*.h
    include/SudokuButton/*.h
)

# Combine source and header files
set(PROJECT_SOURCES
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Sudoku
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Sudoku SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Sudoku
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Sudoku PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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

# Valgrind target with suppressions and logging, without failing on errors
add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --track-origins=yes --gen-suppressions=all --suppressions=${CMAKE_SOURCE_DIR}/.valgrind-suppressions --log-file=${CMAKE_BINARY_DIR}/memcheck.log
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
