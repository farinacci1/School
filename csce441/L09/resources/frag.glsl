#version 120
varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPosCam;

varying vec2 vTex0;
varying vec2 vTex1;
varying vec2 vTex2;
vec3 computeColor(vec3 norm,vec3 kd, vec3 ks)
{
	//compute diffuse
	vec3 lightDir = normalize(lightPosCam - f_pos.xyz);
	float theta = max(0.0,dot(lightDir,norm));
	vec3 diffuse = kd * theta ;
	
	vec3 viewDir = normalize(lightPosCam-f_pos.xyz);
	vec3 h = normalize(viewDir + lightDir);
	vec3 spec = ks * pow(max(0.0,dot(h,norm)),50.0f);
	
	
	return (diffuse + spec);
}
void main()
{
	vec3 norm = normalize(f_nor);
	vec3 kd = texture2D(texture0, vTex0).rgb;
	vec3 ks = texture2D(texture1, vTex1).rgb;
	vec3 clouds = texture2D(texture2, vTex2).rgb;
	vec3 col = computeColor(norm,kd, ks);
	if(clouds.r <= 0.3 && clouds.g <= 0.3 && clouds.b <= 0.3)
	{
		gl_FragColor = vec4(col, 1.0);
	}
	else
	{
		gl_FragColor = vec4(clouds, 1.0);
	}
}
