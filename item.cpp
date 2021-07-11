﻿#include "item.h"

Item::Item(const QString &dir, const QString &file)
{
    // Load mtl file inside
    isClicked = false;
    loadObj(dir, file);
}

bool Item::multiplyMatrix(matrix &A, const matrix &B)
{
    // Rewrites result in matrix A
    bool status = true;
    // Standart multiplication algorithm
    std::size_t rows = A.size();
    std::size_t cols = B[0].size();
    std::size_t nest = B.size();
    if (A[0].size() != nest) status = false;
    else
    {
        matrix temp;
        for (std::size_t i = 0; i < rows; i++)
        {
            std::vector<double> row(cols);
            for (std::size_t j = 0; j < cols; j++)
            {
                for (std::size_t k = 0; k < nest; k++)
                {
                    row[j] += A[i][k] * B[k][j];
                }
            }
            temp.push_back(row);
        }
        A.clear();
        for (std::size_t i = 0; i < rows; i++)
        {
            A.push_back(temp[i]);
        }
    }
    return status;
}

bool Item::multiplyMatrix(const matrix &A, const matrix &B, matrix &C)
{
    // Writes result in matrix C
    bool status = true;
    // Standart multiplication algorithm
    std::size_t rows = A.size();
    std::size_t cols = B[0].size();
    std::size_t nest = B.size();
    if (C.size())
    {
        for (size_t i = 0; i < C.size(); i++)
        {
            C[i].clear();
        }
        C.clear();
    }
    if (A[0].size() != nest) status = false;
    else
    {
        for (std::size_t i = 0; i < rows; i++)
        {
            std::vector<double> row(cols);
            for (std::size_t j = 0; j < cols; j++)
            {
                for (std::size_t k = 0; k < nest; k++)
                {
                    row[j] += A[i][k] * B[k][j];
                }
            }
            C.push_back(row);
        }
    }
    return status;
}

