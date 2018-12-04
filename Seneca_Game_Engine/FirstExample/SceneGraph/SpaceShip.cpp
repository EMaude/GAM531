//A spaceship is an example of a composit object. 
//A composit object consists of a body and other sub-parts.
//A spaceship has one sphere as the main body and four subparts (children).
//As for children, use four other spheres to serve as wheels of this spaceship.
//Notice that you should represent a composit object by using a main body and other sub-parts

#include "SpaceShip.h"

SpaceShip::SpaceShip(glm::mat4 transformation, ShaderInfo some_shaders[10]) : SceneNode(transformation, some_shaders)
{
	//This mesh serves as the main body of this composit object.
	*mesh = StaticMesh("Sphere.obj");

	//Create and attach the children of this composit object.
	Sphere s1 = Sphere(glm::translate(glm::mat4(1.0), glm::vec3(-0.5, -0.5, 0.0)), some_shaders);
	addChild(&s1);

	Sphere s2 = Sphere(glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.0)), some_shaders);
	addChild(&s2);

	Sphere s3 = Sphere(glm::translate(glm::mat4(1.0), glm::vec3(0.5, -0.5, 0.0)), some_shaders);
	addChild(&s3);

	Sphere s4 = Sphere(glm::translate(glm::mat4(1.0), glm::vec3(-0.5, 0.5, 0.0)), some_shaders);
	addChild(&s4);

}

void drawMesh(glm::mat4 transformation)
{
	//To be completed by students
	//Draw the main body of this composit object
}

