//
// Created by remo on 6/19/24.
//
#include "Graphics.h"
#include "LegalSpacesBoard.h"

using tigl::Vertex;

std::list<Drawable *> drawables;
auto boardlogic = new BoardLogic;
int waitCounter = 0; // How long the camera should wait after a move
glm::vec3 whiteCamPos = glm::vec3(-7.28f, -8.0f, -12.0f);
glm::vec3 blackCamPos = glm::vec3(-7.28f, -8.0f, 2.0f);

Graphics::Graphics() = default;

Graphics::~Graphics() = default;

void Graphics::mainLoop() {
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1000, 800, "Chess game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

    while (!glfwWindowShouldClose(window)) {
        update();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Graphics::init() {
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

    // Create camera
    camera = new Camera();
    camera->moveTo(whiteCamPos, glm::vec3(45, 0, 0));

    glfwSetMouseButtonCallback(window, Graphics::handleMouseCallback);

    // Create board
    auto *board = new Board(new Texture("Resources/chessboard.png"));
    board->position = glm::vec3(7.28f, 0, 5.0f); // Board position
    board->scale = glm::vec3(5); // Scale the bord
    drawables.push_back(board);

    // Create legal spaces board
    legalSpacesBoard = new LegalSpacesBoard(BOARD_WIDTH, BOARD_HEIGHT, 0.6165f);
    drawables.push_back(legalSpacesBoard);


    std::vector<std::vector<Pawn *> > boardGrid; // 2D vector to represent the pieces on the chessboard

    createBoardPieces();

    for (auto &d: drawables) {
        d->init();
    }
}

// This method tries to mimmick ray casting but failes enormously, so please ignore the messy code in here (It works in this case)
void Graphics::handleMouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (boardlogic->singlePlayer && !boardlogic->whiteTurn) {
            return;
        }
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Some inaccurate ray casting
        float x = (2.0f * xpos) / width - 1.0f;
        float y = 1.0f - (2.0f * ypos) / height;
        float z = 1.0f;
        glm::vec3 ndc(x, y, z);

        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float) viewport[3], 0.01f,
                                                100.0f);
        glm::mat4 projectionMatrix = projection; // Your projection matrix

        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);

        glm::mat4 viewMatrixSetup(1.0f);
        viewMatrixSetup = glm::rotate(viewMatrixSetup, rotation.x, glm::vec3(1, 0, 0));
        viewMatrixSetup = glm::rotate(viewMatrixSetup, rotation.y, glm::vec3(0, 1, 0));
        viewMatrixSetup = glm::rotate(viewMatrixSetup, rotation.z, glm::vec3(0, 0, 1));
        viewMatrixSetup = glm::translate(viewMatrixSetup, position);

        glm::mat4 viewMatrix = viewMatrixSetup; // Your view matrix

        glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
        glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);

        // yeah... dont ask
        glm::vec4 eyeCoords = inverseProjectionMatrix * glm::vec4(ndc, 1.0f);
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

        glm::vec4 worldCoords = inverseViewMatrix * eyeCoords;
        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));
        glm::vec3 cameraPosition = glm::vec3(-7.28f, -8.0f, -12.0f);

        glm::vec3 planeNormal(0.0f, 0.0f, 1.0f);
        float planeD = 0.0f;

        float t = -(glm::dot(cameraPosition, planeNormal) + planeD) / glm::dot(rayDirection, planeNormal);
        glm::vec3 intersectionPoint = cameraPosition + t * rayDirection;

        float tileSize = 2.0f;

        float gridX = (intersectionPoint.x / tileSize);
        float gridY = (intersectionPoint.y / tileSize);

        gridX = gridX * -1;
        gridY = gridY * -1;

        // End of inaccurate ray casting

        // Weird code to make ray casting accurate
        // Some voodoo gridY coversion
        gridY = (gridY < 1.55f ? 0 : (gridY - 1.55f) * 2.0f);
        gridY = gridY < 1 && gridY > 0 ? 1 : gridY;
        gridY = gridY > 6.7 && gridY < 8.3 ? 7 : gridY;
        int gridYInt = gridY;

        // Some voodoo gridX coversion
        gridX = gridX < 0 ? 0 : gridX * 1.04f + 0.2f;
        float adjustmentYFactor = 1.73 * (gridY / 7);
        auto result = gridX < 4.0
                          ? std::max(0.0f, std::min(
                                         adjustmentYFactor * (
                                             1.0f - (gridX - adjustmentYFactor) / (4.0f - adjustmentYFactor)),
                                         adjustmentYFactor))
                          : std::max(0.0f, adjustmentYFactor * ((gridX - 4.0f) / (6.0f - 4.0f)));

        gridX = gridX < 4 ? gridX - result : gridX + result;
        int gridXint = gridX;

        // Grid conversion cheating for turns
        if (boardlogic->whiteTurn) {
            gridYInt = (gridYInt - 7) * -1;
        } else {
            gridXint = (gridXint - 7) * -1;
        }

        // Boundary check
        gridXint = gridXint > 7 ? 7 : gridXint < 0 ? 0 : gridXint;

        Graphics::handleClickCoords(gridXint, gridYInt);
    }
}

