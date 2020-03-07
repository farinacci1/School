#version 120


uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform float Li1;
uniform float Li2;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space

vec3 computeColor(vec3 lightSource,float intensity,vec3 norm)
{
	//compute ambient (just use ka for this assignment)
	vec3 ambient = ka;

	//compute diffuse
	vec3 lightVec = normalize(lightSource - f_pos.xyz);
	float theta = max(0.0,dot(lightVec,norm));
	vec3 diffuse = kd * theta ;

	//compute specular lighting
	vec3 eye = normalize(-f_pos.xyz);
	vec3 h = normalize(eye + lightVec);
	vec3 spec = ks * pow(max(0.0,dot(h,norm)),s);
	
	
	return intensity * (ambient + diffuse + spec);
}
void main()
{
	//normalize normals
	vec3 norm = normalize(f_nor);
	vec3 final_color =  computeColor(lightPos1,Li1,norm) + computeColor(lightPos2,Li2,norm) ;
	


	//return color
	gl_FragColor = vec4(final_color, 1.0);
	
}
