
///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////

//Elliot Maude
//Final Exam


//NOTE:
// Textures Worked before lighting was implemented, Currently both dont work. I think it might be shader related.



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


GLuint Buffers[3];	//Buffer pointers used to push data into our shaders: vertexPositions, vertexColors and vertex Coordinates. Look at triangles.vert shader
GLuint location;	//Will be used to stire the location of our model_view matrix in VRAM
GLuint location2;	//Will be used to stire the location of our camera matrix in VRAM
GLuint location3;	//Will be used to stire the location of our projection matrix in VRAM

GLuint texture[2];	//Array of pointers to textrure data in VRAM. We will use two different textures in this example


const GLuint NumVertices = 4;
float translate_value = 0;
float rotate_value = 0;
float camera_distance = 0.1;

GLuint program;

//Lighting
typedef glm::vec4 point4;
typedef glm::vec4 color4;

glm::vec3 normals[NumVertices];


//---------------------------------------------------------------------
//
// init
//


void init(void)
{

	//Creating our textures:
	//This is a 2-by-2 pixel texture:
	GLint width, height;
	unsigned char* textureData = SOIL_load_image("sky.jpeg", &width, &height, 0, SOIL_LOAD_RGB);

	//These are the texture coordinates for the first texture
	GLfloat textureCoordinates[4][2] = {
		0.0f, 10.0f,
		10.0f, 10.0f,
		10.0f, 0.0f,
		0.0f, 0.0f
	};

	//Creating our second texture:
	//This texture is loaded from file. To do this, I will use the SOIL (Simple OpenGL Imaging Library) library to import the texture.
	//When using the SOIL_load_image() function, make sure the you are using correct patrameters, or else, your image will NOT be loaded properly, or will not be loaded at all.
	GLint width2, height2;
	unsigned char* textureData2 = SOIL_load_image("ground.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);

	//These are the texture coordinates for the second texture
	GLfloat textureCoordinates2[4][2] = {
		0.0f, 10.0f,
		10.0f, 10.0f,
		10.0f, 0.0f,
		0.0f, 0.0f
	};

	

	//Once we set up our textures, we need to push the texture data for my two textures into VRAM
	//Note: The texture coordinates will be sent to shaders as variable vTexCoord. Take a look at the vertex shader for more details.
	
	//Allocating two buffers in VRAM
	glGenTextures(2, texture);	
	

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


	//Of courese, following the same procedure, you mah load as many textures as your Graphic Card allows you to.

	////////////////////////////////////DONE WITH SETTING-UP THE TEXTURES///////////////////////////

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up

	GLfloat vertices[NumVertices][3] = {
		{ -0.90, -0.90, 0.0 }, // Square
		{ 0.9, -0.9, 0.0 },
		{ 0.90, 0.9, 0.0 },
		{ -0.9, 0.9, 0.0 }
	};

/* LIGHTING
	//Normals
	normals[0] = { 0, 0, 1 }; 
	normals[1] = { 0, 0, 1 }; 
	normals[2] = { 0, 0, 1 };
	normals[3] = { 0, 0, 1 };
	*/
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, Buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),	vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	//Lighting
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vNormal");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);


	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0); //Position of diffuse light
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 0.0, 0.0, 1.0); //Intensity or Lumination
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	//color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 material_specular(1.0, 0.0, 1.0, 1.0);
	float  material_shininess = 5.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	GLfloat tmp1[4] = { ambient_product.r, ambient_product.g, ambient_product.b, ambient_product.a };
	GLfloat tmp2[4] = { diffuse_product.r, diffuse_product.g, diffuse_product.b, diffuse_product.a };
	GLfloat tmp3[4] = { specular_product.r, specular_product.g, specular_product.b, specular_product.a };

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, tmp1);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, tmp2);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, tmp3);

	GLfloat tmp_p[4] = { light_position.x, light_position.y, light_position.z, light_position.w };

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, tmp_p);

	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);





	glEnable(GL_DEPTH_TEST);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model_view = glm::translate(glm::mat4(1.0), glm::vec3(5, 0, 0));
	model_view = glm::scale(model_view, glm::vec3(2.0, 2.0, 1.0));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	glm::mat4 camera_matrix = glm::lookAt(glm::vec3(0.0, 0.0, camera_distance), glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(location2, 1, GL_FALSE, &camera_matrix[0][0]);

	glm::mat4 projection_matrix = glm::frustum(-1.0, +1.0, -1.0, +1.0, 0.01, 10.0);
	glUniformMatrix4fv(location3, 1, GL_TRUE, &projection_matrix[0][0]);


	//The texture coordinates for the first square:
	GLfloat textureCoordinates[4][2] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	//Setting up the buffer in VRAM to pass the texture coordinates

	//Remember that we defined three buffers: position, color and texture coordinates
	//Selecting the buffer that will contain texture coordinates (Buffers[2])
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);

	//Pushing the texture coordinates into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
	glBindAttribLocation(program, 2, "vTexCoord");
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(2);

	//We have two textures defined in VRAM, which one would you like to be applied?
	//Lets say the first one.
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	GLuint faces[4] = {0, 1, 2, 3};
	//Rendering the first square
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, faces);
	//Done !!!

	//Now, rendering the second square with the second texture applied. (The texture that was loaded from file)

	//Definig the texture coordinates that will be pushed into the buffer and from there to shaders
	GLfloat textureCoordinates2[4][2] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	
	//Pushing the texture coordinated for the second geometry into the buffer. Note: these data will be pushed into the last buffer that is bound. A few lines above, we set: glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	//So, these data will be pushed into the Buffers[2] which contains the texture coordinates.
	//In other words, we are re-initializing texture coordinnate values inside the buffer.(Previously, the texture copordinates for our first geometry was here).
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates2), textureCoordinates2, GL_STATIC_DRAW);

	model_view = glm::translate(glm::mat4(1.0), glm::vec3(-5, 0, 0));
	model_view = glm::scale(model_view, glm::vec3(2.0, 2.0, 1.0));
	//rotate second square
	model_view = glm::rotate(model_view, 60.0f, glm::vec3(1.0f, 0.5f, 0.0f));
	glUniformMatrix4fv(location, 1, GL_TRUE, &model_view[0][0]);

	//We have two textures defined in VRAM, which one would you like to be applied?
	//Now, we use the second one.
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	//Rendering the first square
	glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, faces);
	//Done !!!

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

void keyboardHandler(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		translate_value -= 0.1;
	}
	else if (key == 'd')
	{
		translate_value += 0.1;
	}
	else if (key == 'q')
	{
		rotate_value += 0.1;
	}
	else if (key == 'e')
	{
		rotate_value -= 0.1;
	}
	else if (key == '=' | key == '+')
	{
		camera_distance += 0.1;
	}
	else if (key == '-')
	{
		camera_distance -= 0.1;
	}

	glutPostRedisplay();
}
void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = 0.001, zFar = 20.0;

	GLfloat aspect = GLfloat(width) / height;

	if (aspect > 1.0) {
		left *= aspect;
		right *= aspect;
	}
	else {
		top /= aspect;
		bottom /= aspect;
	}

	glm::mat4 projection = glm::ortho(left, right, bottom, top, zNear, zFar);

	glUniformMatrix4fv(location3, 1, GL_TRUE, &projection[0][0]);
}
int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Final Exam");
	glewExperimental = GL_TRUE;

	GLenum err = glGetError();
	std::cout << err;

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardHandler);

	glutIdleFunc(idle);

	glutMainLoop();
	
	

}
