#include "objParser.h"

int objParser::sizesFromFile()
{
	///TODO: Make read vertex and faces sizes from file
	return 0;
}

objParser::objParser(std::string filename, GLuint NumVertices, GLuint NumFaces, GLuint NumTexVert)
{
	this->filename = filename;
	this->NumVertices = NumVertices;
	this->NumFaces = NumFaces;
	this->NumTexVert = NumTexVert;
		
	this->file.open(this->filename);
}

objParser::objParser(std::string filename) 
{
	this->filename = filename;
	this->file.open(this->filename);
	
	//sizesFromFile();

	std::cout << "This constructor is not done, and will cause stupid things to happen" << std::endl;
}

objParser::~objParser()
{
	this->file.close();
}

int objParser::read(GLfloat vertexArray[][3], GLuint faces[][3])
{
	if (file.is_open())
	{
		std::cout << "Reading File" << std::endl;
		std::string line;
		int ln = 0;
		int vertNum = 1; //starts at one as faces starts looking at index 1
		int faceNum = 0;

		while (getline(this->file, line))
		{
			ln++;
			if (!line.empty())
			{
				if (line.at(0) == 'v')
				{
					float temp;
					float temparr[3];
					std::stringstream ss;
					ss << line;

					int i = 0;
					std::string t;
					while (!ss.eof()) {
						ss >> t;
						if (std::stringstream(t) >> temp && i <= 3) {
							temparr[i] = temp;
							i++;
						}
						t = "";
					}

					if (vertNum <= this->NumVertices)
					{
						for (int i = 0; i < 3; i++)
						{
							vertexArray[vertNum][i] = temparr[i];
						}
					}
					else
					{
						std::cout << "ERROR: Could not add vertex to vertexArray" << std::endl;
						return 0;
					}
					vertNum++;
				}

				if (line.at(0) == 'f')
				{
					int temp;
					int temparr[3];
					std::stringstream ss;
					ss << line;

					int i = 0;
					std::string t;
					while (!ss.eof()) {
						ss >> t;
						//std::cout << "Token: " << t << std::endl;
						if (std::stringstream(t) >> temp && i <= 3) {
							temparr[i] = temp;
							i++;
						}
						t = "";
					}

					if (faceNum <= this->NumFaces)
					{
						for (int i = 0; i < 3; i++)
						{
							faces[faceNum][i] = temparr[i];
						}
					}
					else
					{
						std::cout << "ERROR: Could not add face to faces" << std::endl;
						return 0;
					}
					faceNum++;
				}
			}
		}
		std::cout << "Done Reading File" << std::endl;
		return 1;
	}
	return -1;
}

int objParser::read(GLfloat vertexArray[][3], GLuint faces[][3], GLfloat textureArray[][2], GLuint faces_texture[][3])
{
	if (file.is_open())
	{
		std::cout << "Reading File" << std::endl;
		std::string line;
		int ln = 0;
		int vertNum = 1; //starts at one as faces starts looking at index 1
		int faceNum = 0;
		int vtextNum = 1; //starts at one as faces starts looking at index 1

		while (getline(this->file, line))
		{
			ln++;
			if (!line.empty())
			{
				if (line.at(0) == 'v' && line.at(1) == 't')
				{
					float temp;
					float temparr[3];
					//create a string stream from line
					std::stringstream ss;
					ss << line;
					
					int i = 0;
					std::string t;
					//look through tokens of ss
					while (!ss.eof()) {
						ss >> t;
						if (std::stringstream(t) >> temp && i <= 2) {
							temparr[i] = temp;
							i++;
						}
						t = "";
					}

					if (vtextNum <= this->NumTexVert)
					{
						for (int i = 0; i < 2; i++)
						{
							textureArray[vtextNum][i] = temparr[i];
						}
					}
					else
					{
						std::cout << "ERROR: Could not add texture vertex to texture vertex Array" << std::endl;
						return 0;
					}
					vtextNum++;
				}
				else if (line.at(0) == 'v')
				{
					float temp;
					float temparr[3];
					std::stringstream ss;
					ss << line;

					int i = 0;
					std::string t;
					while (!ss.eof()) {
						ss >> t;
						if (std::stringstream(t) >> temp && i <= 3) {
							temparr[i] = temp;
							i++;
						}
						t = "";
					}

					if (vertNum <= this->NumVertices)
					{
						for (int i = 0; i < 3; i++)
						{
							vertexArray[vertNum][i] = temparr[i];
						}
					}
					else
					{
						std::cout << "ERROR: Could not add vertex to vertexArray" << std::endl;
						return 0;
					}
					vertNum++;
				}
				else if (line.at(0) == 'f')
				{
					int temp;
					int temparr[3];
					int temparr2[3];
					std::stringstream ss;
					ss << line;

					int i = 0;
					std::string t;
					while (!ss.eof()) {
						ss >> t;
						//std::cout << "Token: " << t << std::endl;
						if (std::stringstream(t) >> temp && i <= 3) {
							temparr[i] = temp;
							t = t.substr(t.find_first_of('/') + 1);
							if (std::stringstream(t) >> temp) {
								temparr2[i] = temp;
							}

							i++;
						}
						t = "";
					}

					if (faceNum <= this->NumFaces)
					{
						for (int i = 0; i < 3; i++)
						{
							faces[faceNum][i] = temparr[i];
						}
						for (int i = 0; i < 2; i++)
						{
							faces_texture[faceNum][i] = temparr2[i];
						}
					}
					else
					{
						std::cout << "ERROR: Could not add face to faces" << std::endl;
						return 0;
					}

					faceNum++;
				}
			}
		}
		std::cout << "Done Reading File" << std::endl;
		return 1;
	}
	return -1;
}

int objParser::read(GLfloat * vertexArray, GLuint * faces, GLuint * textureArray, GLuint * faces_texture)
{
	///TODO: Create Arrays and read file
	std::cout << "This read is not done, use the other one" << std::endl;
	return 0;
}
int objParser::read(GLfloat *vertexArray, GLuint *faces)
{
	///TODO: Create Arrays and read file
	std::cout << "This read is not done, use the other one" << std::endl;
	return 0;
}
