
vec4 lighting(Material mat, Light lgt, vec4 point, vec4 eyev, vec4 normalv, bool isShadowed)
{
	vec4  eff_color         = mat.color * lgt.color;
	vec4  lightv            = normalize(lgt.position - point);
	vec4  new_ambient       = eff_color * lgt.ambient;
	float light_dot_normal  = dot(normalv, lightv);

	if (isShadowed)
		return new_ambient;

	if (light_dot_normal < 0.0)
	{
		return new_ambient;
	}
	else
	{
		vec4 diffuset = eff_color * 0.9 * light_dot_normal;
		vec4 reflectv = reflect(-lightv, normalv);
		float reflect_dot_eye = dot(reflectv, eyev);

		if (reflect_dot_eye < 0.0)
		{
			// debugData[index] = vec4(mat.color.xyz, 3.0);
			vec4 new_specular = vec4(0, 0, 0, 0);
			return new_ambient + diffuset + new_specular;
		}
		
		// debugData[index] = vec4(mat.color.xyz, 4.0);
		float factor = pow(reflect_dot_eye, 200.0);
		vec4 specularv = lgt.color * lgt.specular * factor;
		return new_ambient + diffuset + specularv;
	}
}