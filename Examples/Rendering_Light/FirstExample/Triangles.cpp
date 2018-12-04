
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



const int NumTimesToSubdivide = 5;
const int NumTriangles = 4096;  // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices = 3 * NumTriangles;
GLfloat theta = 0.0f;

typedef glm::vec4 point4;
typedef glm::vec4 color4;

point4 points[NumVertices];
glm::vec3   normals[NumVertices];
glm::vec3 rotation_axis = glm::vec3(0, 0, 1);

// Model-view and projection matrices uniform location
GLuint  ModelView, Projection;

//----------------------------------------------------------------------------

int Index = 0;


void
triangle(const point4& a, const point4& b, const point4& c)
{
	glm::vec3 tmp_b_a = glm::vec3((b - a).x, (b - a).y,(b - a).z);
	glm::vec3 tmp_c_b = glm::vec3((c - b).x, (c - b).y, (c - b).z);
	glm::vec3  normal = glm::normalize(glm::cross(tmp_b_a, tmp_c_b));

	normals[Index] = normal;  points[Index] = a;  Index++;
	normals[Index] = normal;  points[Index] = b;  Index++;
	normals[Index] = normal;  points[Index] = c;  Index++;
}


//----------------------------------------------------------------------------

point4
unit(const point4& p)
{
	float len = p.x*p.x + p.y*p.y + p.z*p.z;

	point4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

void
divide_triangle(const point4& a, const point4& b, const point4& c, int count)
{
	if (count > 0) {
		point4 v1 = unit(a + b);
		point4 v2 = unit(a + c);
		point4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1);
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

void
tetrahedron(int count)
{
	point4 v[4] = {
		glm::vec4(0.0, 0.0, 1.0, 1.0),
		glm::vec4(0.0, 0.942809, -0.333333, 1.0),
		glm::vec4(-0.816497, -0.471405, -0.333333, 1.0),
		glm::vec4(0.816497, -0.471405, -0.333333, 1.0)
	};
	
	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	// Subdivide a tetrahedron into a sphere
	tetrahedron(NumTimesToSubdivide);

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);


	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	
	

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	// Initialize shader lighting parameters
	point4 light_position(0.0, 0.0, 2.0, 0.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
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
	glm::vec3  up(0.0, 1.0, 0.0);

	glm::mat4 model_view = glm::lookAt(eye, at, up);

	model_view = glm::rotate(model_view, theta, rotation_axis);

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, &model_view[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glutSwapBuffers();
}

void idle()
{
	
	//glutPostRedisplay();
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
	}
	theta += 0.1;
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = -20.0, zFar = 20.0;

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

	glewInit();

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
