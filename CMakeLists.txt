cmake_minimum_required(VERSION 3.30)
project(MONKEY3)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)

FetchContent_MakeAvailable(SFML)

add_executable(MONKEY3 main.cpp
        uiElement.cpp
        uiElement.hpp
        randomWords.hpp
        randomWords.cpp
        gameStatus.hpp
        BestScores.hpp
        bestScores.cpp
        events.hpp
        events.cpp
        gameSave.hpp
        gameSave.cpp
)

target_link_libraries(MONKEY3 PRIVATE SFML::Graphics SFML::Audio)