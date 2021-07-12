#include "item.h"

constexpr int AV_VERTEX_COUNT = 2500;
constexpr int AV_NORMAL_COUNT = 6200;
constexpr int AV_TEXTURE_COUNT = 160;
constexpr int RGB_MAX = 255;

constexpr double FULL_ANGLE = 180.;
constexpr double HALF = 0.5;

Item::Item(const QString &t_dir, const QString &t_file)
    :  m_isNew(true), m_isClicked(false),
       m_borders(qInf(), qInf(), qInf(), qInf(), qInf(), qInf())
{
    m_isClicked = false;

    m_vOriginal.reserve(AV_VERTEX_COUNT);
    m_nOriginal.reserve(AV_NORMAL_COUNT);
    m_textures.reserve(AV_TEXTURE_COUNT);

    m_vPerspective.reserve(AV_VERTEX_COUNT);
    m_nPerspective.reserve(AV_NORMAL_COUNT);
    loadObj(t_dir, t_file);
}

bool Item::multiplyMatrix(matrix &t_A, const matrix &t_B)
{
    // Rewrites result in matrix A
    bool status = true;
    // Standart multiplication algorithm
    std::size_t rows = t_A.size();
    std::size_t cols = t_B[0].size();
    std::size_t nest = t_B.size();
    if (t_A[0].size() != nest) status = false;
    else
    {
        matrix temp;
        temp.reserve(rows);
        for (std::size_t i = 0; i < rows; i++)
        {
            std::vector<double> row(cols);
            for (std::size_t j = 0; j < cols; j++)
            {
                for (std::size_t k = 0; k < nest; k++)
                {
                    row[j] += t_A[i][k] * t_B[k][j];
                }
            }
            temp.emplace_back(row);
        }
        t_A.clear();
        t_A.reserve(rows);
        for (std::size_t i = 0; i < rows; i++)
        {
            t_A.emplace_back(temp[i]);
        }
    }
    return status;
}

bool Item::multiplyMatrix(const matrix &t_A, const matrix &t_B, matrix &t_C)
{
    // Writes result in matrix C
    bool status = true;
    // Standart multiplication algorithm
    std::size_t rows = t_A.size();
    std::size_t cols = t_B[0].size();
    std::size_t nest = t_B.size();
    if (!t_C.empty())
    {
        for (std::vector<double> &c : t_C)
        {
            c.clear();
        }
        t_C.clear();
    }
    if (t_A[0].size() != nest) status = false;
    else
    {
        t_C.reserve(rows);
        for (std::size_t i = 0; i < rows; i++)
        {
            std::vector<double> row(cols);
            for (std::size_t j = 0; j < cols; j++)
            {
                for (std::size_t k = 0; k < nest; k++)
                {
                    row[j] += t_A[i][k] * t_B[k][j];
                }
            }
            t_C.emplace_back(row);
        }
    }
    return status;
}

