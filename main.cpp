#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include "matrix.h"

int main(int argc, char *argv[])
{
    Matrix A, B, C;
    double *array1 = new double[9]{1, -2, 3, -4, 5, -6, 7, -8, 9};
    double *array2 = new double[3]{1, 2, 3};
    A.FillwithArray(array1, 3, 3);
    B.FillwithArray(array1, 3, 3);
    A.printMatrix();
    B.printMatrix();
    A.Multiply(B, C);
    C.printMatrix();

    C.AddRows(array1, 3);
    C.printMatrix();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
