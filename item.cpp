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

void Item::setDepthBuffer(matrix *depthBuffer)
{
    this->depthBuffer = depthBuffer;
}

void Item::setImage(QImage *&image)
{
    this->image = image;
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

void Item::project(const matrix &projection)
{
    /* Projects points and normals,
       result stored in v(n)Perspective matrix
    */
    matrix vCurrent, nCurrent;
    multiplyMatrix(vOriginal, transform, vCurrent);
    multiplyMatrix(nOriginal, transform, nCurrent);
    multiplyMatrix(vCurrent, projection, vPerspective);
    multiplyMatrix(nCurrent, projection, nPerspective);
}

void Item::normalise(const int imageHeight, const int imageWidth)
{
    // Normalise and convert to raster (add conversion to int later)
    for (std::size_t i = 0; i < vPerspective.size(); i++)
    {
        vPerspective[i][0] = (vPerspective[i][0] + 1) * 0.5 * imageWidth;
        vPerspective[i][1] = (1 - vPerspective[i][1]) * 0.5 * imageHeight;
    }
    for (std::size_t i = 0; i < nPerspective.size(); i++)
    {
        nPerspective[i][0] = (nPerspective[i][0] + 1) * 0.5 * imageWidth;
        nPerspective[i][1] = (1 - nPerspective[i][1]) * 0.5 * imageHeight;
    }
}

void Item::rasterise()
{
    // Go through all polygons and scan them with z-buffer
    for (int i = 0; i < polygons.size(); i++)
    {
        // Find points positions, max and min y
        int minY, maxY;
        intPoint_t A, B, C, temp;
        // Unable to use floor in initializer list
        A.x = floor(vPerspective[polygons[i].points[0]][0]);
        A.y = floor(vPerspective[polygons[i].points[0]][1]);
        A.z = floor(vPerspective[polygons[i].points[0]][2]);
        B.x = floor(vPerspective[polygons[i].points[1]][0]);
        B.y = floor(vPerspective[polygons[i].points[1]][1]);
        B.z = floor(vPerspective[polygons[i].points[1]][2]);
        C.x = floor(vPerspective[polygons[i].points[2]][0]);
        C.y = floor(vPerspective[polygons[i].points[2]][1]);
        C.z = floor(vPerspective[polygons[i].points[2]][2]);
        // Finding maxY and minY point
        temp = { A.x, A.y, A.z };
        // maxY point is stored in A
        if (A.y > B.y)
        {
            if (A.y > C.y)
            {
                maxY = A.y;
                if (B.y > C.y)
                {
                    minY = C.y;
                }
                else
                {
                    minY = B.y;
                }
            }
            else
            {
                maxY = C.y;
                minY = B.y;
                A = { C.x, C.y, C.z };
                C = { temp.x, temp.y, temp.z };
            }
        }
        else
        {
            if (B.y > C.y)
            {
                if (A.y > C.y)
                {
                    minY = C.y;
                }
                else
                {
                    minY = A.y;
                }
                maxY = B.y;
                A = { B.x, B.y, B.z };
                B = { temp.x, temp.y, temp.z };
            }
            else
            {
                maxY = C.y;
                minY = B.y;
                A = { C.x, C.y, C.z };
                C = { temp.x, temp.y, temp.z };
            }
        }
        /* Left vector is C, right vector is B
         * If scalar multiplication is positive or zero
         * nothing changes
         * Else change B and C places
         */
        intPoint_t CA, CB;
        CA = { A.x - C.x, A.y - C.y, A.z - C.z };
        CB = { B.x - C.x, B.y - B.y, B.z - C.z };
        if (CA.x * CB.x + CA.y * CB.y + CA.z * CB.z < 0)
        {
            temp = { C.x, C.y, C.z };
            B = { C.x, C.y, C.z };
            C = { temp.x, temp.y, temp.z };
        }
        // Interpolation starts here
        for (int y = maxY; y >= minY; y--)
        {
            int diff = y - A.y;
            float aCoeff = 1 / (C.y - A.y) * diff;
            float bCoeff = 1 / (C.y - A.y) * diff;
            int xa = A.x + (C.x - A.x) * aCoeff;
            int xb = A.x + (B.x - A.x) * bCoeff;
            int za = A.z + (C.z - A.z) * aCoeff;
            int zb = A.z + (B.z - A.z) * bCoeff;
            // Moving from left side to right side and computing z
            for (int x = xa; x < xb; x++)
            {
                double z = za + (zb - za) * (x - xa) * 1 / (xb - xa);
                if ((*depthBuffer)[y][x] > z)
                {
                    (*depthBuffer)[y][x] = z;
                    image->setPixelColor(y, x, materialMap[polygons[i].materialKey].ka);
                }
            }
        }
    }
}
