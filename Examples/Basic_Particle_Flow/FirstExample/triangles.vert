#version 430 core
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;
layout(location = 2) in float vertexSpeed;
layout(location = 3) in float vertexSpawnTime;
layout(location = 4) in float vertexAngle;


uniform float ellapsed_time;
uniform mat4 model_matrix;


out vec4 myColor;

void main()
{
	myColor = vertexColor;

	float gravity = 9.81;
	vec4 tmp_pos = vPosition;
	if (ellapsed_time > vertexSpawnTime)
	{
		float delta_time = ellapsed_time - vertexSpawnTime;
		float vx = vertexSpeed * cos(vertexAngle);
		float vy = vertexSpeed * sin(vertexAngle);

		tmp_pos.x = vx * delta_time;
		tmp_pos.y = vy * delta_time - (1/2.0f) * gravity * delta_time * delta_time;
	}

	gl_Position = model_matrix * tmp_pos;

}
