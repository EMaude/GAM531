#include "objParser.h"

int objParser::sizesFromFile()
{
	return 0;
}

objParser::objParser(std::string filename, GLuint NumVertices, GLuint NumFaces)
{
	this->filename = filename;
	this->NumVertices = NumVertices;
	this->NumFaces = NumFaces;

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
		return -1;
	}
}

int objParser::read(GLfloat *vertexArray, GLuint *faces)
{
	std::cout << "This read is not done, use the other one" << std::endl;
	return 0;
}
