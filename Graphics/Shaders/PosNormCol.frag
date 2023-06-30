#version 450

// IN
layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec4 inWorldPos;
layout (location = 3) in mat4 inViewInverse;

// OUT
layout (location = 0) out vec4 outFragColor;

// VARS
float gShininess = 25.f;

layout (set = 0, binding = 1) uniform SceneData
{
    vec4 fogColor;
	vec4 fogDistance;
	vec4 ambientColor;
	vec4 lightDir;
	vec4 lightColor;
} sceneData;

// HELPERS
vec3 calculate_diffuse(vec3 col, vec3 norm)
{
	float diffuseStrength = dot(norm, -sceneData.lightDir.xyz);
	diffuseStrength = diffuseStrength * 0.5f + 0.5f;
	diffuseStrength = clamp(diffuseStrength, 0.f, 1.f);
	return col * diffuseStrength;
}

// Blinn model
vec3 calculate_specular(vec3 viewDirection, vec3 normal)
{
    // HALFWAY DIRECTION
	vec3 halfAngle = -normalize(viewDirection + sceneData.lightDir.xyz);
	float specularSterngth = clamp(dot(halfAngle, normal), 0.f, 1.f);

	// SPECULAR
	float specularity = pow(specularSterngth, gShininess);

	return vec3(specularity);
}

// MAIN
void main()
{
	// View direction
	// vec3 viewDirection = normalize(inWorldPos.xyz - inViewInverse[3].xyz);

	vec3 diffuse = calculate_diffuse(inColor, inNormal);
	outFragColor = vec4(diffuse + sceneData.ambientColor.xyz * sceneData.ambientColor.w, 1.f);
}