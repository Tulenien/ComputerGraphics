#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include <QDebug>
// Add ZBuffer and
// find the way to change it whenever object rotate


class Scene : public QLabel
{
    Q_OBJECT
public:
    QImage *image;
    Scene(QWidget *parent);
    void setWidth(qreal width);
    void setLength(qreal length);
    void setHeight(qreal height);
    qreal getWidth();
    qreal getLength();
    qreal getHeight();
    void rotate(qreal angleX, qreal angleY);
    void render();

private:
    qreal width, length, height;
    QImage *buffer;
    void transform(qreal distance);
    void draw();
};

#endif // SCENE_H
