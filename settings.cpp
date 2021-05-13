#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    // Load all available interior items to itemCatalog
    const QString catalogPath = "C:/Users/timof/Documents/Programming/ComputerGraphics/QtProjects/InteriorBuilder/models";
    loadItemCatalog(catalogPath);

    connect(ui->addItemsButton, &QPushButton::released, this, &Settings::AddItems);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::loadItemCatalog(const QString &catalogPath)
{
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
        for (int j = 0; j < items.count(); j++)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem(root);
            item->setText(0, items[j].replace(".obj", ""));
            item->setCheckState(0, Qt::Unchecked);
            root->addChild(item);
        }
    }
}

void Settings::AddItems()
{
    for (int i = 0; i < ui->itemCatalogWdt->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = ui->itemCatalogWdt->topLevelItem(i);
        for (int j = 0; j < item->childCount(); j++)
        {
            //qDebug() << item->child(j)->checkState(0);
            // Find all checked items and add them to the Qlist
            // Only use child attribute when traversing the QTreeItem * or Segment fault
            if (item->child(j)->checkState(0) == Qt::Checked)
            {
                // Check if Item is added before
                //ui->ItemListWdt->findItems(item[i].text(0), Qt::MatchExactly).size()
                QListWidgetItem *listItem = new QListWidgetItem;
                listItem->setText(item->child(j)->text(0));
                ui->ItemListWdt->insertItem(0, listItem);
                item->child(j)->setCheckState(0, Qt::Unchecked);
            }
        }
    }
}
