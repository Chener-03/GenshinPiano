cmake_minimum_required(VERSION 3.23)
project(globleHotKey4J)

set(CMAKE_CXX_STANDARD 20)

set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS" )

set(JAVA_INCLUDE "C:\\Java\\jdk-17.0.2\\include")
set(IMGUI_INCLUDE "./3rdParty/imgui/imgui-master/imgui-master/include")


add_definitions(-DUNICODE -D_UNICODE)

file(GLOB_RECURSE IMGUI_SOURCE "./3rdParty/imgui/imgui-master/imgui-master/source/*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/3rdParty/minhook/src/include
        ${JAVA_INCLUDE}/win32
        ${JAVA_INCLUDE}
        ${IMGUI_INCLUDE})


link_directories(${PROJECT_SOURCE_DIR}/3rdParty/minhook/lib/x64)
ADD_LIBRARY(globleHotKey4J SHARED
        src/cpp/main.cpp src/cpp/MessageProcess.cpp
        src/include/MessageProcess.h
        src/include/Stc.h
        src/include/xyz_chener_javafx_jni_HotKey4J.h src/cpp/HotKey4J.cpp src/include/device.h
        ${IMGUI_SOURCE} src/cpp/tWind.cpp src/include/tWind.h src/include/GameVer.h
        src/include/xyz_chener_javafx_jni_DrawCall.h src/cpp/DrawCall.cpp src/cpp/GameVer.cpp)

#[[add_executable(globleHotKey4J
        src/cpp/main.cpp src/cpp/MessageProcess.cpp
        src/include/MessageProcess.h
        src/include/Stc.h
        src/include/xyz_chener_javafx_jni_HotKey4J.h src/cpp/HotKey4J.cpp src/include/device.h
        ${IMGUI_SOURCE} src/cpp/tWind.cpp src/include/tWind.h src/include/GameVer.h
        src/include/xyz_chener_javafx_jni_DrawCall.h src/cpp/DrawCall.cpp src/cpp/GameVer.cpp)]]

target_link_libraries(globleHotKey4J
        MinHook64.lib
        d3d11.lib
        d3dcompiler.lib
        dxgi.lib
        d2d1.lib
        dwrite.lib
)