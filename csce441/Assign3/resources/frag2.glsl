#version 120



//silhouette shader

varying vec4 f_pos; //positions in camera space
varying vec3 f_nor; //normals in camera space


void main()
{

    vec3 color;
	//normalize normals
	vec3 norm = normalize(f_nor);
	vec3 eye = normalize(-f_pos.xyz);
	float val = dot(norm,eye);

    if(val <= 0.4)
    {
        color = vec3(0,0,0);
    }
    else
    {
        color = vec3(1,1,1);
    }



	//return color
	gl_FragColor = vec4(color, 1.0);
	
}
