#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <QFile>
#include <QDebug>
#include <cmath>

#define PI 3.14159265

typedef std::vector<std::vector<double>> matrix;

struct point_t
{
    double x;
    double y;
    double z;
};

struct polygon
{
    // QMap dictionary key
    QString materialKey;
    // Array index in matrix
    int points[3];
    int normals[3];
    int textures[3];
};

struct rgbColor
{
    double red;
    double green;
    double blue;
};

struct material
{
    double illum; // Illumination model
    rgbColor ka;  // Ambient color
    rgbColor kd;  // Diffuse color
    rgbColor ks;  // Specular color
    double ni;    // Optical density
    double ns;    // Focus of specular highlights
};

class Item
{
public:
    Item(QString dir, QString file);
    // Get item center
    point_t centerXZ();
    // If height changes -- change item offset to reach floor on scene
    void setToFloor(double height);
    void setDepthBuffer(matrix *depthBuffer);

    void move(double x, double y, double z);
    // Rotate with scene
    void rotateOY(double angle);
    void rotateOX(double angle);
    void spin(double angle);
    // Projection
    void project(const matrix &projection);
    void normalise(const int imageHeight, const int imageWidth);
    // TODO: use minPoint, maxPoint
    void getBorder();

private:
    // To check collisions with other point
    point_t minPoint, maxPoint;
    double toFloor;
    // Starting points matrix
    matrix vOriginal, nOriginal;
    /* Matrix to save all operations on original matrix:
     * scene rotation
     * item spin
     * item move (x, y, z)
    */
    matrix transform;
    matrix vPerspective, nPerspective;
    matrix textures;
    matrix *depthBuffer = nullptr;

    // Change y-coordinate to put item on floor
    bool multiplyMatrix(matrix &A, const matrix &B);
    bool multiplyMatrix(const matrix &A, const matrix &B, matrix &C);

    void loadObj(const QString dir, const QString file);
    void loadMtl(const QString path);
    QMap<QString, material> materialMap;
    QList<polygon> polygons;
};

#endif // ITEM_H
