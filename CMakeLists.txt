cmake_minimum_required(VERSION 3.28)
project(eindopdracht)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)

# Define the library target
add_library(eindopdracht_lib
        Board.cpp Board.h
        Drawable.cpp Drawable.h
        Camera.cpp Camera.h
        Graphics.cpp Graphics.h
        stb_image.h
        Texture.cpp Texture.h
        tigl.cpp tigl.h
        ObjModel.cpp ObjModel.h
        Pieces/Pawn.cpp Pieces/Pawn.h
        Pieces/Piece.cpp Pieces/Piece.h
        BoardLogic.cpp BoardLogic.h
        Pieces/Rook.cpp Pieces/Rook.h
        Pieces/Bishop.cpp Pieces/Bishop.h
        Pieces/Knight.cpp Pieces/Knight.h
        Pieces/Queen.cpp Pieces/Queen.h
        Pieces/King.cpp Pieces/King.h
        LegalSpacesBoard.cpp LegalSpacesBoard.h
        Tile.cpp Tile.h
        ChessBot.cpp
        ChessBot.h
)

target_link_libraries(eindopdracht_lib GLEW::GLEW glfw OpenGL::GL sfml-audio)

# Define the main executable target
add_executable(eindopdracht main.cpp)
target_link_libraries(eindopdracht eindopdracht_lib)

## Add Google Test
#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/release-1.11.0.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
## Define the test executable target
#add_executable(test_eindopdracht test_main.cpp test_boardlogic.cpp)
#target_link_libraries(test_eindopdracht GTest::gtest_main eindopdracht_lib)
#
#include(GoogleTest)
#gtest_discover_tests(test_eindopdracht)
#
## Copy resources to binary directory
#file(COPY ${CMAKE_SOURCE_DIR}/Resources DESTINATION ${CMAKE_BINARY_DIR})
