# CMakeList.txt: STL-IMCS 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(CMAKE_PROJECT_NAME box2d_test)

file(GLOB_RECURSE TEST_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE TEST_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_executable(${CMAKE_PROJECT_NAME} ${TEST_INCLUDE} ${TEST_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
    datetime tiled

    # GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
    aklomp::base64 box2d::box2d imgui::imgui implot::implot nlohmann_json::nlohmann_json spdlog::spdlog ZLIB::ZLIB
    SDL3::SDL3 $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>
)

# 设置 C++20 标准
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

# 在调试构建中定义 _DEBUG 宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
    )
endif()
