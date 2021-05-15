#include "itemmenu.h"
#include "ui_itemmenu.h"

itemMenu::itemMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemMenu)
{
    ui->setupUi(this);
    // Use modern style lambdas
    connect(ui->xNegBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).Move(-(ui->coordinateInput->text().toDouble()), 0, 0);
    });
    connect(ui->xPosBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).Move((ui->coordinateInput->text().toDouble()), 0, 0);
    });
    connect(ui->zNegBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).Move(0, 0, -(ui->coordinateInput->text().toDouble()));
    });
    connect(ui->zPosBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).Move(0, 0, ui->coordinateInput->text().toDouble());
    });
    // Main direction is AntiClockwise
    connect(ui->rotateClockwise, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).RotateOY(-ui->angleInput->text().toDouble());
    });
    connect(ui->rotateAClockwiseBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).RotateOY(ui->angleInput->text().toDouble());
    });
}

itemMenu::~itemMenu()
{
    delete ui;
}

void itemMenu::setCurrentIndex(int index)
{
    currentIndex = index;
}

void itemMenu::setScene(Scene *&scene)
{
    this->scene = scene;
}
