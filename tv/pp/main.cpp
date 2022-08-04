#include <dvs/dvs.h>

#include <time.h>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <utility>
#include <chrono>

#include "rrt.h"

using namespace dvs;

/*
Select random node
Connect to nearest node in tree - don't place it too far away (max_distance)
Ignore point if inside obstacle
Select random new node
*/

template <typename T, typename... Us>
void drawPoint(const Point2D<T> p, const Us&... settings)
{
    const Vector<double> x = {p.x}, y = {p.y};
    scatter(x, y, settings...);
}

template <typename T, typename... Us>
void drawFromPointAndVec(const Point2D<T>& p, const Vec2D<T>& v, const Us&... settings)
{
    const Point2D<T> pq = p + v;
    drawLineBetweenPoints(p, pq, settings...);
}


void runExp0()
{
    const size_t max_it = 1000;

    const double min_dist_to_goal = 1.0;
    const double max_dist = 1.5;
    const double domain_min = -10.0;
    const double domain_max = 10.0;

    const Vector<double> x0 = {0.3}, y0 = {-9.8};
    const Vector<double> x_g = {8.3}, y_g = {9.2};

    std::vector<double> x_values, y_values;
    std::vector<Edge> edges;

    x_values.reserve(2 * max_it);
    y_values.reserve(2 * max_it);

    x_values.push_back(x0(0));
    y_values.push_back(y0(0));

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for(size_t k = 0; k < max_it; k++)
    {
        rrtStep(x_values, y_values, edges, max_dist, domain_min, domain_max);
        /*if(std::sqrt(sqDist(x_values[x_values.size() - 1], y_values[x_values.size() - 1], x_g(0), y_g(0))) < min_dist_to_goal)
        {
            break;
        }*/
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    Vector<double> x = x_values;
    Vector<double> y = y_values;

    Vector<double> x_lines(edges.size() * 4);
    Vector<double> y_lines(edges.size() * 4);

    setCurrentElement("view_00");
    clearView();
    view(0.0, 90.0);
    axis({domain_min, domain_min, -1.0}, {domain_max, domain_max, 1.0});

    size_t idx = 0;
    for(size_t k = 0; k < edges.size(); k++)
    {
        // drawLineBetweenPoints(edges[k].e0, edges[k].e1);
        x_lines(idx) = edges[k].e0.x;
        y_lines(idx) = edges[k].e0.y;

        x_lines(idx + 1) = edges[k].e1.x;
        y_lines(idx + 1) = edges[k].e1.y;
        idx += 2;
    }

    // axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    // dvs::plot(t_vec, theta_vec);
    scatter(x, y, properties::PointSize(3), properties::Color(0, 0, 0));
    scatter(x0, y0, properties::PointSize(10), properties::Color(255, 0, 0));
    scatter(x_g, y_g, properties::PointSize(10), properties::Color(0, 0, 255));
    plot(x_lines, y_lines, properties::Color(0, 0, 0));
}


struct PointAndAngle
{
    Point2Dd p;
    double alpha;

    PointAndAngle() : p({0.0, 0.0}), alpha(0.0) {}
    PointAndAngle(const Point2Dd p_, const double alpha_) : p(p_), alpha(alpha_) {}
    PointAndAngle(const Point2Dd p0, const Point2Dd p1) : p(p0), alpha(std::atan2(p1.y - p0.y, p1.x - p0.x)) {}

    double getPhi() const
    {
        return M_PI / 2.0 - alpha;
    }

    Point2Dd getCircleCenter(const double r) const
    {
        const double phi = getPhi();
        const Point2Dd pc(r * std::cos(-phi) + p.x, r * std::sin(-phi) + p.y);

        return pc;
    }

    PointAndAngle getCircularPointAhead(const double r, const double segment_angle) const
    {
        const Point2Dd pc = getCircleCenter(r);
        const Point2Dd zero_point = p - pc;
        const Matrixd r_mat = {{std::cos(-segment_angle), -std::sin(-segment_angle)}, {std::sin(-segment_angle), std::cos(-segment_angle)}};
        const Point2Dd pt = (r_mat * zero_point) + pc;
        const PointAndAngle final_point(pt, alpha - segment_angle);

        return final_point;
    }

    void draw()
    {
        const Point2Dd p_end(std::cos(alpha) + p.x, std::sin(alpha) + p.y);
        drawLineBetweenPoints(p, p_end, properties::LineWidth(4));
    }

    void draw(const double r)
    {
        draw();
        const Point2Dd pc = getCircleCenter(r);
        drawLineBetweenPoints(p, pc, properties::Color(255, 0, 255));
    }
};

void drawCircleSegment(const PointAndAngle q, const double segment_angle, const double radius)
{
    const size_t num_pts = 10;

    Vector<double> x(num_pts), y(num_pts);

    PointAndAngle rotated_point;
    const double delta_angle = segment_angle / static_cast<double>(num_pts - 1);

    for(size_t k = 0; k < num_pts; k++)
    {
        rotated_point = q.getCircularPointAhead(radius, static_cast<double>(k) * delta_angle);
        x(k) = rotated_point.p.x;
        y(k) = rotated_point.p.y;
    }

    rotated_point.draw(radius);

    plot(x, y, properties::Color(255, 0, 0));
    // scatter(x, y, properties::Color(255, 0, 0), properties::PointSize(3));
}

void runExp1()
{
    const size_t max_it = 11;

    const size_t num_pts = 10;

    const double domain_min = -10.0;
    const double domain_max = 10.0;

    const double r = 1.4;

    setCurrentElement("view_00");
    clearView();
    view(0.0, 90.0);
    axis({domain_min, domain_min, -1.0}, {domain_max, domain_max, 1.0});

    for(size_t k = 0; k <= 360; k += 4)
    {
        const double k_d = k;
        PointAndAngle q({1.2, 0.4}, k_d * M_PI / 180.0);
        q.draw(-r);
        drawCircleSegment(q, -M_PI / 4.0, r);
        softClearView();
    }
}

void findPoints(std::vector<double>& x, std::vector<double>& y, std::vector<Edge>& edges, RrtNode* parent_node)
{
    const Vec2Dd parent_node_point = parent_node->p;
    x.push_back(parent_node_point.x);
    y.push_back(parent_node_point.y);

    for(RrtNode* child : parent_node->children)
    {
        const Vec2Dd child_point = child->p;
        edges.push_back({parent_node_point, child_point});
        findPoints(x, y, edges, child);
    }
}

void runExp2()
{
    const size_t max_it = 1000;

    const double min_dist_to_goal = 1.0;
    const double max_dist = 2.5;
    const double domain_min = -10.0;
    const double domain_max = 10.0;

    const Vector<double> x0 = {0.3}, y0 = {-9.8};
    const Vector<double> x_g = {8.3}, y_g = {9.2};

    RrtSearcher rrt_searcher(Vec2Dd(x0(0), y0(0)), Vec2Dd(x_g(0), y_g(0)), max_dist, domain_min, domain_max, min_dist_to_goal);

    std::vector<double> x_values, y_values;
    std::vector<Edge> edges;

    x_values.reserve(2 * max_it);
    y_values.reserve(2 * max_it);

    size_t it;

    for(size_t k = 0; k < max_it; k++)
    {
        it = k;
        const bool res = rrt_searcher.step();

        if(res)
        {
            break;
        }
    }

    std::cout << it << std::endl;

    RrtNode root_node = rrt_searcher.getRootNode();
    RrtNode* final_node = rrt_searcher.getFinalNode();

    std::vector<double> x_final, y_final;

    if(final_node != nullptr)
    {
        RrtNode* current_node = final_node;
        while(true)
        {
            x_final.push_back(current_node->p.x);
            y_final.push_back(current_node->p.y);
            current_node = current_node->parent;
            if(current_node == nullptr)
            {
                break;
            }
        }
    }

    Vector<double> x_vec_final = x_final;
    Vector<double> y_vec_final = y_final;

    std::cout << x_final.size() << std::endl;
    for(size_t k = 0; k < x_vec_final.size(); k++)
    {
        std::cout << x_vec_final(k) << std::endl;
    }

    findPoints(x_values, y_values, edges, &root_node);

    Vector<double> x = x_values;
    Vector<double> y = y_values;

    Vector<double> x_lines(edges.size() * 4);
    Vector<double> y_lines(edges.size() * 4);

    setCurrentElement("view_00");
    clearView();
    view(0.0, 90.0);
    axis({domain_min, domain_min, -1.0}, {domain_max, domain_max, 1.0});

    size_t idx = 0;
    for(size_t k = 0; k < edges.size(); k++)
    {
        x_lines(idx) = edges[k].e0.x;
        y_lines(idx) = edges[k].e0.y;

        x_lines(idx + 1) = edges[k].e1.x;
        y_lines(idx + 1) = edges[k].e1.y;
        idx += 2;
    }

    scatter(x, y, properties::PointSize(3), properties::Color(0, 0, 0));
    scatter(x0, y0, properties::PointSize(10), properties::Color(255, 0, 0));
    scatter(x_g, y_g, properties::PointSize(10), properties::Color(0, 0, 255));
    plot(x_lines, y_lines, properties::Color(0, 0, 0));
    plot(x_vec_final, y_vec_final, properties::Color(255, 0, 0), properties::LineWidth(2));
}

struct Polynomial3
{
    Point2Dd p0;
    Point2Dd p1;
    Point2Dd p2;

    Polynomial3() = default;
    Polynomial3(const Point2Dd& p0_, const Point2Dd& p1_, const Point2Dd& p2_) :
        p0(p0_), p1(p1_), p2(p2_)
    {}
};

struct Polynomial4
{
    Point2Dd p0;
    Point2Dd p1;
    Point2Dd p2;
    Point2Dd p3;

    Polynomial3 toPolynomial3() const
    {
        return Polynomial3(p0, p1, p2);
    }

    Polynomial4() = default;
    Polynomial4(const Point2Dd& p0_, const Point2Dd& p1_, const Point2Dd& p2_, const Point2Dd& p3_) :
        p0(p0_), p1(p1_), p2(p2_), p3(p3_)
    {}

    Point2Dd eval0(const double t) const
    {
        const double t2 = t * t;
        const double t3 = t2 * t;

        return {p3.x * t3 + p2.x * t2 + p1.x * t + p0.x,
                p3.y * t3 + p2.y * t2 + p1.y * t + p0.y};
    }

    Point2Dd eval1(const double t) const
    {
        const double t2 = t * t;

        return {3.0 * p3.x * t2 + 2.0 * p2.x * t + p1.x,
                3.0 * p3.y * t2 + 2.0 * p2.y * t + p1.y};
    }

    Point2Dd eval2(const double t) const
    {
        return {6.0 * p3.x * t + 2.0 * p2.x,
                6.0 * p3.y * t + 2.0 * p2.y};
    }

    Point2Dd eval3() const
    {
        return {6.0 * p3.x,
                6.0 * p3.y};
    }
};

struct Node
{
    Point2Dd p;
    Polynomial4 pn;
};

Polynomial4 findPolynomial(const Polynomial3 p_in, const Point2Dd p)
{
    Polynomial4 p_new(p_in.p0, p_in.p1, p_in.p2, {0.0, 0.0});
    p_new.p3 = -p_in.p0 - p_in.p1 - p_in.p2 + p;

    return p_new;
}

void runExp3()
{
    const double domain_min = 0.0;
    const double domain_max = 1.0;

    const size_t num_nodes = 10;
    const size_t num_points = 100;

    const Vector<double> t = linspaceFromPointsAndCount(0.0, 1.0, num_points);

    const Point2Dd p0(0.3, 0.2);
    const Vec2Dd v(0.1, 0.2);
    const Vec2Dd kappa(-0.2, 0.03);

    const double v_len = 1.0;

    std::vector<Node> nodes(10);
    nodes[0].pn = Polynomial4(p0, v, kappa, Point2Dd(0.0, 0.0));
    nodes[0].p = p0;

    const size_t total_num_points = num_points * (num_nodes - 1);

    Vector<double> x(total_num_points), y(total_num_points);
    Vector<double> dx(total_num_points), dy(total_num_points);
    Vector<double> ddx(total_num_points), ddy(total_num_points);
    Vector<double> theta(total_num_points);
    Vector<double> tv(total_num_points);
    size_t idx = 0;
    Vector<double> xp(num_nodes), yp(num_nodes);
    xp(0) = p0.x;
    yp(0) = p0.y;

    for(size_t k = 1; k < num_nodes; k++)
    {
        const Point2Dd rand_point(randNumInDomain(domain_min, domain_max), randNumInDomain(domain_min, domain_max));
        Node& node = nodes[k];
        node.p = rand_point;
        xp(k) = rand_point.x;
        yp(k) = rand_point.y;
    }

    for(size_t k = 0; k < (num_nodes - 1); k++)
    {
        nodes[k].pn = findPolynomial(nodes[k].pn.toPolynomial3(), nodes[k + 1].p);

        nodes[k + 1].pn.p0 = nodes[k].pn.eval0(1.0);
        nodes[k + 1].pn.p1 = nodes[k].pn.eval1(1.0);
        nodes[k + 1].pn.p2 = nodes[k].pn.eval2(1.0);

        nodes[k + 1].pn.p1 = nodes[k + 1].pn.p1.normalized() * v_len;
        nodes[k + 1].pn.p2 = nodes[k + 1].pn.p2.normalized() * v_len;

        const Polynomial4 pn = nodes[k].pn;

        for(size_t i = 0; i < num_points; i++)
        {
            const Point2Dd pq = pn.eval0(t(i));
            const Point2Dd dpq = pn.eval1(t(i));
            const Point2Dd ddpq = pn.eval2(t(i));
            x(idx) = pq.x;
            y(idx) = pq.y;
            dx(idx) = dpq.x;
            dy(idx) = dpq.y;
            theta(idx) = std::atan2(dy(idx), dx(idx));
            ddx(idx) = ddpq.x;
            ddy(idx) = ddpq.y;
            tv(idx) = static_cast<double>(idx) * 0.1;
            idx++;
        }
        std::cout << pn.p1 << std::endl;
    }

    // TODO: Normalize p1, and possibly p2 and p3

    setCurrentElement("view_00");
    clearView();
    view(0.0, 90.0);
    axis({domain_min, domain_min, -1.0}, {domain_max, domain_max, 1.0});
    // drawPoint(p0, properties::PointSize(10), properties::Color(255, 0, 0));
    scatter(xp, yp, properties::PointSize(10), properties::Color(0, 0, 0));
    // drawPoint(pg, properties::PointSize(10), properties::Color(0, 0, 255));

    plot(x, y, properties::Color(0, 0, 127));
    // scatter(x, y, properties::PointSize(10), properties::Color(255, 255, 0));

    setCurrentElement("new-element-0");
    clearView();
    view(0.0, 90.0);
    plot(tv, theta, properties::Color(0, 0, 127));

    setCurrentElement("new-element-1");
    clearView();
    view(0.0, 90.0);
    plot(tv, ddx, properties::Color(0, 0, 127));

}

bool isInFrontOfVec(const Point2Dd& p, const Vec2Dd& v, const Point2Dd& pg)
{
    const Vec2Dd vg = pg - p;
    return (v.x * vg.x + v.y * vg.y) > 0;
}

Vec2Dd calculateFittedVector(const Point2Dd& p, const Vec2Dd& v, const Point2Dd& pg)
{
    const Vec2Dd vg = pg - p;
    Vec2Dd res;
    if(isInFrontOfVec(p, v, pg) > 0)
    {
        const double phi_line = std::atan2(pg.y - p.y, pg.x - p.x);
        const double phi_point = p.angle();
        if(phi_point < phi_line)
        {
            const double phi = phi_point - phi_line;
            const double theta = -2.0 * phi;
            const double ct = std::cos(theta);
            const double st = std::sin(theta);
            const Matrixd r_mat = {{ct, -st}, {st, ct}};

            res = r_mat * v;
        }
        else
        {
            const double phi = phi_line - phi_point;
            const double theta = 2.0 * phi;
            const double ct = std::cos(theta);
            const double st = std::sin(theta);
            const Matrixd r_mat = {{ct, -st}, {st, ct}};

            res = r_mat * v;
        }
        
    }
    else
    {
        
    }

    return res;

}



void runExp4()
{
    const double domain_min = -1.0;
    const double domain_max = 1.0;

    const Point2Dd p0(0.1, 0.2);
    const Vec2Dd v0(-0.7, -0.3);

    const Matrixd R1 = {{std::cos(M_PI / 2.0), -std::sin(M_PI / 2.0)}, {std::sin(M_PI / 2.0), std::cos(M_PI / 2.0)}};
    const Matrixd R2 = {{std::cos(-M_PI / 2.0), -std::sin(-M_PI / 2.0)}, {std::sin(-M_PI / 2.0), std::cos(-M_PI / 2.0)}};

    const Vec2Dd v0_1 = R1 * v0;
    const Vec2Dd v0_2 = R2 * v0;

    const Point2Dd pg(0.7, 0.5);

    std::vector<double> xv_front, yv_front, xv_back, yv_back;
    double t = 0.0;

    for(size_t k = 0; k < 500; k++)
    {
        const double xt = 0.3 * std::cos(t) + p0.x;
        const double yt = 0.3 * std::sin(t) + p0.y;
        t += 2.0 * M_PI / 500.0;

        if(isInFrontOfVec(p0, v0, {xt, yt}))
        {
            xv_front.push_back(xt);
            yv_front.push_back(yt);
        }
        else
        {
            xv_back.push_back(xt);
            yv_back.push_back(yt);
        }

    }

    Vectord x_front = xv_front;
    Vectord y_front = yv_front;
    Vectord x_back = xv_back;
    Vectord y_back = yv_back;


    setCurrentElement("view_00");
    clearView();
    view(0.0, 90.0);
    axis({domain_min, domain_min, -1.0}, {domain_max, domain_max, 1.0});
    // drawPoint(p0, properties::PointSize(10), properties::Color(0, 0, 0));
    // drawPoint(pg, properties::PointSize(10), properties::Color(255, 0, 0));
    drawFromPointAndVec(p0, v0, properties::LineWidth(3), properties::Color(0, 0, 255));
    drawFromPointAndVec(p0, v0_1, properties::LineWidth(3), properties::Color(255, 0, 0));
    drawFromPointAndVec(p0, v0_2, properties::LineWidth(3), properties::Color(255, 0, 0));
    scatter(x_front, y_front, properties::PointSize(5), properties::Color(0, 127, 255));
    scatter(x_back, y_back, properties::PointSize(5), properties::Color(255, 127, 0));

}


int main()
{
    srand(time(NULL));
    runExp4();
    
}
