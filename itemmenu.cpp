﻿#include "itemmenu.h"
#include "ui_itemmenu.h"

itemMenu::itemMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemMenu)
{
    ui->setupUi(this);
    // Use modern style lambdas
    connect(ui->xNegBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).move(-(ui->coordinateInput->text().toDouble()), 0, 0);
        scene->renderScene();
    });
    connect(ui->xPosBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).move((ui->coordinateInput->text().toDouble()), 0, 0);
        scene->renderScene();
    });
    connect(ui->zNegBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).move(0, 0, -(ui->coordinateInput->text().toDouble()));
        scene->renderScene();
    });
    connect(ui->zPosBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).move(0, 0, ui->coordinateInput->text().toDouble());
        scene->renderScene();
    });
    // Main direction is AntiClockwise
    connect(ui->rotateClockwise, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).spin(-ui->angleInput->text().toDouble());
        scene->renderScene();
    });
    connect(ui->rotateAClockwiseBtn, &QPushButton::pressed, [this]()
    {
        scene->getItemByIndex(currentIndex).spin(ui->angleInput->text().toDouble());
        scene->renderScene();
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

int itemMenu::getCurrentIndex()
{
    return currentIndex;
}

void itemMenu::setScene(Scene *&scene)
{
    this->scene = scene;
}
