
#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec4 particlecolor;

// Ouput data
out vec4 color;

uniform sampler2D Texture;

void main(){
	color = texture( Texture, UV ) * particlecolor;
}