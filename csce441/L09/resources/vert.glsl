#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat3 T;

attribute vec4 aPos;
attribute vec3 aNor;
attribute vec2 aTex;

varying vec2 vTex0;
varying vec2 vTex1;
varying vec2 vTex2;

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space
void main()
{
	f_pos =  P * MV * aPos;
	f_nor = (MV * vec4(aNor,1.0)).xyz;
	gl_Position = f_pos;
	vTex0 = aTex;
	vTex1 = aTex;
	vTex2 = (T * vec3(aTex,1.0)).xy;
}
