#version 430 core
layout(location = 0) in vec4 vPosition;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 projection_matrix;

void main()
{
	gl_Position = projection_matrix * camera_matrix * model_matrix * vPosition;
}
