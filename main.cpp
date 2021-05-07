#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
//#include "matrix.h"
#include "item.h"


int main(int argc, char *argv[])
{
    //Test zone
    QString dir = "C:/Users/timof/Documents/Programming/ComputerGraphics/New folder/build-InteriorRedactor-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/models/Столы";
    QString file = "Журнальный стол";
    Item item(dir, file);

    matrix A =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    matrix B =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    matrix C =
    {
        {1},
        {2},
        {3}
    };
    matrix D =
    {
        {1, 2, 3, 4}
    };
    item.multiplyMatrix(A, B);
    item.multiplyMatrix(A, C);
    item.multiplyMatrix(A, D);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
