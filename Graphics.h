//
// Created by remo on 6/19/24.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "tigl.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

#include "Board.h"
#include "BoardLogic.h"
#include "LegalSpacesBoard.h"
#include "ObjModel.h"
#include "Texture.h"
#include "Pieces/Bishop.h"
#include "Pieces/King.h"
#include "Pieces/Knight.h"
#include "Pieces/Pawn.h"
#include "Pieces/Queen.h"
#include "Pieces/Rook.h"

static Camera* camera{};
glm::vec3 getCursorWorldRay();

class Graphics {
    GLFWwindow* window{};
    double timeLastFrame = 0;

public:
    Graphics();
    ~Graphics();

    LegalSpacesBoard* legalSpacesBoard{};

    void mainLoop();
    void init();

    void createBoardPieces();
    static void handleClickCoords(int x, int y);
    static void handleMouseCallback(GLFWwindow* window, int button, int action, int mods);

    void update();
    void draw();
};

#endif
