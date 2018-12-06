#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "vgl.h"

class objParser {
private:
	std::string filename;
	std::ifstream file;

	GLuint NumVertices; //add one as faces starts looking at 1
    GLuint NumFaces;
	GLuint NumTexVert;
	
	int sizesFromFile();

public:
	/*Constructor where user code passes Number of vertices (real number of verts + 1) and number of faces*/
	objParser(std::string filename, GLuint NumVertices, GLuint NumFaces, GLuint NumTexVert = 0);
	/*Constructor where the number of verts and faces will be read from the file*/
	objParser(std::string filename);

	~objParser();

	/*reads data into presized arrays*/
	int read(GLfloat vertexArray[][3], GLuint faces[][3]);
	/*creates arrays of required size and reads data from file*/
	int read(GLfloat *vertexArray, GLuint *faces);

	/*reads data into presized arrays, with texture data*/
	int read(GLfloat vertexArray[][3], GLuint faces[][3], GLfloat textureArray[][2], GLuint faces_texture[][3]);
	/*creates arrays of required size and reads data from file, with texture data*/
	int read(GLfloat *vertexArray, GLuint *faces, GLuint *textureArray, GLuint *faces_texture);
	
};

#endif //OBJPARSER_H