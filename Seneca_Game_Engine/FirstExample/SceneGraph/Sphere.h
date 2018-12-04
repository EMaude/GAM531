#pragma once
#include "SceneNode.h"
#include "StaticMesh.h"
#include "glm\glm.hpp"

class Sphere : public SceneNode
{
public:
	//Constructor
	Sphere(glm::mat4 transformation, ShaderInfo some_shaders[10]);
	void SceneNode::drawMesh(glm::mat4 transformation) override;

};
