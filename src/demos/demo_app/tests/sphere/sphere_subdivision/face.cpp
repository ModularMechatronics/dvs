#include "face.h"

#include <iostream>
namespace sphere
{

Face::Face(int i1_, int i2_, int i3_)
{
    i1 = i1_;
    i2 = i2_;
    i3 = i3_;
}

Face::Face() {}

Facelist::Facelist() {}

bool Facelist::hasFace(Face face)
{
    bool has_face = false;
    for (int k = 0; k < faces.size(); k++)
    {
        if (faces[k] == face)
        {
            has_face = true;
            break;
        }
    }

    return has_face;
}
int Facelist::indexOfFace(Face face)
{
    int face_idx = -1;
    for (int k = 0; k < faces.size(); k++)
    {
        if (faces[k] == face)
        {
            face_idx = k;
            break;
        }
    }

    return face_idx;
}

void Facelist::printFaces()
{
    for (int k = 0; k < faces.size(); k++)
    {
        Face face = faces[k];
        std::cout << "[ " << face.i1 << ", " << face.i2 << ", " << face.i3 << " ]" << std::endl;
    }
}

void Facelist::removeDuplicates()
{
    std::vector<bool> is_duplicate;

    for (int k = 0; k < faces.size(); k++)
    {
        is_duplicate.push_back(false);
    }

    for (int k = 0; k < faces.size(); k++)
    {
        if (!is_duplicate[k])
        {
            Face face = faces[k];

            for (int j = 0; j < faces.size(); j++)  // Could probably be j = k+1, but unsure
            {
                if (j != k)
                {
                    if (faces[j] == face)
                    {
                        is_duplicate[j] = true;
                    }
                }
            }
        }
    }

    for (int k = faces.size() - 1; k >= 0; k--)
    {
        if (is_duplicate[k])
        {
            faces.erase(faces.begin() + k);
        }
    }
}
}  // namespace sphere
