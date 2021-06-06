#include "scene.h"

Scene::Scene(QWidget *parent)
{
    width = 200;
    length = 200;
    height = 250;
    this->setMinimumSize(640, 480);
    QSize size = this->size();
    imageWidth = size.width();
    imageHeight = size.height();
    image = new QImage(size, QImage::Format_RGBA8888);
    setupImage();
}

void Scene::resizeEvent(QResizeEvent *event)
{
    QImage other(this->size(), QImage::Format_RGBA8888);
    imageWidth = this->size().width();
    imageHeight = this->size().height();
    image->swap(other);
    this->setPixmap(QPixmap::fromImage(*image));
    renderScene();
}

void Scene::setupImage()
{
    image->fill(QColor(255, 255, 255));
    // Camera setup: fovX, fovY, focalLength, apertureWidth, apertureHeight, near, far
    cam = {90, 90, 20, 24, 18, 1, 1000};
    for (size_t i = 0; i < depthBuffer.size(); i++)
    {
        depthBuffer[i].clear();
    }
    depthBuffer.clear();
    for (int i = 0; i < image->size().width(); i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < image->size().height(); j++)
        {
            temp.push_back(qInf());
        }
        depthBuffer.push_back(temp);
    }
    clickSearch.clear();
}

void Scene::addItem(QString dir, QString item)
{
    items.push_back(Item(dir, item));
}

void Scene::deleteItem(int index)
{
    items.remove(items.size() - index - 1);
    renderScene();
}

void Scene::deleteItems()
{
    items.clear();
    renderScene();
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
}

Item &Scene::getItemByIndex(int index)
{
    return items[items.size() - index - 1];
}

void Scene::renderScene()
{
    setupImage();
    if (!items.isEmpty())
    {
        //computeScreenCoordinates();
        const matrix projection = computeProjectionMatrix();
        for (int i = 0; i < items.size(); i++)
        {
            items[i].rasterise(projection, imageWidth, imageHeight);
            items[i].render(depthBuffer, image, clickSearch, imageWidth, imageHeight);
        }
    }
    setPixmap(QPixmap::fromImage(*image));
    this->show();
}

void Scene::computeScreenCoordinates()
{
//    imageTop = cam.apertureWidth / 2 / cam.focalLength * cam.near;
//    imageRight = cam.apertureHeight / 2 / cam.focalLength * cam.near;
//    imageBottom = -imageTop;
//    imageLeft = -imageRight;
    imageTop = imageHeight / 2 * cam.near;
    imageRight = imageWidth / 2 * cam.near;
    imageBottom = -imageTop;
    imageLeft = -imageRight;
}

void Scene::rotateSceneOX(double angle)
{
    for (int i = 0; i < items.size(); i++) items[i].rotateOX(angle);
}

void Scene::rotateSceneOY(double angle)
{
    for (int i = 0; i < items.size(); i++) items[i].rotateOY(angle);
}

const matrix Scene::computeProjectionMatrix()
{
    double scale = 1;//1 / tanh(cam.fovX * 0.5 * PI / 180);
    double a = 1;//imageWidth / imageHeight;
    double q = cam.far / (cam.far - cam.near);
    const matrix projection =
    {
        {a * scale, 0, 0, 0},
        {0, scale, 0, 0},
        {0, 0, -q, -1},
        {0, 0, -cam.near * q, 0}
    };
    return projection;
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    QString key = QString::number(event->x()) + "." + QString::number(event->y());
//    qDebug() << key << " " << clickSearch[key];
    if (clickSearch[key])
    {
        // Right after selected item rendered, change clicked
        clickSearch[key]->changeIsClicked();
    }
}
