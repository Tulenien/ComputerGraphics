#ifndef ITEM_H
#define ITEM_H

#include <cmath>
#include <QFile>
#include <vector>
#include <QColor>
#include <QDebug>
#include <QImage>


#define PI 3.14159265

typedef std::vector<std::vector<double>> matrix;

struct point_t
{
    double x;
    double y;
    double z;
};

struct intPoint_t
{
    int x;
    int y;
    int z;
};

struct polygon
{
    // QMap dictionary key
    QString materialKey;
    // Array index in matrix
    std::size_t points[3];
    std::size_t normals[3];
    std::size_t textures[3];
};

struct material
{
    double illum; // Illumination model
    QColor ka;  // Ambient color
    QColor kd;  // Diffuse color
    QColor ks;  // Specular color
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
    void setImage(QImage *&image);

    void move(double x, double y, double z);
    // Rotate with scene
    void rotateOY(double angle);
    void rotateOX(double angle);
    void spin(double angle);
    // Projection
    void project(const matrix &projection);
    void normalise(const int imageHeight, const int imageWidth);
    void rasterise();
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

    // Originated in Scene
    matrix *depthBuffer;
    QImage *image;

    // Change y-coordinate to put item on floor
    bool multiplyMatrix(matrix &A, const matrix &B);
    bool multiplyMatrix(const matrix &A, const matrix &B, matrix &C);

    void loadObj(const QString dir, const QString file);
    void loadMtl(const QString path);
    QMap<QString, material> materialMap;
    QList<polygon> polygons;
};

#endif // ITEM_H
