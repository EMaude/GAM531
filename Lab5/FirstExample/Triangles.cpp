
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
#include "SOIL.h"

#include <iostream>

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[3];
GLuint location;
GLuint location2;
GLuint location3;

GLuint texture[1];

const GLuint NumVertices = 24;

float translate_value = 0;
float rotate_value = 0.5;
float camera_distance = 2.0;

float camz = 2.0f;
float camx = 0.0f;
float camy = 0.0f;


void drawCube()
{
	GLubyte top_face[] = { 0, 1, 2, 3 };
	GLubyte bottom_face[] = { 4, 5, 6, 7 };
	GLubyte left_face[] = { 8, 9, 10, 11 };
	GLubyte right_face[] = { 12, 13, 14, 15 };
	GLubyte front_face[] = { 16, 17, 18, 19 };
	GLubyte back_face[] = { 20, 21, 22, 23 };
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, top_face);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, bottom_face);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, left_face);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, right_face);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, front_face);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, back_face);
}



//---------------------------------------------------------------------
//
// init
//


void init(void)
{
	glEnable(GL_DEPTH_TEST); //Enable 3d texture depth

	GLfloat textureCoordinates[24][2] = {
		//top 1
		0.0f, 0.0f,
		0.3f, 0.0f,
		0.3f, 0.5f,
		0.0f, 0.5f,

		//bottom 2
		0.3f, 0.0f,
		0.6f, 0.0f,
		0.6f, 0.5f,
		0.3f, 0.5f,

		//left 3
		0.6f, 0.0f,
		1.0f, 0.0f,
		1.0f, 0.5f,
		0.6f, 0.5f,

		//right 4
		0.0f, 0.5f,
		0.3f, 0.5f,
		0.3f, 1.0f,
		0.0f, 1.0f,

		//front 5
		0.3f, 0.5f,
		0.6f, 0.5f,
		0.6f, 1.0f,
		0.38f, 1.0f,

		//back 6
		0.6f, 0.5f,
		1.0f, 0.5f,
		1.0f, 1.0f,
		0.6f, 1.0f,
		
	};



	glGenTextures(100, texture);

	GLint width, height;
	unsigned char* image = SOIL_load_image("myDice.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (!image)
	{
		std::cout << "Error Loading Image" << std::endl;
	}

	std::cout << sizeof(image) << std::endl;

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);



	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	GLfloat vertices[NumVertices][3] = {
		//top
		{-1.0, -1.0,  1.0}, //v0
		{1.0, -1.0,  1.0},  //v1
		{1.0,  1.0,  1.0 }, //v2
		{-1.0,  1.0,  1.0}, //v3

		//bottom
		{-1.0, -1.0, -1.0}, //v4
		{1.0, -1.0, -1.0 }, //v5
		{1.0,  1.0, -1.0 }, //v6
		{-1.0,  1.0, -1.0}, //v7

		//left 
		{-1.0, -1.0,  1.0}, //v0
		{-1.0, -1.0, -1.0}, //v4
		{-1.0,  1.0, -1.0}, //v7
		{-1.0,  1.0,  1.0}, //v3

		//right
		{1.0, -1.0,  1.0},  //v1
		{1.0, -1.0, -1.0 }, //v5
		{1.0,  1.0, -1.0 }, //v6
		{1.0,  1.0,  1.0 }, //v2

		//back
		{1.0,  1.0,  1.0 }, //v2
		{-1.0,  1.0,  1.0}, //v3
		{-1.0,  1.0, -1.0}, //v7
		{1.0,  1.0, -1.0 }, //v6
		

		//front
		{-1.0, -1.0,  1.0}, //v0
		{1.0, -1.0,  1.0},  //v1	
		{1.0, -1.0, -1.0 }, //v5
		{-1.0, -1.0, -1.0}, //v4
	};


	GLfloat colorData[NumVertices][3] = {
		{ 1,0,0 }, // color for vertices
		{ 0,1,0 },
		{ 0,0,1 },
		{ 1,1,1 }
	};

	glGenBuffers(3, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vertexColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);

	location = glGetUniformLocation(program, "model_matrix");
	location2 = glGetUniformLocation(program, "camera_matrix");
	location3 = glGetUniformLocation(program, "projection_matrix");
	
}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Depth buffer bit needed for GL_DEPTH_TEST

	glm::mat4 model_view = glm::translate(glm::mat4(1.0), glm::vec3(translate_value, 0.0, 0.0));

	model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);


	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(camx, camy, camz), glm::vec3(0.0, 0.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 projection_matrix = glm::frustum(-1, +1, -1, +1, 1, 100);
	//glm::mat4 projection_matrix = glm::perspective(90.0f, 1024.0f / 1024.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(location3, 1, GL_FALSE, &projection_matrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	drawCube();


	glFlush();
}

void idle()
{
	/*translate_value += 0.0001;
	rotate_value += 0.0001;
	glutPostRedisplay();*/
}

//---------------------------------------------------------------------
//
// main
//

void keyboardHandler(unsigned char key, int x, int y)
{
	if (key == '+')
	{
		camz -= 0.01f;
	}
	if (key == '-')
	{
		camz += 0.01f;
	}

	if (key == 'a')
	{
		camx -= 0.01;
	}

	if (key == 'd')
	{
		camx += 0.01;
	}

	if (key == 'w')
	{
		camy -= 0.01;
	}

	if (key == 's')
	{
		camy += 0.01;
	}

	glutPostRedisplay();
}

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboardHandler);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
