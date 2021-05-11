#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->scene->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_scene_triggered()
{
    Settings *windowSettings = new Settings();
    windowSettings->update();
    windowSettings->show();
}