void Item::rotateOX(double angleOX)
{
    double radAngle = angleOX * PI / 180.;
    const matrix rotate =
    {
        {1, 0, 0, 0},
        {0, cos(radAngle), sin(radAngle), 0},
        {0, -sin(radAngle), cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(transform, rotate);
}

void Item::rotateOY(double angle)
{
    double radAngle = angle * PI / 180.;
    const matrix rotate =
    {
        {cos(radAngle), 0, sin(radAngle), 0},
        {0, 1, 0, 0},
        {-sin(radAngle), 0, cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(transform, rotate);
}

void Item::rotateOZ(double angle)
{
    double radAngle = angle * PI / 180.;
    const matrix rotate =
    {
        {cos(radAngle), sin(radAngle), 0, 0},
        {-sin(radAngle), cos(radAngle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    multiplyMatrix(transform, rotate);
}

void Item::move(double x, double y, double z)
{
    matrix move =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {x, y, z, 1}
    };
    multiplyMatrix(transform, move);
}

point_t Item::getCenter()
{
    point_t center =
    {
        (borders.minX + borders.maxX) * 0.5,
        (borders.minY + borders.maxY) * 0.5,
        (borders.minZ + borders.maxZ) * 0.5,
    };
    return center;
}

volumeBorder &Item::getBorders()
{
    findBorders();
    return borders;
}

void Item::findBorders()
{
    if (!transform.size())
    {
        transform =
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, -1, 0},
            {0, 0, 0, 1},
        };
    }
    matrix current;
    multiplyMatrix(vOriginal, transform, current);
    double minX = current[0][0], maxX = current[0][0];
    double minY = current[0][1], maxY = current[0][1];
    double minZ = current[0][2], maxZ = current[0][2];
    if (current.size() > 1)
    {
        for (std::size_t i = 1; i < current.size(); i++)
        {
            double currentX = current[i][0];
            double currentY = current[i][1];
            double currentZ = current[i][2];
            if (currentX > maxX) maxX = currentX;
            else if (currentX < minX) minX = currentX;
            if (currentY > maxY) maxY = currentY;
            else if (currentY < minY) minY = currentY;
            if (currentZ > maxZ) maxZ = currentZ;
            else if (currentZ < minZ) minZ = currentZ;
        }
        borders =
        {
            minX, maxX,
            minY, maxY,
            minZ, maxZ,
        };
    }
}

void Item::spinOY(double angle)
{
    double radAngle = angle * PI / 180.;
    findBorders();
    point_t center = getCenter();
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
    multiplyMatrix(transform, T);
    multiplyMatrix(transform, rotate);
    multiplyMatrix(transform, antiT);
}

const matrix Item::topViewMatrix(double radAngle)
{
    findBorders();
    point_t center = getCenter();
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
        {0, -center.y, -center.z, 1} // Possibly other sign on z...
    };
    const matrix rotate =
    {
        {1, 0, 0, 0},
        {0, cos(radAngle), -sin(radAngle), 0},
        {0, sin(radAngle), cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    const matrix antiT =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, center.y, center.z, 1} // Possibly other sign on z...
    };
    multiplyMatrix(current, T);
    multiplyMatrix(current, rotate);
    multiplyMatrix(current, antiT);
    return current;
}

void Item::loadObj(const QString &dir, const QString &file)
{
    QString path = dir + "/" + file + ".obj";
    QFile objFile(path);
    QTextStream pipeline(&objFile);
    pipeline.setCodec("Windows-1251");
    if (!objFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not opened";
        return;
    }
    QString line;
    QStringList stringList;
    std::vector<double> temp(3);
    QString currentMaterial;
    while(!pipeline.atEnd())
    {
        line = pipeline.readLine();
        stringList = line.split(" ", Qt::SkipEmptyParts);
        if (stringList.size())
        {
            // If 0 then similar
            if (!QString::compare(stringList[0], "mtllib"))
            {
                QString path = dir + "/" + stringList[1];
                loadMtl(path);
            }
            else if (!QString::compare(stringList[0], "v"))
            {
                temp =
                {
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
                    1
                };
                vOriginal.push_back(temp);
            }
            else if (!QString::compare(stringList[0], "vt"))
            {
                temp =
                {
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
                    1
                };
                textures.push_back(temp);
            }
            else if (!QString::compare(stringList[0], "vn"))
            {
                temp =
                {
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
                    1
                };
                nOriginal.push_back(temp);
            }
            else if(!QString::compare(stringList[0], "usemtl"))
            {
                currentMaterial = stringList[1];
            }
            else if(!QString::compare(stringList[0], "f"))
            {
                QStringList polygonP1 = stringList[1].split("/", Qt::KeepEmptyParts);
                QStringList polygonP2 = stringList[2].split("/", Qt::KeepEmptyParts);
                QStringList polygonP3 = stringList[3].split("/", Qt::KeepEmptyParts);
                polygon tempPolygon
                {
                    // Material, points, normals, textures
                    currentMaterial,
                    {
                        polygonP1[0].toULongLong() - 1,
                        polygonP2[0].toULongLong() - 1,
                        polygonP3[0].toULongLong() - 1,
                    },
                    {
                        polygonP1[2].toULongLong() - 1,
                        polygonP2[2].toULongLong() - 1,
                        polygonP3[2].toULongLong() - 1,
                    },
                    {
                        polygonP1[1].toULongLong() - 1,
                        polygonP2[1].toULongLong() - 1,
                        polygonP3[1].toULongLong() - 1,
                    }
                };
                polygons.append(tempPolygon);
            }
        }
    }
    objFile.close();
}

void Item::loadMtl(const QString &path)
{
    QFile mtlFile(path);
    QTextStream pipeline(&mtlFile);
    pipeline.setCodec("Windows-1251");
    if (!mtlFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "File not opened";
        return;
    }
    QString line;
    QStringList stringList;
    QString currentMaterial;
    material temp;
    bool materialEnd = false;
    // End of file status
    bool eof = false;
    while(!eof)
    {
        line = pipeline.readLine();
        stringList = line.split(" ", Qt::SkipEmptyParts);
        if (stringList.size())
        {
            // If 0 then similar
            if (!QString::compare(stringList[0], "newmtl"))
            {
                if (materialEnd)
                {
                    // Load to QMap
                    materialMap.insert(currentMaterial, temp);
                }
                // illum, ka, kd, ks, ni, ns
                temp = {0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0, 0};
                currentMaterial = stringList[1];
                materialEnd = true;
            }
            else if (!QString::compare(stringList[0], "illum"))
            {
                temp.illum = stringList[1].toInt();
            }
            else if (!QString::compare(stringList[0], "Ka"))
            {
                temp.ka.setRedF(stringList[1].toDouble());
                temp.ka.setGreenF(stringList[2].toDouble());
                temp.ka.setBlueF(stringList[3].toDouble());
            }
            else if (!QString::compare(stringList[0], "Kd"))
            {
                temp.kd.setRedF(stringList[1].toDouble());
                temp.kd.setGreenF(stringList[2].toDouble());
                temp.kd.setBlueF(stringList[3].toDouble());
            }
            else if (!QString::compare(stringList[0], "Ks"))
            {
                temp.ks.setRedF(stringList[1].toDouble());
                temp.ks.setGreenF(stringList[2].toDouble());
                temp.ks.setBlueF(stringList[3].toDouble());
            }
            else if (!QString::compare(stringList[0], "Ns"))
            {
                temp.ns = stringList[1].toDouble();
            }
            else if (!QString::compare(stringList[0], "Ni"))
            {
                temp.ni = stringList[1].toDouble();
            }
            eof = pipeline.atEnd();
            if (eof)
            {
                materialMap.insert(currentMaterial, temp);
            }
        }
    }
    mtlFile.close();
}

void Item::rasterise(const matrix &projection, const int &imageWidth, const int &imageHeight, double radAngle)
{
/*
    double distance = floorLevel - borders.minY;
    if (abs(distance) < 1e-5) distance = 0;
    else
    {
        transform[3][1] += distance;
        borders.maxY += distance;
        borders.minY += distance;
    }
    transform =
    {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, -1, 0},
        {0, 0, distance, 1}
    };

    const matrix rotateDown = topViewMatrix(radAngle);
    matrix temp = transform;
    multiplyMatrix(temp, rotateDown);
    multiplyMatrix(vOriginal, temp, vPerspective);
    temp[3][0] = 0;
    temp[3][1] = 0;
    temp[3][2] = 0;
    multiplyMatrix(nOriginal, temp, nPerspective);


    multiplyMatrix(vOriginal, transform, vPerspective);
    double transX = transform[3][0];
    double transY = transform[3][1];
    double transZ = transform[3][2];
    transform[3][0] = 0;
    transform[3][1] = 0;
    transform[3][2] = 0;
    multiplyMatrix(nOriginal, transform, nPerspective);
    transform[3][0] = transX;
    transform[3][1] = transY;
    transform[3][2] = transZ;
    multiplyMatrix(vPerspective, projection);
*/

    multiplyMatrix(vOriginal, transform, vPerspective);
    double transX = transform[3][0];
    double transY = transform[3][1];
    double transZ = transform[3][2];
    transform[3][0] = 0;
    transform[3][1] = 0;
    transform[3][2] = 0;
    multiplyMatrix(nOriginal, transform, nPerspective);
    transform[3][0] = transX;
    transform[3][1] = transY;
    transform[3][2] = transZ;
    multiplyMatrix(vPerspective, projection);

    // Convert to raster DO NOT TOUCH!
    for (size_t i = 0; i < vPerspective.size(); i++)
    {
        if (!qIsNull(vPerspective[i][3]))
        {
            // Normalise if w is different than zero
            double coeff = 1 / vPerspective[i][3];
            vPerspective[i][0] *= coeff;
            vPerspective[i][1] *= coeff;
            vPerspective[i][2] *= coeff;
        }
        vPerspective[i][0]++;
        vPerspective[i][1] *= -1;
        vPerspective[i][1]++;
//        const int ratio = std::min(imageWidth, imageHeight);
        vPerspective[i][0] *= 0.5 * imageWidth;
        vPerspective[i][1] *= 0.5 * imageHeight;
        vPerspective[i][2] *= -1;
    }
}

void Item::render(matrix &buffer, QImage &image, QMap<QString, Item *> &clickSearch, const int &width, const int &height)
{
    ldx = width;
    ldy = height;
    rux = 0;
    ruy = 0;
    // Camera position, move to camera struct later
    std::vector<double> camera = {0, 0, 0};
    for (int i = 0; i < polygons.size(); i++)
    {
        // Polygon points
        const std::vector<double> p1 = vPerspective[polygons[i].points[0]];
        const std::vector<double> p2 = vPerspective[polygons[i].points[1]];
        const std::vector<double> p3 = vPerspective[polygons[i].points[2]];
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
            if (!(xmin > width - 1 || xmax < 0 || ymin > height - 1 || ymax < 0))
            {
                // Starting points of interpolation
                const int x0 = std::max(0, int(xmin));
                const int x1 = std::min(width - 1, int(xmax));
                const int y0 = std::max(0, int(ymin));
                const int y1 = std::min(height - 1, int(ymax));

                // Border coords
                if (x0 < ldx) ldx = x0;
                if (x1 > rux) rux = x1;
                if (y0 < ldy) ldy = y0;
                if (y1 > ruy) ruy = y1;

                double area = edgeCheck(p1, p2, p3);

                const std::vector<double> n1 = nPerspective[polygons[i].normals[0]];
                const std::vector<double> n2 = nPerspective[polygons[i].normals[1]];
                const std::vector<double> n3 = nPerspective[polygons[i].normals[2]];
                for (int y = y0; y <= y1; y++)
                {
                    for (int x = x0; x <= x1; x++)
                    {
                        std::vector<double> sample = {x + 0.5, y + 0.5, 0.};
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
                            if (z < buffer[x][y])
                            {
                                buffer[x][y] = z;
                                double cosn = w1 * n1[2] + w2 * n2[2] + w3 * n3[2];
                                QColor ambientColor = materialMap[polygons[i].materialKey].ka;
                                QColor diffuseColor = materialMap[polygons[i].materialKey].kd;
                                qreal ar, ag, ab;
                                qreal dr, dg, db;
                                ambientColor.getRgbF(&ar, &ag, &ab);
                                diffuseColor.getRgbF(&dr, &dg, &db);
                                ambientColor.setRgbF((ar + dr * cosn) / 2, (ag + dg * cosn) / 2, (ab + db * cosn) / 2);
                                image.setPixelColor(x, y, ambientColor);
                                const QString key = QString::number(x) + "." + QString::number(y);
                                clickSearch[key] = this;
                                /* Debug polygons
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
    if (isClicked) outline(image);
}

double Item::edgeCheck(const std::vector<double> &a, const std::vector<double> &b, const std::vector<double> &c)
{
//    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
    return (a[0] - c[0]) * (b[1] - c[1]) - (a[1] - c[1]) * (b[0] - c[0]);
}

bool Item::changeIsClicked()
{
    isClicked = !isClicked;
    return isClicked;
}

void Item::outline(QImage &image)
{
    // Connect bounding box
    QColor line_color(0, 0, 0);
    int x0 = ldx;
    int y0 = ldy;
    int r, g, b;
    int direction[4][2] = {{ldx, ruy}, {rux, ruy}, {rux, ldy}, {ldx, ldy}};
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
        image.setPixel(x1, y1, line_color.rgba());
        while(x0 != x1 || y0 != y1)
        {
            QColor contrastColor(image.pixel(x0, y0));
            contrastColor.getRgb(&r, &g, &b);
            contrastColor.setRgb(127 - r, 127 - g, 127 - b);
            image.setPixel(x0, y0, contrastColor.rgba());
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
