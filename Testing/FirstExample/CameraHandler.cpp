#include "CameraHandler.h"

void CameraHandler::init()
{
    cam_matrix_location_in_vram = glGetUniformLocation(program, "camera_matrix");
}

void CameraHandler::draw()
{
    //Setting up camera matrix and initialize the camera_matrix in VRAM	
	glm::mat4 camera_matrix = glm::lookAt(m_pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(cam_matrix_location_in_vram, 1, GL_FALSE, &camera_matrix[0][0]);
}

void CameraHandler::update(glm::vec3 pos)
{
    m_pos = pos;
}

glm::vec3 CameraHandler::getPos()
{
    return m_pos;
}