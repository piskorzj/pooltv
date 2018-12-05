cmake_minimum_required(VERSION 3.10.1)

project(sender)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)
find_package(PkgConfig)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

set(SOURCE_FILES
  main.cpp
)

add_executable(sender ${SOURCE_FILES})
target_include_directories(sender PRIVATE ${GST_INCLUDE_DIRS})
target_compile_options(sender PRIVATE ${GST_CFLAGS})
target_link_libraries(sender PRIVATE ${GST_LIBRARIES} fmt)

install(TARGETS sender
        RUNTIME DESTINATION bin)
