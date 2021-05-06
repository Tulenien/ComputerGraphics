#ifndef ITEM_H
#define ITEM_H

#include "matrix.h"
#include <QFile>

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
    point_t Center();
    void Move();
    // Rotate Item around
    void Spin(double angleOY);
    // Rotate with scene: center always (0, 0, 0)
    void Rotate(double angleOX, double angleOY);

private:
    // To check collisions with other point
    point_t minPoint, maxPoint;
    void getBorder();

    // Starting points matrix
    Matrix vOriginal, nOriginal;
    // Matrix to save operations of shift and rotate
    // Able to change Items positions
    Matrix transform;
    // Matrix to save after transformation for further do
    Matrix vShifted, nShifted;
    // Matrix to save coordinates on scene and to rotate
    // For rotating scene only
    Matrix vScene, nScene;
    // Not able to move/rotate nor items nor scene
    Matrix vPerspective, nPerspective;
    // Textures, not changed
    Matrix textures;

    // Load .obj file, call loadMtl from inside
    void loadObj(const QString dir, const QString file);

    // Read from .mtl file
    QMap<QString, material> materialMap;
    void loadMtl(const QString path);

    QList<polygon> polygons;
};

#endif // ITEM_H
