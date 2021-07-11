#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QTreeWidget>
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
    Scene *scene = nullptr;
    itemMenu *imenu = nullptr;

private:
    Ui::Settings *ui;
    const QString catalogPath;
    void loadItemCatalog(const QString &);

private slots:
    void addItems();
    void deleteItem();
    void deleteAllItems();
    void changeViewMode();
    void sceneSizeChanged();
    void openItemMenu(QListWidgetItem *item);
};

#endif // SETTINGS_H
