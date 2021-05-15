#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include <QDebug>
#include "item.h"
// Add ZBuffer and
// find the way to change it whenever object rotate


class Scene : public QLabel
{
    Q_OBJECT
public:
    QImage *image;
    Scene(QWidget *parent);
    void addItem(QString dir, QString item);
    void setSize(double &width, double &length, double &height);
    double getWidth();
    double getLength();
    double getHeight();
    // Add Rotation on OX and OY separately
    //void rotateOX(qreal angleX, qreal angleY);
    void render();

private:
    double width, length, height;
    QImage *buffer;
    QVector<Item> items;
    void transform(qreal distance);
    void draw();
};

#endif // SCENE_H
