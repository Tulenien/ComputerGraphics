#include "item.h"

Item::Item(QString dir, QString file)
{
    // Load mtl file inside
    loadObj(dir, file);
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
                        polygonP1[0].toInt() - 1,
                        polygonP2[0].toInt() - 1,
                        polygonP3[0].toInt() - 1,
                    },
                    {
                        polygonP1[2].toInt() - 1,
                        polygonP2[2].toInt() - 1,
                        polygonP3[2].toInt() - 1,
                    },
                    {
                        polygonP1[1].toInt() - 1,
                        polygonP2[1].toInt() - 1,
                        polygonP3[1].toInt() - 1,
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
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
                };
            }
            else if (!QString::compare(stringList[0], "Kd"))
            {
                temp.kd =
                {
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
                };
            }
            else if (!QString::compare(stringList[0], "Ks"))
            {
                temp.ks =
                {
                    stringList[1].toDouble(),
                    stringList[2].toDouble(),
                    stringList[3].toDouble(),
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

bool Item::multiplyMatrix(matrix &A, const matrix &B)
{
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

void Item::multiplyMatrix(const matrix &A, const matrix &B, matrix &C)
{
    // No size-check, result saved in C
    // Standart multiplication algorithm
    std::size_t rows = A.size();
    std::size_t cols = B[0].size();
    std::size_t nest = B.size();
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

/*void Item::RotateOX(double angleOX)
//{
//    // Angle must be inversed:
//    // cosine of negative number is the same
//    // as cosine of positive one
//    // negative sine of negative number
//    // is the same as sine of this positive number
//    // Angle in radians
//    double radAngle = angleOX * PI / 180.;
//    const matrix rotate =
//    {
//        {1, 0, 0, 0},
//        {0, cos(radAngle), sin(radAngle), 0},
//        {0, sin(-radAngle), cos(radAngle), 0},
//        {0, 0, 0, 1}
//    };
//    if(transform.empty())
//    {
//        for (std::size_t i = 0; i < 4; i++)
//        {
//            transform.push_back(rotate[i]);
//        }
//    }
//    else multiplyMatrix(transform, rotate);
//    // points and normals changed from original state to last change
//    multiplyMatrix(vOriginal, transform, vShifted);
//    multiplyMatrix(nOriginal, transform, nShifted);
//}*/

void Item::RotateOY(double angle)
{
    // Angle in radians
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
    // points and normals changed from original state to last change
    multiplyMatrix(vOriginal, transform, vShifted);
    multiplyMatrix(nOriginal, transform, nShifted);
}

void Item::Move(double x, double y, double z)
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
    // points and normals changed from original state to last change
    multiplyMatrix(vOriginal, transform, vShifted);
    multiplyMatrix(nOriginal, transform, nShifted);
}

void Item::setToFloor(double height)
{
    toFloor = height;
}

point_t Item::CenterXZ()
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
    // -x, 0, -z --> OY axis coordinates
    point_t center =
    {
        (minX + maxX) * 0.5,
        0,
        (minZ + maxZ) * 0.5
    };
    return center;
}

void Item::Spin(double angle)
{
    double radAngle = angle * PI / 180.;
    point_t center = CenterXZ();
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
    multiplyMatrix(vOriginal, transform, vShifted);
    multiplyMatrix(nOriginal, transform, nShifted);
}
