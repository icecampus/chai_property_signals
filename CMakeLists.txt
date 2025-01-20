cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(ImGuiExample VERSION 0.1.0 LANGUAGES CXX C)
set (CMAKE_CXX_STANDARD 17)


 find_package(Boost REQUIRED COMPONENTS signals2)

 find_path(CHAISCRIPT_INCLUDE_DIRS "chaiscript/chaiscript.hpp")
  

    
add_executable(App src/app/main.cpp)
target_link_libraries(App PRIVATE Boost::signals2)
target_include_directories(App PRIVATE ${CHAISCRIPT_INCLUDE_DIRS})
target_precompile_headers(App
  PUBLIC
    src/app/pch.h
)


if(MSVC)
    target_compile_options(App PRIVATE "/bigobj")
endif()