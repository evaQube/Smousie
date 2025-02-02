cmake_minimum_required(VERSION 3.23)
project(Smousie LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


set(PROJECT_HEADERS 
    include/Utils/Constants.h

    include/Core/AssetManager.h
    include/Core/Game.h
    include/Core/World.h

    include/Gameplay/Fruit.h
    include/Gameplay/Fruit_moving.h
    include/Gameplay/Fruit_static.h
    include/Gameplay/GameManager.h
    include/Gameplay/GameObject.h
    include/Gameplay/Mouse.h
    include/Gameplay/Pointer.h

    include/UI/UIButton.h
    include/UI/UIManager.h
    include/UI/UIScreen.h
    include/UI/UIScreen_Credits.h
    include/UI/UIScreen_GameOver.h
    include/UI/UIScreen_MainMenu.h
    include/UI/UIScreen_PauseMenu.h
)

set(PROJECT_SOURCES 

    src/Core/AssetManager.cpp
    src/Core/Game.cpp
    src/Core/World.cpp

    src/Gameplay/Fruit.cpp
    src/Gameplay/Fruit_moving.cpp
    src/Gameplay/Fruit_static.cpp
    src/Gameplay/GameManager.cpp
    src/Gameplay/Mouse.cpp
    src/Gameplay/Pointer.cpp

    src/UI/UIManager.cpp
    src/UI/UIScreen_Credits.cpp
    src/UI/UIScreen_GameOver.cpp
    src/UI/UIScreen_MainMenu.cpp
    src/UI/UIScreen_PauseMenu.cpp

    src/main.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set Debugger Working Directory
set_property(TARGET ${PROJECT_NAME} PROPERTY 
    VS_DEBUGGER_WORKING_DIRECTORY 
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")

# Copy the data directory after building
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data"
)


target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
