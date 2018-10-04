#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "vgl.h"

int load(std::string filename, GLfloat vertexArray[][3], GLuint faces[][3]) {
	//open file
	std::cout << " -- Read file started -- " << std::endl;

	std::ifstream file(filename);

	if (file.is_open())
	{
		std::cout << " --File Opened --" << std::endl;
		std::string line;
		int ln = 0;
		int vertNum = 1; //starts at one as faces starts looking at index 1
		int faceNum = 0;
 		while (getline(file, line))
		{
			std::cout << "Reading Line: " << ln << " : " << line << std::endl;
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
						//std::cout << "Token: " << std::endl;
						if (std::stringstream(t) >> temp && i <= 3) {
							//std::cout << "Store: " << temp << std::endl;
							temparr[i] = temp;
							i++;
						}
						t = "";
					}
					
					if (true) //put out of bounds checking
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
					
					if (true) //put out of bounds checking here
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
		file.close();
		std::cout << "Done!" << std::endl;
		return 1;
	}
	else
	{
		std::cout << " ERROR: Cannot open file " << filename << std::endl;
		return -1;
	}
}