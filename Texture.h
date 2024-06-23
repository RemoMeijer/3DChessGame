#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>

class Texture {
	GLuint id;
public:
	explicit Texture(const std::string& fileName);
	void bind();
};

#endif