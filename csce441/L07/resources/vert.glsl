#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space

void main()
{
	f_pos =  MV * aPos;
	f_nor = mat3(MV) * aNor;
	gl_Position = P * MV * aPos;
	
}
