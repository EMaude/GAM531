#ifndef PROJECTIONHANDLER_H
#define PROJECTIONHANDLER_H

#include "vgl.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class ProjectionHandler{
    private:
		GLuint projection_matrix_location_in_vram;

    public:
		void init(GLuint& program);
		void draw();
};
#endif