void Graphics::createBoardPieces() {

    Piece::loadModels();

    // Pawn
    for (int i = 0; i < 8; i++) {
        glm::ivec2 gridPosBlack(i, 1);
        Piece *pawnBlack = new Pawn(this, gridPosBlack, BLACK);
        boardlogic->AddPiece(pawnBlack);
        drawables.push_back(pawnBlack);

        glm::ivec2 gridPosWhite(i, 6);
        Piece *pawnWhite = new Pawn(this, gridPosWhite, WHITE);
        boardlogic->AddPiece(pawnWhite);
        drawables.push_back(pawnWhite);
    }

    // Rook
    int x = 7;
    int y = 0;
    for (int i = 0; i < 2; i++) {
        glm::ivec2 gridPos(y, 0);
        Piece *rook = new Rook(this, gridPos, BLACK);
        boardlogic->AddPiece(rook);
        drawables.push_back(rook);

        glm::ivec2 gridPosWhite(x, 7);
        Piece *rookWhite = new Rook(this, gridPosWhite, WHITE);
        boardlogic->AddPiece(rookWhite);
        drawables.push_back(rookWhite);

        // Switch corners
        x = 0;
        y = 7;
    }

    // Knight
    x = 6;
    y = 1;
    for (int i = 0; i < 2; i++) {
        glm::ivec2 gridPos(y, 0);
        Piece *knight = new Knight(this, gridPos, BLACK);
        boardlogic->AddPiece(knight);
        drawables.push_back(knight);

        glm::ivec2 gridPosWhite(x, 7);
        Piece *knightWhite = new Knight(this, gridPosWhite, WHITE);
        boardlogic->AddPiece(knightWhite);
        drawables.push_back(knightWhite);

        // Switch sides
        x = 1;
        y = 6;
    }

    // Bishop
    x = 5;
    y = 2;
    for (int i = 0; i < 2; i++) {
        glm::ivec2 gridPos(y, 0);
        Piece *bishop = new Bishop(this, gridPos, BLACK);
        boardlogic->AddPiece(bishop);
        drawables.push_back(bishop);

        glm::ivec2 gridPosWhite(x, 7);
        Piece *bishopWhite = new Bishop(this, gridPosWhite, WHITE);
        boardlogic->AddPiece(bishopWhite);
        drawables.push_back(bishopWhite);

        // Switch sides
        x = 2;
        y = 5;
    }

    // Queen
    glm::ivec2 gridPos(3, 0);
    Piece *queen = new Queen(this, gridPos, BLACK);
    boardlogic->AddPiece(queen);
    drawables.push_back(queen);

    glm::ivec2 gridPos2(3, 7);
    Piece *queen2 = new Queen(this, gridPos2, WHITE);
    boardlogic->AddPiece(queen2);
    drawables.push_back(queen2);

    // King
    glm::ivec2 gridPos3(4, 0);
    Piece *king = new King(this, gridPos3, BLACK);
    boardlogic->AddPiece(king);
    drawables.push_back(king);

    glm::ivec2 gridPos4(4, 7);
    Piece *king2 = new King(this, gridPos4, WHITE);
    boardlogic->AddPiece(king2);
    drawables.push_back(king2);
}

void Graphics::handleClickCoords(int x, int y) {
    switch (boardlogic->currentState) {
        case SELECTING:
            boardlogic->selectPiece(x, y);
            break;
        case MOVING:
            boardlogic->makeMove(x, y);
            break;
        default:
            std::cout << "Board state error(?) How'd ya manage this?" << std::endl;
        // Fix error
            boardlogic->currentState = SELECTING;
    }
}


void Graphics::update() {
    // Calculate time between this frame and last frame
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - timeLastFrame;
    timeLastFrame = currentTime;

    if (boardlogic->singlePlayer && !boardlogic->whiteTurn) {
        if (waitCounter > 80) {
            boardlogic->makeBotMove();
            waitCounter = 0;
        }
        waitCounter++;
    }


    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            legalSpacesBoard->getTile(x, y).setHighlight(false);
        }
    }

    for (const auto &move: boardlogic->validTiles) {
        legalSpacesBoard->getTile(move.x, move.y).setHighlight(true);
    }

    for (const auto &move: boardlogic->captureTiles) {
        legalSpacesBoard->getTile(move.x, move.y).setHighlight(true);
    }


    if (boardlogic->turnChanged && !boardlogic->singlePlayer) {
        if (waitCounter > 80) {
            boardlogic->turnChanged = false;
            if (boardlogic->whiteTurn) {
                camera->animateTo(whiteCamPos, glm::vec3(45, 0, 0));
            } else {
                camera->animateTo(blackCamPos, glm::vec3(45, 180, 0));
            }
            waitCounter = 0;
        }
        waitCounter++;
    }

    camera->update(deltaTime);

    // Update all drawables
    for (auto &d: drawables) {
        d->update(deltaTime);
    }
}

void Graphics::draw() {
    glClearColor(0.0941f, 0.1451f, 0.5334f, 0.4f); // Bg color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float) viewport[3], 0.01f, 100.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getViewMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);
    tigl::shader->enableAlphaTest(true);
    tigl::shader->enableColorMult(true);
    tigl::shader->enableLighting(true);

    // lighting definitions
    tigl::shader->setLightPosition(0, glm::vec3(1.0f, 20.0f, 0.0f));
    tigl::shader->setLightAmbient(0, glm::vec3(0.2f, 0.2f, 0.2f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.8f, 0.8f, 0.8f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw all drawables
    for (auto &d: drawables) {
        d->draw();
    }
}
