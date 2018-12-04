#include "SceneNode.h"

#pragma once
///////////////////////////////////////////////////////////////////////
//
// SceneNode.cpp
//
///////////////////////////////////////////////////////////////////////
using namespace std;


SceneNode::SceneNode(glm::mat4 aTransformation, ShaderInfo some_shaders)
{
	transformation = aTransformation;
	parent = nullptr;
	children = std::vector<SceneNode*>();
	shaders = some_shaders;
}

StaticMesh SceneNode::getMesh()
{
	return *mesh;
}
void SceneNode::setMesh(StaticMesh *newMesh)
{
	mesh = newMesh;
}


void SceneNode::setParent(SceneNode* p)
{
	parent = p;
}

void SceneNode::addChild(SceneNode* child)
{
	children.push_back(child);
	(*child).setParent(this);

}

glm::mat4 SceneNode::getTransformation()
{
	return transformation;
}

void SceneNode::setTransformation(glm::mat4 aTransformation)
{
	transformation = aTransformation;
}


std::vector<SceneNode*> SceneNode::getChildren()
{
	return children;
}
SceneNode* SceneNode::getParent()
{
	return parent;
}

void SceneNode::render()
{
	render(transformation);
}

void SceneNode::render(glm::mat4 transform)
{
	//Draw myself
	drawMesh(transform);
	
	//Render My Children
	for (int i = 0; i < children.size(); i++)
	{
		SceneNode* tmp = children.at(i);
		(*tmp).render(tmp->getTransformation() * transform);
	}
	return;
}


