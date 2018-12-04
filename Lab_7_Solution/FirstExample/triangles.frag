#version 430 core

in vec4 myColor;
out vec4 fColor;

void main()
{
	fColor = myColor;
	//fColor=vec4(1.0, 1.0, 0.0, 1.0);
}