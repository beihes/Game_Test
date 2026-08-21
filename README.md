```CMake

Total install time: 3.4 h
Installed contents are licensed to you by owners. Microsoft is not responsible for, nor does it grant any licenses to, third-party packages.
Some packages did not declare an SPDX license. Check the `copyright` file for each package for more information about their licensing.
Packages installed in this vcpkg installation declare the following licenses:
(BSD-2-Clause OR BSD-3-Clause OR Artistic-1.0 OR GPL-2.0-only OR LGPL-2.0-only)
Apache-2.0
BSD-2-Clause
BSD-3-Clause
ICU
LGPL-2.1-or-later
MIT
MIT-Modern-Variant
Python-2.0
Zlib
blessing
libtiff
ffmpeg provides CMake integration:

  find_package(FFMPEG REQUIRED)
  target_include_directories(main PRIVATE ${FFMPEG_INCLUDE_DIRS})
  target_link_directories(main PRIVATE ${FFMPEG_LIBRARY_DIRS})
  target_link_libraries(main PRIVATE ${FFMPEG_LIBRARIES})

ffmpeg provides pkg-config modules:

  # FFmpeg codec library
  libavcodec

  # FFmpeg device handling library
  libavdevice

  # FFmpeg audio/video filtering library
  libavfilter

  # FFmpeg container format library
  libavformat

  # FFmpeg utility library
  libavutil

  # FFmpeg audio resampling library
  libswresample

  # FFmpeg image rescaling library
  libswscale

The package glm provides CMake targets:

    find_package(glm CONFIG REQUIRED)
    target_link_libraries(main PRIVATE glm::glm)

    # Or use the header-only version
    find_package(glm CONFIG REQUIRED)
    target_link_libraries(main PRIVATE glm::glm-header-only)

The package gtest is compatible with built-in CMake targets:

    enable_testing()

    find_package(GTest CONFIG REQUIRED)
    target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    add_test(AllTestsInMain main)

sdl3 provides CMake targets:

  find_package(SDL3 CONFIG REQUIRED)
  target_link_libraries(main PRIVATE SDL3::SDL3)

imgui provides CMake targets:

  # this is heuristically generated, and may not be correct
  find_package(imgui CONFIG REQUIRED)
  target_link_libraries(main PRIVATE imgui::imgui)

hello-imgui provides CMake integration:

  set(CMAKE_CXX_STANDARD 17)
  find_package(hello-imgui CONFIG REQUIRED)
  # Usage with `hello_imgui_add_app` (recommended)
  hello_imgui_add_app(main my_main.cpp)
  # Usage with `target_link_libraries` (no tooling, no asset deployment)
  target_link_libraries(main PRIVATE hello-imgui::hello_imgui)

    ########################################################################
       !!!!                    WARNING                              !!!!!
       !!!!   Installed hello-imgui without a viable backend        !!!!!
    ########################################################################

    When installing hello-imgui, you should specify:

     - At least one (or more) rendering backend (OpenGL3, Metal, Vulkan, DirectX11, DirectX12)
       Make your choice according to your needs and your target platforms, between:
          opengl3-binding              # This is the recommended choice, especially for beginners
          metal-binding                # Apple only, advanced users only
          experimental-vulkan-binding  # Advanced users only
          experimental-dx11-binding    # Windows only, still experimental
          experimental-dx12-binding    # Windows only, advanced users only, still experimental

     - At least one (or more) platform backend (Glfw3*):
       Make your choice according to your needs and your target platforms, between:
          glfw-binding
       *) This port currently does not offer an SDL platform backend.

    For example, you could use:
        vcpkg install "hello-imgui[opengl3-binding,glfw-binding]"

    ########################################################################
       !!!!                    WARNING                              !!!!!
       !!!!   Installed hello-imgui without a viable backend        !!!!!
    ########################################################################

icu provides pkg-config modules:

  # International Components for Unicode: Internationalization library
  icu-i18n

  # International Components for Unicode: Stream and I/O Library
  icu-io

  # International Components for Unicode: Common and Data libraries
  icu-uc

implot provides CMake targets:

  # this is heuristically generated, and may not be correct
  find_package(implot CONFIG REQUIRED)
  target_link_libraries(main PRIVATE implot::implot)

implot3d provides CMake targets:

  # this is heuristically generated, and may not be correct
  find_package(implot3d CONFIG REQUIRED)
  target_link_libraries(main PRIVATE implot3d::implot3d)

nativefiledialog-extended provides CMake targets:

  # this is heuristically generated, and may not be correct
  find_package(nfd CONFIG REQUIRED)
  target_link_libraries(main PRIVATE nfd::nfd)

sdl3-image provides CMake targets:

  find_package(SDL3_image CONFIG REQUIRED)
  target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

sdl3-image provides pkg-config modules:

  # image loading library for Simple DirectMedia Layer
  sdl3-image

sdl3-mixer provides CMake targets:

  find_package(SDL3_mixer CONFIG REQUIRED)
  target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer::SDL3_mixer-static>)

sdl3-net provides CMake targets:

  find_package(SDL3_net CONFIG REQUIRED)
  target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:SDL3_net::SDL3_net>,SDL3_net::SDL3_net,SDL3_net::SDL3_net-static>)

sdl3-ttf provides CMake targets:

  find_package(SDL3_ttf CONFIG REQUIRED)
  target_link_libraries(main PRIVATE SDL3_ttf::SDL3_ttf)

The package spdlog provides CMake targets:

    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(main PRIVATE spdlog::spdlog)

    # Or use the header-only version
    find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(main PRIVATE spdlog::spdlog_header_only)

sqlitecpp provides CMake targets:

  # this is heuristically generated, and may not be correct
  find_package(SQLiteCpp CONFIG REQUIRED)
  target_link_libraries(main PRIVATE SQLiteCpp)

```