#include "mainwindow.h"
#include <QApplication>
#include "item.h"


int main(int argc, char *argv[])
{
    //Test zone
//    QString dir = "C:/Users/timof/Documents/Programming/ComputerGraphics/New folder/build-InteriorRedactor-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/models/Столы";
//    QString file = "Журнальный стол";
//    Item item(dir, file);

//    item.RotateOY(90);
//    item.Move(10, -10, 100);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
