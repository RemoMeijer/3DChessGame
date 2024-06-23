#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "tigl.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using tigl::Vertex;

class Drawable {
public:
	std::vector<Vertex> vertices;

public:
	Drawable();

	~Drawable();

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	virtual void init();

	virtual void update(float deltaTime);

	virtual void draw();
};

#endif
