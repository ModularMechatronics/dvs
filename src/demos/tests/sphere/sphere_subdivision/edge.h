#ifndef EDGE_H
#define EDGE_H

#include <vector>

namespace sphere
{

class Edge
{
private:
public:
    int i1;
    int i2;
    Edge();
    Edge(int i1_, int i2_);

    bool operator==(const Edge e)
    {
        bool c1 = e.i1 == this->i1 && e.i2 == this->i2;
        bool c2 = e.i1 == this->i2 && e.i2 == this->i1;
        return c1 || c2;
    }
    bool operator!=(const Edge e)
    {
        bool c1 = e.i1 == this->i1 && e.i2 == this->i2;
        bool c2 = e.i1 == this->i2 && e.i2 == this->i1;
        return !(c1 || c2);
    }
};

class Edgelist
{
private:
public:
    Edgelist();
    std::vector<Edge> edges;

    bool hasEdge(Edge edge);
    int indexOfEdge(Edge edge);
    void removeDuplicates();
    void printEdges();
};

}  // namespace sphere

#endif