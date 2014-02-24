/*
    Purpose: This code describes the function of Prometheus's vertex shader.  Implementing the graphics using the
    OpenGL Shading Language (GLSL) ensures efficient visualization and contributes to the code's longevity. Atom
    coordinates are passed to this shader in a vertex buffer object (VBO), but this particular shader doesn't do too much.
    Still, we leverage the power of the GPU to do as much of the heavy lifting as possible.

*/

uniform int representationMethod;

varying vec3 N;
varying vec3 v;

varying vec4 Color;

void main(void)
{
    // Grab color input.
    Color = gl_Color;

    // Calculate the vertex's position in eye (or camera) space...  used in lighting calculations.
    v = vec3(gl_ModelViewMatrix * gl_Vertex);

    // Calculate the vertex's normal...  used in lighting calculations.
    N = normalize(gl_NormalMatrix * gl_Normal);

    // Calculate the vertex's position in 2D screen space.
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
