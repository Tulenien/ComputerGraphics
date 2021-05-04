#ifndef ITEM_H
#define ITEM_H

#include "matrix.h"

struct Polygon
{
    double *& point0;
    double *& point1;
    double *& point2;
};

struct Point
{
    double x;
    double y;
    double z;

};

class Item
{
public:
    Item();
    Point Center();
    void Move();
    void RotateOY();

private:
    Point center;
    Matrix PointsMatrix;
    // References points from PointsMatrix
    Polygon *polygons;
    // Normals, Colors
};

#endif // ITEM_H
