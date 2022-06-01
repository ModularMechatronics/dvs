#ifndef RRT_H_
#define RRT_H_

#include <dvs/dvs.h>

#include <time.h>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <utility>
#include <chrono>

#include "utils.h"

using namespace dvs;


struct Edge
{
    Vec2Dd e0;
    Vec2Dd e1;
};

struct RrtNode
{
    Point2Dd p;
    RrtNode* parent;
    double cost_to_come;
    size_t num_nodes_before;
    std::vector<RrtNode*> children;

    RrtNode() : parent(nullptr) {}
    RrtNode(const Vec2Dd p_, const double cost_to_come_) : p(p_), parent(nullptr), cost_to_come(cost_to_come_), num_nodes_before(0) {}
};

std::pair<size_t, double> findNearestNeighbour(const Point2Dd& p, const std::vector<double>& x, const std::vector<double>& y)
{
    double min_dist = sqDist(p.x, x[0], p.y, y[0]);
    size_t min_idx = 0;
    for(size_t k = 1; k < x.size(); k++)
    {
        const double current_dist = sqDist(p.x, x[k], p.y, y[k]);
        if(current_dist < min_dist)
        {
            min_idx = k;
            min_dist = current_dist;
        }
    }

    return std::pair<size_t, double>(min_idx, min_dist);
}

std::pair<RrtNode*, double> findNearestNeighbour(const Point2Dd& p, RrtNode* node, RrtNode* current_min_node, const double current_min_distance)
{
    const double node_dist = sqDist(p.x, node->p.x, p.y, node->p.y);

    double min_dist;
    RrtNode* min_dist_node;

    if(node_dist < current_min_distance)
    {
        min_dist = node_dist;
        min_dist_node = node;
    }
    else
    {
        min_dist = current_min_distance;
        min_dist_node = current_min_node;
    }

    for(RrtNode* child : node->children)
    {
        const std::pair<RrtNode*, double> new_pair = findNearestNeighbour(p, child, min_dist_node, min_dist);
        if(new_pair.second < min_dist)
        {
            min_dist = new_pair.second;
            min_dist_node = new_pair.first;
        }
    }

    return std::pair<RrtNode*, double>(min_dist_node, min_dist);
}

std::pair<RrtNode*, double> findNearestNeighbour(const Point2Dd& p, RrtNode* root_node)
{
    double min_dist = sqDist(p.x, root_node->p.x, p.y, root_node->p.y);
    RrtNode* min_dist_node = root_node;

    for(RrtNode* child : root_node->children)
    {
        const std::pair<RrtNode*, double> new_pair = findNearestNeighbour(p, child, min_dist_node, min_dist);
        if(new_pair.second < min_dist)
        {
            min_dist_node = new_pair.first;
            min_dist = new_pair.second;
        }
    }

    return std::pair<RrtNode*, double>(min_dist_node, min_dist);
}

class RrtSearcher
{
private:
    RrtNode root_node_;
    RrtNode *final_node_;
    Vec2Dd start_point_;
    Vec2Dd goal_point_;
    double max_dist_;
    double domain_min_;
    double domain_max_;
    double max_dist_to_goal_sq_;

    size_t cnt_;

public:
    RrtSearcher() = default;
    RrtSearcher(const Vec2Dd& start_point, const Vec2Dd& goal_point, const double max_dist, const double domain_min, const double domain_max, const double max_dist_to_goal) :
        root_node_(start_point, 0.0), final_node_(nullptr), start_point_(start_point), goal_point_(goal_point), max_dist_(max_dist), domain_min_(domain_min), domain_max_(domain_max),
        max_dist_to_goal_sq_(max_dist_to_goal * max_dist_to_goal), cnt_(0)
    {}

    RrtNode getRootNode() const
    {
        return root_node_;
    }

    RrtNode* getFinalNode() const
    {
        return final_node_;
    }

    bool step()
    {
        const double xn = randNumInDomain(domain_min_, domain_max_);
        const double yn = randNumInDomain(domain_min_, domain_max_);

        const Vec2Dd rand_point(xn, yn);

        std::pair<RrtNode*, double> np = findNearestNeighbour(rand_point, &root_node_);

        RrtNode* min_dist_node = np.first;
        const double min_dist = np.second;

        const double dist_mul = min_dist > max_dist_ ? max_dist_ / min_dist : 1.0;

        const Point2Dd pt_nearest(min_dist_node->p.x, min_dist_node->p.y);

        const Vec2Dd vec_to_new = rand_point - pt_nearest;
        const Vec2Dd vec_to_new_norm = vec_to_new * dist_mul;
        const Point2Dd new_point = pt_nearest + vec_to_new_norm;

        RrtNode* new_node = new RrtNode(new_point, min_dist_node->cost_to_come + min_dist);
        new_node->parent = min_dist_node;
        new_node->num_nodes_before = min_dist_node->num_nodes_before + 1;

        cnt_++;

        min_dist_node->children.push_back(new_node);

        const double sq_dist = sqDist(goal_point_.x, new_node->p.x, goal_point_.y, new_node->p.y);

        if(sq_dist < max_dist_to_goal_sq_)
        {
            final_node_ = new_node;
            return true;
        }
        else
        {
            return false;
        }
    }
};

void rrtStep(std::vector<double>& x, std::vector<double>& y, std::vector<Edge>& edges, const double max_dist, const double domain_min, const double domain_max)
{
    const double xn = randNumInDomain(domain_min, domain_max);
    const double yn = randNumInDomain(domain_min, domain_max);

    const std::pair<size_t, double> nearest = findNearestNeighbour({xn, yn}, x, y);

    const double dist_mul = nearest.second > max_dist ? max_dist / nearest.second : 1.0;

    const Point2Dd pt_nearest(x[nearest.first], y[nearest.first]);

    const Vec2Dd vec_to_new = Vec2Dd(xn, yn) - pt_nearest;
    const Vec2Dd vec_to_new_norm = vec_to_new * dist_mul;
    const Point2Dd new_point = pt_nearest + vec_to_new_norm;

    edges.push_back({pt_nearest, new_point});

    x.push_back(new_point.x);
    y.push_back(new_point.y);
}



#endif
