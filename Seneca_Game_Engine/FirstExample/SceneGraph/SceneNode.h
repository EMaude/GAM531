#ifndef SCENENODE_HEADER
#define SCENENODE_HEADER

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\matrix.hpp"
#include <vector>
#include "GL\glut.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "StaticMesh.h"
#include "LoadShaders.h"

class SceneNode
{
	protected:
		std::vector<SceneNode*> children;
		SceneNode* parent;
		StaticMesh* mesh;
		glm::mat4 transformation;
		ShaderInfo shaders[10];
		void SceneNode::render(glm::mat4 transform);

	public:
		//Constructor
		SceneNode(glm::mat4 transformation, ShaderInfo some_shaders[10]);

		//Transformations
		glm::mat4 getTransformation();
		void setTransformation(glm::mat4 aTransformation);


		//Parent_Child relationship
		void SceneNode::addChild(SceneNode*);
		std::vector<SceneNode*> getChildren();
		SceneNode* SceneNode::getParent();
		void SceneNode::setParent(SceneNode* p);
		
		//Static Mesh		
		StaticMesh SceneNode::getMesh();
		void SceneNode::setMesh(StaticMesh* newMesh);

		//Visuals
		void SceneNode::render();
		
		//Virtual Method
		virtual void SceneNode::drawMesh(glm::mat4 transform) = 0;

};
#endif // !SCENENODE_HEADER