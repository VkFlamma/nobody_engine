#version 460 core

layout(constant_id = 0) const uint SPECIALIZATION = 0u;

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 size;
layout(location = 2) in uint index;

layout(location = 0) flat out vec2 fragSize;
layout(location = 1) out vec2 uv;
layout(location = 2) flat out uint fragIndex;

void main() {
	gl_Position = vec4(position, 0.0f, 1.0f);
	uv = vec2((gl_VertexIndex >> 1u) & 1u, gl_VertexIndex & 1u);
	fragSize = size;
	fragIndex = index;
}

 