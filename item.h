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

struct rgbColor{ double red, green, blue;};

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
    point_t CenterXZ();
    // If height changes -- change item offset to reach floor on scene
    void setToFloor(double height);
    void Move(double x, double y, double z);
    // Rotate with scene: center always (0, 0, 0)
    void RotateOY(double angle);
    // Rotate item separately: need center
    // Only around OY: find max-min X, max-min Z, find average
    void Spin(double angle);

    // Temporary in public. Hide realization after test
    // Matrix operations functions
    // Returns False if sizes are wrong, rewrites results in A
    bool multiplyMatrix(matrix &A, const matrix &B);
    // Returns Copy of matrix in C
    void multiplyMatrix(const matrix &A, const matrix &B, matrix &C);

private:
    // To check collisions with other point
    point_t minPoint, maxPoint;
    void getBorder();

    // Change y-coordinate to put item on floor
    double toFloor;
    // Starting points matrix
    matrix vOriginal, nOriginal;
    // Matrix to save operations of shift and rotate
    // Able to change Items positions
    matrix transform;
    // Matrix to save after transformation for further do
    matrix vShifted, nShifted;
    // Matrix to save coordinates on scene and to rotate
    // For rotating scene only
    matrix vScene, nScene;
    // Not able to move/rotate nor items nor scene
    matrix vPerspective, nPerspective;
    // Textures, not changed
    matrix textures;
    // TODO: Add matrix to store transformations

    // Load .obj file, call loadMtl from inside
    void loadObj(const QString dir, const QString file);
    // Read from .mtl file
    QMap<QString, material> materialMap;
    void loadMtl(const QString path);

    QList<polygon> polygons;
};

#endif // ITEM_H
