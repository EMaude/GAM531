#version 430 core

layout ( points ) in;
layout ( triangle_strip, max_vertices = 4 ) out;

in float e_time[];

out float ee_time;

void main(void){

	ee_time = e_time[0];

	vec4 offset = vec4(-0.01, 0.01, 0.0, 0.0);
	vec4 vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = vertexPos;
	EmitVertex();
	
	offset = vec4(-0.01, -0.01, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = vertexPos;
	EmitVertex();
	
	offset = vec4(0.01, 0.01, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = vertexPos;
	EmitVertex();
	
	offset = vec4(0.01, -0.01, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = vertexPos;
	EmitVertex();
	
	EndPrimitive();
	

}