void Item::rotateOX(double t_angle)
{
    double radAngle = t_angle * PI / FULL_ANGLE;
    const matrix rotate =
    {
        {1, 0, 0, 0},
        {0, cos(radAngle), sin(radAngle), 0},
        {0, -sin(radAngle), cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(m_transform, rotate);
}

void Item::rotateOY(double t_angle)
{
    double radAngle = t_angle * PI / FULL_ANGLE;
    const matrix rotate =
    {
        {cos(radAngle), 0, sin(radAngle), 0},
        {0, 1, 0, 0},
        {-sin(radAngle), 0, cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(m_transform, rotate);
}

void Item::rotateOZ(double t_angle)
{
    double radAngle = t_angle * PI / FULL_ANGLE;
    const matrix rotate =
    {
        {cos(radAngle), sin(radAngle), 0, 0},
        {-sin(radAngle), cos(radAngle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(m_transform, rotate);
}

void Item::move(double t_x, double t_y, double t_z)
{
    matrix move =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {t_x, t_y, t_z, 1}
    };
    multiplyMatrix(m_transform, move);
}

Point<double> Item::getCenter()
{
    Point<double> center =
    {
        (m_borders.minX + m_borders.maxX) * HALF,
        (m_borders.minY + m_borders.maxY) * HALF,
        (m_borders.minZ + m_borders.maxZ) * HALF,
    };
    return center;
}

VolumeBorder &Item::getBorders()
{
    findBorders();
    return m_borders;
}

void Item::findBorders()
{
    if (m_transform.empty())
    {
        m_transform =
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, -1, 0},
            {0, 0, 0, 1},
        };
    }
    matrix current;
    multiplyMatrix(m_vOriginal, m_transform, current);
    double minX = current[0][0];
    double maxX = current[0][0];
    double minY = current[0][1];
    double maxY = current[0][1];
    double minZ = current[0][2];
    double maxZ = current[0][2];
    if (current.size() > 1)
    {
        for (std::size_t i = 1; i < current.size(); i++)
        {
            const double currentX = current[i][0];
            const double currentY = current[i][1];
            const double currentZ = current[i][2];
            if (currentX > maxX) maxX = currentX;
            else if (currentX < minX) minX = currentX;
            if (currentY > maxY) maxY = currentY;
            else if (currentY < minY) minY = currentY;
            if (currentZ > maxZ) maxZ = currentZ;
            else if (currentZ < minZ) minZ = currentZ;
        }
        m_borders =
        {
            minX, maxX,
            minY, maxY,
            minZ, maxZ,
        };
    }
}

void Item::spinOY(double t_angle)
{
    double radAngle = t_angle * PI / FULL_ANGLE;
    findBorders();
    Point<double> center = getCenter();
    const matrix T =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {-center.x, 0, -center.z, 1}
    };
    const matrix rotate =
    {
        {cos(radAngle), 0, -sin(radAngle), 0},
        {0, 1, 0, 0},
        {sin(radAngle), 0, cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    const matrix antiT =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {center.x, 0, center.z, 1}
    };
    multiplyMatrix(m_transform, T);
    multiplyMatrix(m_transform, rotate);
    multiplyMatrix(m_transform, antiT);
}

matrix Item::topViewMatrix(double t_radAngle)
{
    findBorders();
    Point<double> center = getCenter();
    matrix current =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    const matrix T =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, -center.y, -center.z, 1}
    };
    const matrix rotate =
    {
        {1, 0, 0, 0},
        {0, cos(t_radAngle), -sin(t_radAngle), 0},
        {0, sin(t_radAngle), cos(t_radAngle), 0},
        {0, 0, 0, 1}
    };
    const matrix antiT =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, center.y, center.z, 1}
    };
    multiplyMatrix(current, T);
    multiplyMatrix(current, rotate);
    multiplyMatrix(current, antiT);
    return current;
}

