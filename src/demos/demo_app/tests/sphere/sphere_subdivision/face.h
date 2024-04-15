#ifndef FACE_H
#define FACE_H

#include <vector>

namespace sphere
{

class Face
{
private:
public:
    int i1;
    int i2;
    int i3;
    Face();
    Face(int i1_, int i2_, int i3_);

    bool operator==(const Face e)
    {
        bool c0 = e.i1 == this->i1 && e.i2 == this->i2 && e.i3 == this->i3;

        bool c1 = e.i1 == this->i1 && e.i2 == this->i3 && e.i3 == this->i2;

        bool c2 = e.i1 == this->i2 && e.i2 == this->i1 && e.i3 == this->i3;

        bool c3 = e.i1 == this->i2 && e.i2 == this->i3 && e.i3 == this->i1;

        bool c4 = e.i1 == this->i3 && e.i2 == this->i2 && e.i3 == this->i1;

        bool c5 = e.i1 == this->i3 && e.i2 == this->i1 && e.i3 == this->i2;

        return c0 || c1 || c2 || c3 || c4 || c5;
    }

    bool operator!=(const Face e)
    {
        bool c0 = e.i1 == this->i1 && e.i2 == this->i2 && e.i3 == this->i3;

        bool c1 = e.i1 == this->i1 && e.i2 == this->i3 && e.i3 == this->i2;

        bool c2 = e.i1 == this->i2 && e.i2 == this->i1 && e.i3 == this->i3;

        bool c3 = e.i1 == this->i2 && e.i2 == this->i3 && e.i3 == this->i1;

        bool c4 = e.i1 == this->i3 && e.i2 == this->i2 && e.i3 == this->i1;

        bool c5 = e.i1 == this->i3 && e.i2 == this->i1 && e.i3 == this->i2;

        return !(c0 || c1 || c2 || c3 || c4 || c5);
    }
};

class Facelist
{
private:
public:
    Facelist();
    std::vector<Face> faces;

    bool hasFace(Face face);
    int indexOfFace(Face face);
    void removeDuplicates();
    void printFaces();
};

}  // namespace sphere

#endif
