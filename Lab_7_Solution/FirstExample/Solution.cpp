
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include "..\SOIL\src\SOIL.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "vgl.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[4];
GLuint location;
GLuint cam_mat_location;
GLuint proj_mat_location;
GLuint program;

const GLuint NumVertices = 530;
const GLuint NumFaces = 1024;
GLfloat vertices[NumVertices][3];
GLuint faces[NumFaces][3];

const GLuint Sphere_NumVertices = 483;
const GLuint Sphere_NumFaces = 960;
GLfloat Sphere_vertices[Sphere_NumVertices][3];
GLuint Sphere_faces[Sphere_NumFaces][3];


//Player motion speed and key controls
float height = 0.8f;
float yaw_speed = 0.1f;
float travel_speed = 40.0f;
float mouse_sensitivity = 0.01f;
float alpha = 0.0f;

//Used for tracking mouse cursor position on screen
int x0 = 0;

//Transformation matrices and camera vectors
glm::mat4 model_view;
glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, height);
glm::vec3 forward_vector = glm::vec3(1, 1, 0);
glm::vec3 up_vector = glm::vec3(0, 0, 1);
glm::vec3 side_vector = glm::cross(up_vector, forward_vector);

//Used to measure time between two frames
int oldTimeSinceStart = 0;
int deltaTime;

//Creating and rendering bunch of objects on the scene to interact with
const int Num_Obstacles = 10;
float obstacle_data[Num_Obstacles][3];

int load(std::string filename, GLfloat vertexArray[][3], GLuint faces[][3]);

//Helper function to generate a random float number within a range
float randomFloat(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

// inititializing buffers, coordinates, setting up pipeline, etc.
void init(void)
{
	glEnable(GL_DEPTH_TEST);
	load("Teapot.obj", vertices, faces);
	load("Sphere.obj", Sphere_vertices, Sphere_faces);

	//Randomizing the position and scale of obstacles

	for (int i = 0; i < Num_Obstacles; i++)
	{
		obstacle_data[i][0] = randomFloat(-50, 50); //X
		obstacle_data[i][1] = randomFloat(-50, 50); //Y
		obstacle_data[i][2] = randomFloat(0.01, 0.1); //Scale
	}

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};


	GLfloat sphere_color[Sphere_NumVertices][3];

	for (int i = 0; i < Sphere_NumVertices; i++)
	{
		sphere_color[i][0] = randomFloat(1, 1);
		sphere_color[i][1] = randomFloat(0, 1);
		sphere_color[i][2] = randomFloat(0, 1);
	}


	GLfloat teapot_color[NumVertices][3];

	for (int i = 0; i < NumVertices; i++)
	{
		teapot_color[i][0] = 1.0;
		teapot_color[i][1] = 1.0;
		teapot_color[i][2] = 0.0;
	}

	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	glGenBuffers(4, Buffers);


	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindAttribLocation(program, 0, "vPosition");

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Sphere_vertices), Sphere_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindAttribLocation(program, 0, "vPosition");

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_color), sphere_color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindAttribLocation(program, 1, "vColor");

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(teapot_color), teapot_color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindAttribLocation(program, 1, "vColor");

	location = glGetUniformLocation(program, "model_matrix");
	cam_mat_location = glGetUniformLocation(program, "camera_matrix");
	proj_mat_location = glGetUniformLocation(program, "projection_matrix");
}


void drawTeapot(float scale)
{
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	model_view = glm::scale(model_view, glm::vec3(scale, scale, scale));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
	for (int i = 0; i < NumFaces; i++)
	{
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces[i]);
	}

}


void drawSphere(float scale)
{
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	model_view = glm::scale(model_view, glm::vec3(scale, scale, scale));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
	for (int i = 0; i < Sphere_NumFaces; i++)
	{
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, Sphere_faces[i]);
	}

}
//Renders level
void draw_level()
{
	//Rendering obstacles obstacles
	for (int i = 0; i < Num_Obstacles; i++)
	{
		model_view = glm::translate(model_view, glm::vec3(obstacle_data[i][0], obstacle_data[i][1], 0.0));
		model_view = glm::rotate(model_view, alpha, glm::vec3(0, 0, 1));
		glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);
		drawTeapot(obstacle_data[i][2]);
		model_view = glm::mat4(1.0);
	}

	for (int i = 0; i < Num_Obstacles; i++)
	{
		model_view = glm::translate(model_view, glm::vec3(obstacle_data[i][0] + 3, obstacle_data[i][1] + 4, 0.0));

		model_view = glm::rotate(model_view, alpha, glm::vec3(0, 0, 1));

		glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

		drawSphere(4 * obstacle_data[i][2]);

		model_view = glm::mat4(1.0);
	}


}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model_view = glm::mat4(1.0);
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	up_vector = glm::normalize(up_vector);

	forward_vector = glm::normalize(forward_vector);
	glm::vec3 look_at = glm::vec3(cam_pos.x + forward_vector.x, cam_pos.y + forward_vector.y, height + forward_vector.z);

	model_view = glm::translate(model_view, glm::vec3(cam_pos.x + forward_vector.x / 8.0f, cam_pos.y + forward_vector.y / 8.0f, cam_pos.z + forward_vector.z / 8.0f));
	model_view = glm::mat4(1.0);

	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(cam_pos.x, cam_pos.y, cam_pos.z), glm::vec3(look_at.x, look_at.y, look_at.z), up_vector);

	glUniformMatrix4fv(cam_mat_location, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 proj_matrix = glm::frustum(-0.01f, +0.01f, -0.01f, +0.01f, 0.01f, 100.0f);
	glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &proj_matrix[0][0]);

	draw_level();
	//alpha += 0.1f;
	glutSwapBuffers();
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
	side_vector = glm::cross(forward_vector, up_vector);
	//cout << "Delta_X" <<  delta_x << endl;
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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Geometry Loading");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutIdleFunc(idle);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();



}