void Item::loadObj(const QString &t_dir, const QString &t_file)
{
    QString path = t_dir + "/" + t_file + ".obj";
    QFile objFile(path);
    QTextStream pipeline(&objFile);
    pipeline.setCodec("Windows-1251");
    if (!objFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not opened";
        return;
    }
    QString currentMaterial;
    while(!pipeline.atEnd())
    {
        const QString line = pipeline.readLine();
        const QStringList stringList = line.split(" ", Qt::SkipEmptyParts);
        if (!stringList.empty())
        {
            const QString parameter = stringList[0];
            // If 0 then similar
            if (!QString::compare(parameter, "mtllib"))
            {
                QString path = t_dir + "/" + stringList[1];
                loadMtl(path);
            }
            else if (!QString::compare(parameter, "v"))
            {
                m_vOriginal.push_back({stringList[1].toDouble(),
                                         stringList[2].toDouble(),
                                         stringList[3].toDouble(), 1.});
            }
            else if (!QString::compare(parameter, "vt"))
            {
                m_textures.push_back({ stringList[1].toDouble(),
                                         stringList[2].toDouble(),
                                         stringList[3].toDouble(), 1.});
            }
            else if (!QString::compare(parameter, "vn"))
            {
                m_nOriginal.push_back( {stringList[1].toDouble(),
                                          stringList[2].toDouble(),
                                          stringList[3].toDouble(), 1.});
            }
            else if(!QString::compare(parameter, "usemtl"))
            {
                currentMaterial = stringList[1];
            }
            else if(!QString::compare(parameter, "f"))
            {
                const QStringList polygonP1 = stringList[1].split("/", Qt::KeepEmptyParts);
                const QStringList polygonP2 = stringList[2].split("/", Qt::KeepEmptyParts);
                const QStringList polygonP3 = stringList[3].split("/", Qt::KeepEmptyParts);
                const Polygon tempPolygon
                {
                    // Material, points, normals, m_textures
                    currentMaterial,
                    polygonP1[0].toULongLong() - 1,
                    polygonP2[0].toULongLong() - 1,
                    polygonP3[0].toULongLong() - 1,
                    polygonP1[2].toULongLong() - 1,
                    polygonP2[2].toULongLong() - 1,
                    polygonP3[2].toULongLong() - 1,
                    polygonP1[1].toULongLong() - 1,
                    polygonP2[1].toULongLong() - 1,
                    polygonP3[1].toULongLong() - 1
                };
                m_polygons.append(tempPolygon);
            }
        }
    }
    m_vOriginal.shrink_to_fit();
    m_nOriginal.shrink_to_fit();
    m_textures.shrink_to_fit();
    objFile.close();
}

void Item::loadMtl(const QString &t_path)
{
    QFile mtlFile(t_path);
    QTextStream pipeline(&mtlFile);
    pipeline.setCodec("Windows-1251");
    if (!mtlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not opened";
        return;
    }
    QString currentMaterial;
    Material temp{0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0};
    bool materialEnd = false;
    bool eof = false;
    while(!eof)
    {
        const QString line = pipeline.readLine();
        const QStringList stringList = line.split(" ", Qt::SkipEmptyParts);
        if (!stringList.empty())
        {
            const QString parameter = stringList[0];
            // If 0 then similar
            if (!QString::compare(parameter, "newmtl"))
            {
                if (materialEnd)
                {
                    // Load to QMap
                    m_materialMap.insert(currentMaterial, temp);
                }
                // illum, ka, kd, ks, ni, ns
                Material temp = {0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0};
                currentMaterial = stringList[1];
                materialEnd = true;
            }
            else if (!QString::compare(parameter, "illum"))
            {
                temp.illum = stringList[1].toInt();
            }
            else if (!QString::compare(parameter, "Ka"))
            {
                temp.ka.setRgbF(stringList[1].toDouble(), stringList[2].toDouble(),
                        stringList[3].toDouble());
            }
            else if (!QString::compare(parameter, "Kd"))
            {
                temp.kd.setRgbF(stringList[1].toDouble(), stringList[2].toDouble(),
                        stringList[3].toDouble());
            }
            else if (!QString::compare(parameter, "Ks"))
            {
                temp.ks.setRgbF(stringList[1].toDouble(), stringList[2].toDouble(),
                        stringList[3].toDouble());
            }
            else if (!QString::compare(parameter, "Ns"))
            {
                temp.ns = stringList[1].toDouble();
            }
            else if (!QString::compare(parameter, "Ni"))
            {
                temp.ni = stringList[1].toDouble();
            }
            eof = pipeline.atEnd();
            if (eof)
            {
                m_materialMap.insert(currentMaterial, temp);
            }
        }
    }
    mtlFile.close();
}

