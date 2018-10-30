
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

using namespace std;

#include <string>
#include <iostream>

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

//-- Custom Headers --// 
#include "objParser.h"
#include "CameraHandler.h"
#include "InputHandler.h"
#include "ProjectionHandler.h"


enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
GLuint location;


const GLuint NumVertices = 482 + 1; //add one as faces starts looking at 1
const GLuint NumFaces = 960;

GLfloat vertices[NumVertices][3];
GLuint faces[NumFaces][3];

float rotate_value = 0;


void draw()
{
	for (int i = 0; i < NumFaces; i++)
	{
		glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, faces[i]);
	}
}

//---------------------------------------------------------------------
// Setting up our pipeline and preparing to draw 
void init(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//Defining the name of our shader files
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and attaching shaders to our pipeline
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up


	//Read From obj File

	objParser obj("Sphere.txt", NumVertices, NumFaces);

	int state = obj.read(vertices, faces);
	if (state != 1)
	{
		std::cout << "ERROR: Cannot load obj data - Exiting" << std::endl;
		exit(state);
	}

	/*
	for (int i = 0; i < NumVertices; i++) //DEBUG Vertex Data dump
	{ 
		std::cout << "Vertex Number: " << i << " : " << vertices[i][0] << " : " << vertices[i][1] << " : " << vertices[i][2] << std::endl;
	}
	
	
	for (int i = 0; i < NumFaces; i++) //DEBUG Faces Data dump
	{
		std::cout << faces[i][0] << " : " << faces[i][1] << " : " << faces[i][2] << std::endl;
	}
	*/


	GLfloat colorData[1][3] = {
		{0, 0, 0}
	};
	

	//Allocating buffers in VRAM and pushing vertex data (position and color) into the data.
	//In the following section, we will transfer the vertex data from RAM into VRAM using OpenGL library
	//Do NOT go inside the details. I will teach them in details next week.
	glGenBuffers(2, Buffers);
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
	
	//Retrieving the location of the matrices from VRAM and storing them inside these variables
	location = glGetUniformLocation(program, "model_matrix");
	
	CameraHandler::init(program);
	ProjectionHandler::init(program);
}


//---------------------------------------------------------------------
//
// The following function is named display function in OpenGL. The task of the display function is to render the scene
// The name of the display function is defined by the user (in this case, it is called drawScene).
// In the main function, you will need to register the name of the display function in main() method.
//This is done by using glutDisplayFunc function. Look at the main method
void drawScene(void)
{
	//Clear the screen and preparing to draw
	glClear(GL_COLOR_BUFFER_BIT);

	CameraHandler::draw();

	

	//Setting up projection matrix and initialize the projection_matrix in VRAM
	ProjectionHandler::draw();

	//Creating rotation matrix using glm livrary. In this case, rotating rotat_value degree about Z axis (0, 0, 1)
	
	glm::mat4 model_view = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
	//The following function passes the generated rotation function into the vertex-shader
	//I will explain this in details next week.
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Starting the pipeline
	//Rendering the sun
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();

	//Transformations required to render the earth
	model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::translate(model_view, glm::vec3(20.0, 0.0, 0.0));
	model_view = glm::rotate(model_view, 4*rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::scale(model_view, glm::vec3(0.5, 0.5, 0.5));

	//Sending the generated transformation matrix to the vertex shader
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Rendering the earth
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();


	//Transformations required to render the moon
	model_view = glm::scale(model_view, glm::vec3(0.7, 0.7, 0.7));
	model_view = glm::rotate(model_view, -20*rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::translate(model_view, glm::vec3(10.0, 0.0, 0.0));
	model_view = glm::rotate(model_view, -30*rotate_value, glm::vec3(0.0, 0.0, 1.0f));

	//Sending the generated transformation matrix to the vertex shader
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Rendering the moon
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();




	//Flush the image onto the window (screen)
	glFlush();
}

//This function is called "idle" function. This function is invoked by OpenGL ever frame (60 frame per second).
//This function is used for animation, since you can animate your scene for every frame.
//You will need to register the name of your idle function to OpenGL (It is "runEveryFrame" in here).
//The registration happens in the main() function using glutIdleFunc(runEveryFrame) function.
void runEveryFrame()
{
	//Increasing our rotation angle
	rotate_value += 0.001;

	//glutpostRedisplay() finction calls the display function in order to re-draw the scene
	//In here, glutPostRedisplay function is used inside the idle function to re-draw the scene for every frame.
	//So, in here, we increase the rotation value and then redraw the scene with the new value for the rotation.
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
	//Initializing to draw
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Lab Four: OBJ Loader");
	glewInit();	

	//init function is defined above
	init();

	//Registering the display function
	//Read the comments for the drawScene function above
	glutDisplayFunc(drawScene);

	//Registering the idle function
	//Read the comments for the runEveryFrame function above
	glutIdleFunc(runEveryFrame);

	glutKeyboardFunc(InputHandler::keyboard);

	//glutMainLoop enters the event processing loop
	glutMainLoop();

}
