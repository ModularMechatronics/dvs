#ifndef POINT_H
#define POINT_H
namespace sphere
{

class Point
{
private:
public:
    float x;
    float y;
    float z;

    void operator=(const Point p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }

    Point operator*(const float f)
    {
        Point po;
        po.x = this->x * f;
        po.y = this->y * f;
        po.z = this->z * f;

        return po;
    }

    Point operator/(const float f)
    {
        Point po;
        po.x = this->x / f;
        po.y = this->y / f;
        po.z = this->z / f;

        return po;
    }

    Point operator+(const Point p)
    {
        Point po;
        po.x = this->x + p.x;
        po.y = this->y + p.y;
        po.z = this->z + p.z;

        return po;
    }

    Point operator-(const Point p)
    {
        Point po;
        po.x = this->x - p.x;
        po.y = this->y - p.y;
        po.z = this->z - p.z;

        return po;
    }

    bool operator==(const Point p)
    {
        bool c1 = this->x == p.x;
        bool c2 = this->y == p.y;
        bool c3 = this->z == p.z;

        return c1 && c2 && c3;
    }
    bool operator!=(const Point p)
    {
        bool c1 = this->x == p.x;
        bool c2 = this->y == p.y;
        bool c3 = this->z == p.z;

        return !(c1 && c2 && c3);
    }

    Point();
    Point(float x, float y, float z);
    void printPoint();
    void normalize();
};

}  // namespace sphere

#endif
