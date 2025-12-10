shader_type spatial;
render_mode unshaded;

uniform sampler2D grid_map;

void fragment() {
	vec2 uv = UV * 8.;
	uv.y *= 4.;
	uv.y -= TIME * 6.;
	vec4 col = texture(grid_map, uv);
	ALBEDO = col.rgb;
	float proximity = pow(UV.y, 3);
	ALPHA = col.a * proximity * .1;
}