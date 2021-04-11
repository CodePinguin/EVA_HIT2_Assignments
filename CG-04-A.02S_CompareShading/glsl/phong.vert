// phong shading vertex shader

#version 400 core


void doPhongShading(in mat4 matModelView, in vec3 vecPosition, in vec3 vecNormal, out vec3 vsVertex, out vec3 vsNormal)
{
    // calculate eye coordinates of vertex- and normal vectors, because
    // specular reflection is calculated in eye space coordinates
    vsVertex = vec3(matModelView * vec4(vecPosition, 1.0));

    // transform vertex normal orientation into eye coordinates using normal matrix
    mat3 normal_matrix = transpose( inverse( mat3(matModelView) ) );
    vsNormal = normalize( normal_matrix * vecNormal );
}
