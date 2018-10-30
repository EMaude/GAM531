
#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H

#include "vgl.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"


class CameraHandler{
    private:
    glm::vec3 m_pos;
    GLuint cam_matrix_location_in_vram;

    public:
	CameraHandler();
    void init(GLuint& program);
    void draw();
	void update(glm::vec3);
    glm::vec3 getPos();
};

#endif