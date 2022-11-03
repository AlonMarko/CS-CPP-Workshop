//
// Created by alonn on 6/9/2020.
//

#include "Activation.h"
#include <cmath>

/**
    * @brief activation type constructor
    * @param actType enum representing the activation type
    */
Activation::Activation(ActivationType actType) : _type(actType)
{
    if(actType != Softmax && actType != Relu)
    {
        std::cerr << "Error: invalid activation type" << std::endl;
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief apllies relu on the matrix and returns a new one
 * @param m the matrix
 * @return relu'd matrix
 */
Matrix relu(const Matrix &m)
{
    Matrix mat = Matrix(m.getRows(), m.getCols());
    for (int i = 0; i < m.getCols() * m.getRows(); i++)
    {
        if (m[i] >= 0)
        {
            mat[i] = m[i];
        }
        else
        {
            mat[i] = 0;
        }
    }
    return mat;
}

/**
 * @brief applies softMax on matrix and returns a new one
 * @param m the matrix
 * @return softMax'd matrix
 */
Matrix softMax(const Matrix &m)
{
    Matrix mat = Matrix(m.getRows(), m.getCols());
    float zK = 0, exponentT = 0, leftScalar = 0;
    for (int i = 0; i < m.getCols() * m.getRows(); i++)
    {
        zK = std::exp(m[i]);
        exponentT += zK;
        mat[i] = zK;
    }
    leftScalar = 1 / exponentT;
    mat = leftScalar * mat;
    return mat;
}

/**
     * @brief applies activation func - relu or softMax on the input matrix we recieved
     * @param m input matrix
     * @return matrix after activation - does not change the original matrix
     */
Matrix Activation::operator()(const Matrix &m) const
{
    if (_type == Softmax)
    {
        return softMax(m);
    }
    return relu(m);
}

/**
     * @brief returns the act type to whoever asks for it - const ver?
     * @return act type
     */
ActivationType Activation::getActivationType() const
{
    return _type;
}

