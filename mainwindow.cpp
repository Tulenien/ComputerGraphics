#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_scene_triggered()
{
    if (windowSettings)
    {
        windowSettings->deleteLater();
    }
    windowSettings = new Settings();
    windowSettings->scene = ui->scene;
    windowSettings->show();
}

void MainWindow::on_actionScene_triggered()
{
    if (windowSettings) windowSettings->show();
}
