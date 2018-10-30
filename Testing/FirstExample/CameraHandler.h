
#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include "vgl.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class CameraHandler{
    private:
    static glm::vec3 m_pos;
    static GLuint cam_matrix_location_in_vram;

    public:
    static void init(GLuint& program);
    static void draw();
	static void update(glm::vec3);
    static glm::vec3 getPos();
};

#endif