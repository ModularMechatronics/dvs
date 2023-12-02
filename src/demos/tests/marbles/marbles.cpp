#include "tests/marbles/marbles.h"

#include "tests/sphere/sphere_subdivision/point.h"
#include "tests/sphere/sphere_subdivision/subdivisor.h"

namespace marbles
{
struct Marble
{
    Vector<IndexTriplet> indices;
    Vector<double> x;
    Vector<double> y;
    Vector<double> z;
    Marble() = delete;
    Marble(const int num_subdivisions)
    {
        sphere::Subdivisor subdivisor{num_subdivisions, 1.0};
        int N = subdivisor.P_subdivided.size();

        x.resize(N);
        y.resize(N);
        z.resize(N);

        for (int c = 0; c < N; c++)
        {
            sphere::Point p = subdivisor.P_subdivided[c];
            x(c) = p.x;
            y(c) = p.y;
            z(c) = p.z;
        }

        indices.resize(subdivisor.facelist_subdivided.faces.size());
        for (int c = 0; c < subdivisor.facelist_subdivided.faces.size(); c++)
        {
            const sphere::Face f = subdivisor.facelist_subdivided.faces[c];
            const sphere::Edge e1 = subdivisor.edgelist_subdivided.edges[f.i1];
            const sphere::Edge e2 = subdivisor.edgelist_subdivided.edges[f.i2];
            const sphere::Edge e3 = subdivisor.edgelist_subdivided.edges[f.i3];

            std::vector<int> ind = {e1.i1, e1.i2, e2.i1, e2.i2, e3.i1, e3.i2};

            std::sort(ind.begin(), ind.end());
            ind.erase(std::unique(ind.begin(), ind.end()), ind.end());

            indices(c) = IndexTriplet(ind[0], ind[1], ind[2]);
        }
    }
};

class Marbles
{
public:
    std::vector<Marble> marbles_;

public:
    Marbles() = delete;
    Marbles(const int num_marbles)
    {
        for (size_t k = 0; k < num_marbles; k++)
        {
            marbles_.emplace_back(2);
        }
    }

    ~Marbles() {}

    void run();
};

void testBasic()
{
    const std::string project_file_path = "../../project_files/small_demo.dvs";
    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    std::vector<ItemId> ids;

    ids.push_back(properties::ID0);
    ids.push_back(properties::ID1);
    ids.push_back(properties::ID2);
    ids.push_back(properties::ID3);

    Marbles marbles(ids.size());

    globalIllumination({2.0, 2.0, 2.0});

    axis({-1.0, -1.0, -5.0}, {10.0, 10.0, 5.0});
    for (size_t k = 1; k < ids.size(); k++)
    {
        double kd = k;
        drawMesh(marbles.marbles_[k].x,
                 marbles.marbles_[k].y,
                 marbles.marbles_[k].z + kd,
                 marbles.marbles_[k].indices,
                 ids[k],
                 properties::EdgeColor::NONE);
        setTransform(ids[k], diagMatrix<double>({1.0, 1.0, 1.0}), rotationMatrixZ<double>(0), {kd, 0, 0});
        std::cout << "Plotting marble " << k << std::endl;

        // setProperties(
        //     ids[k], properties::Transform{diagMatrix<double>({1.0, 1.0, 1.0}), rotationMatrixZ<double>(0), {kd, 0,
        //     0}});
    }
    // scatter3(marbles.x, marbles.y, marbles.z, properties::PointSize(20), properties::ScatterStyle::DISC);
}

}  // namespace marbles
