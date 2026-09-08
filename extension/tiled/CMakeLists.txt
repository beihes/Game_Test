set(CMAKE_PROJECT_NAME tiled)

file(GLOB_RECURSE TILED_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE TILED_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} STATIC ${TILED_INCLUDE} ${TILED_SOURCES})

find_package(base64 CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(SDL3_net CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(ZLIB CONFIG REQUIRED)

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    aklomp::base64 nlohmann_json::nlohmann_json spdlog::spdlog ZLIB::ZLIB
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