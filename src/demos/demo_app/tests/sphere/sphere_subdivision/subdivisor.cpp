#include "subdivisor.h"

#include <math.h>

#include <fstream>
#include <iostream>

namespace sphere
{

#define NUM_EDGES 30
#define NUM_POINTS 12

int edges_array[2][NUM_EDGES] = {
    {0, 0, 4, 0, 9, 9, 5, 4, 5, 8, 8, 10, 8, 3, 5, 5, 2, 2, 7, 7, 7, 6, 7, 11, 11, 0, 1, 9, 9, 11},
    {4, 1, 1, 9, 4, 5, 4, 8, 8, 1, 10, 1, 3, 10, 3, 2, 3, 7, 3, 10, 6, 10, 11, 6, 0, 6, 6, 11, 2, 2}};

float points_array[3][NUM_POINTS] = {{-0.5257311121,
                                      0.5257311121,
                                      -0.5257311121,
                                      0.5257311121,
                                      0,
                                      0,
                                      0,
                                      0,
                                      0.8506508084,
                                      -0.8506508084,
                                      0.8506508084,
                                      -0.8506508084},
                                     {0,
                                      0,
                                      0,
                                      0,
                                      0.8506508084,
                                      0.8506508084,
                                      -0.8506508084,
                                      -0.8506508084,
                                      0.5257311121,
                                      0.5257311121,
                                      -0.5257311121,
                                      -0.5257311121},
                                     {0.8506508084,
                                      0.8506508084,
                                      -0.8506508084,
                                      -0.8506508084,
                                      0.5257311121,
                                      -0.5257311121,
                                      0.5257311121,
                                      -0.5257311121,
                                      0,
                                      0,
                                      0,
                                      0}};

bool pointExists(Point p, std::vector<Point> point_list)
{
    bool b = false;

    for (int k = 0; k < point_list.size(); k++)
    {
        if (p == point_list[k])
        {
            b = true;
            break;
        }
    }
    return b;
}

int indexOfPoint(Point p, std::vector<Point> point_list)
{
    int idx = -1;

    for (int k = 0; k < point_list.size(); k++)
    {
        if (p == point_list[k])
        {
            idx = k;
            break;
        }
    }
    return idx;
}

void calculateNewPointsAndEdges(
    Edgelist edgelist, std::vector<Point> P, Facelist facelist, Edgelist& new_edges, std::vector<Point>& Pn)
{
    Pn = P;

    for (int k = 0; k < facelist.faces.size(); k++)
    {
        Face face = facelist.faces[k];

        Edge edge1 = edgelist.edges[face.i1];
        Edge edge2 = edgelist.edges[face.i2];
        Edge edge3 = edgelist.edges[face.i3];

        Point p11 = P[edge1.i1];
        Point p12 = P[edge1.i2];

        Point p21 = P[edge2.i1];
        Point p22 = P[edge2.i2];

        Point p31 = P[edge3.i1];
        Point p32 = P[edge3.i2];

        int idx11 = indexOfPoint(p11, Pn);
        int idx12 = indexOfPoint(p12, Pn);

        int idx21 = indexOfPoint(p21, Pn);
        int idx22 = indexOfPoint(p22, Pn);

        int idx31 = indexOfPoint(p31, Pn);
        int idx32 = indexOfPoint(p32, Pn);

        Point p1 = (P[edge1.i1] + P[edge1.i2]) / 2.0f;
        Point p2 = (P[edge2.i1] + P[edge2.i2]) / 2.0f;
        Point p3 = (P[edge3.i1] + P[edge3.i2]) / 2.0f;

        p1.normalize();
        p2.normalize();
        p3.normalize();

        bool p1_exists = false;
        bool p2_exists = false;
        bool p3_exists = false;
        int p1_idx, p2_idx, p3_idx;

        if (pointExists(p1, Pn))
        {
            p1_exists = true;
            p1_idx = indexOfPoint(p1, Pn);
        }
        if (pointExists(p2, Pn))
        {
            p2_exists = true;
            p2_idx = indexOfPoint(p2, Pn);
        }
        if (pointExists(p3, Pn))
        {
            p3_exists = true;
            p3_idx = indexOfPoint(p3, Pn);
        }

        if (!p1_exists)
        {
            Pn.push_back(p1);
            p1_idx = Pn.size() - 1;
        }
        if (!p2_exists)
        {
            Pn.push_back(p2);
            p2_idx = Pn.size() - 1;
        }
        if (!p3_exists)
        {
            Pn.push_back(p3);
            p3_idx = Pn.size() - 1;
        }

        new_edges.edges.push_back(Edge(p1_idx, p2_idx));
        new_edges.edges.push_back(Edge(p1_idx, p3_idx));
        new_edges.edges.push_back(Edge(p2_idx, p3_idx));

        new_edges.edges.push_back(Edge(p1_idx, idx11));
        new_edges.edges.push_back(Edge(p1_idx, idx12));

        new_edges.edges.push_back(Edge(p2_idx, idx21));
        new_edges.edges.push_back(Edge(p2_idx, idx22));

        new_edges.edges.push_back(Edge(p3_idx, idx31));
        new_edges.edges.push_back(Edge(p3_idx, idx32));
    }

    new_edges.removeDuplicates();
}

Facelist calculateFaces(Edgelist edgelist)
{
    Facelist facelist;
    int idx_e = 0;
    int q_c, q_n, e_d;
    for (auto e : edgelist.edges)
    {
        int idx_q = 0;

        for (auto q : edgelist.edges)
        {
            if (e != q)
            {
                if ((q.i1 == e.i1 && q.i2 != e.i2) || (q.i1 == e.i2 && q.i2 != e.i1))
                {
                    if (e.i1 == q.i1 || e.i2 == q.i1)
                    {
                        q_n = q.i2;
                    }
                    else
                    {
                        q_n = q.i1;
                    }

                    if (e.i1 == q.i1 || e.i1 == q.i2)
                    {
                        e_d = e.i2;
                    }
                    else
                    {
                        e_d = e.i1;
                    }
                    int idx_w = 0;
                    for (auto w : edgelist.edges)
                    {
                        if (w != e && w != q)
                        {
                            Edge new_edge(e_d, q_n);
                            if (w == new_edge)
                            {
                                Face new_face(idx_e, idx_q, idx_w);
                                facelist.faces.push_back(new_face);
                            }
                        }
                        idx_w++;
                    }
                }
            }
            idx_q++;
        }
        idx_e++;
    }
    facelist.removeDuplicates();

    return facelist;
}

void subdivide(Edgelist edgelist,
               std::vector<Point> P,
               Facelist facelist,
               Edgelist& new_edges,
               std::vector<Point>& Pn,
               Facelist& new_faces)
{
    calculateNewPointsAndEdges(edgelist, P, facelist, new_edges, Pn);
    new_faces = calculateFaces(new_edges);
}

void findPointConnectivity(Edgelist edgelist,
                           std::vector<Point> P,
                           std::vector<std::vector<int>>& connectivity,
                           std::vector<int>& num_edges)
{
    for (int idx_p = 0; idx_p < P.size(); idx_p++)
    {
        std::vector<int> indices;
        for (int idx_e = 0; idx_e < edgelist.edges.size(); idx_e++)
        {
            Edge e = edgelist.edges[idx_e];
            if (e.i1 == idx_p)
            {
                indices.push_back(e.i2);
            }
            else if (e.i2 == idx_p)
            {
                indices.push_back(e.i1);
            }
        }
        connectivity.push_back(indices);
        num_edges.push_back(indices.size());
    }
}

void findPointsIndicesToEdgeIndices(std::vector<std::vector<int>>& point_indices_to_edge_index,
                                    Edgelist edgelist,
                                    std::vector<Point> P)
{
    /*Facelist facelist_subdivided;
    Edgelist edgelist_subdivided;
    std::vector<Point> P_subdivided;

    std::vector<std::vector<int>> connectivity_subdivided;
    std::vector<int> num_edges_subdivided;*/
    point_indices_to_edge_index.resize(P.size());

    for (int i = 0; i < P.size(); i++)
    {
        point_indices_to_edge_index[i].resize(P.size());
        for (int j = 0; j < P.size(); j++)
        {
            if (i != j)
            {
                for (int k = 0; k < edgelist.edges.size(); k++)
                {
                    Edge e = edgelist.edges[k];
                    if (e.i1 == i && e.i2 == j)
                    {
                        point_indices_to_edge_index[i][j] = k;
                    }
                    else if (e.i1 == j && e.i2 == i)
                    {
                        point_indices_to_edge_index[i][j] = k;
                    }
                }
            }
        }
    }

    {
    }
}

std::vector<std::vector<int>> findFaceIndices(Facelist& facelist, Edgelist& edgelist)
{
    // Contains vector where every vector represents point indices for
    // than face
    std::vector<std::vector<int>> face_indices;
    for (int k = 0; k < facelist.faces.size(); k++)
    {
        std::vector<int> index_vector;
        index_vector.resize(6);

        Face f = facelist.faces[k];

        Edge e1 = edgelist.edges[f.i1];
        Edge e2 = edgelist.edges[f.i2];
        Edge e3 = edgelist.edges[f.i3];
        index_vector[0] = e1.i1;
        index_vector[1] = e1.i2;
        index_vector[2] = e2.i1;
        index_vector[3] = e2.i2;
        index_vector[4] = e3.i1;
        index_vector[5] = e3.i2;

        std::sort(index_vector.begin(), index_vector.end());
        auto last = std::unique(index_vector.begin(), index_vector.end());
        index_vector.erase(last, index_vector.end());

        face_indices.push_back(index_vector);
    }
    return face_indices;
}

void Subdivisor::saveToText()
{
    std::ofstream facelist_file;
    std::ofstream edgelist_file;
    std::ofstream connectivity_file;
    std::ofstream points_file;
    std::ofstream num_edges_file;

    facelist_file.open("../data/facelist.txt");
    edgelist_file.open("../data/edgelist.txt");
    connectivity_file.open("../data/connectivity.txt");
    points_file.open("../data/points.txt");
    num_edges_file.open("../data/num_edges.txt");

    for (Face f : facelist_subdivided.faces)
    {
        facelist_file << std::to_string(f.i1) + "," + std::to_string(f.i2) + "," + std::to_string(f.i3) << std::endl;
    }
    for (Edge e : edgelist_subdivided.edges)
    {
        edgelist_file << std::to_string(e.i1) + "," + std::to_string(e.i2) << std::endl;
    }
    for (std::vector<int> c : connectivity_subdivided)
    {
        std::string s = "";
        for (int i : c)
        {
            s = s + std::to_string(i);
            if (i != c.back())
            {
                s = s + ",";
            }
        }
        connectivity_file << s << std::endl;
    }
    for (Point p : P_subdivided)
    {
        points_file << std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) << std::endl;
    }
    for (int i : num_edges_subdivided)
    {
        num_edges_file << std::to_string(i) << std::endl;
    }

