#version 330 core

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

layout(location = 0) in vec4 waypoint;

void main() {
   gl_Position = P * V * M * waypoint;
}
