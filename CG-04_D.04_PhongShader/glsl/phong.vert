// phong shading vertex shader
// (for one light only and without emissive component)

#version 400


layout (location = 0) in vec3 vecPosition;
layout (location = 1) in vec3 vecNormal;

uniform mat4 matModelView;
uniform mat4 matProjection;

out vec3 vecPositionEC;
out vec3 vecNormalEC;



void main(void)
{
    // calculate eye coordinates of vertex- and normal vectors, because
    // specular reflection is calculated in eye space coordinates
    vecPositionEC = vec3(matModelView * vec4(vecPosition, 1.0));

    // transform vertex normal orientation into eye coordinates using normal matrix
    mat3 normal_matrix = transpose( inverse( mat3(matModelView) ) );
    vecNormalEC = normalize( normal_matrix * vecNormal );

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}