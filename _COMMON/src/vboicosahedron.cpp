// system includes ////////////////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cmath>

// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <glm/gtc/constants.hpp>

#include "../inc/vboicosahedron.h"


static GLsizei IndexCount;
static const GLuint PositionSlot = 0;

VBOIcosahedron::VBOIcosahedron()
{
    const int Faces[] = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,

        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,

        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,

        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10 };

    const float Verts[] = {
        0.000f,  0.000f,  1.000f,
        0.894f,  0.000f,  0.447f,
        0.276f,  0.851f,  0.447f,
        -0.724f,  0.526f,  0.447f,
        -0.724f, -0.526f,  0.447f,
        0.276f, -0.851f,  0.447f,
        0.724f,  0.526f, -0.447f,
        -0.276f,  0.851f, -0.447f,
        -0.894f,  0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
        0.724f, -0.526f, -0.447f,
        0.000f,  0.000f, -1.000f };

    IndexCount = sizeof(Faces) / sizeof(Faces[0]);

    for (int i = 0; i<IndexCount; i++)
    {
        //glm:vec3()
    }


    // Create the VAO:
    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Create the VBO for positions:
    GLuint positions;
    GLsizei stride = 3 * sizeof(float);
    glGenBuffers(1, &positions);
    glBindBuffer(GL_ARRAY_BUFFER, positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(PositionSlot);
    glVertexAttribPointer(PositionSlot, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Create the VBO for indices:
    GLuint indices;
    glGenBuffers(1, &indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Faces), Faces, GL_STATIC_DRAW);
}

void VBOIcosahedron::draw() const {
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_PATCHES, IndexCount, GL_UNSIGNED_INT, 0);
}


int VBOIcosahedron::getVertexArrayHandle() {
    return this->vaoHandle;
}

