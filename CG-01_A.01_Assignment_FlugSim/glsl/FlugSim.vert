// texturing vertex shader (using gouraud shading)

#version 420

layout (location = 0) in vec3 vecPosition;
layout (location = 2) in vec2 vecTexture;
layout (location = 3) in vec4 vecColor;

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matTexture;

out vec4 vecSurfaceColor;
out vec4 texCoord;
out vec4 use_texture;


void main(void)
{
    use_texture = vec4(1,0,0,0);

    if (vecColor.x > 0 || vecColor.y > 0 || vecColor.z > 0)
    {
        gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
        vecSurfaceColor = vecColor;
        use_texture = vec4(0,0,0,0); //tell frag shader to use color normally
        return;
    }

    // output total fragment color
    vecSurfaceColor = vec4(1,1,1,1); // Itotal;

    // use model texture coordinates and transform with texture matrix
    texCoord = matTexture * vec4(vecTexture, 0.0, 1.0);

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}