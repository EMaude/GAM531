#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in float vertexSpeed;
layout(location = 3) in float vertexSpawnTime;
layout(location = 4) in float vertexAngle;


uniform float ellapsed_time;
uniform mat4 model_matrix;


//out vec4 myColor;
out float e_time;

void main()
{
	
	float fi = ellapsed_time;
	float gravity = 9.81;
	vec4 tmp_pos = vPosition;
	mat4 rot_y;
	if (ellapsed_time > vertexSpawnTime)
	{
		float delta_time = ellapsed_time - vertexSpawnTime;
		e_time = delta_time;
		float vx = vertexSpeed * cos(vertexAngle);
		float vy = vertexSpeed * sin(vertexAngle);
		
		rot_y[0] = vec4(cos(fi), 0, -sin(fi), 0);
		rot_y[1] = vec4(0, 1, 0, 0);
		rot_y[2] = vec4(sin(fi), 0, cos(fi), 0);
		rot_y[3] = vec4(0, 0, 0, 1);

		tmp_pos.x = vx * delta_time;
		tmp_pos.y = 0.8 + vy * delta_time - (1/2.0f) * gravity * delta_time * delta_time;
		
	}

	gl_Position = model_matrix * tmp_pos;

}
