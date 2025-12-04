shader_type spatial;
render_mode cull_disabled;

uniform sampler2D tex;

void vertex() {
	/*
	vec3 face_vec3 = -MODEL_MATRIX[0].xyz;
	vec4 face_vec4 = -MODEL_MATRIX[0];
	
	float vec_dif = abs(dot(MODEL_MATRIX[0].xyz, CAMERA_DIRECTION_WORLD));
	
	MODELVIEW_MATRIX = VIEW_MATRIX * 
	mat4(
		vec4(normalize(cross(face_vec3, INV_VIEW_MATRIX[2].xyz)), 0.0),
		face_vec4,
		vec4(normalize(cross(INV_VIEW_MATRIX[0].xyz, face_vec3)), 0.0),
		MODEL_MATRIX[3]
	);
	// */
}

void fragment() {
	vec4 col = texture(tex, UV);
	ALBEDO = col.rgb;
	ALPHA = col.a * .2;
}
