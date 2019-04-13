#version 430 core

in vec4 vertexColor;
layout(location = 2) uniform int thickness;
layout(location = 3) uniform vec2 viewport;
uniform sampler2D tex;

out vec4 finalColor;

void main()
{
	finalColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if (thickness > 0) {
		float dx = 1.0 / viewport.x;
		float dy = 1.0 / viewport.y;
		float x = gl_FragCoord.x * dx;
		float y = gl_FragCoord.y * dy;

		float v = 0;
		float yy = y - dy * thickness;
		for (int w = 0; w <= 2 * thickness; w++) {
			float xx = x - dx * thickness;
			for (int h = 0; h <= 2 * thickness; h++) {
				v += float(texture(tex, vec2(xx, yy)).r < 0.1);
				xx += dx;
			}
			yy += dy;
		}
		finalColor = v > 0.0f ? vec4(0.1, 0.1, 0.1, 1.0) : finalColor;
	} else
		finalColor = vec4(1.0, 1.0, 1.0, 1.0);
}