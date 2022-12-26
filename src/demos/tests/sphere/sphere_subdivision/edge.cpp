#include "edge.h"

#include <iostream>

namespace sphere
{

Edge::Edge(int i1_, int i2_)
{
    i1 = i1_;
    i2 = i2_;
}

Edge::Edge() {}

Edgelist::Edgelist() {}

bool Edgelist::hasEdge(Edge edge)
{
    bool has_edge = false;
    for (int k = 0; k < edges.size(); k++)
    {
        if (edges[k] == edge)
        {
            has_edge = true;
            break;
        }
    }

    return has_edge;
}
int Edgelist::indexOfEdge(Edge edge)
{
    int edge_idx = -1;
    for (int k = 0; k < edges.size(); k++)
    {
        if (edges[k] == edge)
        {
            edge_idx = k;
            break;
        }
    }

    return edge_idx;
}

void Edgelist::printEdges()
{
    for (int k = 0; k < edges.size(); k++)
    {
        Edge edge = edges[k];
        std::cout << "[ " << edge.i1 << ", " << edge.i2 << " ]" << std::endl;
    }
}

void Edgelist::removeDuplicates()
{
    std::vector<bool> is_duplicate;

    for (int k = 0; k < edges.size(); k++)
    {
        is_duplicate.push_back(false);
    }

    for (int k = 0; k < edges.size(); k++)
    {
        if (!is_duplicate[k])
        {
            Edge edge = edges[k];

            for (int j = k; j < edges.size(); j++)  // Could probably be j = k+1, but unsure
            {
                if (j != k)
                {
                    if (edges[j] == edge)
                    {
                        is_duplicate[j] = true;
                    }
                }
            }
        }
    }

    for (int k = edges.size() - 1; k >= 0; k--)
    {
        if (is_duplicate[k])
        {
            edges.erase(edges.begin() + k);
        }
    }
}

}  // namespace sphere
