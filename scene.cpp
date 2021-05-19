#include "scene.h"

Scene::Scene(QWidget *parent)
{
    width = 200;
    length = 200;
    height = 250;
    QSize size = this->size();
    //qDebug() << size; // 640x480
    image = new QImage(size, QImage::Format_RGBA8888);
    image->fill(QColor(0, 0, 0));
    this->setPixmap(QPixmap::fromImage(*image));
    // Camera setup: fovX, fovY, focalLength, near, far
    cam = {90, 90, 35, 0.1, 100};
    // Set depth buffer's size equal to QImage size
    for (int i = 0; i < image->size().height(); i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < image->size().width(); j++)
        {
            temp.push_back(Q_INFINITY);
        }
        depthBuffer.push_back(temp);
    }
    for (int i = 0; i < items.size(); i++)
    {
        items[i].setDepthBuffer(&depthBuffer);
    }
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

void Scene::projectScene()
{
    double scaleX = 1;  //1 / tanh(cam.fovX * 0.5 * PI / 180);
    double scaleY = 1;  //1 / tanh(cam.fovY * 0.5 * PI / 180);
    double coeff = 1 / cam.far - cam.near;
    const matrix projection =
    {
        {scaleX, 0, 0, 0},
        {0, scaleY, 0, 0},
        {0, 0, -cam.far * coeff, -1},
        {0, 0, -cam.far * cam.near * coeff, 0}
    };
    for (int i = 0; i < items.size(); i++)
    {
        items[0].project(projection);
    }
}

void Scene::normaliseScene()
{
    // Working with projected points
    for (int i = 0; i < items.size(); i++)
    {
        items[0].normalise(image->size().height(), image->size().width());
    }
}
