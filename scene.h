#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include "item.h"

/*TODO:
 * Add ZBuffer Algorithm
 * Add 3D Clipping method
*/

struct camera
{
    /* Needed for image scaling:
     * Field of View - angle between sides of camera view
     * Focal length - distance from camera point to far clipping plane
    */
    double fovX;
    double fovY;
    double focalLength;
    // Near and far clipping planes
    double near;
    double far;
};

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
    double getDepthBufferElement(size_t i, size_t j);
    void setDepthBufferElement(size_t i, size_t j, double element);
    void setImageElement(int i, int j, QColor color);
    // Add rotation
    void rotateSceneOX(double angle);
    void rotateSceneOY(double angle);
    Item &getItemByIndex(int index);
    // Incapsulates all transformations and Zbuffer
    void render();
    // Then included in render method:
    void projectScene();
    // To NDC coordinates
    void normaliseScene();
    void rasteriseScene();

private:
    camera cam;
    QImage *image;
    QVector<Item> items;
    matrix depthBuffer;
    double width, length, height;
};

#endif // SCENE_H
