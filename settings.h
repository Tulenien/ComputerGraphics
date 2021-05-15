#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDebug>
#include <QWidget>
#include <iostream>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidget>
#include <QFileSystemModel>
#include "scene.h"
#include "itemmenu.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    Scene *scene;

private:
    Ui::Settings *ui;
    const QString *catalogPath;
    void loadItemCatalog(const QString &);

private slots:
    void AddItems();
    void sceneSizeChanged();
    void openItemMenu(QListWidgetItem *item);
};

#endif // SETTINGS_H
