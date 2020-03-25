#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec2 vTex0;
varying vec2 vTex1;

void main()
{
	vec4 color0 = texture2D(texture0, vTex0);
	vec4 color1 = texture2D(texture1, vTex1);
	if(color1.r >= .95 && color1.g >= .95 && color1.b >= .95)
	{
		gl_FragColor = color0;
	}
	else
	{
		gl_FragColor = color1;
	}
	
}
