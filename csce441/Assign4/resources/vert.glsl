#version 120


uniform mat4 P;
uniform mat4 MV;
uniform mat4 T;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space

void main()
{
	vec4 worldPos = T *  aPos;
	f_pos = P * MV * worldPos;
	gl_Position = f_pos;
	f_nor = mat3(MV) * aNor;
	
	
}