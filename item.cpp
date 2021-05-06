#include "item.h"

Item::Item(QString dir, QString file)
{
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
    vOriginal.setCols(3);
    nOriginal.setCols(3);
    textures.setCols(3);
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
                temp[0] = stringList[1].toDouble();
                temp[1] = stringList[2].toDouble();
                temp[2] = stringList[3].toDouble();
                vOriginal.AddRows(temp, 1);
            }
            else if (!QString::compare(stringList[0], "vt"))
            {
                temp[0] = stringList[1].toDouble();
                temp[1] = stringList[2].toDouble();
                temp[2] = stringList[3].toDouble();
                textures.AddRows(temp, 1);
            }
            else if (!QString::compare(stringList[0], "vn"))
            {
                temp[0] = stringList[1].toDouble();
                temp[1] = stringList[2].toDouble();
                temp[2] = stringList[3].toDouble();
                nOriginal.AddRows(temp, 1);
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
                polygon tempPolygon;
                tempPolygon.materialKey = currentMaterial;
                tempPolygon.points[0] = polygonP1[0].toInt() - 1;
                tempPolygon.points[1] = polygonP2[0].toInt() - 1;
                tempPolygon.points[2] = polygonP3[0].toInt() - 1;
                tempPolygon.textures[0] = polygonP1[1].toInt() - 1;
                tempPolygon.textures[1] = polygonP2[1].toInt() - 1;
                tempPolygon.textures[2] = polygonP3[1].toInt() - 1;
                tempPolygon.normals[0] = polygonP1[2].toInt() - 1;
                tempPolygon.normals[1] = polygonP2[2].toInt() - 1;
                tempPolygon.normals[2] = polygonP3[2].toInt() - 1;
                polygons.append(tempPolygon);
            }
        }
    }
    vOriginal.setRowsFromVector();
    nOriginal.setRowsFromVector();
    textures.setRowsFromVector();
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
    bool eof;
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
                temp.ka.red = stringList[1].toDouble();
                temp.ka.green = stringList[2].toDouble();
                temp.ka.blue = stringList[3].toDouble();
            }
            else if (!QString::compare(stringList[0], "Kd"))
            {
                temp.kd.red = stringList[1].toDouble();
                temp.kd.green = stringList[2].toDouble();
                temp.kd.blue = stringList[3].toDouble();
            }
            else if (!QString::compare(stringList[0], "Ks"))
            {
                temp.ks.red = stringList[1].toDouble();
                temp.ks.green = stringList[2].toDouble();
                temp.ks.blue = stringList[3].toDouble();
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
