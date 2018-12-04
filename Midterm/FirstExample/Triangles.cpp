
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
#include "SOIL.h"

#include "objParser.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];


GLuint Buffers[3];	//Buffer pointers used to push data into our shaders: vertexPositions, vertexColors and vertex Coordinates. Look at triangles.vert shader
GLuint location;	//Will be used to stire the location of our model_view matrix in VRAM
GLuint location2;	//Will be used to stire the location of our camera matrix in VRAM
GLuint location3;	//Will be used to stire the location of our projection matrix in VRAM

GLuint texture[2];	//Array of pointers to textrure data in VRAM. We will use two different textures in this example


const GLuint NumVertices = 529 + 1;
const GLuint NumFaces = 1024;

GLfloat vertices[NumVertices][3];
GLuint faces[NumFaces][3];

float rotate_value = 0.0;
float camera_distance = 2.0;

float camz = 50.0f;
float camx = 0.0f;
float camy = 0.0f;

GLuint program;

//---------------------------------------------------------------------
//
// init
//

void draw()
{
	for (int i = 0; i < NumFaces; i++)
	{
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces[i]);
	}
}

void init(void)
{

	objParser obj("Earth.obj", NumVertices, NumFaces);
	//objParser obj("Sphere.txt", NumVertices, NumFaces, NumTvert);

	int state = obj.read(vertices, faces);
	if (state != 1)
	{
		std::cout << "ERROR: Cannot load obj data - Exiting" << std::endl;
		exit(state);
	}


	glGenTextures(2, texture);

	//-- Ground --//

	//Creating our textures:
	//This is a 2-by-2 pixel texture: 
	GLint width, height;
	unsigned char* textureData = SOIL_load_image("ground.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	//These are the texture coordinates for the first texture
	GLfloat textureCoordinates[4][2] = {
		0.0f, 10.0f,
		10.0f, 10.0f,
		10.0f, 0.0f,
		0.0f, 0.0f
	};

	///////////////////////FIRST TEXTURE/////////////////////////

	//Set the type of the first buffer as "TEXTURE_2D"
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	//Loading the first texture into the first allocated buffer:
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	//Setting up parameters for the texture that recently pushed into VRAM
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	/////////////////////////////////////////////////////////////

	/*s

	//-- SKY --//
	GLint width2, height2;
	unsigned char* textureData2 = SOIL_load_image("sky.jpeg", &width2, &height2, 0, SOIL_LOAD_RGB);

	//These are the texture coordinates for the second texture
	GLfloat textureCoordinates2[4][2] = {
		0.0f, 10.0f,
		10.0f, 10.0f,
		10.0f, 0.0f,
		0.0f, 0.0f
	};

	///////////////////////SECOND TEXTURE////////////////////////

	//Set the type of the second buffer as "TEXTURE_2D"
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	//Loading the second texture into the second allocated buffer:
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData2);

	//Setting up parameters for the texture that recently pushed into VRAM
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//////////////////////////////////////////////////////////////

	*/

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	GLfloat vertices2[4][2] = {
		{ -100, -100 }, // Square
		{ 100, -100 },
		{ 100, 100 },
		{ -100, 100 }
	};


	GLfloat colorData[NumVertices][3] = {
		{ 1,0,0 }, // color for vertices
		{ 0,1,0 },
		{ 0,0,1 },
		{ 1,1,1 }
	};

	glGenBuffers(3, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vertexColor");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

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
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 model_view = glm::mat4(1.0);

	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(camx, camy, camz), glm::vec3(0.0, 0.0, -1.0), glm::vec3(1.0, 1.0, 1.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 projection_matrix = glm::frustum(-1, +1, -1, +1, 1, 100);
	//glm::mat4 projection_matrix = glm::perspective(90.0f, 1024.0f / 1024.0f, 1.0f, 100.0f);
	glUniformMatrix4fv(location3, 1, GL_FALSE, &projection_matrix[0][0]);

	
	//The texture coordinates for the first square:
	GLfloat textureCoordinates[4][2] = {
		0.0f, 10.0f,
		10.0f, 10.0f,
		10.0f, 0.0f,
		0.0f, 0.0f
	};

	//Setting up the buffer in VRAM to pass the texture coordinates

	//Remember that we defined three buffers: position, color and texture coordinates
	//Selecting the buffer that will contain texture coordinates (Buffers[2])
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);

	model_view = glm::rotate(model_view, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Pushing the texture coordinates into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);

	//We have two textures defined in VRAM, which one would you like to be applied?
	//Lets say the first one.
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	GLuint tmp[4] = { 530 + 4, 530 + 1, 530 + 2, 530 + 3 };
	//Rendering the first square
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, tmp);
	//Done !!!

	//Now, rendering the second square with the second texture applied. (The texture that was loaded from file)
	/*
	//Definig the texture coordinates that will be pushed into the buffer and from there to shaders
	GLfloat textureCoordinates2[4][2] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates2), textureCoordinates2, GL_STATIC_DRAW);

	model_view = glm::rotate(glm::mat4(1.0), 0.0f ,glm::vec3(1.0f, 0, 0));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//We have two textures defined in VRAM, which one would you like to be applied?
	//Now, we use the second one.
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	//Rendering the first square
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, tmp);
	//Done !!!*/


	draw();


	glFlush();
}


void keyboard(unsigned char key, int x, int y)
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

void mouse(int x, int y)
{


}

void idle()
{
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Hello World");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);
	
	glutPassiveMotionFunc(mouse);

	glutIdleFunc(idle);

	glutMainLoop();



}
