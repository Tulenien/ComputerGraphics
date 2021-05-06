#include "matrix.h"

Matrix::Matrix()
{
    rows_num = 0;
    cols_num = 0;
}

Matrix::~Matrix()
{

}

void Matrix::FillwithArray(const std::vector<double> &array, int rows, int cols)
{
    rows_num = rows;
    cols_num = cols;
    std::vector<double> temp(cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            temp[j] = array[i * cols_num + j];
        }
        container.push_back(temp);
    }
}

void Matrix::AddRows(const std::vector<double> &array, int rows_number)
{
    std::vector<double> temp(cols_num);
    for (int i = 0; i < rows_number; i++)
    {
        for (int j = 0; j < cols_num; j++)
        {
            temp[j] = array[i * cols_num + j];
        }
        container.push_back(temp);
    }
    rows_num += rows_number;
}

void Matrix::Multiply(const Matrix &right, Matrix &result)
{
    // Standart multiplication algorithm
    int rows = this->rows_num;
    int cols = right.cols_num;
    int nest = this->cols_num;
    result.rows_num = rows;
    result.cols_num = cols;
    for (int i = 0; i < rows; i++)
    {
        std::vector<double> temp(cols);
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < nest; k++)
            {
                temp[j] = this->container[i][k] * right.container[k][j];
            } 
        }
        result.container.push_back(temp);
    }
}

int Matrix::getRows()
{
    return rows_num;
}

int Matrix::getCols()
{
    return cols_num;
}

void Matrix::setCols(int cols)
{
    cols_num = cols;
}

void Matrix::setRows(int rows)
{
    rows_num = rows;
}

void Matrix::setRowsFromVector()
{
    rows_num = container.size();
}

void Matrix::printMatrix()
{
    // Function for testing
    for (int i = 0; i < rows_num; i++)
    {
        for (int j = 0; j < cols_num; j++)
        {
            qDebug() << container[i][j] << " ";
        }

    }
    qDebug() << "\n";
}
