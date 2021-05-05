#ifndef MATRIX_H
#define MATRIX_H

#define OK 0
#define ERROR 1

#include "QDebug"
#include <vector>

typedef std::vector<std::vector<double>> matrix;

class Matrix
{
public:
    Matrix();
    ~Matrix();
    void FillwithArray(double *, int rows, int cols);
    void AddRows(double *, int rows_num);
    void Multiply(const Matrix &, Matrix &);
    int getRows();
    int getCols();
    void printMatrix();

private:
    matrix container;
    int rows_num;
    int cols_num;
};

#endif // MATRIX_H
