# Only build toolchain supported now is mingw32 on windows.

cmake_minimum_required(VERSION 3.1...3.15)
project(opengl-raymarcher)

add_executable(raymarcher
  src/main.cpp src/shader.cpp src/shader.hpp src/camera.cpp src/camera.hpp
  src/glad.c include/glad/glad.h include/KHR/khrplatform.h
  )
target_include_directories(raymarcher PUBLIC include c:/mingw64/include)

# Add here the directory of sdl2.lib
target_link_directories(raymarcher PUBLIC C:/mingw64/lib)

## Use this one if you want to use mingw32-make and g++ to compile
target_link_libraries(raymarcher -lmingw32 -lSDL2main -lSDL2)
target_link_options(raymarcher PUBLIC "LINKER:-subsystem,console")

## cpp std 17 features
target_compile_features(raymarcher PUBLIC cxx_std_17)


# Hack to run a command every build
add_custom_target(do_always ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/lib/SDL2.dll
  ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/test.glsl
  ${CMAKE_CURRENT_BINARY_DIR}
  )

target_compile_options(raymarcher PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
