# CMakeList.txt: STL-IMCS 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(CMAKE_PROJECT_NAME engine)

file(GLOB_RECURSE ENGINE_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE ENGINE_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} STATIC ${ENGINE_INCLUDE} ${ENGINE_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    app
    datetime font raii theme tiled
    EnTT::EnTT imgui::imgui implot3d::implot3d spdlog::spdlog
    SDL3::SDL3 $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static> $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static> $<IF:$<TARGET_EXISTS:SDL3_net::SDL3_net>,SDL3_net::SDL3_net,SDL3_net::SDL3_net-static> SDL3_ttf::SDL3_ttf
)

# 设置 C++20 标准
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

# 在调试构建中定义 _DEBUG 宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
    )
endif()

# TODO: 如有需要，请添加测试并安装目标。
