set(CMAKE_PROJECT_NAME "main")

file(GLOB_RECURSE MAIN_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE MAIN_SOURCES
    "*.cpp"
    "*.c"
    "*.rc"
)

# 将源代码添加到此项目的可执行文件。
add_executable(${CMAKE_PROJECT_NAME} ${MAIN_INCLUDE} ${MAIN_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    app engine
    datetime sdl
    hello-imgui::hello_imgui imgui::imgui implot::implot spdlog::spdlog
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