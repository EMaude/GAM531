#include "InputHandler.h"


void InputHandler::keyboard(unsigned char key, int x, int y)
{
    glm::vec3 pos = CameraHandler::getPos();

    if (key == '+')
	{
		pos.z -= 0.01f;
	}
	if (key == '-')
	{
		pos.z += 0.01f;
	}

	if (key == 'a')
	{
		pos.x -= 0.01;
	}

	if (key == 'd')
	{
		pos.x += 0.01;
	}

	if (key == 'w')
	{
		pos.y -= 0.01;
	}

	if (key == 's')
	{
		pos.y += 0.01;
	}

    CameraHandler::update(pos);
    
	glutPostRedisplay();
}