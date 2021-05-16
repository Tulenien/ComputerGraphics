#include "scene.h"

Scene::Scene(QWidget *parent)
{
    width = 200;
    length = 200;
    height = 250;
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

void Scene::addItem(QString dir, QString item)
{
    items.push_back(Item(dir, item));
    items.last().setToFloor(-height / 2);
}

double Scene::getWidth()
{
    return width;
}

double Scene::getLength()
{
    return length;
}

double Scene::getHeight()
{
    return height;
}

void Scene::setSize(double &width, double &length, double &height)
{
    this->width = width;
    this->length = length;
    this->height = height;
    double offset = -height / 2;
    for (int i = 0; i < items.size(); i++)
    {
        items[i].setToFloor(offset);
    }
}

Item &Scene::getItemByIndex(int index)
{
    return items[index];
}

void Scene::rotateSceneOX(double angle)
{
    for (int i = 0; i < items.size(); i++) items[i].rotateOX(angle);
}

void Scene::rotateSceneOY(double angle)
{
    for (int i = 0; i < items.size(); i++) items[i].rotateOY(angle);
}
