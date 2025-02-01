cmake_minimum_required(VERSION 3.10)
project(QRCodeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(CORE_SOURCES
    src/core/input_handler.cpp
    src/core/qr_code_generator.cpp
)

set(RENDER_SOURCES
    src/render/renderer.cpp
)

add_library(QRCodeCore ${CORE_SOURCES})

add_executable(QRCodeMain src/main.cpp)
target_link_libraries(QRCodeMain QRCodeCore)

# Google Test
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add tests
add_executable(QRCodeTests
    tests/test_main.cpp
)
target_link_libraries(QRCodeTests QRCodeCore ${GTEST_LIBRARIES} pthread)
add_test(NAME QRCodeTests COMMAND QRCodeTests)