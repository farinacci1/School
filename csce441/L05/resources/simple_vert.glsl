#version 120

uniform mat4 P;
attribute vec3 color;
attribute vec3 vertPos;

varying vec4 f_color;
void main()
{
	gl_Position = P * vec4(vertPos, 1.0);
	f_color = vec4(color, 1.0);
}
