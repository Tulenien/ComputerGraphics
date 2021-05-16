﻿#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include "item.h"

/*TODO:
 * Add Perspective transformation
 * Add Camera
 * Add ZBuffer Structure
 * Add ZBuffer Algorithm
 * Add 3D Clipping method
*/

class Scene : public QLabel
{
    Q_OBJECT
public:
    Scene(QWidget *parent);
    void addItem(QString dir, QString item);
    void setSize(double &width, double &length, double &height);
    double getWidth();
    double getLength();
    double getHeight();
    // Add rotation
    void rotateSceneOX(double angle);
    void rotateSceneOY(double angle);
    // Incapsulates all transformations and Zbuffer
    void render();
    Item &getItemByIndex(int index);

private:
    QImage *image;
    QVector<Item> items;
    double width, length, height;
};

#endif // SCENE_H
