set(CMAKE_PROJECT_NAME font)

file(GLOB_RECURSE font_INCLUDE
    "*.h"
    "*.hpp"
)

# 将源代码添加到此项目的可执行文件。
add_library(${CMAKE_PROJECT_NAME} INTERFACE ${font_INCLUDE} )

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/inc")

# 设置 C++20 标准
target_compile_features(${CMAKE_PROJECT_NAME} INTERFACE c_std_17 cxx_std_20)

target_sources(${CMAKE_PROJECT_NAME} INTERFACE ${font_INCLUDE})

# 在调试构建中定义 _DEBUG 宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} INTERFACE
        $<$<CONFIG:Debug>:_DEBUG>
    )
endif()