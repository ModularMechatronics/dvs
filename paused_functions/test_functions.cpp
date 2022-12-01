
void testDrawArrow()
{
    using tp = double;

    Point2<tp> p(0.1, 0.2);
    Vec2<tp> v(0.5, -0.3);

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawArrow(p, v, properties::Color(12, 14, 244));
}

void testQuiver()
{
    using tp = float;
    const size_t num_rows = 100, num_cols = 100;

    Matrix<tp> x, y, u, v;

    const std::pair<Matrix<tp>, Matrix<tp>> mesh_grid = meshGrid(0.1, 100.0, 0.1, 100.0, num_cols, num_rows);
    x = mesh_grid.first;
    y = mesh_grid.second;

    u = dvs::cos(x);
    v = dvs::sin(y);

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    quiver(x, y, u, v, properties::Color(255, 0, 0));
}

void testDrawLine3D()
{
    using tp = double;

    Line3D<tp> line(Point3<tp>(0.0, 0.0, 0.0), Vec3<tp>(1.0, 1.0, 1.0));
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x{VectorInitializer<double>{0.0, 1.0}};
    Vector<double> y{VectorInitializer<double>{0.0, 1.0}};
    Vector<double> z{VectorInitializer<double>{0.0, 1.0}};

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine(line, t0, t1, properties::Color(12, 244, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLine2D()
{
    using tp = double;

    ParametricLine2D<tp> line(Point2<tp>(0.0, 0.0), Vec2<tp>(1.0, 1.0));
    HomogeneousLine2D<tp> line_h(0.1, 0.2, 0.3);
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x{VectorInitializer<double>{0.0, 1.0}};
    Vector<double> y{VectorInitializer<double>{0.0, 1.0}};
    Vector<double> z{VectorInitializer<double>{0.0, 0.0}};

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine2D(line, t0, t1, properties::Color(12, 244, 244));
    drawLine2DBetweenXValues(line_h, -3.0, 3.0, properties::Color(12, 15, 244));
    drawLine2DBetweenYValues(line_h, -2.0, 2.0, properties::Color(12, 15, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLineBetweenPoints()
{
    using tp = double;

    Point3<tp> p0(0.1, 0.2, 0.2);
    Point3<tp> p1(2.0, 1.0, 1.0);

    Vector<double> x{VectorInitializer{p0.x, p1.x}};
    Vector<double> y{VectorInitializer{p0.y, p1.y}};
    Vector<double> z{VectorInitializer{p0.z, p1.z}};

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLineBetweenPoints(p0, p1, properties::Color(12, 244, 0));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}
