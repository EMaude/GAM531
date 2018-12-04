
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
#include <iostream>
#include "mat.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[5];
GLuint location;
GLuint program;

const GLuint NumParticles = 1000000;

GLfloat vertices[NumParticles][2];
GLfloat vertexColors[NumParticles][3];
GLfloat vertexSpeed[NumParticles];
GLfloat vertexSpawnTime[NumParticles];
GLfloat vertexAngle[NumParticles];



float translate_value = 0;
float rotate_value = 0;

//---------------------------------------------------------------------
//
// init
//


void
init(void)
{

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	for (int i = 0; i < NumParticles; i++)
	{
		float x = 0.0f; //((rand() % 200 + 1) / 100.0f) - 1;
		float y = 0.0f; //((rand() % 200 + 1) / 100.0f) - 1;
		vertices[i][0] = x;
		vertices[i][1] = y;


		float r = (rand() % 100 + 1) / 100.0f;
		float g = (rand() % 100 + 1) / 100.0f;
		float b = (rand() % 100 + 1) / 100.0f;
		vertexColors[i][0] = r;
		vertexColors[i][1] = g;
		vertexColors[i][2] = b;

		vertexSpeed[i] = (rand() % 100 + 1) / 100.0f;
		vertexSpawnTime[i] = (rand() % 9000 + 1) / 100.0f;
		vertexAngle[i] = M_PI * (rand() % 90 + 1 + 50) / 180;


	}


	glGenBuffers(5, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vertexColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexSpeed), vertexSpeed, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vertexSpeed");
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexSpawnTime), vertexSpawnTime, GL_STATIC_DRAW);
	glBindAttribLocation(program, 3, "vertexSpawnTime");
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAngle), vertexAngle, GL_STATIC_DRAW);
	glBindAttribLocation(program, 4, "vertexAngle");
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(4);


	location = glGetUniformLocation(program, "model_matrix");
	
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 model_view = glm::translate(glm::mat4(1.0), glm::vec3(translate_value, 0.0, 0.0));

	//model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	float ellapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

	//cout << ellapsed_time << "\n";

	glUniform1f(glGetUniformLocation(program, "ellapsed_time"), ellapsed_time);

	glDrawArrays(GL_POINTS, 0, NumParticles);

	glFlush();
}

void idle()
{
	//translate_value += 0.0001;
	//rotate_value += 0.0001;
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
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
