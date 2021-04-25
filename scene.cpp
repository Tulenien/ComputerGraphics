#include "scene.h"

Scene::Scene(QWidget *parent)
{
    QSize size = this->size();
    qDebug() << size;
    image = new QImage(size, QImage::Format_RGBA8888);
    QColor line_color(0, 0, 0);
    for(int i = 0; i < size.width(); i++)
    {
        for(int j = 0; j < size.height(); j++)
        image->setPixel(i, j, line_color.rgba());
    }
    this->setPixmap(QPixmap::fromImage(*image));
}
