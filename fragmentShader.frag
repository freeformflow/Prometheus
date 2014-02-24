/*
    Purpose: This code describes the function of Prometheus's fragment shader.  Implementing the graphics using the
    OpenGL Shading Language (GLSL) will ensure efficient visualization and contribute to the code's longevity.  After recieving
    vertex coordinates from the vertex shader, this shader will color and light the surface using Phong Shading
    (B. T. Phong, Illumination for computer generated pictures, Communications of ACM 18 (1975), no. 6, 311–317)

    We leverage the power of the GPU to do as much of the heavy lifting as possible.

*/

uniform int representationMethod;

varying vec3 N;
varying vec3 v;

varying vec4 Color;

void main(void)
{
    // If the User has selected either vector or enhanced vector, we need only light the model with ambient light.  Only with the more sophisticated models
    // do we need to use solid surface lighting methods.

    vec4 Iamb, Idiff, Ispec;

    if (representationMethod == 3 || representationMethod == 4)
    {
        // We are dealing with a vector respresentation.  This display type doesn't really use solid surfaces, so
        // we need not resort to sophisticated shading methods.  Use only Ambient Lighting.
        Iamb = vec4(1.0, 1.0, 1.0, 1.0);
        Idiff = vec4(0.0, 0.0, 0.0, 1.0);
        Ispec = vec4 (0.0, 0.0, 0.0, 1.0);
    }
    else if (representationMethod == -1)
    {
        // We are dealing with a "surface" that represents a hightlighted atom within the editor.  It is a re-drawing of the object slightly larger.
        // This new surface is transparent, so the original object can be seen underneath.

        // Calculate Ambient Light Term:
        Iamb = gl_FrontLightProduct[0].ambient;
        //Iamb[4] = 0.6;


        // Calculate Diffuse Light Term:
        //      Using Lambert's Cosine Law, we can determine how brightly a surface is lit from a diffuse light source.
        //      We must take the dot product between the LightSource - Surface vector and the Surface - Camera vector.
        //      N = The Surface - Camera vector       L = The LightSource - Surface vector
        vec3 L = normalize(gl_LightSource[0].position.xyz - v);
        Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);
        //Idiff[4] = 0.6;


        // Calculate Specular Light Term:
        //      In this case we desire no specular highlight.
        Ispec = vec4(0.0, 0.0, 0.0, 0.5);
    }
    else if (representationMethod == -2)
    {
        // We are dealing with the selection box the User sweeps out over the screen.  It is transparent, but we don't want any special lighting tricks done on it.
        // All ambient lighting.

        // Calculate Ambient Light Term:
        Iamb = vec4(1, 1, 1, 0.4);

        // Calculate Diffuse Light Term:
        Idiff = vec4(0, 0, 0, 0.4);

        // Calculate Specular Light Term:
        Ispec = vec4(0.0, 0.0, 0.0, 0.4);
    }
    else if (representationMethod == -3)
    {
            // We are dealing with the Bitmap Creator Tool. It is solid, but we don't want any special lighting tricks done on it.
            // All ambient lighting.

            // Calculate Ambient Light Term:
            Iamb = vec4(1, 1, 1, 1);

            // Calculate Diffuse Light Term:
            Idiff = vec4(0, 0, 0, 0.4);

            // Calculate Specular Light Term:
            Ispec = vec4(0.0, 0.0, 0.0, 0.4);
    }
    else
    {
        // We are dealing with a solid surface model... use the more sophisticated Phong Shading.

        // Calculate Ambient Light Term:
        Iamb = gl_FrontLightProduct[0].ambient;


        // Calculate Diffuse Light Term:
        //      Using Lambert's Cosine Law, we can determine how brightly a surface is lit from a diffuse light source.
        //      We must take the dot product between the LightSource - Surface vector and the Surface - Camera vector.
        //      N = The Surface - Camera vector (from vertex shader)      L = The LightSource - Surface vector

        vec3 L = normalize(gl_LightSource[0].position.xyz - v);
        Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);



        // Calculate Specular Light Term:
        //      Specular reflection is the glare coming off a highly reflective surface.  It's visibility is sensitive to position, so it is approximated
        //      with an exponential die-off.  We need the direction of incoming light to get a reflection vector (R), and we need a vector in the direction of the viewer.
        vec3 E = normalize(-v);
        vec3 R = normalize(-reflect(L,N));
        Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*128.0);
        Ispec = clamp(Ispec, 0.0, 1.0);


     }
        // Compute the Total Color.

        gl_FragColor = (Iamb + Idiff + Ispec) * Color;

}
