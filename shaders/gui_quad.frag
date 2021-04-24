#version 460 core

#define HOVERED_TITLE_COLOR vec4(0.16f, 0.29f, 0.48f, 1.0f)
#define ITEM_ACTIVE_COLOR vec4(0.14f, 0.27f, 0.42f, 1.0f)
#define ITEM_COLOR vec4(0.11f, 0.18f, 0.28f, 1.0f)
#define WINDOW_COLOR vec4(0.05f, 0.05f, 0.05f, 1.0f)
#define WINDOW_BORDER_COLOR vec4(0.24f, 0.24f, 0.28f, 1.0f)
#define CLOSE_COLOR vec4(1.0f, 1.0f, 1.0f, 1.0f)

#define WINDOW_TITLE_HEIGHT 20.0f
#define WINDOW_CORNER 10.0f
#define BORDER_WIDTH 1.0f
#define RESIZE_CORNER_SIZE 12.0f
#define CLOSE_SIZE 8.0f
#define CLOSE_MARGIN 7.0f
#define CLOSE_WIDTH 0.7f

#define LETTER_SIZE vec2(10.0f, 14.0f)
#define LETTER_ADVANCE vec2(12.0f, 19.0f)
#define TEXTURE_SIZE vec2(360.0f, 128.0f)
#define TEXTURE_OFFSET vec2(4.0f, 4.0f)

layout(constant_id = 0) const uint SPECIALIZATION = 0u;

layout(location = 0) flat in vec2 size;
layout(location = 1) in vec2 uv;
layout(location = 2) flat in uint index;

layout(location = 0) out vec4 color;

layout(binding = 0) uniform sampler2D font;

float line(vec2 p, vec2 a, vec2 b) {
	vec2 pa = p - a;
	vec2 ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0f, 1.0f);
    return length(pa - ba * h);
}

float closeButton() {
	vec2 p = vec2(size.x - CLOSE_MARGIN - CLOSE_SIZE, CLOSE_MARGIN);
	float d1 = 1.0f - min(line(uv * size, p + vec2(0.0f, 0.0f), p + vec2(CLOSE_SIZE, CLOSE_SIZE)) * CLOSE_WIDTH, 1.0f);
	float d2 = 1.0f - min(line(uv * size, p + vec2(CLOSE_SIZE, 0.0f), p + vec2(0.0f, CLOSE_SIZE)) * CLOSE_WIDTH, 1.0f);
	return max(d1, d2);
}

void main() {
	if (SPECIALIZATION == 0) {
		vec2 c = (1.0f - abs(uv * 2.0f - 1.0f)) / (WINDOW_CORNER * 2.0f / size);
		if (c.x < 1.0f && c.y < 1.0f && length(1.0f - c) > 1.0f) {
			color.a = 0.0f;
		} else if (uv.x < BORDER_WIDTH / size.x || uv.y < BORDER_WIDTH / size.y ||
			uv.x > 1.0f - BORDER_WIDTH / size.x || uv.y > 1.0f - BORDER_WIDTH / size.y ||
			(c.x < 1.0f && c.y < 1.0f && length(1.0f - c) > 1.0f - BORDER_WIDTH / WINDOW_CORNER)) {
			color = WINDOW_BORDER_COLOR;
		} else if (uv.x * size.x + uv.y * size.y > size.x - RESIZE_CORNER_SIZE + size.y - RESIZE_CORNER_SIZE) {
			color = ITEM_COLOR;
		} else if (uv.y < WINDOW_TITLE_HEIGHT / size.y) {
			float close = closeButton();
			color = mix(HOVERED_TITLE_COLOR, CLOSE_COLOR, close);
		} else {
			color = WINDOW_COLOR;
		}
	} else if (SPECIALIZATION == 1) {
		// vec2 P = (vec2(uv * LETTER_SIZE) + vec2(2.0f, 4.0f) + vec2(12.0f * (index % 26), 19.0f * (index / 26.0f))) / textureSize(font, 0);
		vec2 ip = vec2(index % 26, index / 26);
		vec2 P = (uv * LETTER_SIZE + TEXTURE_OFFSET + ip * LETTER_ADVANCE) / TEXTURE_SIZE;
		color = texture(font, P);
		if (color == vec4(1.0f)) {
			color = vec4(0.0f);
		} else {
			color = vec4(1.0f);
		}
	} else if (SPECIALIZATION == 2) {
		color = ITEM_COLOR;
	} else {
		color = vec4(1.0f);
	}
	color.rgb *= color.a;
}
