#include "scene.h"

Scene::Scene(QWidget *parent)
    : width(200), length(200), height(250)
{
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
    // Camera setup: fov, near, far
    cam = {90, 10, length + 10};
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

void Scene::setSize(const double &width, const double &length, const double &height)
{
    this->width = width;
    this->length = length;
    this->height = height;
    renderScene();
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
        double radAngle = -PI * 0.5;
        double temp;
        double minZ = qInf(), maxZ = - qInf();
        double yOffset = 0;
        for (int i = 0; i < items.size(); i++)
        {
            volumeBorder &borders = items[i].getBorders();
            yOffset = -height * 0.5 - borders.minY;
            if (abs(yOffset) < 1e-5) yOffset = 0;
            if (items[i].ageCheck())
            {
                items[i].move(-(borders.maxX + borders.minX) * 0.5, yOffset,
                              -(borders.maxZ + borders.minZ) * 0.5);
                temp = borders.minX;
                borders.minX = (borders.minX - borders.maxX) * 0.5;
                borders.maxX = (borders.maxX - temp) * 0.5;
                borders.minY = borders.minY + yOffset;
                borders.maxY = borders.maxY + yOffset;
                temp = borders.minZ;
                borders.minZ = (borders.minZ - borders.maxZ) * 0.5;
                borders.maxZ = (borders.maxZ - temp) * 0.5;
                if (borders.minZ < minZ) minZ = borders.minZ;
                if (borders.maxZ > maxZ) maxZ = borders.maxZ;
            }
        }
        double distance = 0;
        if (!qIsInf(minZ))
        {
            distance = maxZ - minZ;
            cam.near = -minZ;
            cam.far  = -minZ + 3 * distance;
        }
        const matrix projection = computeProjectionMatrix();
        const matrix orthograpic = computeOrthographicMatrix();
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i].ageCheck())
            {
                items[i].move(0, 0, -2 * distance);
                items[i].ageChange();
            }
//            if (viewMode) items[i].rasterise(orthograpic, imageWidth, imageHeight, -height * 0.5, radAngle);
//            else items[i].rasterise(projection, imageWidth, imageHeight, -height * 0.5, qInf());
            items[i].rasterise(projection, imageWidth, imageHeight, qInf());
            items[i].render(depthBuffer, *image, clickSearch, imageWidth, imageHeight);
        }
    }
    setPixmap(QPixmap::fromImage(*image));
    this->show();
}

void Scene::computeScreenCoordinates()
{
    imageTop = imageHeight / 2 * cam.near;
    imageRight = imageWidth / 2 * cam.near;
    imageBottom = -imageTop;
    imageLeft = -imageRight;
}

void Scene::rotateSceneOY(double angle)
{
    for (int i = 0; i < items.size(); i++) items[i].spinOY(angle);
    renderScene();
}

const matrix Scene::computeProjectionMatrix()
{
    double scale = 1;   //1 / tanh(cam.fovX * 0.5 * PI / 180);
    double a = 1;       //imageWidth / imageHeight;
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

const matrix Scene::computeOrthographicMatrix()
{
    double max =  std::max(width, height);
    double rml = 1 / max;
    double tmb = 1 / max;
    double fmn = 1 / (cam.far - cam.near);
    const matrix projection =
    {
        {rml, 0, 0, 0},
        {0, tmb, 0, 0},
        {0, 0, -2 * fmn, 0},
        {0, 0, -(cam.far + cam.near) * fmn, 1}
    };
    return projection;
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    QString key = QString::number(event->x()) + "." + QString::number(event->y());
    if (clickSearch[key])
    {
        bool status = clickSearch[key]->changeIsClicked();
        if (status)
        {
            clickSearch[key]->outline(*image);
            setPixmap(QPixmap::fromImage(*image));
            this->show();
        }
        else renderScene();
    }
}