void Item::rasterise(const matrix &t_projection, const int &t_imageWidth,
                     const int &t_imageHeight)
{
    multiplyMatrix(m_vOriginal, m_transform, m_vPerspective);
    m_vPerspective.shrink_to_fit();
    double transX = m_transform[3][0];
    double transY = m_transform[3][1];
    double transZ = m_transform[3][2];
    m_transform[3][0] = 0;
    m_transform[3][1] = 0;
    m_transform[3][2] = 0;
    multiplyMatrix(m_nOriginal, m_transform, m_nPerspective);
    m_nOriginal.shrink_to_fit();
    m_transform[3][0] = transX;
    m_transform[3][1] = transY;
    m_transform[3][2] = transZ;
    multiplyMatrix(m_vPerspective, t_projection);



//    for (size_t i = 0; i < m_vPerspective.size(); i++)
    for (std::vector<double> &row : m_vPerspective)
    {
        if (!qIsNull(row[3]))
        {
            // Normalise if w is different than zero
            double coeff = 1 / row[3];
            row[0] *= coeff;
            row[1] *= coeff;
            row[2] *= coeff;
        }
        row[0]++;
        row[1] *= -1;
        row[1]++;
//        const int ratio = std::min(imageWidth, imageHeight);
        row[0] *= HALF * t_imageWidth;
        row[1] *= HALF * t_imageHeight;
        row[2] *= -1;
    }
}

void Item::render(matrix &t_buffer, QImage &t_image, QMap<QString, Item *> &t_clickSearch,
                  const int &t_width, const int &t_height)
{
    m_ldx = t_width;
    m_ldy = t_height;
    m_rux = 0;
    m_ruy = 0;
    // Camera position, move to camera struct later
    std::vector<double> camera = {0, 0, 0};
    for (Polygon &m_polygon : m_polygons)
    {
        // Polygon points
        const std::vector<double> p1 = m_vPerspective[m_polygon.points[0]];
        const std::vector<double> p2 = m_vPerspective[m_polygon.points[1]];
        const std::vector<double> p3 = m_vPerspective[m_polygon.points[2]];
        /* Polygon normal:
         * Cross product of two vectors on the plane
         * p1->p2 and p1->p3
         */
        std::vector<double> line1 = {(p2[0] - p1[0]), (p2[1] - p1[1]), (p2[2] - p1[2])};
        std::vector<double> line2 = {(p3[0] - p1[0]), (p3[1] - p1[1]), (p3[2] - p1[2])};
        std::vector<double> n =
        {
            {
                line1[1] * line2[2] - line1[2] * line2[1],
                line1[2] * line2[0] - line1[0] * line2[2],
                line1[0] * line2[1] - line1[1] * line2[0]
            }
        };
        // Normalize polygon normal
        double ncoeff = 1 / sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
        n[0] *= ncoeff;
        n[1] *= ncoeff;
        n[2] *= ncoeff;

        if (n[0] * (p1[0] - camera[0]) +
            n[1] * (p1[1] - camera[1]) +
            n[2] * (p1[2] - camera[2]) < 0.)
        {
            // Find bounding box
            const double xmin = std::min(p1[0], std::min(p2[0], p3[0]));
            const double ymin = std::min(p1[1], std::min(p2[1], p3[1]));
            const double xmax = std::max(p1[0], std::max(p2[0], p3[0]));
            const double ymax = std::max(p1[1], std::max(p2[1], p3[1]));
            if (!(xmin > t_width - 1 || xmax < 0 || ymin > t_height - 1 || ymax < 0))
            {
                // Starting points of interpolation
                const int x0 = std::max(0, int(xmin));
                const int x1 = std::min(t_width - 1, int(xmax));
                const int y0 = std::max(0, int(ymin));
                const int y1 = std::min(t_height - 1, int(ymax));

                // Border coords
                if (x0 < m_ldx) m_ldx = x0;
                if (x1 > m_rux) m_rux = x1;
                if (y0 < m_ldy) m_ldy = y0;
                if (y1 > m_ruy) m_ruy = y1;

                double area = edgeCheck(p1, p2, p3);

                const std::vector<double> n1 = m_nPerspective[m_polygon.normals[0]];
                const std::vector<double> n2 = m_nPerspective[m_polygon.normals[1]];
                const std::vector<double> n3 = m_nPerspective[m_polygon.normals[2]];
                for (int y = y0; y <= y1; y++)
                {
                    for (int x = x0; x <= x1; x++)
                    {
                        std::vector<double> sample = {x + HALF, y + HALF, 0.};
                        double w1 = edgeCheck(p2, p3, sample);
                        double w2 = edgeCheck(p3, p1, sample);
                        double w3 = edgeCheck(p1, p2, sample);
                        if (w1 >= 0 && w2 >= 0 && w3 >= 0)
                        {
                            double coeff = 1 / area;
                            w1 *= coeff;
                            w2 *= coeff;
                            w3 *= coeff;
                            double oneOverZ = p1[2] * w1 + p2[2] * w2 + p3[2] * w3;
                            double z = 1 / oneOverZ;
                            if (z < t_buffer[x][y])
                            {
                                t_buffer[x][y] = z;
                                double cosn = w1 * n1[2] + w2 * n2[2] + w3 * n3[2];
                                QColor ambientColor = m_materialMap[m_polygon.materialKey].ka;
                                QColor diffuseColor = m_materialMap[m_polygon.materialKey].kd;
                                qreal ar; qreal ag; qreal ab;
                                qreal dr; qreal dg; qreal db;
                                ambientColor.getRgbF(&ar, &ag, &ab);
                                diffuseColor.getRgbF(&dr, &dg, &db);
                                ambientColor.setRgbF((ar + dr * cosn) / 2, (ag + dg * cosn) / 2, (ab + db * cosn) / 2);
                                t_image.setPixelColor(x, y, ambientColor);
                                const QString key = QString::number(x) + "." + QString::number(y);
                                t_clickSearch[key] = this;
                                /* Debug m_polygons
                                    QColor fillColor;
                                    qreal r = w1 * 0 + w2 * 0 + w3 * 1;
                                    qreal g = w1 * 0 + w2 * 1 + w3 * 0;
                                    qreal b = w1 * 1 + w2 * 0 + w3 * 0;
                                    r *= z;
                                    g *= z;
                                    b *= z;
                                    buffer[x][y] = z;
                                    fillColor.setRgbF(r, g, b);
                                    image->setPixelColor(x, y, fillColor);
                                */
                            }
                        }
                    }
                }
            }
        }
    }
    if (m_isClicked) outline(t_image);
}

