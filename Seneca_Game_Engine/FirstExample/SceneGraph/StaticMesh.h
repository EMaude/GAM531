#ifndef STATICMESH_HEADER
#define STSTAICMESH_HEADER

#include <../glm/glm/glm.hpp>

class StaticMesh
{
	
	public:
		int number_of_vertices;
		int number_of_faces;
		float ** vertices;
		float ** faces;

		//Constructors
		StaticMesh();	
		StaticMesh(char* fileName);
		StaticMesh(float vertices[][3], float faces[][3]);

};
#endif