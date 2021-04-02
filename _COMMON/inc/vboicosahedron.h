#ifndef VBOICOSAHEDRON_H
#define VBOICOSAHEDRON_H


class VBOIcosahedron
{
private:
    unsigned int vaoHandle;

public:
    VBOIcosahedron();

    void draw() const;

    int getVertexArrayHandle();
};

#endif // VBOICOSAHEDRON_H
