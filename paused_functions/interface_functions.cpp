// These functions shall not be included now

template <typename T, typename... Us>
void quiver(const Matrix<T>& x, const Matrix<T>& y, const Matrix<T>& u, const Matrix<T>& v, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::QUIVER};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, u, v);
}

template <typename... Us> void drawArrow(const Point2<double>& p, const Vec2<double> v, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_ARROW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Point2<double>> points{VectorInitializer{p, v}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2D(const ParametricLine2D<double>& line, const double t0, const double t1, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_LINE3D};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0 = line.eval(t0);
    const Point2<double> p1 = line.eval(t1);
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2DBetweenXValues(const HomogeneousLine2D<double>& line,
                              const double x0,
                              const double x1,
                              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_LINE3D};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0(x0, line.evalX(x0));
    const Point2<double> p1(x1, line.evalX(x1));
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2DBetweenYValues(const HomogeneousLine2D<double>& line,
                              const double y0,
                              const double y1,
                              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_LINE3D};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0(line.evalX(y0), y0);
    const Point2<double> p1(line.evalX(y1), y1);
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLineBetweenPoints(const Point3<double>& p0, const Point3<double>& p1, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_LINE_BETWEEN_POINTS_3D};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Point3<double>> points{VectorInitializer{p0, p1}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLineBetweenPoints(const Point2<double>& p0, const Point2<double>& p1, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_LINE_BETWEEN_POINTS_3D};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point3<double> p0_3d(p0.x, p0.y, 0.0), p1_3d(p1.x, p1.y, 0.0);
    Vector<Point3<double>> points{VectorInitializer{p0_3d, p1_3d}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}