#ifndef SCENE_H
#define SCENE_H

#include <QLabel>
#include <QMouseEvent>
#include "item.h"

/*TODO:
 * Add 3D Clipping method
 */

struct Camera
{
    double fov;
    double near;
    double far;

    Camera(double t_fov, double t_near, double t_far): fov(t_fov), near(t_near), far(t_far){}
};

class Scene : public QLabel
{
    Q_OBJECT
public:
    Scene(QWidget*& t_parent);
    void addItem(QString t_dir, QString t_item);
    void deleteItem(int t_index);
    void deleteItems();
    void setSize(const double &t_width, const double &t_length, const double &t_height);
    void changeViewMode() { m_viewMode = !m_viewMode; }
    double getWidth() const { return m_width; }
    double getLength() const { return m_length; }
    double getHeight() const { return m_height; }
    bool getViewMode() const { return m_viewMode; }
    void rotateSceneOY(double t_angle);
    Item &getItemByIndex(int t_index);
    void renderScene();

private:
    QWidget *m_parent;
    Camera m_camera;
    QImage *m_image;
    bool m_viewMode;
    int m_imageWidth, m_imageHeight;
    double m_width, m_length, m_height;
    matrix m_depthBuffer;
    QVector<Item> m_items;
    QMap<QString, Item *> m_clickSearch;

    void setupImage();
    const matrix *computeProjectionMatrix();
    const matrix *computeOrthographicMatrix();
    void resizeEvent(QResizeEvent *t_event);
    void mousePressEvent(QMouseEvent *t_event);
};

#endif // SCENE_H
