#include "ProjectionHandler.h"

void ProjectionHandler::init()
{
    projection_matrix_location_in_vram = glGetUniformLocation(program, "projection_matrix");
}

void ProjectionHandler::draw()
{
    glm::mat4 projection_matrix = glm::frustum(-0.01f, +0.01f, -0.01f, +0.01f, 0.01f, 10.0f);
	glUniformMatrix4fv(projection_matrix_location_in_vram, 1, GL_FALSE, &projection_matrix[0][0]);
}