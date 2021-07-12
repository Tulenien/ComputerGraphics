#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include <QMouseEvent>
#include "item.h"

/*TODO:
 * Add 3D Clipping method
 */

struct camera
{
    double fov;
    double near;
    double far;
};

class Scene : public QLabel
{
    Q_OBJECT
public:
    Scene(QWidget*& parent);
    void addItem(QString dir, QString item);
    void deleteItem(int index);
    void deleteItems();
    void setSize(const double &width, const double &length, const double &height);
    void changeViewMode() { viewMode = !viewMode; }
    double getWidth() const { return width; }
    double getLength() const { return length; }
    double getHeight() const { return height; }
    bool getViewMode() const { return viewMode; }
    void rotateSceneOY(double angle);
    Item &getItemByIndex(int index);
    void renderScene();

private:
    QWidget *m_parent;
    bool viewMode = true;
    int imageWidth, imageHeight;
    double imageTop, imageRight, imageBottom, imageLeft;
    camera cam;
    QImage *image;
    QVector<Item> items;
    QMap<QString, Item *> clickSearch;
    matrix depthBuffer;
    double width, length, height;

    void resizeEvent(QResizeEvent *event);
    void computeScreenCoordinates();
    void setupImage();
    const matrix computeProjectionMatrix();
    const matrix computeOrthographicMatrix();
    void mousePressEvent(QMouseEvent *event);
};

#endif // SCENE_H
