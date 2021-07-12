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

template<typename T>
struct Point
{
    T x;
    T y;
    T z;

    Point (int t_x, int t_y, int t_z) : x(t_x), y(t_y), z(t_z){}
    Point (double t_x, double t_y, double t_z) : x(t_x), y(t_y), z(t_z){}
};

struct Polygon
{
    // QMap dictionary key
    QString materialKey;
    // Array index in matrix
    std::size_t points[3];
    std::size_t normals[3];
    std::size_t textures[3];

    Polygon() {}

    Polygon(const QString &t_materialKey,
            std::size_t p1, std::size_t p2, std::size_t p3,
            std::size_t n1, std::size_t n2, std::size_t n3,
            std::size_t t1, std::size_t t2, std::size_t t3)
        : materialKey(t_materialKey), points{p1, p2, p3},
          normals{n1, n2, n3}, textures{t1, t2, t3}{}

};

struct Material
{
    double illum; // Illumination model
    QColor ka;    // Ambient color
    QColor kd;    // Diffuse color
    QColor ks;    // Specular color
    double ni;    // Optical density
    double ns;    // Focus of specular highlights

    Material() {}

    Material(double t_illum, QColor t_ka, QColor t_kd, QColor t_ks, double t_ni, double t_ns)
        : illum(t_illum), ka(t_ka), kd(t_kd), ks(t_ks), ni(t_ni), ns(t_ns){}
};

struct VolumeBorder
{
    double minX, maxX;
    double minY, maxY;
    double minZ, maxZ;

    VolumeBorder(double t_minx, double t_maxx, double t_miny, double t_maxy, double t_minz, double t_maxz)
        : minX(t_minx), maxX(t_maxx), minY(t_miny), maxY(t_maxy), minZ(t_minz), maxZ(t_maxz){}
};

struct XY
{
    int x, y;

    XY(int x_t, int y_t) : x(x_t), y(y_t){}
};

class Item
{
public:
    Item(const QString &t_dir, const QString &t_file);
    Point<double> getCenter();
    VolumeBorder &getBorders();
    bool ageCheck() const { return m_isNew; }
    void ageChange() { m_isNew = false; }

    void move(double t_x, double t_y, double t_z);
    // Rotate with scene
    void rotateOY(double t_angle);
    void rotateOX(double t_angle);
    void rotateOZ(double t_angle);
    // Self-rotate
    void spinOY(double t_angle);
    matrix topViewMatrix(double t_radAngle);
    void rasterise(const matrix &t_projection, const int &t_imageWidth,
                   const int &t_imageHeight);
    void render(matrix &t_buffer, QImage &t_image, QMap<QString, Item *> &t_clickSearch,
                const int &t_width, const int &t_height);
    bool changeIsClicked();
    void outline(QImage &t_image);

private:
    bool m_isNew;
    bool m_isClicked;
    VolumeBorder m_borders;
    // Border left-down and right-up screen coordinates
    QList<Polygon> m_polygons;
    QMap<QString, Material> m_materialMap;

    int m_ldx{}, m_ldy{}, m_rux{}, m_ruy{};
    matrix m_vOriginal, m_nOriginal;
    matrix m_transform;
    matrix m_vPerspective, m_nPerspective;
    matrix m_textures;

    void findBorders();
    bool multiplyMatrix(matrix &A, const matrix &B);
    bool multiplyMatrix(const matrix &A, const matrix &B, matrix &C);
    void loadObj(const QString &dir, const QString &file);
    void loadMtl(const QString &path);

    double edgeCheck(const std::vector<double> &t_a, const std::vector<double> &t_b, const std::vector<double> &t_c) const
    {
        //    return (t_c[0] - t_a[0]) * (t_b[1] - t_a[1]) - (t_c[1] - t_a[1]) * (t_b[0] - t_a[0]);
        return (t_a[0] - t_c[0]) * (t_b[1] - t_c[1]) - (t_a[1] - t_c[1]) * (t_b[0] - t_c[0]);
    }
};

#endif // ITEM_H
