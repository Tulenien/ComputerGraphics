#ifndef ITEM_H
#define ITEM_H

#include <cmath>
#include <QFile>
#include <vector>
#include <QColor>
#include <QDebug>
#include <QImage>
#include <algorithm>
#include <QFileSystemModel>

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
    QColor ka;    // Ambient color
    QColor kd;    // Diffuse color
    QColor ks;    // Specular color
    double ni;    // Optical density
    double ns;    // Focus of specular highlights
};

struct volumeBorder
{
    double minX, maxX;
    double minY, maxY;
    double minZ, maxZ;
};

struct XY { int x, y; };

class Item
{
public:
    Item(QString dir, QString file);
    // Dynamicly change Borders
    // when Item is rotated/ translated
    // Use Z coordinates later to choose closest and farthest
    // point and set Znear, Zfar with it
    point_t getCenter();
    void findBorders();
    bool compareViewModes(bool sceneMode);

    void move(double x, double y, double z);
    // Rotate with scene
    void rotateOY(double angle);
    void rotateOX(double angle);
    void rotateOZ(double angle);
    // Self-rotate
    void spinOY(double angle);
    void spinOX(double radAngle);
    void rasterise(const matrix &projection, const int &imageWidth, const int &imageHeight, double floorLevel);
    void render(matrix &buffer, QImage *&image, QMap<QString, Item *> &clickSearch, int width, int height);
    bool changeIsClicked();
    void outline(QImage *&image);

private:
    // Border left-down and right-up screen coordinates
    int ldx, ldy, rux, ruy;
    bool isClicked = false;
    bool itemView = false;
    volumeBorder borders =
    {
        qInf(), qInf(),
        qInf(), qInf(),
        qInf(), qInf()
    };
    matrix vOriginal, nOriginal;
    matrix transform;
    matrix vPerspective, nPerspective;
    matrix textures;

    bool multiplyMatrix(matrix &A, const matrix &B);
    bool multiplyMatrix(const matrix &A, const matrix &B, matrix &C);
    void loadObj(const QString dir, const QString file);
    void loadMtl(const QString path);
    QMap<QString, material> materialMap;
    QList<polygon> polygons;

    double edgeCheck(const std::vector<double> &a, const std::vector<double> &b, const std::vector<double> &c);
};

#endif // ITEM_H
