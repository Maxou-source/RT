vec4 lighting(Material mat, PointLight lgt, vec4 point, vec4 eyev, vec4 normalv)
{
	vec4  eff_color         = mat.color * lgt.color;
	vec4  lightv            = normalize(lgt.position - point);
	vec4  new_ambient       = eff_color * lgt.ambient;
	float light_dot_normal  = dot(normalv, lightv);

	int px = int(gl_FragCoord.x);
	int py = int(gl_FragCoord.y);
	int index = py * 1080 + px; 
		debugData[index] = vec4(mat.color.xyz, 1.0);

	if (light_dot_normal < 0.0)
	{
		debugData[index] = vec4(mat.color.xyz, 2.0);
		return new_ambient;
	}
	else
	{
		vec4 diffuset = eff_color * 0.9 * light_dot_normal;
		// GLSL's built-in reflect(I, N) expects I as the incident vector
		// (pointing INTO the surface), which is exactly -lightv here —
		// same convention as your CPU Tuple::reflect(negate(lightv), normalv)
		vec4 reflectv = reflect(-lightv, normalv);
		float reflect_dot_eye = dot(reflectv, eyev);

		if (reflect_dot_eye < 0.0)
		{
			debugData[index] = vec4(mat.color.xyz, 3.0);
			vec4 new_specular = vec4(0, 0, 0, 0);
			return new_ambient + diffuset + new_specular;
		}
		
		debugData[index] = vec4(mat.color.xyz, 4.0);
		float factor = pow(reflect_dot_eye, lgt.shininess);
		vec4 specularv = lgt.color * lgt.specular * factor;
		return new_ambient + diffuset + specularv;
	}
}
