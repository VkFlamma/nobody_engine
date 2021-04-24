#version 460 core

#define MAX_VOXEL_OBJECT 256
#define MAX_BONES 8

layout(constant_id = 0) const uint SPECIALIZATION = 0u;

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 inColor;
layout(location = 2) in int boneIndex;

layout(location = 0) out vec4 outColor;

layout(std430, binding = 0, set = 0) restrict readonly buffer ProjectionBuffer {
	mat4 projection;
};

struct Uniform {
	mat4 modelMatrix;
	mat4 boneMatrices[MAX_BONES];
};

layout(std430, binding = 0, set = 1) restrict readonly buffer UniformBuffer {
	Uniform u[];
};

layout(push_constant) uniform PushConst {
	uint index;
};

void main() {
	gl_Position = projection * u[index].modelMatrix * u[index].boneMatrices[boneIndex] * vec4(position, 1.0f);
	outColor = inColor;
}

 