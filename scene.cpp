﻿#include "scene.h"

Scene::Scene(QWidget *parent)
{
    width = 200;
    length = 200;
    height = 250;
    QSize size = this->size();
    //qDebug() << size; // 640x480
    imageWidth = size.width();
    imageHeight = size.height();
    image = new QImage(size, QImage::Format_RGBA8888);
    image->fill(QColor(255, 255, 255));
    setPixmap(QPixmap::fromImage(*image));
    /* Full Aperture 35mm camera parametersn in mm:
     * Aperture width = 24
     * Aperture height = 18
     * Focal length = 20
     * Near clipping plane = 1
     * Far clipping plane = 1000
     */
    // Camera setup: fovX, fovY, focalLength, apertureWidth, apertureHeight, near, far
    cam = {90, 90, 20, 24, 18, 1, 1000};
    // Set depth buffer's size equal to QImage size
    for (int i = 0; i < image->size().height(); i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < image->size().width(); j++)
        {
            // Set to far clipping plane z-value
            temp.push_back(cam.far);
        }
        depthBuffer.push_back(temp);
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

void Scene::renderScene()
{
    rasteriseScene();
    for (int i = 0; i < items.size(); i++)
    {
        items[0].render(depthBuffer, image, imageWidth, imageHeight);
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
    imageTop = imageHeight / 2;
    imageRight = imageWidth / 2;
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
    double scaleX = 1;  //1 / tanh(cam.fovX * 0.5 * PI / 180);
    double scaleY = 1;  //1 / tanh(cam.fovY * 0.5 * PI / 180);
    double coeff = 1 / (cam.far - cam.near);
    const matrix projection =
    {
        {scaleX, 0, 0, 0},
        {0, scaleY, 0, 0},
//        {0, 0, -cam.far * coeff, -1},
//        {0, 0, -cam.far * cam.near * coeff, 0}
        {0, 0, 1, -1},
        {0, 0, 100, 0}
    };
    return projection;
}

void Scene::rasteriseScene()
{
    computeScreenCoordinates();
    const matrix projectionMatrix = computeProjectionMatrix();
    for (int i = 0; i < items.size(); i++)
    {
        items[0].rasterise(projectionMatrix, imageLeft, imageRight, imageTop, imageBottom,
                           cam.near, imageWidth, imageHeight);
    }
}