bool Item::changeIsClicked()
{
    m_isClicked = !m_isClicked;
    return m_isClicked;
}

void Item::outline(QImage &t_image)
{
    // Connect bounding box
    QColor line_color(0, 0, 0);
    int x0 = m_ldx;
    int y0 = m_ldy;
    int r; int g; int b;
    std::vector<std::vector<int>> direction = {{m_ldx, m_ruy}, {m_rux, m_ruy}, {m_rux, m_ldy}, {m_ldx, m_ldy}};
    for (int i = 0; i < 4; i++)
    {
        int x1 = direction[i][0];
        int y1 = direction[i][1];
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int signX = x0 < x1 ? 1 : -1;
        int signY = y0 < y1 ? 1 : -1;
        int error = dx - dy;
        int double_error = 0;
        t_image.setPixel(x1, y1, line_color.rgba());
        while(x0 != x1 || y0 != y1)
        {
            QColor contrastColor(t_image.pixel(x0, y0));
            contrastColor.getRgb(&r, &g, &b);
            contrastColor.setRgb(abs(RGB_MAX - r), abs(RGB_MAX - g), abs(RGB_MAX - b));
            t_image.setPixel(x0, y0, contrastColor.rgba());
            double_error = error << 1;
            if (double_error > -dy)
            {
                error -= dy;
                x0 += signX;
            }
            if (double_error < dx)
            {
                error += dx;
                y0 += signY;
            }
        }
        x0 = x1;
        y0 = y1;
    }
}
