
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
#include "Angel.h"
#include "mat.h"

using namespace std;



const int NumVertices = 26;
GLfloat theta = 0.0f;
GLfloat speed = 0.0F;
bool stop = false;
typedef glm::vec4 point4;
typedef glm::vec4 color4;

glm::vec3 normals[NumVertices];
glm::vec3 rotation_axis = glm::vec3(0, 0, 1);

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;


//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	
	GLfloat vertices[NumVertices][4] = { 
		{-0.4, -0.15, 0, 1}, {-0.4, 0.15, 0, 1}, {-0.1, 0.15, 0, 1},
		{-0.4, -0.15, 0, 1}, {-0.1, 0.15, 0, 1}, {-0.1, -0.15, 0, 1},
		{-0.1, -0.15, 0, 1}, {-0.1, 0.15, 0, 1}, {0.1, 0.15, 0.3, 1},
		{-0.1, -0.15, 0, 1}, {0.1, 0.15, 0.3, 1}, {0.1, -0.15, 0.3, 1},
		{0.1, -0.15, 0.3, 1}, {0.1, 0.15, 0.3, 1}, {0.4, 0.15, 0.3, 1},
		{0.1, -0.15, 0.3, 1}, {0.4, 0.15, 0.3, 1}, {0.4, -0.15, 0.3, 1},

		{ -0.4, -0.65, 0, 1 },{ -0.4, -0.35, 0, 1 },{ -0.1, -0.35, 0, 1 },{ -0.1, -0.65, 0, 1 },
		{ 0.1, -0.35, 0.3, 1 },{ 0.4, -0.35, 0.3, 1 },{ 0.1, -0.65, 0.3, 1 },{ 0.4, -0.65, 0.3, 1 }
		
	};

	normals[0] = {0, 0, 1}; normals[1] = {0, 0, 1}; normals[2] = {0, 0, 1}; 
	normals[3] = {0, 0, 1};normals[4] = {0, 0, 1}; normals[5] = {0, 0, 1}; 
	
	normals[6] = { -sqrt(2) / 2, 0, sqrt(2) / 2 }; normals[7] = { -sqrt(2) / 2, 0, sqrt(2) / 2 }; normals[8] = { -sqrt(2) / 2, 0, sqrt(2) / 2 };
	normals[9] = { -sqrt(2) / 2, 0, sqrt(2) / 2 }; normals[10] = { -sqrt(2) / 2, 0, sqrt(2) / 2 }; normals[11] = { -sqrt(2) / 2, 0, sqrt(2) / 2 };

	normals[12] = { 0, 0, 1 }; normals[13] = { 0, 0, 1 }; normals[14] = { 0, 0, 1 }; 
	normals[15] = { 0, 0, 1 }; normals[16] = { 0, 0, 1 }; normals[17] = { 0, 0, 1 };

	normals[18] = { 0, 0, 1 }; normals[19] = { 0, 0, 1 };
	normals[20] = { -sqrt(2) / 4, 0, (2 - sqrt(2)) / 2 }; normals[21] = { -sqrt(2) / 4, 0, (2 - sqrt(2)) / 2 };
	normals[22] = { -sqrt(2) / 4, 0, (2 - sqrt(2)) / 2 }; normals[24] = { -sqrt(2) / 4, 0, (2 - sqrt(2)) / 2 };

	normals[23] = { 0, 0, 1 }; normals[25] = { 0, 0, 1 };
	


	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer[2];
	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindAttribLocation(program, 0, "vPosition");
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindAttribLocation(program, 1, "vNormal");
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(1);

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0); //Position of diffuse light
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 0.0, 1.0); //Intensity or Lumination
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	//color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
	color4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 material_specular(1.0, 0.0, 1.0, 1.0);
	float  material_shininess = 5.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;
	
	GLfloat tmp1[4] = {ambient_product.r, ambient_product.g, ambient_product.b, ambient_product.a };
	GLfloat tmp2[4] = {diffuse_product.r, diffuse_product.g, diffuse_product.b, diffuse_product.a };
	GLfloat tmp3[4] = {specular_product.r, specular_product.g, specular_product.b, specular_product.a};

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),1, tmp1);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),1, tmp2);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),1, tmp3);

	GLfloat tmp_p[4] = { light_position.x, light_position.y, light_position.z, light_position.w };

	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, tmp_p);

	glUniform1f(glGetUniformLocation(program, "Shininess"),material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");
	
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */

	
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::vec3 at(0.0, 0.0, 0.0);
	glm::vec3 eye(0.0, 0.0, 0.5);
	glm::vec3 up(0.0, 1.0, 0.0);

	glm::mat4 model_view = glm::lookAt(eye, at, up);

	model_view = glm::rotate(model_view, theta, rotation_axis);

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, &model_view[0][0]);



	
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);
	glDrawArrays(GL_TRIANGLES, 12, 3);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	
	GLubyte elements[18] = { 18, 19, 20, 18, 20, 21, 20, 21, 22, 21, 22, 24, 22, 23, 24, 23, 24, 25 };


	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, elements);
	




	glutSwapBuffers();


}

void idle()
{
	if (!stop)
	{
		theta += speed;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{

		case 'x':
			rotation_axis = glm::vec3(1, 0, 0);
			break;
		case 'y':
			rotation_axis = glm::vec3(0, 1, 0);
			break;
		case 'z':
			rotation_axis = glm::vec3(0, 0, 1);
			break;
		case 's':
			stop = !stop;
			break;
		case '=':
			speed  += 0.001;
			break;
		case '-':
			speed -= 0.001;
			if (speed <= 0) speed = 0.01;
			break;

	}
}

//----------------------------------------------------------------------------

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
	
	glUniformMatrix4fv(Projection, 1, GL_TRUE, &projection[0][0]);
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Sphere");
	glewExperimental = GL_TRUE;
	glewInit();

	GLenum err = glGetError();
	std::cout << err;

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
