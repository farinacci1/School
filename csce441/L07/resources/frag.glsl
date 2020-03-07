#version 120

uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space

void main()
{
	//normalize normals
	vec3 norm = normalize(f_nor);
	
	

	//compute ambient (just use ka for this assignment)
	vec3 ambient = ka;

	//compute diffuse
	vec3 lightVec = normalize(lightPos - f_pos.xyz);
	float theta = max(0.0,dot(lightVec,norm));
	vec3 diffuse = kd * theta ;

	//compute specular lighting
	vec3 eye = normalize(-f_pos.xyz);
	vec3 h = normalize(eye + lightVec);
	vec3 spec = ks * pow(max(0.0,dot(h,norm)),s);
	
	//compute color
	vec3 color =  ambient + diffuse + spec;
	//return color
	gl_FragColor = vec4(color.r,color.g,color.b, 1.0);
	
}
