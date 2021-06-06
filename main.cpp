#include "mainwindow.h"
#include <QApplication>
#include "item.h"

int main(int argc, char *argv[])
{
    // Do not touch
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
