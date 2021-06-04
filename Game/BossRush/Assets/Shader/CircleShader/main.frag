#version 450

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vNormal;
in float time;

uniform vec2 resolution;
uniform vec3 SColor;

void main() {

	vec2 position = vec2((gl_FragCoord.x / resolution.x) - 0.5, (gl_FragCoord.y / resolution.y) - 0.5);
	position =vec2((gl_FragCoord.x / resolution.x) - 0., (gl_FragCoord.y / resolution.y));
	vec2 coord = mod(position,1.0);	// coordinate of single effect window (0.0 - 1.0)
	float fade = 0.0;
	float gradient = 0.0;
	vec2 centered_coord = coord - vec2(0.5);
			float z = (0.125 - centered_coord.x * centered_coord.x - centered_coord.y * centered_coord.y);
			vec2 tex = vec2(centered_coord.x * 16.0 / z, centered_coord.y * 16.0/z);
			fade = z*1.1;
			vec2 discolamp = vec2(pow(sin(tex.x + sin(0.000001 * time) * 64.0) + 1.0, 2.0), pow(sin(tex.y + sin(0.000001 * time) * 128.0) + 1.0, 2.0));
			gradient = (4.0 - discolamp.x - discolamp.y) * fade;
			vec3 color = vec3(gradient * 3.0 + SColor.x, gradient + SColor.y, gradient / 1.0 + SColor.z);


	gl_FragColor = vec4(color, 34.0);
}





































