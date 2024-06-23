#include "Board.h"


Board::Board(Texture* tex) 
{
	texture = tex;
}

Board::~Board()
{
}

void Board::init()
{
	// Bind texture
	texture->bind();

	// Make single quad
	vertices.push_back(Vertex::PT(glm::vec3(-1, 0, 1), glm::vec2(0, 0)));
	vertices.push_back(Vertex::PT(glm::vec3(-1, 0, -1), glm::vec2(0, 1)));
	vertices.push_back(Vertex::PT(glm::vec3(1, 0, -1), glm::vec2(1, 1)));
	vertices.push_back(Vertex::PT(glm::vec3(1, 0, 1), glm::vec2(1, 0)));
}

void Board::draw()
{
	Drawable::draw();
	texture->bind();
	tigl::shader->setColorMult(glm::vec4(1, 1, 1, 1));
	tigl::drawVertices(GL_QUADS, vertices);
}
