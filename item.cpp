#include "item.h"

Item::Item(QString dir, QString file)
{
    // Load mtl file inside
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
    if(transform.empty())
    {
        for (std::size_t i = 0; i < 4; i++)
        {
            transform.push_back(rotate[i]);
        }
    }
    else multiplyMatrix(transform, rotate);
    // Multiply [vOriginal][transform] and show the result on screen
}

void Item::rotateOY(double angle)
{
    double radAngle = angle * PI / 180.;
    const matrix rotate =
    {
        {cos(radAngle), 0, -sin(radAngle), 0},
        {0, 1, 0, 0},
        {sin(radAngle), 0, cos(radAngle), 0},
        {0, 0, 0, 1}
    };
    if(transform.empty())
    {
        for (std::size_t i = 0; i < 4; i++)
        {
                transform.push_back(rotate[i]);
        }
    }
    else multiplyMatrix(transform, rotate);
    // Multiply [vOriginal][transform] and show the result on screen
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
    if(transform.empty())
    {
        for (std::size_t i = 0; i < 4; i++)
        {
                transform.push_back(move[i]);
        }
    }
    else multiplyMatrix(transform, move);
    // Multiply [vOriginal][transform] and show the result on screen
}

void Item::setToFloor(double height)
{
    toFloor = height;
}

point_t Item::centerXZ()
{
    // Find min and max coordinates of x and z
    double minX = vOriginal[0][0], maxX = vOriginal[0][0];
    double minZ = vOriginal[0][2], maxZ = vOriginal[0][2];
    // If only one point --> crash
    for (std::size_t i = 1; i < vOriginal.size(); i++)
    {
        double currentX = vOriginal[i][0];
        double currentZ = vOriginal[i][2];
        if (currentX > maxX) maxX = currentX;
        else if (currentX < minX) minX = currentX;
        if (currentZ > maxZ) maxZ = currentZ;
        else if (currentZ < minZ) minZ = currentZ;
    }
    point_t center =
    {
        (minX + maxX) * 0.5,
        0,
        (minZ + maxZ) * 0.5
    };
    return center;
}

void Item::spin(double angle)
{
    // Rotate item around its local OY axis
    double radAngle = angle * PI / 180.;
    point_t center = centerXZ();
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
    if(transform.empty())
    {
        for (std::size_t i = 0; i < 4; i++)
        {
            transform.push_back(T[i]);
        }
    }
    else multiplyMatrix(transform, T);
    multiplyMatrix(transform, rotate);
    multiplyMatrix(transform, antiT);
    // Multiply [vOriginal][transform] and show the result on screen
}

void Item::loadObj(const QString dir, const QString file)
{
    QString path = dir + "/" + file + ".obj";
    QFile objFile(path);
    QTextStream pipeline(&objFile);
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

void Item::loadMtl(const QString path)
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
                temp.ka =
                {
                    stringList[1].toInt(),
                    stringList[2].toInt(),
                    stringList[3].toInt(),
                };
            }
            else if (!QString::compare(stringList[0], "Kd"))
            {
                temp.kd =
                {
                    stringList[1].toInt(),
                    stringList[2].toInt(),
                    stringList[3].toInt(),
                };
            }
            else if (!QString::compare(stringList[0], "Ks"))
            {
                temp.ks =
                {
                    stringList[1].toInt(),
                    stringList[2].toInt(),
                    stringList[3].toInt(),
                };
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

void Item::rasterise(
        matrix &camera, const matrix &projection,
        const double &left, const double &right,
        const double &top, const double &bottom,
        const double &near, const double &imageWidth,
        const double &imageHeight)
{
    /* Projects points and normals,
       result stored in v(n)Perspective matrix
    */
    if (transform.size())
    {
        multiplyMatrix(transform, camera);
        multiplyMatrix(transform, projection);
        multiplyMatrix(vOriginal, transform, vPerspective);
        multiplyMatrix(nOriginal, transform, nPerspective);
    }
    else
    {
        // World space to camera space and to perspective projection
        multiplyMatrix(camera, projection);
        multiplyMatrix(vOriginal, camera, vPerspective);
        multiplyMatrix(nOriginal, camera, nPerspective);
    }
    // Convert to raster
    for (size_t i = 0; i < vPerspective.size(); i++)
    {
        if (!(abs(vPerspective[i][3] - 1.) < .1e-15))
        {
            // Normalise if w is different than 1 and not zero
            double coeff = 1 / vPerspective[i][3];
            if (qIsInf(coeff))
            {
                vPerspective[i][3] = 1;
            }
            else
            {
                vPerspective[i][0] *= coeff;
                vPerspective[i][1] *= coeff;
                vPerspective[i][2] *= coeff;
            }
        }
        // Convert to NDC
        vPerspective[i][0] = 2 * (vPerspective[i][0] - right - left) / (right - left);
        vPerspective[i][1] = 2 * (vPerspective[i][1] - top - bottom) / (top - bottom);
        vPerspective[i][0]++;
        vPerspective[i][1] *= -1;
        vPerspective[i][1]++;
        vPerspective[i][0] *= 0.5 * imageWidth;
        vPerspective[i][1] *= 0.5 * imageHeight;
        vPerspective[i][2] *= -1;
    }
}

void Item::render(matrix &buffer, QImage *&image, double width, double height)
{
    for (int i = 0; i < polygons.size(); i++)
    {
        // Triangle points
        std::vector<double> p1 = vPerspective[polygons[i].points[0]];
        std::vector<double> p2 = vPerspective[polygons[i].points[1]];
        std::vector<double> p3 = vPerspective[polygons[i].points[2]];
        // Find rectangle boundaries
        double xmin = std::min(p1[0], std::min(p2[0], p3[0]));
        double ymin = std::min(p1[1], std::min(p2[1], p3[1]));
        double xmax = std::max(p1[0], std::max(p2[0], p3[0]));
        double ymax = std::max(p1[1], std::max(p2[1], p3[1]));
        // The triangle is out of screen
        if (!(xmin > width - 1 || xmax < 0 || ymin > height - 1 || ymax < 0))
        {
            // Starting points
            int x0 = std::max(0, (int)floor(xmin));
            int x1 = std::min((int)width - 1, (int)floor(xmax));
            int y0 = std::max(0, (int)floor(ymin));
            int y1 = std::min((int)height - 1, (int)floor(ymax));

            double area = edgeCheck(p1, p2, p3);

            for (int y = y0; y <= y1; ++y)
            {
                for (int x = x0; x <= x1; x++)
                {
                    std::vector<double> sample = {x + 0.5, y + 0.5, 0};
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
                        if (z < buffer[y][x])
                        {
                            buffer[y][x] = z;
                            //image->setPixelColor(y, x, materialMap[polygons[i].materialKey].ka);
                            image->setPixelColor(y, x, QColor(i * 20, 0, 0));
                        }
                    }
                }
            }
        }
    }
}

double Item::edgeCheck(const std::vector<double> &a, const std::vector<double> &b, const std::vector<double> &c)
{
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}
