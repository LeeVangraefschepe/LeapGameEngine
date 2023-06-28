#version 450

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;

layout (location = 0) out vec3 outColor;

// Push constant
layout (push_constant) uniform constants
{
	vec4 data;
	mat4 wvp;
} pushConstants;

void main()
{
    gl_Position = pushConstants.wvp * vec4(vPosition, 1.0);
    outColor = vColor;
}