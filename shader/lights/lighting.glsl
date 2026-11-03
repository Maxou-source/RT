
vec4 lighting(Material mat, Light lgt, vec4 point, vec4 eyev, vec4 normalv, bool isShadowed)
{
	vec4  eff_color         = mat.color * lgt.color;
	vec4  lightv            = normalize(lgt.position - point);
	float light_dot_normal  = dot(normalv, lightv);

	if (isShadowed)
		return vec4(0.0);

	if (light_dot_normal < 0.0)
		return vec4(0.0);

	vec4 diffuset = eff_color * 0.9 * light_dot_normal;
	vec4 reflectv = reflect(-lightv, normalv);
	float reflect_dot_eye = dot(reflectv, eyev);

	if (reflect_dot_eye < 0.0)
	{
		vec4 new_specular = vec4(0, 0, 0, 0);
		return diffuset + new_specular;
	}
	
	float factor = pow(reflect_dot_eye, 200.0);
	vec4 specularv = lgt.color * lgt.specular * factor;
	return diffuset + specularv;
}