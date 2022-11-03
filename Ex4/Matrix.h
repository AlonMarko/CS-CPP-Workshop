// Matrix.h
#include <iostream>
#include <fstream>

#ifndef MATRIX_H
#define MATRIX_H


/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

// Insert Matrix class here...
/**
 * @brief class that represents a matrix and contains all the relevant mthods and declarations that
 * are implemented in the cpp file.
 * class has 2 fields - the matrix itself which is 1d array and a struct dimension.
 */
class Matrix
{
public:
    /**
     * @brief matrix constructor - all elements will be 0
     * @param rows obvious
     * @param cols obvious
     */
    Matrix(int rows, int cols);

    /**
     * @brief default constructor - build 1x1 matrix with 0 inside
     */
    Matrix();

    /**
     * @brief copy constructor - builds a matrix from another matrix
     * @param m the matrix we are copying to the new one
     */
    Matrix(const Matrix &m);

    /**
     * @brief matrix destructor!!!!! basiclly destroys the matrix
     */
    ~Matrix();


    /**
     * @brief returns the row number - const ver
     * @return int - row
     */
    int getRows() const;

    /**
    * @brief returns the col number - const ver
    * @return int col
    */
    int getCols() const;


    /**
     * @brief turns the matrix into a one col matrix.
     */
    Matrix vectorize();

    /**
     * @brief prints the matrix - not nicley as 3.4.
     */
    void plainPrint() const;

    /**
     * @brief acts as a = operator and puts the input matrix into the current one
     * @param m - the input matrix
     * @return
     */
    Matrix &operator=(const Matrix &m);

    /**
     * @brief matrix multiplication - one matrix with another
     * @param m - the other matrix
     * @return the multiplied matrix
     */
    Matrix operator*(const Matrix &m) const;

    /**
     * @brief matrix multiplication from the right with a scalar
     * @param c the scalar
     * @return the multiplied matrix
     */
    Matrix operator*(const float c);

    /**
     * @brief matrix multiplication from the left with a scalar
     * @param c - the scalar
     * @param m - the matrix
     * @return the multiplied matrix
     */
    friend Matrix operator*(const float c, Matrix &m);

    /**
     * @brief addition of two matrixes
     * @param m the second matrix to add
     * @return the new matrix
     */
    Matrix operator+(const Matrix &m) const;

    /**
     * @brief addition accumulation to a matrix
     * @param m the mtrix were adding
     * @return the matrix added
     */
    Matrix &operator+=(const Matrix &m);

    /**
     * @brief matrix parenthesis indexing const method
     * @param i row
     * @param j col
     * @return the value inside the index
     */
    const float &operator()(int i, int j) const;

    /**
     * @brief matrix parenthesis indexing not const
     * @param i row
     * @param j col
     * @return the value inside the index
     */
    float &operator()(int i, int j);

    /**
     * @brief matrix bracket indexing - returns the item in this location - const ver
     * @param i - the index to return
     * @return the item inside the requested location
     */
    const float &operator[](int i) const;

    /**
     * @brief matrix bracket indexing - returns the item in this location - normal ver
     * @param i - the index to return
     * @return the item inside the requested location
     */
    float &operator[](int i);

    /**
     * @brief prints the matrix nice and beautiful
     * @param s the out stream
     * @param m the matrix to print
     * @return the stream
     */
    friend std::ostream &operator<<(std::ostream &s, const Matrix &m);

    /**
     * @brief reads data into the matrix
     * @param s the input strean
     * @param m the matrix that we paste the data into
     * @return the stream
     */
    friend std::istream &operator>>(std::istream &s, Matrix &m);


private:
    MatrixDims dimension{}; // matrix dimensions.
    float *_mat; // the pointer to a float array.
};

#endif //MATRIX_H
