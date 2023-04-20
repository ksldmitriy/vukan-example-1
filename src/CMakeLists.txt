cmake_minimum_required(VERSION 3.13.4)

project(best_program)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-g)

#project

file(GLOB vulkan_test_SRC *.cpp vk/*.cpp)
list(FILTER vulkan_test_SRC EXCLUDE REGEX ${CMAKE_SOURCE_DIR}/\#*.cpp)
list(FILTER vulkan_test_SRC EXCLUDE REGEX ${CMAKE_SOURCE_DIR}/*/\#*.cpp)

add_executable(${PROJECT_NAME} ${vulkan_test_SRC}) 

# libs

link_directories(/usr/lib)

target_link_libraries(${PROJECT_NAME} libvulkan.so libglfw.so)


