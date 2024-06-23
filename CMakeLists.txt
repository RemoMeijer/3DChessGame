cmake_minimum_required(VERSION 3.28)
project(eindopdracht)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(eindopdracht
        Board.cpp Board.h
        Drawable.cpp Drawable.h
        Camera.cpp Camera.h
        Graphics.cpp Graphics.h
        main.cpp stb_image.h
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
        LegalSpacesBoard.cpp
        LegalSpacesBoard.h
        Tile.cpp
        Tile.h
)
target_link_libraries(eindopdracht GLEW::GLEW glfw OpenGL::GL dl pthread X11 sfml-audio)