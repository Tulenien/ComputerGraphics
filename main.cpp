#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
//#include "matrix.h"
#include "item.h"


int main(int argc, char *argv[])
{
//    Matrix A, B, C;
//    std::vector<double> array1 = {1, -2, 3, -4, 5, -6, 7, -8, 9};
//    std::vector<double> array2 = {1, 2, 3};
//    A.FillwithArray(array1, 3, 3);
//    B.FillwithArray(array1, 3, 3);
//    A.printMatrix();
//    B.printMatrix();
//    A.Multiply(B, C);
//    C.printMatrix();

//    C.AddRows(array1, 3);
//    C.printMatrix();
    QString dir = "C:/Users/timof/Documents/Programming/ComputerGraphics/New folder/build-InteriorRedactor-Desktop_Qt_5_14_1_MinGW_64_bit-Debug/models/Столы";
    QString file = "Журнальный стол";
    Item item(dir, file);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
