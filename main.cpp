#include <QApplication>

#include "mainwindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainwindow;

    mainwindow.show();
    return a.exec();
}
