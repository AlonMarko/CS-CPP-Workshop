//
// Created by alonn on 6/9/2020.
//
#include "Matrix.h"
#include <iostream>
#include <cstring>

#define DEFAULT_SIZE 1 // for normal matrix of size 1 on 1

/**
     * @brief matrix constructor
     * @param rows obvious
     * @param cols obvious
*/
Matrix::Matrix(int rows, int cols)
{
    if (rows > 0 && cols > 0)
    {
        dimension.cols = cols;
        dimension.rows = rows;
        _mat = new(std::nothrow)float[cols * rows]{0};
        if (_mat == nullptr)
        {
            std::cerr << "Error: matrix cannot be created" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::cerr << "Error: Rows or Cols cannot be negative or 0" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
    * @brief default constructor - build 1x1 matrix with 0 inside
    */
Matrix::Matrix() : Matrix(DEFAULT_SIZE, DEFAULT_SIZE)
{
}

/**
    * @brief copy constructor - builds a matrix from another matrix
    * @param m the matrix we are copying to the new one
    */
Matrix::Matrix(const Matrix &m) : Matrix(m.dimension.rows, m.dimension.cols)
{
    for (int i = 0; i < dimension.rows * dimension.cols; i++)
    {
        _mat[i] = m._mat[i];
    }
}

/**
     * @brief matrix destructor!!!!! basiclly destroys the matrix
     */
Matrix::~Matrix()
{
    delete[] _mat;
}


/**
     * @brief acts as a = operator and puts the input matrix into the current one
     * @param m - the input matrix
     * @return
     */
Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }
    delete[] _mat;
    dimension.cols = m.dimension.cols;
    dimension.rows = m.dimension.rows;
    _mat = new(std::nothrow)float[dimension.rows * dimension.cols]{0};
    if (_mat == nullptr)
    {
        std::cerr << "Error: matrix cannot be created" << std::endl;
        exit(EXIT_FAILURE);
    }
    mempcpy(_mat, m._mat, dimension.rows * dimension.cols * sizeof(float));
    return *this;
}

/**
     * @brief matrix multiplication - one matrix with another
     * @param m - the other matrix
     * @return the multiplied matrix
     */
Matrix Matrix::operator*(const Matrix &m) const
{
    if (dimension.cols != m.dimension.rows) //checks if can multiply
    {
        std::cerr << "Error: multiplication invalid" << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix multiplied = Matrix(dimension.rows, m.dimension.cols);
    int i, j, h;
    for (i = 0; i < dimension.rows; i++)
    {
        for (j = 0; j < m.dimension.cols; j++)
        {
            for (h = 0; h < dimension.cols; h++)
            {
                multiplied(i, j) += m._mat[h * m.dimension.cols + j] * _mat[i * dimension.cols + h];
            }
        }
    }
    return multiplied;
}

/**
     * @brief matrix multiplication from the right with a scalar
     * @param c the scalar
     * @return the multiplied matrix
     */
Matrix Matrix::operator*(const float c)
{
    Matrix mt = Matrix(dimension.rows, dimension.cols);
    for (int i = 0; i < dimension.rows; i++)
    {
        for (int j = 0; j < dimension.cols; j++)
        {
            mt(i, j) = _mat[dimension.cols * i + j] * c;
        }
    }
    return mt;
}

/**
     * @brief matrix multiplication from the left with a scalar
     * @param c - the scalar
     * @param m - the matrix
     * @return the multiplied matrix
     */
Matrix operator*(const float c, Matrix &m)
{
    Matrix multiplied = Matrix(m.getRows(), m.getCols());
    for (int i = 0; i < m.dimension.rows; i++)
    {
        for (int j = 0; j < m.dimension.cols; j++)
        {
            multiplied(i, j) = c * m(i, j);
        }
    }
    return multiplied;
}

/**
     * @brief addition of two matrixes
     * @param m the second matrix to add
     * @return the new matrix
     */
Matrix Matrix::operator+(const Matrix &m) const
{
    if (dimension.rows != m.dimension.rows || dimension.cols != m.dimension.cols)
    {
        std::cerr << "Error: addition invalid - matrices not the same size" << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix added = Matrix(dimension.rows, dimension.cols);
    for (int i = 0; i < m.dimension.rows; i++)
    {
        for (int j = 0; j < m.dimension.cols; j++)
        {
            added(i, j) = _mat[dimension.cols * i + j] + m(i, j);
        }
    }
    return added;
}

/**
    * @brief addition accumulation to a matrix
    * @param m the mtrix were adding
    * @return the matrix added
    */
Matrix &Matrix::operator+=(const Matrix &m)
{
    if (dimension.rows != m.dimension.rows || dimension.cols != m.dimension.cols)
    {
        std::cerr << "Error: addition invalid - matrices not the same size" << std::endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < m.dimension.rows; i++)
    {
        for (int j = 0; j < m.dimension.cols; j++)
        {
            _mat[dimension.cols * i + j] = _mat[dimension.cols * i + j] + m._mat[m.dimension.cols
                                                                                 * i + j];
        }
    }
    return *this;
}

/**
     * @brief matrix parenthesis indexing const method
     * @param i row
     * @param j col
     * @return the value inside the index
     */
const float &Matrix::operator()(int i, int j) const
{
    if (j >= dimension.cols || i >= dimension.rows || j < 0 || i < 0)
    {
        std::cerr << "Error: location out of bounds dawg" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _mat[i * dimension.cols + j];
}

/**
     * @brief matrix parenthesis indexing not const
     * @param i row
     * @param j col
     * @return the value inside the index
     */
float &Matrix::operator()(int i, int j)
{
    if (j >= dimension.cols || i >= dimension.rows || j < 0 || i < 0)
    {
        std::cerr << "Error: location out of bounds dawg" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _mat[i * dimension.cols + j];
}

/**
     * @brief matrix bracket indexing - returns the item in this location - const ver
     * @param i - the index to return
     * @return the item inside the requested location
     */
const float &Matrix::operator[](int i) const
{
    if (i < 0 || i >= dimension.cols * dimension.rows)
    {
        std::cerr << "Error: location out of bounds homie" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _mat[i];
}

/**
     * @brief matrix bracket indexing - returns the item in this location - normal ver
     * @param i - the index to return
     * @return the item inside the requested location
     */
float &Matrix::operator[](int i)
{
    if (i < 0 || i >= dimension.cols * dimension.rows)
    {
        std::cerr << "Error: location out of bounds homie" << std::endl;
        exit(EXIT_FAILURE);
    }
    return _mat[i];
}

/**
     * @brief prints the matrix nice and beautiful
     * @param s the out stream
     * @param m the matrix to print
     * @return the stream
     */
std::ostream &operator<<(std::ostream &s, const Matrix &m)
{
    for (int i = 0; i < m.dimension.rows; i++)
    {
        for (int j = 0; j < m.dimension.cols; j++)
        {
            if (m(i, j) <= 0.1f)
            {
                s << "  ";
            }
            else
            {
                s << "**";
            }
        }
        s << std::endl;
    }
    return s;
}

/**
     * @brief reads data into the matrix
     * @param s the input strean
     * @param m the matrix that we paste the data into
     * @return the stream
     */
std::istream &operator>>(std::istream &s, Matrix &m)
{
    s.read((char *) m._mat, m.dimension.rows * m.dimension.cols * sizeof(float));
    if (!s.good() || s.peek() != EOF)
    {
        std::cerr << "Error: input stream failure" << std::endl;
        exit(EXIT_FAILURE);
    }
    return s;
}

/**
     * @brief turns the matrix into a one col matrix
     * since we use 1d array to represent 2d array it is easy only dimension change
     * with no real change besides that.
     */
Matrix Matrix::vectorize()
{
    dimension.rows = dimension.rows * dimension.cols;
    dimension.cols = 1;
    return *this;
}

/**
     * @brief prints the matrix - not nicley as 3.4.
     */
void Matrix::plainPrint() const
{
    for (int i = 0; i < dimension.rows; i++)
    {
        for (int j = 0; j < dimension.cols; j++)
        {
            std::cout << _mat[i * dimension.cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

/**
    * @brief returns the row number - const ver
    * @return int - row
    */
int Matrix::getRows() const
{
    return dimension.rows;
}

/**
   * @brief returns the col number - const ver
   * @return int col
   */
int Matrix::getCols() const
{
    return dimension.cols;
}






