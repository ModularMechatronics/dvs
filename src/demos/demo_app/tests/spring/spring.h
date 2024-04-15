#ifndef DEMOS_TESTS_SPRING_SPRING_H_
#define DEMOS_TESTS_SPRING_SPRING_H_

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace spring_structure
{

class SpringStructure
{
public:
    float m;
    float C;
    float K;
    float g;
    float h;

    float* iP;
    float* P;
    float* V;
    float* A;
    float* Vp;
    float F;
    float R;
    float* Rs;

    float* Ls;  // Length between connections

    int* conn;
    size_t N;

    float Ca;  // Air resistance

    float vp[3];
    float v[3];
    float a[3];

    int idx_0, idx_1, idx_2, cidx;
    int lim;

    float friction_threshold;
    float speed_threshold;
    float mu;  // Friction
    float Nf;
    float v_speed;
    float v_0, v_2;

    int Ne;  // Number of edges

    float r0, r1, r2;
    float vproj[3];
    int max_length;

    int* edge_conn;
    int* point_to_edge_index;
    int max_connections;

    void update();
    void setVelocity(float dir[3], float amp);
    void calculateEdges(int* conn);
    SpringStructure(const std::string& conn_file_name,
                    const std::string& pos_file_name,
                    float h_,
                    float C_,
                    float K_,
                    float m_,
                    float Ca_,
                    float x0,
                    float y0,
                    float z0);
    void resetStructure();
    SpringStructure() = default;
};

void testBasic();

}  // namespace spring_structure

#endif  // DEMOS_TESTS_SPRING_SPRING_H_
