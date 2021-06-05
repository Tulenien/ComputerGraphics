#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include "item.h"

/*TODO:
 * Add button to Show rendered
 * Add Update function
 * Put all methods of render into 'render'
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
    double apertureWidth;
    double apertureHeight;
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
    void deleteItem(int index);
    void deleteItems();
    void setSize(double &width, double &length, double &height);
    double getWidth();
    double getLength();
    double getHeight();
    // Add rotation
    void rotateSceneOX(double angle);
    void rotateSceneOY(double angle);
    Item &getItemByIndex(int index);
    // Incapsulates all transformations and Zbuffer
    void renderScene();

private:
    int imageWidth, imageHeight;
    double imageTop, imageRight, imageBottom, imageLeft;
    camera cam;
    QImage *image;
    QVector<Item> items;
    matrix depthBuffer;
    double width, length, height;

    void resizeEvent(QResizeEvent *event);
    // Then included in render method:
    void computeScreenCoordinates();
    void setupImage();
    const matrix computeProjectionMatrix();
};

#endif // SCENE_H
