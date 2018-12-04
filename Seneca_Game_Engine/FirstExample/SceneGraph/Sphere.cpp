#include "Sphere.h"

Sphere::Sphere(glm::mat4 transformation, ShaderInfo some_shaders[10]) : SceneNode(transformation, some_shaders)
{
	mesh = StaticMesh("Sphere.obj");

}

void drawMesh(glm::mat4 transformation)
{
	//To be completed by students
}

