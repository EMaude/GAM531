#pragma once
#include "SceneNode.h"
#include "Sphere.h"
class SpaceShip : public SceneNode
{
public:
	//Constructor
	SpaceShip(glm::mat4 transformation, ShaderInfo some_shaders[10]);
	void SceneNode::drawMesh(glm::mat4 transformation) override;

};

