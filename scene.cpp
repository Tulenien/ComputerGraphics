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
    setupImage();
}

void Scene::setupImage()
{
    image->fill(QColor(255, 255, 255));
    // Delete later
    setPixmap(QPixmap::fromImage(*image));
    // Camera setup: fovX, fovY, focalLength, apertureWidth, apertureHeight, near, far
    cam = {90, 90, 20, 24, 18, 1, 10000};
    // Clear zBuffer
    // Set depth buffer's size equal to QImage size
    for (size_t i = 0; i < depthBuffer.size(); i++)
    {
        depthBuffer[i].clear();
    }
    depthBuffer.clear();
    for (int i = 0; i < image->size().height(); i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < image->size().width(); j++)
        {
            temp.push_back(qInf());
        }
        depthBuffer.push_back(temp);
    }
}

void Scene::addItem(QString dir, QString item)
{
    items.push_back(Item(dir, item));
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
    return items[index];
}

void Scene::renderScene()
{
    setupImage();
    rasteriseScene();
    for (int i = 0; i < items.size(); i++)
    {
        items[i].render(depthBuffer, image, imageWidth, imageHeight);
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
    // World to camera transformation
    double q = cam.far / (cam.far - cam.near);
    const matrix projection =
    {
        {a * scale, 0, 0, 0},
        {0, scale, 0, 0},
        {0, 0, q, 1},
        {0, 0, -cam.near * q, 0}
    };
    return projection;
}

void Scene::rasteriseScene()
{
    //computeScreenCoordinates();
    const matrix projection = computeProjectionMatrix();
    for (int i = 0; i < items.size(); i++)
    {
        items[i].rasterise(projection, imageWidth, imageHeight);
    }
}
