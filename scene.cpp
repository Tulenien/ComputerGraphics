#include "scene.h"

#include <memory.h>

Scene::Scene(QWidget*& t_parent)
    : m_parent(t_parent), m_camera(90, 10, 210),
      m_image(new QImage(this->size(), QImage::Format_RGBA8888)),
      m_viewMode(true), m_imageWidth(this->width()), m_imageHeight(this->height()),
      m_width(200), m_length(200), m_height(250)
{
    this->setMinimumSize(640, 480);
    setupImage();
}

void Scene::resizeEvent(QResizeEvent *t_event)
{
    t_event->accept();
    const QSize newSize = this->size();
    QImage other(newSize, QImage::Format_RGBA8888);
    m_imageWidth = newSize.width();
    m_imageHeight = newSize.height();
    m_image->swap(other);
    this->setPixmap(QPixmap::fromImage(*m_image));
    renderScene();
}

void Scene::setupImage()
{
    m_image->fill(QColor(255, 255, 255));
    for (size_t i = 0; i < m_depthBuffer.size(); i++)
    {
        m_depthBuffer[i].clear();
    }
    m_depthBuffer.clear();
    for (int i = 0; i < m_image->size().width(); i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < m_image->size().height(); j++)
        {
            temp.push_back(qInf());
        }
        m_depthBuffer.push_back(temp);
    }
    m_clickSearch.clear();
}

void Scene::addItem(QString t_dir, QString t_item)
{
    m_items.push_back(Item(t_dir, t_item));
}

Item &Scene::getItemByIndex(int t_index)
{
    return m_items[m_items.size() - t_index - 1];
}

void Scene::deleteItem(int t_index)
{
    m_items.remove(m_items.size() - t_index - 1);
    renderScene();
}

void Scene::deleteItems()
{
    m_items.clear();
    renderScene();
}

void Scene::setSize(const double &t_width, const double &t_length, const double &t_height)
{
    m_width = t_width;
    m_length = t_length;
    m_height = t_height;
    renderScene();
}

void Scene::renderScene()
{
    setupImage();
    if (!m_items.isEmpty())
    {
//        double radAngle = -PI * 0.5;
        double temp;
        double minZ = qInf(), maxZ = - qInf();
        double yOffset = 0;
        for (int i = 0; i < m_items.size(); i++)
        {
            VolumeBorder &borders = m_items[i].getBorders();
            yOffset = -m_height * 0.5 - borders.minY;
            if (abs(yOffset) < 1e-5) yOffset = 0;
            if (m_items[i].ageCheck())
            {
                m_items[i].move(-(borders.maxX + borders.minX) * 0.5, yOffset,
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
            m_camera.near = -minZ;
            m_camera.far  = -minZ + 3 * distance;
        }
        std::unique_ptr<const matrix> projection(computeProjectionMatrix());
        std::unique_ptr<const matrix> orthograpic(computeOrthographicMatrix());
        for (int i = 0; i < m_items.size(); i++)
        {
            if (m_items[i].ageCheck())
            {
                m_items[i].move(0, 0, -2 * distance);
                m_items[i].ageChange();
            }
//            if (viewMode) items[i].rasterise(orthograpic, imageWidth, imageHeight, -height * 0.5, radAngle);
//            else items[i].rasterise(projection, imageWidth, imageHeight, -height * 0.5, qInf());
            m_items[i].rasterise(*projection, m_imageWidth, m_imageHeight, qInf());
            m_items[i].render(m_depthBuffer, *m_image, m_clickSearch, m_imageWidth, m_imageHeight);
        }
    }
    setPixmap(QPixmap::fromImage(*m_image));
    this->show();
}

void Scene::rotateSceneOY(double angle)
{
    for (int i = 0; i < m_items.size(); i++) m_items[i].spinOY(angle);
    renderScene();
}

const matrix *Scene::computeProjectionMatrix()
{
    double scale = 1;   //1 / tanh(cam.fovX * 0.5 * PI / 180);
    double a = 1;       //imageWidth / imageHeight;
    double q = m_camera.far / (m_camera.far - m_camera.near);
    const matrix *projection = new matrix
    {
        {a * scale, 0, 0, 0},
        {0, scale, 0, 0},
        {0, 0, -q, -1},
        {0, 0, -m_camera.near * q, 0}
    };
    return projection;
}

const matrix *Scene::computeOrthographicMatrix()
{
    double max =  std::max(m_width, m_height);
    double rml = 1 / max;
    double tmb = 1 / max;
    double fmn = 1 / (m_camera.far - m_camera.near);
    const matrix *projection = new matrix
    {
        {rml, 0, 0, 0},
        {0, tmb, 0, 0},
        {0, 0, -2 * fmn, 0},
        {0, 0, -(m_camera.far + m_camera.near) * fmn, 1}
    };
    return projection;
}

void Scene::mousePressEvent(QMouseEvent *t_event)
{
    QString key = QString::number(t_event->x()) + "." + QString::number(t_event->y());
    if (m_clickSearch[key])
    {
        bool status = m_clickSearch[key]->changeIsClicked();
        if (status)
        {
            m_clickSearch[key]->outline(*m_image);
            setPixmap(QPixmap::fromImage(*m_image));
            this->show();
        }
        else renderScene();
    }
}

