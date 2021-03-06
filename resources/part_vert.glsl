#version 330 core

layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color;

out vec2 UV;
out vec4 particlecolor;

uniform vec3 CameraRight;
uniform vec3 CameraUp;
uniform mat4 VP; 

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight * squareVertices.x * particleSize
		+ CameraUp * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);

	// UV of the vertex. No special space for this one.
	UV = squareVertices.xy + vec2(0.5, 0.5);
	particlecolor = color;
}