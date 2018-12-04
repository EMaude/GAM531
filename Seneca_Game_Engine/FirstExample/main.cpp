
///////////////////////////////////////////////////////////////////////
//
// Scene.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "SceneGraph\Sphere.h"
#include "SceneGraph\SpaceShip.h"
#include <iostream>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint location;
GLuint cam_mat_location;
GLuint proj_mat_location;

//Player motion speed and key controls
float height = 0.8f;
float yaw_speed = 0.1f;
float travel_speed = 60.0f;
float mouse_sensitivity = 0.01f;

//Used for tracking mouse cursor position on screen
int x0 = 0;	
int y_0 = 0;

//Transformation matrices and camera vectors
glm::mat4 model_view;
glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, height);
glm::vec3 forward_vector = glm::vec3(1, 1, 0);
glm::vec3 up_vector = glm::vec3(0, 0, 1);
glm::vec3 side_vector = glm::cross(up_vector, forward_vector);

//Used to measure time between two frames
int oldTimeSinceStart = 0;
int deltaTime;

void draw_level()
{
	ShaderInfo shaders[10] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};


	Sphere s1 = Sphere(glm::mat4(1.0), shaders);
	
	SpaceShip sp = SpaceShip(glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0.7, 0.0)), shaders);

	//To be developed by students:
	//Create an array of SceneNodes and add s1, sp to the array.
	//Name the array to be sceneGraph
	//To draw the wholse scene:
	//Loop through the array
	//{
			//getElement().render();
	//}

}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
}


//
// display
//
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model_view = glm::mat4(1.0);

	up_vector = glm::normalize(up_vector);

	forward_vector = glm::normalize(forward_vector);
	glm::vec3 look_at = glm::vec3(cam_pos.x + forward_vector.x, cam_pos.y + forward_vector.y, height);

	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z), glm::vec3(look_at.x, look_at.y, look_at.z), up_vector);
	glUniformMatrix4fv(cam_mat_location, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 proj_matrix = glm::frustum(-0.01f, +0.01f, -0.01f, +0.01f, 0.01f, 100.0f);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &proj_matrix[0][0]);

	draw_level();

	glFlush();
}


void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		//Moving camera along opposit direction of side vector
		cam_pos += glm::cross(up_vector, forward_vector) * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 'd')
	{
		//Moving camera along side vector
		cam_pos -= glm::cross(up_vector, forward_vector) * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 'w')
	{
		//Moving camera along forward vector. To be more realistic, we use X=V.T equation in physics
		cam_pos += forward_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}
	if (key == 's')
	{
		//Moving camera along backward vector. To be more realistic, we use X=V.T equation in physics
		cam_pos -= forward_vector * travel_speed * ((float)deltaTime) / 1000.0f;
	}
}

void mouse(int x, int y)
{
	int delta_x = x - x0;
	forward_vector = glm::rotate(forward_vector, -delta_x * mouse_sensitivity, up_vector);
	side_vector = glm::cross(up_vector, forward_vector);
	//cout << x0 << " " << x << " " << delta_x << endl;
	x0 = x;
	
}

void idle()
{
	//Calculating the delta time between two frames
	//We will use this delta time when moving forward (in keyboard function)
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	//cout << timeSinceStart << " " << oldTimeSinceStart << " " << deltaTime << endl;
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Final Project");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutIdleFunc(idle);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	
	

}
