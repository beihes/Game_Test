set(CMAKE_PROJECT_NAME datetime)

file(GLOB_RECURSE DATETIME_INCLUDE
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE DATETIME_SOURCES
    "*.cpp"
    "*.c"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} STATIC ${DATETIME_INCLUDE} ${DATETIME_SOURCES})

find_package(SDL3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    spdlog::spdlog
    SDL3::SDL3
)

# 设置 C++20 标准
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

# 在调试构建中定义 _DEBUG 宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:_DEBUG>
    )
endif()