    facelist_file.close();
    edgelist_file.close();
    connectivity_file.close();
    points_file.close();
    num_edges_file.close();
}

void Subdivisor::readTextFiles()
{
    Facelist facelist_read;
    Edgelist edgelist_read;
    std::vector<Point> P_read;
    std::vector<std::vector<int>> connectivity_read;
    std::vector<int> num_edges_read;

    std::ifstream facelist_file("../data/facelist.txt");
    std::ifstream edgelist_file("../data/edgelist.txt");
    std::ifstream connectivity_file("../data/connectivity.txt");
    std::ifstream points_file("../data/points.txt");
    std::ifstream num_edges_file("../data/num_edges.txt");

    std::string s = "";

    while (std::getline(facelist_file, s))
    {
        std::cout << s << std::endl;
    }
}

Subdivisor::Subdivisor(int num_subdivisions, float radius)
{
    for (int k = 0; k < NUM_POINTS; k++)
    {
        P_0.push_back(Point(points_array[0][k], points_array[1][k], points_array[2][k]));
    }

    for (int k = 0; k < NUM_EDGES; k++)
    {
        edgelist_0.edges.push_back(Edge(edges_array[0][k], edges_array[1][k]));
    }

    facelist_0 = calculateFaces(edgelist_0);

    Facelist facelist_old = facelist_0;
    Edgelist edgelist_old = edgelist_0;
    std::vector<Point> P_old = P_0;

    // Subdivide
    for (int k = 0; k < num_subdivisions; k++)
    {
        facelist_subdivided.faces.clear();
        edgelist_subdivided.edges.clear();
        P_subdivided.clear();

        subdivide(edgelist_old, P_old, facelist_old, edgelist_subdivided, P_subdivided, facelist_subdivided);

        edgelist_old = edgelist_subdivided;
        facelist_old = facelist_subdivided;
        P_old = P_subdivided;
    }

    findPointConnectivity(edgelist_subdivided, P_subdivided, connectivity_subdivided, num_edges_subdivided);

    for (int k = 0; k < P_subdivided.size(); k++)
    {
        Point p = P_subdivided[k];
        const float f = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
        p.x = radius * p.x / f;
        p.y = radius * p.y / f;
        p.z = radius * p.z / f;
        P_subdivided[k] = p;
    }

    face_point_indices = findFaceIndices(facelist_subdivided, edgelist_subdivided);
}

Subdivisor::Subdivisor() {}

}  // namespace sphere
