
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


#include "objParser.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[3];
GLuint location;
GLuint location2;
GLuint location3;


const GLuint NumVertices = 482 + 1; //add one as faces starts looking at 1
const GLuint NumFaces = 960;
const GLuint NumTvert = 559;

GLfloat vertices[NumVertices][3];
GLuint faces[NumFaces][3];
GLfloat textvert[NumTvert][2];
GLuint textfaces[NumFaces][3];


GLuint texture[1];


float translate_value = 0;
float rotate_value = 0.5;
float camera_distance = 2.0;

float camz = 50.0f;
float camx = 0.0f;
float camy = 0.0f;


void draw()
{
	for (int i = 0; i < NumFaces; i++)
	{
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces[i]);
	}
}

//---------------------------------------------------------------------
//
// init
//
void init(void)
{
	glEnable(GL_DEPTH_TEST); //Enable 3d texture depth

	objParser obj("Earth.obj", NumVertices, NumFaces, NumTvert);
	//objParser obj("Sphere.txt", NumVertices, NumFaces, NumTvert);

	int state = obj.read(vertices, faces, textvert, textfaces);
	if (state != 1)
	{
		std::cout << "ERROR: Cannot load obj data - Exiting" << std::endl;
		exit(state);
	}

	//DEBUG: Array Dumps
	
	//for (int i = 0; i < NumVertices; i++) { std::cout << "Vertex Number: " << i << " : " << vertices[i][0] << " : " << vertices[i][1] << " : " << vertices[i][2] << std::endl; }//DEBUG Vertex Data dump
	//for (int i = 0; i < NumTvert; i++){ std::cout << "Texture Vertex Number: " << i << " : " << textvert[i][0] << " : " << textvert[i][1] << std::endl; } //DEBUG Texture Vertex Data dump
	//for (int i = 0; i < NumFaces; i++) { std::cout << faces[i][0] << " : " << faces[i][1] << " : " << faces[i][2] << std::endl; }//DEBUG Faces Data dump
	// for (int i = 0; i < NumFaces; i++){ std::cout << textfaces[i][0] << " : " << textfaces[i][1] << " : " << textfaces[i][2] << std::endl;} //DEBUG texture Faces Data dump
	




	glGenTextures(100, texture);

	GLint width, height;
	unsigned char* image = SOIL_load_image("Earth.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	if (!image)
	{
		std::cout << "Error Loading Image" << std::endl;
	}

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




	glGenBuffers(2, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textvert), textvert, GL_STATIC_DRAW);
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
	draw();


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
	float keyscale = 1.0f;
	if (key == '+')
	{
		camz -= keyscale;
	}
	if (key == '-')
	{
		camz += keyscale;
	}

	if (key == 'a')
	{
		camx -= keyscale;
	}

	if (key == 'd')
	{
		camx += keyscale;
	}

	if (key == 'w')
	{
		camy -= keyscale;
	}

	if (key == 's')
	{
		camy += keyscale;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Lab 6");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboardHandler);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
