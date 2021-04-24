#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in float alive;

layout(std430, binding = 0, set = 0) restrict readonly buffer ProjectionBuffer {
	mat4 projection;
};

struct Uniform {
	mat4 modelMatrix;
};

layout(std430, binding = 0, set = 1) restrict readonly buffer UniformBuffer {
	Uniform u[];
};

layout(push_constant) uniform PushConst {
	uint index;
};

void main() {
	if (alive != 1.0f) {
		gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}
	vec2 pos = vec2(float(gl_VertexIndex & 1u), float((gl_VertexIndex >> 1u) & 1u));
	vec3 pos2 = position + vec3(pos * 0.1f, 0.0f);
	gl_Position = projection * u[index].modelMatrix * vec4(pos2, 1.0f);
}

 