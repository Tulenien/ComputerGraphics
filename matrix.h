#ifndef MATRIX_H
#define MATRIX_H

#define OK 0
#define ERROR 1

#include "QDebug"

class Matrix
{
public:
    Matrix();
    ~Matrix();
    void FillwithArray(double *&, int, int);
    void Multiply(const Matrix &, Matrix &);
    int getRows();
    int getCols();
    void setRows(int rows);
    void setCols(int cols);
    void printMatrix();

private:
    double **container;
    int rows_num;
    int cols_num;
};

#endif // MATRIX_H
