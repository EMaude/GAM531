#ifndef MODELHANDLER_H
#define MODELHANDLER_H

#include "vgl.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class ModelHandler
{
	GLuint location;

public:
	float rotate_value;
	void init(GLuint program);
	void draw();
};

#endif

