cmake_minimum_required(VERSION 3.22)
project(AudioPlayer)

set(CMAKE_CXX_STANDARD 20)

find_package(crossguid CONFIG REQUIRED)

add_executable(AudioPlayer src/main.cpp src/thirdparty/miniaudio.h src/thirdparty/miniaudio.c src/Player.cpp src/Player.h src/audio/AudioEngine.cpp src/audio/AudioEngine.h src/Playback.cpp src/Playback.h)

target_link_libraries(AudioPlayer PRIVATE crossguid)
