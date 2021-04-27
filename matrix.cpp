#include "matrix.h"

Matrix::Matrix(int rows, int cols)
{
    rows_num = rows;
    cols_num = cols;
    container = new double* [rows];
    for (int i = 0; i < rows; i++)
    {
        container[i] = new double[cols];
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i < rows_num; i++)
    {
        delete [] container[i];
    }
    delete [] container;
    rows_num = 0;
    cols_num = 0;
}

void Matrix::FillwithArray(double **array)
{
    for(int i = 0; i < rows_num; i++)
    {
        for(int j = 0; i < cols_num; j++)
        {
            container[i][j] = *array[i * cols_num + j];
        }
    }
}

void Matrix::Multiply(const Matrix &right, Matrix &result)
{
    // Standart multiplication algorithm
    int rows = this->rows_num;
    int cols = right.cols_num;
    int nest = this->cols_num;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result.container[i][j] = 0;
            for (int k = 0; k < nest; k++)
            {
                result.container[i][j] =
                 this->container[i][k] * right.container[k][j];
            }
        }
    }
}

void Matrix::setRows(int rows)
{
    rows_num = rows;
}

void Matrix::setCols(int cols)
{
    cols_num = cols;
}

int Matrix::getRows()
{
    return rows_num;
}

int Matrix::getCols()
{
    return cols_num;
}
