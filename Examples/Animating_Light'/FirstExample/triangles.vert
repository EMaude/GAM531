#version 430 

layout (location=0) in vec4 vPosition;
layout (location=1) in vec3 vNormal;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;  //Camera Position
out  vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

void main()
{
    fN = vNormal;
    fE = (ModelView*vPosition).xyz;
    
	if( LightPosition.w == 0.0 ) {	//Directional Light
	fL = LightPosition.xyz;
    }

    if( LightPosition.w != 0.0 ) {	//Positional Light
	fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*vPosition;
}
