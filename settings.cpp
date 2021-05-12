#include "settings.h"
#include "ui_settings.h"

//bring to other .h files
#include <iostream>
#include <QFileSystemModel>
#include <functional>
#include <QTreeWidgetItem>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    // Load all available interior items to itemCatalog
    const QString catalogPath = "C:/Users/timof/Documents/Programming/ComputerGraphics/QtProjects/InteriorBuilder/models";
    QDir path(catalogPath);
    QDir filepath("");
    QStringList items;
    qDebug() << path.entryList(QDir::NoDotAndDotDot | QDir::AllDirs) << path.absolutePath();
    QStringList folders = path.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (int i = 0; i < folders.count(); i++)
    {
        QTreeWidgetItem *root = new QTreeWidgetItem(ui->itemCatalogWdt);
        root->setText(0, folders[i]);
        filepath.setPath(path.absolutePath() + "/" + folders[i]);
        items = filepath.entryList(QStringList("*.obj"));
        for (int i = 0; i < items.count(); i++)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            item->setText(0, items[i].replace(".obj", ""));
            root->addChild(item);
        }
    }
    ui->itemCatalogWdt->show();
}

Settings::~Settings()
{
    delete ui;
}
