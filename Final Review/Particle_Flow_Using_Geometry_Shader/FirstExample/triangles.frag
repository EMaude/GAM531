#version 430 core

in float ee_time;
out vec4 fColor;

void main()
{
	if (ee_time >0.8f) discard;
	fColor = vec4(1.5*(ee_time - int(ee_time)), 1 - ee_time - int(ee_time), ee_time - int(ee_time), 1.0);
}