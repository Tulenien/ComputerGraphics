#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    // Load all available interior items to itemCatalog
    catalogPath = new QString("C:/Users/timof/Documents/Programming/ComputerGraphics/QtProjects/InteriorBuilder/models");
    loadItemCatalog(*catalogPath);
    // Set validators to prevent bad values
    QDoubleValidator verticalValidator(1, 501, 6);
    QDoubleValidator horizontalValidator(1, 10001, 6);
    ui->heightField->setValidator(&verticalValidator);
    ui->lengthField->setValidator(&horizontalValidator);
    ui->widthField->setValidator(&horizontalValidator);
    connect(ui->addItemsButton, &QPushButton::released, this, &Settings::addItems);
    connect(ui->setSceneSize, &QPushButton::released, this, &Settings::sceneSizeChanged);
    connect(ui->ItemListWdt, &QListWidget::itemDoubleClicked, this, &Settings::openItemMenu);
    connect(ui->showButton, &QPushButton::released, this, &Settings::showScene);
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
    //qDebug() << path.entryList(QDir::NoDotAndDotDot | QDir::AllDirs) << path.absolutePath();
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
        }
    }
}

void Settings::addItems()
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
                QString itemName = item->child(j)->text(0);
                listItem->setText(itemName);
                ui->ItemListWdt->insertItem(0, listItem);
                item->child(j)->setCheckState(0, Qt::Unchecked);
                scene->addItem(*catalogPath + "/" + item->text(0), itemName);
            }
        }
    }
    scene->renderScene();
}

void Settings::sceneSizeChanged()
{
    double width = ui->widthField->text().toDouble();
    double length = ui->lengthField->text().toDouble();
    double height = ui->heightField->text().toDouble();
    // call Size Change in scene
    scene->setSize(width, length, height);
}

void Settings::openItemMenu(QListWidgetItem *item)
{
    // Change item in window if it exists
    if (!imenu)
    {
        imenu = new itemMenu();
        imenu->setScene(this->scene);
    }
    // Set index of item in Scene's QList of Items
    imenu->setCurrentIndex(ui->ItemListWdt->row(item));
    imenu->setWindowTitle(item->text());
    imenu->show();
}

void Settings::showScene()
{
    scene->renderScene();
}
