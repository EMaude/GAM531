
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#define USE_GL3

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint location;

const GLuint NumVertices = 1000;

GLfloat vertices[NumVertices][2];

GLfloat vertexColors[NumVertices][3];
float translate_value = 0;
float rotate_value = 0;

//---------------------------------------------------------------------
//
// init
//


void init(void)
{

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	for (int i = 0; i < NumVertices; i++)
	{
		float x = ((rand() % 200 + 1) / 100.0f) - 1;
		float y = ((rand() % 200 + 1) / 100.0f) - 1;
		float z = ((rand() % 200 + 1) / 100.0f) - 1;

		float scale = 0.5f;
		vertices[i][0] = x * scale;
		vertices[i][1] = y * scale;
		vertices[i][2] = z * scale;

		float r = (rand() % 100 + 1) / 100.0f;
		float g = (rand() % 100 + 1) / 100.0f;
		float b = (rand() % 100 + 1) / 100.0f;

		vertexColors[i][0] = r;
		vertexColors[i][1] = g;
		vertexColors[i][2] = b;

	}


	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vertexColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	location = glGetUniformLocation(program, "model_matrix");

	glEnable(GL_DEPTH_TEST);
	
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model_view = glm::rotate(glm::mat4(1.0), rotate_value, glm::vec3(1.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	glDrawArrays(GL_QUADS, 0, NumVertices);

	glutSwapBuffers();
}

void idle()
{
	translate_value += 0.0001;
	rotate_value += 0.001;
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Welcome To 3DGraphics");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
