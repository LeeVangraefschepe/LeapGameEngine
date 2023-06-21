// GLSL 4.5
#version 450

void main()
{
    // Const array of triangle positions
    const vec3 positions[3] = vec3[3](
        vec3(1.f, 1.f, 0.f),
        vec3(-1.f, 1.f, 0.f),
        vec3(0.f, -1.f, 0.f)
    );

    gl_Position = vec4(positions[gl_VertexIndex], 1.f);
}