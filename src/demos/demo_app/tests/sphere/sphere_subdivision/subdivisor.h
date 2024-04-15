#ifndef SUBDIVISOR_H
#define SUBDIVISOR_H

#include <vector>

#include "edge.h"
#include "face.h"
#include "point.h"
namespace sphere
{

class Subdivisor
{
private:
public:
    Subdivisor(int num_subdivisions, float radius);
    Subdivisor();

    Facelist facelist_0;
    Edgelist edgelist_0;
    std::vector<Point> P_0;

    Facelist facelist_subdivided;
    Edgelist edgelist_subdivided;
    std::vector<Point> P_subdivided;
    std::vector<std::vector<int>> connectivity_subdivided;
    std::vector<int> num_edges_subdivided;
    std::vector<std::vector<int>> face_point_indices;

    std::vector<std::vector<int>> point_indices_to_edge_index;

    void saveToText();
    void readTextFiles();
};
}  // namespace sphere

#endif